#include "OfflineRenderController.h"
#include <QSettings>
#include "RayTracingRenderer.h"

#include <QDebug>
#include <QDir>
#include <QElapsedTimer>
#include <QFileInfo>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QOpenGLShaderProgram>
#include <QSurfaceFormat>

//与BaseOpenGLWidget::compileShader一致:着色器从qrc中读取,可在任意线程编译
static bool compileProgram(QOpenGLShaderProgram& program, const QString& shaderName, const QString& vertName = QString())
{
	const QString vertPath = ":/" + (vertName.isEmpty() ? shaderName : vertName) + ".vert";
	const QString fragPath = ":/" + shaderName + ".frag";

	if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, vertPath)) {
		qDebug() << "ERROR:" << program.log();
		return false;
	}
	if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, fragPath)) {
		qDebug() << "ERROR:" << program.log();
		return false;
	}
	if (!program.link()) {
		qDebug() << "ERROR:" << program.log();
		return false;
	}
	return true;
}

void OfflineRenderWorker::runJob(const OfflineRenderJob& job, std::atomic<bool>* cancelFlag)
{
	QElapsedTimer timer;
	timer.start();

	const int maxFrames = qMax(1, static_cast<int>(job.option.maxFrameCounter));
	const int previewInterval = 10; //每渲染若干帧回传一次预览图和进度

	//独立线程里的离屏OpenGL上下文,与界面线程的QOpenGLWidget上下文互不影响
	QSurfaceFormat format;
	format.setDepthBufferSize(24);

	QOpenGLContext context;
	context.setFormat(format);
	if (!context.create()) {
		emit failed("离屏渲染失败:无法创建OpenGL上下文");
		return;
	}

	QOffscreenSurface surface;
	surface.setFormat(context.format());
	surface.create();
	if (!surface.isValid()) {
		emit failed("离屏渲染失败:无法创建离屏绘图表面");
		return;
	}

	if (!context.makeCurrent(&surface)) {
		emit failed("离屏渲染失败:无法激活OpenGL上下文");
		return;
	}

	QOpenGLShaderProgram pathTracingProgram;
	QOpenGLShaderProgram accumProgram;
	QOpenGLShaderProgram outputProgram;
	QOpenGLShaderProgram denoiseMomentsProgram;
	QOpenGLShaderProgram denoiseAtrousProgram;
	if (!compileProgram(pathTracingProgram, "pathTracing")
		|| !compileProgram(accumProgram, "accum", "pathTracing")
		|| !compileProgram(outputProgram, "output", "pathTracing")
		|| !compileProgram(denoiseMomentsProgram, "denoise_moments", "pathTracing")
		|| !compileProgram(denoiseAtrousProgram, "denoise_atrous", "pathTracing")) {
		context.doneCurrent();
		emit failed("离屏渲染失败:着色器编译失败");
		return;
	}

	QMap<QString, QOpenGLShaderProgram*> shaderMap;
	shaderMap.insert("pathTracing", &pathTracingProgram);
	shaderMap.insert("accum", &accumProgram);
	shaderMap.insert("output", &outputProgram);
	shaderMap.insert("denoise_moments", &denoiseMomentsProgram);
	shaderMap.insert("denoise_atrous", &denoiseAtrousProgram);

	QOpenGLExtraFunctions* functions = context.extraFunctions();
	RayTracingRenderer* renderer = new RayTracingRenderer(shaderMap, functions,
		job.option.resolutionX, job.option.resolutionY);

	renderer->getOption() = job.option; //把本次渲染参数(分辨率/帧数/降噪步长/输出路径)交给渲染器
	renderer->setStandaloneMode(true); //独立模式:不显示到屏幕,也不自动保存
	//离屏渲染也遵循设置里的降噪选项(GPU a-trous 或 OIDN 最终降噪)
	{
		QSettings settings("settings.ini", QSettings::IniFormat);
		renderer->setRealTimeDenoise(settings.value("denoise/enabled", true).toBool());
		renderer->setFireflyClamp(settings.value("denoise/fireflyClamp", 8.0).toFloat());
		renderer->setUseOIDNFinalDenoise(settings.value("denoise/useOIDNFinal", false).toBool());
	}
	renderer->resize(job.option.resolutionX, job.option.resolutionY); //FBO按渲染分辨率重建
	renderer->setCameraSnapshot(job.camera.pos, job.camera.view, job.camera.zoom);
	renderer->initSkyboxTextureFromImages(job.skyboxFaces);
	renderer->uploadRenderData(job.data, job.background);

	bool cancelled = false;
	for (int i = 0; i < maxFrames; ++i) {
		if (cancelFlag && cancelFlag->load()) {
			cancelled = true;
			break;
		}

		renderer->render();

		if (i % previewInterval == 0 || i == maxFrames - 1) {
			emit progress(static_cast<int>(renderer->getFrameCounter()), maxFrames, timer.elapsed() / 1000.0f);
			QImage image = renderer->grabResultImage();
			if (!image.isNull())
				emit previewReady(image.scaled(960, 540, Qt::KeepAspectRatio, Qt::SmoothTransformation));
		}
	}

	if (!cancelled) {
		QDir().mkpath(QFileInfo(job.option.outputPath).absolutePath()); //输出目录不存在时自动创建
		renderer->saveRenderResult(job.option.outputPath, job.option.quality);
	}

	renderer->destoryData();
	delete renderer;

	context.doneCurrent();
	surface.destroy();

	emit finished(job.option.outputPath, timer.elapsed() / 1000.0f, !cancelled);
}

OfflineRenderController::OfflineRenderController(QObject* parent) :
	QObject(parent),
	worker(new OfflineRenderWorker),
	cancelFlag(false),
	running(false)
{
	worker->moveToThread(&thread);
	connect(&thread, &QThread::finished, worker, &QObject::deleteLater);
	connect(worker, &OfflineRenderWorker::progress, this, &OfflineRenderController::progress);
	connect(worker, &OfflineRenderWorker::previewReady, this, &OfflineRenderController::previewReady);
	connect(worker, &OfflineRenderWorker::failed, this, &OfflineRenderController::failed);
	connect(worker, &OfflineRenderWorker::finished, this, &OfflineRenderController::onWorkerFinished);
	thread.start();
}

OfflineRenderController::~OfflineRenderController()
{
	cancelFlag.store(true);
	thread.quit();
	if (!thread.wait(15000))
		thread.terminate();
}

bool OfflineRenderController::isRunning() const
{
	return running.load();
}

void OfflineRenderController::start(const OfflineRenderJob& job)
{
	if (running.load())
		return;

	cancelFlag.store(false);
	running.store(true);

	OfflineRenderWorker* workerPtr = worker;
	std::atomic<bool>* cancelPtr = &cancelFlag;
	QMetaObject::invokeMethod(worker, [workerPtr, cancelPtr, job]() {
		workerPtr->runJob(job, cancelPtr);
	}, Qt::QueuedConnection);
}

void OfflineRenderController::cancel()
{
	cancelFlag.store(true);
}

void OfflineRenderController::onWorkerFinished(const QString& path, float seconds, bool success)
{
	running.store(false);
	emit finished(path, seconds, success);
}
