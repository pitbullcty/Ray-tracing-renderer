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

Skybox::Skybox(QVector<QString> _path)
{
  
    for (int i = 0; i < _path.size(); i++) {
        skyboxes.push_back(QImage(_path[i]).convertToFormat(QImage::Format_RGB888));
    }
    
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


