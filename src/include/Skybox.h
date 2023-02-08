#ifndef __SKYBOX__
#define __SKYBOX__
#include <QString>
#include <qDebug>
#include <QMatrix4x4>
#include <QVector3D>
#include <QImage>

class Skybox
{
public:
	static QSharedPointer<Skybox>& GetInstance();
	static void destory(Skybox* skybox);
    QVector<float> vertices;
	QMap<QString, QString> path;

private:
	static QSharedPointer<Skybox> instance;
	Skybox();
	~Skybox() = default;
};

#endif 
