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

void ModelLoader::removePath(const QString& path)
{
    pathLoaded.removeAll(path);
}


void ModelLoader::clearPathes()
{
    pathLoaded.clear();
}

void ModelLoader::addPath(const QString& path)
{
    if(!pathLoaded.contains(path))
        pathLoaded.push_back(path);
}

bool ModelLoader::checkPath(const QString& path)
{
    return pathLoaded.contains(path);
}

void ModelLoader::setContext(QOpenGLExtraFunctions* functions, QOpenGLShaderProgram* shaderProgram)
{
    this->functions = functions;
    this->shaderProgram = shaderProgram;
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


LOADRESULT ModelLoader::loadModel(const QString& path, Model &model, bool isLight) {
    
    this->path = path;
    meshes.clear();
    textures_loaded.clear();
    nodeCenters.clear();
    center = QVector3D(); //加载前清空
    if (pathLoaded.contains(path)) {
        return RELOADED;
    } //若模型已经加载过则直接复制
    pathLoaded.push_back(path);
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path.toStdString(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals| aiProcess_CalcTangentSpace); //读入场景,基于y轴翻转纹理坐标,换所有的模型的原始几何形状为三角形
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)//检查场景和其根节点不为null，并且检查了标记(Flag)，查看返回的数据完整性。
    {
        auto error = import.GetErrorString();
        emit Error("ASSIMP ERRPR:" + QString(error));
        return FAILED;
    }
    if(!isLight)
        emit Info(QString("模型%1网格数量：").arg(path) + QString::number(scene->mNumMeshes) + " 模型材质数量：" + QString::number(scene->mNumMaterials) + " 模型纹理数量：" + QString::number(scene->mNumTextures));
    processNode(scene->mRootNode, scene);
    for (auto& nodeCenter : nodeCenters) {
        center += nodeCenter;
    }
    for (auto& mesh : meshes) {
        model.getBound().Union(mesh->bound);
    }
    model.setData(meshes, path, center/nodeCenters.size());
    return SUCCESS;
}

ModelLoader::~ModelLoader()
{
    functions = nullptr;
    shaderProgram = nullptr;
}

ModelLoader::ModelLoader():functions(nullptr),shaderProgram(nullptr)
{
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
        auto temp = processMesh(mesh, scene);
        nodeCenter += temp->center;
        meshes.emplace_back(temp);
    }
    if (node->mNumMeshes != 0) {
        nodeCenter /= node->mNumMeshes;
        nodeCenters.emplace_back(nodeCenter);
    }
    // 递归处理子节点重复这一过程
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}


QSharedPointer<Mesh> ModelLoader::processMesh(aiMesh* mesh, const aiScene* scene)
{
    QSharedPointer<Mesh> m(new Mesh(functions, shaderProgram));
    aiVector3D meshCenter(0, 0, 0);

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        QVector3D vector{ mesh->mVertices[i].x, mesh->mVertices[i].y,mesh->mVertices[i].z };//顶点坐标
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

        m->vertices.push_back(vertex);
        m->bound.Union(vertex.pos); //计算bound
    }

    //检索相应的顶点索引。
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        aiVector3D faceCenter(0, 0, 0); //面中心
        // 将所有面的索引数据添加到索引数组中
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            m->indices.emplace_back(face.mIndices[j]);
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
        m->textures.emplace_back(texture);

    // 2. 金属度贴图

    QVector<Texture> mentalnessMaps = loadTexture(material, aiTextureType_METALNESS, "texture_metalness");
    for (auto& texture : mentalnessMaps)
        m->textures.emplace_back(texture);

    // 3. 法向量图
    QVector<Texture> normalMaps;
    if (path.contains(".obj")) //针对obj文件采用另外方式读取法线贴图
        normalMaps = loadTexture(material, aiTextureType_HEIGHT, "texture_normal");
    else
        normalMaps = loadTexture(material, aiTextureType_NORMALS, "texture_normal");
    for (auto& texture : normalMaps)
        m->textures.emplace_back(texture);

    // 4. 自发光图
    QVector<Texture> emissiveMaps = loadTexture(material, aiTextureType_EMISSIVE, "texture_emissive");
    for (auto& texture : emissiveMaps)
        m->textures.emplace_back(texture);

    QVector3D center = assimp2QVector(meshCenter);

    m->center = center;

    return m;
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
        for (const auto& j: textures_loaded)
        {
            if (std::strcmp(j.path.toStdString().c_str(), str.C_Str()) == 0)
            {
                textures.emplace_back(j);
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
                textures.emplace_back(tex);
                textures_loaded.emplace_back(tex);
            }
            else {
                emit Warning("未能加载纹理" + texpath);
                int index = pathLoaded.indexOf(this->path);
                if(index!=-1)
                    pathLoaded.remove(index); //删除未加载成功模型
            }
        }
    }
    return textures;
}