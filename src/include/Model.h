#ifndef __MODEL__
#define __MODEL__

#include "Mesh.h"
#include <QDebug>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
public:
	Model(QString _path);
	~Model();

private:
	QString path;
	QVector<Texture> textures_loaded;  //材质
	QVector<Mesh> meshes; //纹理

	bool createModel(QString path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	QVector<Texture> loadTexture(aiMaterial* material, aiTextureType type, QString name);

};



#endif 