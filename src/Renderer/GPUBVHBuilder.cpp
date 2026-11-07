#include "GPUBVHBuilder.h"

#include <QElapsedTimer>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLContext>
#include <QSurfaceFormat>
#include <QVector3D>
#include <cstring>

namespace {
	//把float的可排序位模式还原成float(与 bvh_aabb.comp 中的 orderFloat 互逆)
	float decodeOrderedFloat(unsigned int ordered) {
		unsigned int bits = (ordered & 0x80000000u) != 0u ? (ordered & 0x7FFFFFFFu) : ~ordered;
		float value = 0.0f;
		std::memcpy(&value, &bits, sizeof(float));
		return value;
	}

	bool compileComputeProgram(QOpenGLShaderProgram* program, const QString& path, QString* error) {
		QOpenGLShader* shader = new QOpenGLShader(QOpenGLShader::Compute, program);
		if (!shader->compileSourceFile(path)) {
			*error = QString("compute shader编译失败 %1: %2").arg(path, shader->log());
			return false;
		}
		program->addShader(shader);
		if (!program->link()) {
			*error = QString("compute program链接失败 %1: %2").arg(path, program->log());
			return false;
		}
		return true;
	}

	int nextPowerOfTwo(int value) {
		int result = 1;
		while (result < value)
			result <<= 1;
		return result;
	}

	const int COMPUTE_GROUP_SIZE = 256;
}

GPUBVHBuilder::~GPUBVHBuilder()
{
	release();
}

bool GPUBVHBuilder::initialize(QOpenGLExtraFunctions* functions)
{
	f = functions;
	initialized = true;
	supported = false;
	status.clear();

	if (f == nullptr) {
		status = "当前没有可用的OpenGL函数";
		return false;
	}

	QOpenGLContext* context = QOpenGLContext::currentContext();
	if (context == nullptr) {
		status = "当前没有有效的OpenGL上下文";
		return false;
	}

	QSurfaceFormat format = context->format();
	const int major = format.majorVersion();
	const int minor = format.minorVersion();
	if (major < 4 || (major == 4 && minor < 3)) {
		status = QString("当前OpenGL版本为%1.%2(%3),需要4.3及以上才能使用GPU构建BVH")
			.arg(major).arg(minor).arg(glVersionString(f));
		return false;
	}

	if (!compilePrograms()) {
		supported = false;
		return false;
	}

	//三个SSBO: 场景包围盒、键值对、节点数据
	f->glGenBuffers(1, &sceneAabbBufferId);
	f->glGenBuffers(1, &keyBufferId);
	f->glGenBuffers(1, &nodeBufferId);
	f->glBindBuffer(GL_SHADER_STORAGE_BUFFER, sceneAabbBufferId);
	f->glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(unsigned int) * 6, nullptr, GL_DYNAMIC_DRAW);
	f->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	supported = true;
	status = QString("GPU构建BVH可用 (%1)").arg(glVersionString(f));
	return true;
}

QString GPUBVHBuilder::glVersionString(QOpenGLExtraFunctions* functions)
{
	if (functions == nullptr)
		return QString();
	const char* version = reinterpret_cast<const char*>(functions->glGetString(GL_VERSION));
	const char* renderer = reinterpret_cast<const char*>(functions->glGetString(GL_RENDERER));
	return QString("OpenGL %1 / %2").arg(version ? version : "unknown", renderer ? renderer : "unknown");
}

bool GPUBVHBuilder::compilePrograms()
{
	QString error;
	aabbProgram = new QOpenGLShaderProgram();
	mortonProgram = new QOpenGLShaderProgram();
	sortProgram = new QOpenGLShaderProgram();
	leafProgram = new QOpenGLShaderProgram();
	reduceProgram = new QOpenGLShaderProgram();

	struct ProgramItem {
		QOpenGLShaderProgram* program;
		const char* path;
	};
	const ProgramItem items[] = {
		{ aabbProgram, ":/bvh_aabb.comp" },
		{ mortonProgram, ":/bvh_morton.comp" },
		{ sortProgram, ":/bvh_sort.comp" },
		{ leafProgram, ":/bvh_leaf.comp" },
		{ reduceProgram, ":/bvh_reduce.comp" },
	};

	for (const ProgramItem& item : items) {
		if (!compileComputeProgram(item.program, QString::fromLatin1(item.path), &error)) {
			status = error;
			for (ProgramItem cleanup : items) {
				if (cleanup.program != nullptr) {
					cleanup.program->removeAllShaders();
				}
			}
			return false;
		}
	}
	return true;
}

bool GPUBVHBuilder::ensureCapacity(int paddedCount, int triangleCount)
{
	Q_UNUSED(triangleCount);
	if (paddedCount <= capacityLeaves)
		return true;

	//节点使用堆布局: 根为1, 叶子层有paddedCount个节点, 总节点数 2*paddedCount
	const int heapSize = paddedCount * 2;
	const qint64 keyBytes = qint64(paddedCount) * sizeof(unsigned int) * 2;
	nodeBytes = qint64(heapSize) * 12 * sizeof(float);

	f->glBindBuffer(GL_SHADER_STORAGE_BUFFER, keyBufferId);
	f->glBufferData(GL_SHADER_STORAGE_BUFFER, keyBytes, nullptr, GL_DYNAMIC_DRAW);
	f->glBindBuffer(GL_SHADER_STORAGE_BUFFER, nodeBufferId);
	f->glBufferData(GL_SHADER_STORAGE_BUFFER, nodeBytes, nullptr, GL_DYNAMIC_DRAW);
	f->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	capacityLeaves = paddedCount;
	return true;
}

GPUBVHBuilder::BuildResult GPUBVHBuilder::build(unsigned int triangleBuffer, int triangleCount)
{
	BuildResult result;
	result.triangleCount = triangleCount;

	if (!initialized)
		initialize(f);
	if (!supported) {
		result.error = status.isEmpty() ? QString("GPU构建BVH不可用") : status;
		return result;
	}
	if (triangleBuffer == 0 || triangleCount <= 0) {
		result.error = "没有可构建的三角形数据";
		return result;
	}

	const int paddedCount = nextPowerOfTwo(triangleCount);
	if (paddedCount > (1 << 22)) {
		result.error = QString("三角形数量过多(%1),暂不支持GPU构建").arg(triangleCount);
		return result;
	}
	if (!ensureCapacity(paddedCount, triangleCount)) {
		result.error = "分配GPU缓冲失败";
		return result;
	}

	//场景包围盒缓冲的初始值: 最小值为最大位模式, 最大值为0
	unsigned int aabbInit[6] = { 0xFFFFFFFFu, 0u, 0xFFFFFFFFu, 0u, 0xFFFFFFFFu, 0u };
	f->glBindBuffer(GL_SHADER_STORAGE_BUFFER, sceneAabbBufferId);
	f->glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(aabbInit), aabbInit);

	f->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, sceneAabbBufferId);
	f->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, triangleBuffer);
	f->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, keyBufferId);
	f->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, nodeBufferId);

	QElapsedTimer timer;
	timer.start();

	//1. 计算整体包围盒
	aabbProgram->bind();
	aabbProgram->setUniformValue("triCount", triangleCount);
	f->glDispatchCompute((triangleCount + 63) / 64, 1, 1);
	f->glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_BUFFER_UPDATE_BARRIER_BIT);

	unsigned int aabbBits[6] = { 0 };
	f->glBindBuffer(GL_SHADER_STORAGE_BUFFER, sceneAabbBufferId);
	void* mapped = f->glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(aabbBits), GL_MAP_READ_BIT);
	if (mapped != nullptr) {
		std::memcpy(aabbBits, mapped, sizeof(aabbBits));
		f->glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	}
	const QVector3D sceneMin(decodeOrderedFloat(aabbBits[0]), decodeOrderedFloat(aabbBits[2]), decodeOrderedFloat(aabbBits[4]));
	const QVector3D sceneMax(decodeOrderedFloat(aabbBits[1]), decodeOrderedFloat(aabbBits[3]), decodeOrderedFloat(aabbBits[5]));

	//2. 计算每个三角形的Morton码
	mortonProgram->bind();
	mortonProgram->setUniformValue("triCount", triangleCount);
	mortonProgram->setUniformValue("paddedCount", paddedCount);
	mortonProgram->setUniformValue("sceneMin", sceneMin);
	mortonProgram->setUniformValue("sceneMax", sceneMax);
	mortonProgram->setUniformValue("emptyKey", -1); //0xFFFFFFFF, 补齐项排序后落到末尾
	f->glDispatchCompute((paddedCount + COMPUTE_GROUP_SIZE - 1) / COMPUTE_GROUP_SIZE, 1, 1);
	f->glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	//3. 双调排序(外层k逐级扩大, 内层j逐级缩小)
	sortProgram->bind();
	sortProgram->setUniformValue("count", paddedCount);
	for (int k = 2; k <= paddedCount; k <<= 1) {
		for (int j = k >> 1; j > 0; j >>= 1) {
			sortProgram->setUniformValue("kStep", k);
			sortProgram->setUniformValue("jStep", j);
			f->glDispatchCompute((paddedCount + COMPUTE_GROUP_SIZE - 1) / COMPUTE_GROUP_SIZE, 1, 1);
			f->glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		}
	}

	//4. 写叶子节点
	leafProgram->bind();
	leafProgram->setUniformValue("triCount", triangleCount);
	leafProgram->setUniformValue("paddedCount", paddedCount);
	leafProgram->setUniformValue("leafBase", paddedCount);
	f->glDispatchCompute((paddedCount + COMPUTE_GROUP_SIZE - 1) / COMPUTE_GROUP_SIZE, 1, 1);
	f->glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	//5. 自底向上合并出内部节点: 叶子在层L(log2(paddedCount)), 内部节点层依次向上到根(层0)
	int maxLevel = 0;
	while ((1 << maxLevel) < paddedCount)
		++maxLevel;

	for (int level = maxLevel - 1; level >= 0; --level) {
		const int levelBase = 1 << level;
		reduceProgram->bind();
		reduceProgram->setUniformValue("levelBase", levelBase);
		reduceProgram->setUniformValue("levelCount", levelBase);
		reduceProgram->setUniformValue("heapSize", paddedCount * 2);
		f->glDispatchCompute((levelBase + COMPUTE_GROUP_SIZE - 1) / COMPUTE_GROUP_SIZE, 1, 1);
		f->glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}

	f->glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);
	f->glFinish();

	//解绑所有SSBO绑定点: 节点缓冲随后会被当成纹理缓冲使用, 某些驱动对同一缓冲同时挂着SSBO绑定会出问题
	for (unsigned int binding = 0; binding < 4; ++binding)
		f->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, 0);
	f->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	lastMs = timer.nsecsElapsed() / 1000000.0;
	lastNodeCount = paddedCount * 2;


	result.ok = true;
	result.gpuMs = lastMs;
	result.nodeCount = lastNodeCount;
	result.leafCapacity = paddedCount;
	return result;
}

bool GPUBVHBuilder::readbackNodes(QVector<float>& nodes)
{
	if (f == nullptr || nodeBufferId == 0 || nodeBytes <= 0 || lastNodeCount <= 0)
		return false;

	nodes.resize(lastNodeCount * 12);
	f->glBindBuffer(GL_SHADER_STORAGE_BUFFER, nodeBufferId);
	void* mapped = f->glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, nodeBytes, GL_MAP_READ_BIT);
	if (mapped == nullptr) {
		f->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		return false;
	}
	std::memcpy(nodes.data(), mapped, size_t(nodeBytes));
	f->glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	f->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	return true;
}

void GPUBVHBuilder::release()
{
	//只有在GL线程且上下文有效时才能安全删除GL对象
	if (f != nullptr && QOpenGLContext::currentContext() == nullptr) {
		sceneAabbBufferId = 0;
		keyBufferId = 0;
		nodeBufferId = 0;
		f = nullptr;
		initialized = false;
		supported = false;
		return;
	}
	if (f != nullptr) {
		if (sceneAabbBufferId != 0) f->glDeleteBuffers(1, &sceneAabbBufferId);
		if (keyBufferId != 0) f->glDeleteBuffers(1, &keyBufferId);
		if (nodeBufferId != 0) f->glDeleteBuffers(1, &nodeBufferId);
	}
	sceneAabbBufferId = 0;
	keyBufferId = 0;
	nodeBufferId = 0;
	capacityLeaves = 0;
	nodeBytes = 0;

	auto deleteProgram = [](QOpenGLShaderProgram*& program) {
		if (program != nullptr) {
			program->removeAllShaders();
			delete program;
			program = nullptr;
		}
	};
	deleteProgram(aabbProgram);
	deleteProgram(mortonProgram);
	deleteProgram(sortProgram);
	deleteProgram(leafProgram);
	deleteProgram(reduceProgram);
	f = nullptr;
	initialized = false;
	supported = false;
}
