#ifndef __SKYBOX__
#define __SKYBOX__
#include <QString>
#include <qDebug>
#include <QMatrix4x4>
#include <QVector3D>
#include <QImage>
#include <QOpenGLWidget>

class Skybox
{
public:
	static QSharedPointer<Skybox>& GetInstance();
	static void destory(Skybox* skybox);
    QVector<float> vertices;
	QVector<QImage> skyboxes;

private:
	static QSharedPointer<Skybox> instance;
	Skybox(QVector<QString> _path = {
		":/skybox/px.png",
		":/skybox/nx.png",
		":/skybox/py.png",
		":/skybox/ny.png",
		":/skybox/pz.png",
		":/skybox/nz.png"
		});
	~Skybox() = default;
};

#endif 
