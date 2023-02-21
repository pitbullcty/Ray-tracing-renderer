#ifndef __SKYBOX__
#define __SKYBOX__

#include <QString>
#include <QJsonObject>
#include <QMatrix4x4>
#include <QVector3D>
#include <QImage>

class Skybox
{
public:
	static QSharedPointer<Skybox>& GetInstance();
	static void destory(Skybox* skybox);
    QVector<float> vertices;
	QMap<QString, QString> pathes;
	void setPath(const QString& location, const QString& path);
	QJsonObject toJson();
	void prase(QJsonObject skybox);

private:
	static QSharedPointer<Skybox> instance;
	Skybox();
	~Skybox() = default;
};

#endif 
