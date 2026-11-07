#ifndef __OFFLINE_RENDER_JOB__
#define __OFFLINE_RENDER_JOB__

#include "src/Manager/DataBuilder.h"
#include "RenderOption.h"
#include <QImage>
#include <QVector>
#include <QVector3D>
#include <QMatrix4x4>

//相机快照,离屏渲染在后台线程执行,不能直接读取界面线程的Camera单例
struct CameraSnapshot
{
	QVector3D pos;
	QMatrix4x4 view;
	float zoom = 45.0f;
};

//一次离屏渲染任务所需的所有数据,在界面线程准备好后整体拷贝给后台线程
//(QVector/QImage 都是隐式共享,拷贝代价很低,且之后数据重建不会影响这份快照)
struct OfflineRenderJob
{
	RenderData data;             //编码后的渲染数据(BVH/三角形/材质/贴图)
	QImage background;           //无场景时的背景图
	QVector<QImage> skyboxFaces; //天空盒六个面(+x,-x,+y,-y,+z,-z)
	CameraSnapshot camera;       //点击渲染那一刻的相机状态
	RenderOption option;         //渲染设置(分辨率/采样数/降噪/输出路径等)
};

#endif // !__OFFLINE_RENDER_JOB__
