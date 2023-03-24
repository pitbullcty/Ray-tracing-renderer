#include "Skybox.h"

QSharedPointer<Skybox> Skybox::instance = nullptr;

QSharedPointer<Skybox>& Skybox::GetInstance()
{
	if (instance.isNull())
		instance = QSharedPointer<Skybox>(new Skybox(), Skybox::destory);
	return instance;
}

void Skybox::destory(Skybox* skybox)
{

	delete skybox;
}

void Skybox::setPath(const QString& location, const QString& path)
{
    if (pathes.contains(location)) {
        pathes[location] = path;
    }
}

QJsonObject Skybox::toJson()
{
    QJsonObject skybox;
    for (auto it = pathes.begin(); it != pathes.end(); it++) {
        skybox.insert(it.key(), it.value());
    }
    return skybox;
}

void Skybox::prase(const QJsonObject& skybox)
{
    auto keys = skybox.keys();
    for (int i = 0; i < keys.size(); i++) {
        auto& key = keys.at(i);
        auto newPath = skybox[key].toString();
        if (newPath != pathes[key]) {
            pathes[key] = newPath;
        }
    }
}

Skybox::Skybox()
{
    pathes = {
        {"+x", ":/skybox/px.png"},
        {"-x", ":/skybox/nx.png"},
        {"+y", ":/skybox/py.png"},
        {"-y", ":/skybox/ny.png"},
        {"+z", ":/skybox/pz.png"},
        {"-z", ":/skybox/nz.png"},
    };

	vertices = {
        // back face
       -1.0f, -1.0f, -1.0f,   // bottom-left
        1.0f,  1.0f, -1.0f,  // top-right
        1.0f, -1.0f, -1.0f, // bottom-right         
        1.0f,  1.0f, -1.0f, // top-right
       -1.0f, -1.0f, -1.0f, // bottom-left
       -1.0f,  1.0f, -1.0f, // top-left
       // front face
       -1.0f, -1.0f,  1.0f,  // bottom-left
        1.0f, -1.0f,  1.0f,  // bottom-right
        1.0f,  1.0f,  1.0f,  // top-right
        1.0f,  1.0f,  1.0f,  // top-right
       -1.0f,  1.0f,  1.0f,  // top-left
       -1.0f, -1.0f,  1.0f,  // bottom-left
       // left face
       -1.0f,  1.0f,  1.0f,  // top-right
       -1.0f,  1.0f, -1.0f,  // top-left
       -1.0f, -1.0f, -1.0f, // bottom-left
       -1.0f, -1.0f, -1.0f, // bottom-left
       -1.0f, -1.0f,  1.0f, // bottom-right
       -1.0f,  1.0f,  1.0f, // top-right
       // right face
        1.0f,  1.0f,  1.0f,// top-left
        1.0f, -1.0f, -1.0f,// bottom-right
        1.0f,  1.0f, -1.0f,// top-right         
        1.0f, -1.0f, -1.0f,// bottom-right
        1.0f,  1.0f,  1.0f,// top-left
        1.0f, -1.0f,  1.0f, // bottom-left     
        // bottom face
        -1.0f, -1.0f, -1.0f,// top-right
         1.0f, -1.0f, -1.0f,// top-left
         1.0f, -1.0f,  1.0f,// bottom-left
         1.0f, -1.0f,  1.0f,// bottom-left
        -1.0f, -1.0f,  1.0f,// bottom-right
        -1.0f, -1.0f, -1.0f,// top-right
        // top face
        -1.0f,  1.0f, -1.0f,// top-left
         1.0f,  1.0f , 1.0f,// bottom-right
         1.0f,  1.0f, -1.0f,// top-right     
         1.0f,  1.0f,  1.0f,// bottom-right
        -1.0f,  1.0f, -1.0f,// top-left
        -1.0f,  1.0f,  1.0f, // bottom-left        
    };

}


