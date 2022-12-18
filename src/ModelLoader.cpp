#include "ModelLoader.h"

QSharedPointer<ModelLoader> ModelLoader::instance = nullptr;

static QVector3D assimp2QVector(const aiVector3D& vector) {
    return QVector3D(vector.x, vector.y, vector.z);
}

static aiVector3D QVector2assimp(const QVector3D& vector) {
    return aiVector3D(vector.x(), vector.y(), vector.z());
}

QSharedPointer<ModelLoader>& ModelLoader::GetInstance()
{
	if (instance.isNull())
		instance = QSharedPointer<ModelLoader>(new ModelLoader(), ModelLoader::destory);
	return instance;
}

void ModelLoader::destory(ModelLoader* modelLoader) {
	delete modelLoader;
}

static QSharedPointer<QOpenGLTexture> textureFromFile(const QString& path)
{
    QImage image(path);
    if (image.isNull()) {
        QSharedPointer<QOpenGLTexture> texture(nullptr);
        return texture;
    }
    QSharedPointer<QOpenGLTexture> texture(new QOpenGLTexture(image));
    texture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
    texture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    return texture;
}


Model ModelLoader::loadModel(QString path) {
    this->path = path;
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path.toStdString(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals); //读入场景,基于y轴翻转纹理坐标,换所有的模型的原始几何形状为三角形
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)//检查场景和其根节点不为null，并且检查了标记(Flag)，查看返回的数据完整性。
    {
        qDebug() << "ERROR::" << import.GetErrorString();
        return Model();
    }
    qDebug() << "网格：" << scene->mNumMeshes;
    qDebug() << "材质：" << scene->mNumMaterials;
    qDebug() << "纹理：" << scene->mNumTextures;
    qDebug() << "动画：" << scene->mNumAnimations;
    processNode(scene->mRootNode, scene);
    for (auto& nodeCenter : nodeCenters) {
        center += nodeCenter;
    }
    Model m(meshes, center/nodeCenters.size());
    meshes.clear();
    textures_loaded.clear();
    nodeCenters.clear();
    center.setX(0);
    center.setY(0);
    center.setZ(0);
    return m;
}

/*
* 递归处理子节点
*/
void ModelLoader::processNode(aiNode* node, const aiScene* scene)
{
    // 处理节点所有的网格（如果有的话
    QVector3D nodeCenter(0, 0, 0);
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        Mesh temp = processMesh(mesh, scene);
        nodeCenter += temp.center;
        meshes.push_back(temp);
    }
    if (node->mNumMeshes != 0) {
        nodeCenter /= node->mNumMeshes;
        nodeCenters.push_back(nodeCenter);
    }
    // 递归处理子节点重复这一过程
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}


Mesh ModelLoader::processMesh(aiMesh* mesh, const aiScene* scene)
{
    QVector<Vertex> vertices;
    QVector<unsigned int> indices;
    QVector<Texture> textures; //存放
    aiVector3D meshCenter(0, 0, 0);

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        QVector3D vector;

        //顶点坐标
        vector.setX(mesh->mVertices[i].x);
        vector.setY(mesh->mVertices[i].y);
        vector.setZ(mesh->mVertices[i].z);
        vertex.pos = vector;

        // 法向量
        if (mesh->mNormals) {
            vector.setX(mesh->mNormals[i].x);
            vector.setY(mesh->mNormals[i].y);
            vector.setZ(mesh->mNormals[i].z);
            vertex.normal = vector;
        }

        // 纹理坐标
        if (mesh->mTextureCoords[0])
        {
            QVector2D vec;
            //一个顶点最多可以包含8个不同的纹理坐标,我们使用第一个
            vec.setX(mesh->mTextureCoords[0][i].x);
            vec.setY(mesh->mTextureCoords[0][i].y);
            vertex.texCoords = vec;
        }
        else {
            vertex.texCoords = QVector2D(0, 0);
        }

        vertices.push_back(vertex);
    }

    //检索相应的顶点索引。
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        aiVector3D faceCenter(0, 0, 0); //面中心
        // 将所有面的索引数据添加到索引数组中
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
            faceCenter = faceCenter + mesh->mVertices[face.mIndices[j]];
        }
        faceCenter /= face.mNumIndices;
        meshCenter += faceCenter;
    }
    meshCenter /= mesh->mNumFaces;

    // 处理材质
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    // 1. 漫反射贴图
    QVector<Texture>  diffuseMaps = loadTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
    for (auto& texture : diffuseMaps)
        textures.push_back(texture);

    // 2. 镜面贴图
    QVector<Texture> specularMaps = loadTexture(material, aiTextureType_SPECULAR, "texture_specular");
    for (auto& texture : specularMaps)
        textures.push_back(texture);

    // 3. 法向量图
    QVector<Texture> normalMaps = loadTexture(material, aiTextureType_HEIGHT, "texture_normal");
    for (auto& texture : normalMaps)
        textures.push_back(texture);

    // 4. 高度图
    QVector<Texture> heightMaps = loadTexture(material, aiTextureType_AMBIENT, "texture_height");
    for (auto& texture : heightMaps)
        textures.push_back(texture);

    QVector3D center = assimp2QVector(meshCenter);

    return Mesh(vertices, indices, textures, center);

}

QVector<Texture> ModelLoader::loadTexture(aiMaterial* material, aiTextureType type, QString name) {
    QVector<Texture> textures;

    unsigned int count = material->GetTextureCount(type);

    for (unsigned int i = 0; i < count; i++)
    {
        aiString str;
        material->GetTexture(type, i, &str);//获取每个纹理的文件位置，它会将结果储存在一个aiString中

        // 检查纹理是否在之前加载过，如果是，则继续到下一个迭代:跳过加载新纹理
        bool skip = false;
        for (int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.toStdString().c_str(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; //带有相同filepath的纹理已经加载，继续到下一个
                break;
            }
        }
        if (!skip)
        {   // 如果材质还没有加载，加载它
            QString texpath = path.left(path.lastIndexOf("/")) + "/" + QString(str.C_Str()).replace('\\', '/');
            Texture tex;
            tex.texture = textureFromFile(texpath);
            if (!tex.texture.isNull()) {
                tex.type = name;
                tex.path = texpath;
                textures.push_back(tex);
                textures_loaded.push_back(tex);
                qDebug() << "纹理加载成功：" << texpath;
            }
            else {
                qDebug() << "未能成功加载纹理：" << texpath;
            }
        }
    }
    return textures;
}