#ifndef __MODEL_LOADER__
#define __MODEL_LOADER__

#include <QSharedPointer>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Model.h"
#include <QOpenGLContext>

enum LOADRESULT {
	SUCCESS,
	FAILED,
	RELOADED
}; //模型加载结果

class ModelLoader {
public:
	static QSharedPointer<ModelLoader>& GetInstance();
	static void destory(ModelLoader* modelLoader);
	void clearPathes();
	LOADRESULT loadModel(const QString& path, Model& model);

private:
	QString path; //当前加载路径
	QVector<QString> pathLoaded; //已经加载路径
	QVector<Texture> textures_loaded;  //本次模型已加载材质
	QVector<Mesh> meshes; //纹理
	QVector<QVector3D> nodeCenters; //各结点中心
	QVector3D center; //材质中心


	static QSharedPointer<ModelLoader> instance;
	~ModelLoader() = default;
	ModelLoader() = default;
	
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	QVector<Texture> loadTexture(aiMaterial* material, aiTextureType type, QString name);

};


#endif