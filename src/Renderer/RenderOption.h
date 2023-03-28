#ifndef __RENDER__OPENTION__
#define __RENDER__OPENTION__

#include <QDir>

struct RenderOption
{
	unsigned int frameCounter;
	unsigned int maxFrameCounter;
	int depth;
	bool isRealTimeDenoising;
	int denoiserStep;
	int resolutionX;
	int resolutionY;
	int quality;
	QString outputPath;

	void reset() {
		isRealTimeDenoising = false;
		outputPath = QDir::currentPath() + "/outputs/default.png";
		frameCounter = 0;
		denoiserStep = 40;
		maxFrameCounter = 200;
		resolutionX = 1280;
		resolutionY = 720;
		quality = 50;
		depth = 2;
	}

	RenderOption()
	{
		reset();
		QDir dir;
		if (!dir.exists(QDir::currentPath() + "/outputs")){
			dir.mkdir(QDir::currentPath() + "/outputs");
		}
	}

};

#endif // !__RENDER_OPENTION__
