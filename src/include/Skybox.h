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
	QMap<QString, QString> path;

private:
	static QSharedPointer<Skybox> instance;
	Skybox(QMap<QString, QString> _path = {
		{"+x", ":/skybox/px.png"},
		{"-x", ":/skybox/nx.png"},
		{"+y", ":/skybox/py.png"},
		{"-y", ":/skybox/ny.png"},
		{"+z", ":/skybox/pz.png"},
		{"-z", ":/skybox/nz.png"},
		});
	~Skybox() = default;
};

#endif 
