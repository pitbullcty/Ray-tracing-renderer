#ifndef __OFFLINE_RENDER_CONTROLLER__
#define __OFFLINE_RENDER_CONTROLLER__

#include <QObject>
#include <QThread>
#include <QImage>
#include <atomic>
#include "OfflineRenderJob.h"

//离屏渲染工作者:运行在独立线程中,自己创建离屏OpenGL上下文,
//因此界面线程的OpenGL上下文与渲染循环完全不受影响
class OfflineRenderWorker : public QObject
{
	Q_OBJECT

public:
	OfflineRenderWorker() = default;
	void runJob(const OfflineRenderJob& job, std::atomic<bool>* cancelFlag);

signals:
	void progress(int frame, int maxFrame, float seconds);
	void previewReady(const QImage& image);
	void finished(const QString& path, float seconds, bool success);
	void failed(const QString& error);
};

//离屏渲染控制器:持有工作线程,负责把任务丢给后台线程并转发进度
class OfflineRenderController : public QObject
{
	Q_OBJECT

public:
	explicit OfflineRenderController(QObject* parent = nullptr);
	~OfflineRenderController() override;

	bool isRunning() const;
	void start(const OfflineRenderJob& job);
	void cancel();

signals:
	void progress(int frame, int maxFrame, float seconds);
	void previewReady(const QImage& image);
	void finished(const QString& path, float seconds, bool success);
	void failed(const QString& error);

private slots:
	void onWorkerFinished(const QString& path, float seconds, bool success);

private:
	QThread thread;
	OfflineRenderWorker* worker;
	std::atomic<bool> cancelFlag;
	std::atomic<bool> running;
};

#endif // !__OFFLINE_RENDER_CONTROLLER__
