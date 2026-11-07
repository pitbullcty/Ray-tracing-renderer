#include "Renderer.h"

const int TEXTURESIZE = 1024;

QSharedPointer<Camera> Renderer::getCamera()
{
	return camera;
}

void Renderer::initSkyboxTexture(bool isEmpty)
{
	QVector<QImage> faces;
	faces.reserve(6);

	QString keys[6] = { "+x","-x","+y","-y","+z","-z" };
	for (auto& key : keys)
	{
		QImage tex;
		if (!isEmpty) {
			tex = QImage(skybox->pathes[key]);
			tex = tex.convertToFormat(QImage::Format_RGB888).scaled(TEXTURESIZE, TEXTURESIZE);
		}
		else {
			tex =QImage(TEXTURESIZE, TEXTURESIZE, QImage::Format_RGB888);
			tex.fill(Qt::black);
		}
		faces.append(tex);
	}

	initSkyboxTextureFromImages(faces);
}

void Renderer::initSkyboxTextureFromImages(const QVector<QImage>& faces)
{
	functions->glDeleteTextures(1, &envCubemap);
	functions->glGenTextures(1, &envCubemap);

	functions->glActiveTexture(GL_TEXTURE0);
	functions->glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap); //生成立方体截图

	for (int i = 0; i < 6; ++i)
	{
		QImage tex = faces.value(i);
		if (tex.isNull()) {
			tex = QImage(TEXTURESIZE, TEXTURESIZE, QImage::Format_RGB888);
			tex.fill(Qt::black);
		}
		tex = tex.convertToFormat(QImage::Format_RGB888).scaled(TEXTURESIZE, TEXTURESIZE);
		functions->glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, TEXTURESIZE, TEXTURESIZE, 0, GL_RGB, GL_UNSIGNED_BYTE, tex.bits());
	}

	functions->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	functions->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	functions->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	functions->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	functions->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	functions->glBindTexture(GL_TEXTURE_CUBE_MAP, 0); //生成立方体截图
}


Renderer::Renderer(QMap<QString, QOpenGLShaderProgram*> shaderProgram, QOpenGLExtraFunctions* functions, int width, int height):
	shaderProgram(shaderProgram),
	functions(functions),
	width(width),
	height(height),
	camera(Camera::GetInstance()),
	skybox(Skybox::GetInstance()),
	envCubemap(-1)
{
	projection.perspective(camera->getZoom(), width / (float)height, 0.1f, 500.0f);
}

unsigned int Renderer::getEnvMap()
{
	return envCubemap;
}
