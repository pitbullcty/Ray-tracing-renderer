#ifndef __SKYBOX__
#define __SKYBOX__

#include <QString>
#include <QJsonObject>
#include <QMatrix4x4>
#include <QVector3D>
#include <QImage>

class Skybox: public QObject
{
	Q_OBJECT
public:
	static QSharedPointer<Skybox>& GetInstance();
	static void destory(Skybox* skybox);

    QVector<float> vertices;
	QMap<QString, QString> pathes;
	QMap<QString, QString> backUpPathes;

	QJsonObject toJson();
	void prase(const QJsonObject& skybox);

	void reset();

signals:
	void sendUpdateSkybox(bool isEmpty);

private:
	static QSharedPointer<Skybox> instance;
	Skybox();
	~Skybox() = default;
};

#endif 
