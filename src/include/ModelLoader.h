#ifndef __MODEL_LOADER__
#define __MODEL_LOADER__

#include <QSharedPointer>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Model.h"
#include "Console.h"

enum LOADRESULT {
	SUCCESS,
	FAILED,
	RELOADED
}; //模型加载结果

class ModelLoader:public QObject{
	Q_OBJECT
public:
	static QSharedPointer<ModelLoader>& GetInstance();
	static void destory(ModelLoader* modelLoader);
	void removePath(const QString& path);
	void clearPathes();
	void addPath(const QString& path);
	bool checkPath(const QString& path);
	void setContext(QOpenGLExtraFunctions* functions, QOpenGLShaderProgram* shaderProgram);
	LOADRESULT loadModel(const QString& path, Model& model, bool isLight = false);

signals:
	void Info(QString info);
	void Warning(QString warning);
	void Error(QString error);

private:
	QString path; //当前加载路径
	QVector<QString> pathLoaded; //已经加载路径
	QVector<Texture> textures_loaded;  //本次模型已加载材质
	QVector<QSharedPointer<Mesh>> meshes; //纹理
	QVector<QVector3D> nodeCenters; //各结点中心
	QVector3D center; //材质中心

	QOpenGLExtraFunctions* functions;
	QOpenGLShaderProgram* shaderProgram; //上下文

	static QSharedPointer<ModelLoader> instance;
	~ModelLoader();
	ModelLoader();
	
	void processNode(aiNode* node, const aiScene* scene);
	QSharedPointer<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
	QVector<Texture> loadTexture(aiMaterial* material, aiTextureType type, QString name);

};


#endif