#ifndef __GPU_BVH_BUILDER__
#define __GPU_BVH_BUILDER__

#include <QString>
#include <QVector>
#include <QOpenGLFunctions>

class QOpenGLExtraFunctions;
class QOpenGLShaderProgram;

//GPU侧动态BVH构建器: Morton编码 + 双调排序 + 自底向上合并,全部由compute shader完成
//节点数据写成与 pathTracing.frag 中 getBVHNode() 完全一致的12个float布局,
//因此构建结果可以直接用 glTexBuffer 绑成 samplerBuffer 供光追遍历使用。
class GPUBVHBuilder
{
public:
	struct BuildResult {
		bool ok = false;
		double gpuMs = 0.0;
		int triangleCount = 0;
		int nodeCount = 0;
		int leafCapacity = 0;
		QString error;
	};

	GPUBVHBuilder() = default;
	~GPUBVHBuilder();

	GPUBVHBuilder(const GPUBVHBuilder&) = delete;
	GPUBVHBuilder& operator=(const GPUBVHBuilder&) = delete;

	//必须在GL线程且当前上下文有效时调用
	bool initialize(QOpenGLExtraFunctions* functions);
	bool isSupported() const { return supported; }
	QString statusText() const { return status; }

	//triangleBuffer: 已有的三角形纹理缓冲对象(9个vec3/三角形),
	//triangleCount: 三角形数量
	BuildResult build(unsigned int triangleBuffer, int triangleCount);

	double lastGpuMs() const { return lastMs; }
	int lastGpuNodeCount() const { return lastNodeCount; }
	//把节点数据读回CPU(12个float/节点, 共lastNodeCount个)
	bool readbackNodes(QVector<float>& nodes);
	void release();

private:
	bool compilePrograms();
	bool ensureCapacity(int paddedCount, int triangleCount);
	static QString glVersionString(QOpenGLExtraFunctions* functions);

	QOpenGLExtraFunctions* f = nullptr;
	bool initialized = false;
	bool supported = false;
	QString status;

	QOpenGLShaderProgram* aabbProgram = nullptr;
	QOpenGLShaderProgram* mortonProgram = nullptr;
	QOpenGLShaderProgram* sortProgram = nullptr;
	QOpenGLShaderProgram* leafProgram = nullptr;
	QOpenGLShaderProgram* reduceProgram = nullptr;

	unsigned int sceneAabbBufferId = 0;
	unsigned int keyBufferId = 0;
	unsigned int nodeBufferId = 0;        //compute shader写入的SSBO
	qint64 nodeBytes = 0;                 //节点缓冲字节数

	int capacityLeaves = 0; //keyBuffer/nodeBuffer当前容纳的叶子数
	double lastMs = 0.0;
	int lastNodeCount = 0;
};

#endif
