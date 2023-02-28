#include "SceneManager.h"

QSharedPointer<SceneManager> SceneManager::instance = nullptr;

QSharedPointer<SceneManager>& SceneManager::GetInstance()
{
	if (instance.isNull())
		instance = QSharedPointer<SceneManager>(new SceneManager(), SceneManager::destory);
	return instance;
}

void SceneManager::destory(SceneManager* sceneManager)
{
	sceneManager->modelToCopy = nullptr;
	delete sceneManager;
}

QString SceneManager::addModel(const QString& path, const QString& modelName, bool isCopy, bool isLight)
{
	QString newname;
	QString pathSep = path.right(path.size() - path.lastIndexOf('/') - 1);
	QString name = pathSep.left(pathSep.lastIndexOf('.'));
	QString modelLoaded;
	QRegularExpression exp("\\d*$");
	QString maxNum = "-1";

	if (models.contains(modelName)) {
		return "";
	} //不能加载重名

	for (auto it = models.begin(); it != models.end(); it++) {
		QString key = it.key();
		if (key.contains(name)) { //如果包含前缀
			QString num = exp.match(key).captured(0);
			if (num.toInt() > maxNum.toInt()) maxNum = num;
		}
		auto& model = it.value();
		if (model.getPath() == path && modelLoaded.isEmpty() && !model.isCopy()) {
			modelLoaded = key;
		}
	} //提取最大的编号,并且查找是否有相同路径模型
	
	if (modelName.isEmpty()) {
		if (maxNum.toInt() == -1) {
			newname = name;
		} //没有重复
		else if (maxNum.toInt() + 1 < 10) {
			maxNum = "0" + QString::number(maxNum.toInt() + 1);
			newname = name + maxNum;
		}
		else {
			maxNum = QString::number(maxNum.toInt() + 1);
			newname = name + maxNum;
		}
	} //未指定名字则自动生成
	else {
		newname = modelName;
	} //指定名字则
	
	QElapsedTimer timer;
	timer.start();
	auto loadRes = modelLoader->loadModel(path, models[newname], isLight);
	if (loadRes == RELOADED) {
		models[newname].setCopy(&models[modelLoaded]); //如果已经加载过则直接复制
		if(!isCopy && !isLight)  emit Info("模型" + path + "已加载！"); 
		emit updateList(&models);
	}
	else if (loadRes == SUCCESS) {
		models[newname].updateBound();
		if (!isLight) {
			QString loadModelTime = "模型" + newname + "加载耗时" + QString::number(timer.elapsed(), 'f', 2) + "ms";
			emit Info(loadModelTime);
		}
		else {
			models[newname].setType(LIGHT);
		}
		emit updateList(&models);
	}
	else newname = "";
	return newname;
}

void SceneManager::pasteModel(QPoint pos)
{
	float x = 2.0f * pos.x() / width - 1.0f;
	float y = 1.0f - (2.0f * pos.y() / height);

	QMatrix4x4 projection;
	projection.perspective(camera->getZoom(), width / (float)height, 0.1f, 500.0f);
	QMatrix4x4 view = camera->getView();

	Ray ray(x, y, projection, view);
	ray.direction = (ray.pos - camera->getPos()).normalized(); //构建射线

	pasteModel(camera->getPos() + 50 *ray.direction); //往射线方向移动

}


void SceneManager::copyModel(Model* model)
{
	this->modelToCopy = model;
}

Model* SceneManager::removeModelByName(const QString& name)
{
	auto& model = models[name];
	Model* change = nullptr;
	if (!model.isCopy()) {
		 //用于转移的复制
		for (auto it = models.begin(); it != models.end(); it++) {
			auto& copyModel = it.value();
			if (copyModel.isCopy() && copyModel.getCopy() == &model) { 
				change = &copyModel;
				break;
			} //统计copy数量
		}
		if (change) {
			change->setData(model.getMeshes(), model.getPath(), model.getCenter());
			for (auto it = models.begin(); it != models.end(); it++) {
				auto& copyModel = it.value();
				if (copyModel.getCopy() == &model && &copyModel!=change) {
					copyModel.setCopy(change, false);
				}
			}
		} //如果有物体的复制,转移数据
		else {
			modelLoader->removePath(model.getPath());
			model.destroyTextures(); //可以删除材质
		} //如果没有说明是最后一个物体,清除modelLoader保存信息

	} //如果要删除的是原始数据
	models.remove(name);
	emit updateList(&models);
	emit sendEditModel(nullptr);
	return change;
}

void SceneManager::removeModel(Model* model)
{
	for (auto it = models.begin(); it != models.end(); it++) {
		if (model == &it.value()) { //如果指向同一个物体
			removeModelByName(it.key());
			return;
		}
	}
}

void SceneManager::copyModel(const QString& name)
{
	this->modelToCopy = &models[name];
}

void SceneManager::lookAtModel(const QString& name)
{
	auto& model = models[name];
	QVector3D trans = model.transform.getTranslation();
	camera->setPos(trans + model.getCenter());
	emit sendEditModel(&model);
}

void SceneManager::pasteModel(QVector3D pos)
{
	if (modelToCopy) {
		QString name = addModel(modelToCopy->getPath(),"",true);
		models[name].transform.translationX = pos.x();
		models[name].transform.translationY = pos.y();
		models[name].transform.translationZ = pos.z();
		models[name].transform.calcModel();
		models[name].updateBound();
		emit sendEditModel(&models[name]);
	} //如果有要复制的模型
}


void SceneManager::pasteByName(const QString& name)
{
	copyModel(&models[name]);
	pasteModel(models[name].transform.getTranslation());
}

void SceneManager::rename(const QString& oldname, const QString& newname)
{
	auto model = models[oldname]; //直接复制
	auto change = removeModelByName(oldname);
	if (change) {
		model.getMeshes().clear();
		model.setCopy(change, false);
	} //如果需要
	models.insert(newname, model);
	emit sendEditModel(&models[newname]);
	emit updateList(&models);
}

void SceneManager::clearModels()
{
	for (auto it = models.begin(); it != models.end(); it++) {
		it->destroyTextures();
	}
	models.clear();
}


Model* SceneManager::getSelected(int posx, int posy)
{
	float x = 2.0f * posx / width - 1.0f;
	float y = 1.0f - (2.0f * posy / height);

	QMatrix4x4 projection;
	projection.perspective(camera->getZoom(), width / (float)height, 0.1f, 500.0f);
	QMatrix4x4 view = camera->getView();

	Ray ray(x, y, projection, view);
	ray.direction = (ray.pos - camera->getPos()).normalized(); //构建射线

	Model* selected = nullptr;

	float t = 1e10; //存储最大t值

	for (auto it = models.begin(); it != models.end(); it++) {
		auto& model = it.value();
		float t0 = ray.hitAABB(model.getDectionBound());
		if (t0 != -1) {
			if (t0 < t) {
				t = t0;
				selected = &model;
			}
		} //寻找最近交点
	}

	return selected;
}

void SceneManager::setSize(int width, int height)
{
	this->width = width;
	this->height = height;
}


QSharedPointer<Camera> SceneManager::getCamera()
{
	return camera;
}

QMap<QString, Model>* SceneManager::getModels()
{
	return &models;
}

STATE SceneManager::getState()
{
	return state;
}

void SceneManager::loadScene(const QString& path)
{

	if (!dealDifference()) return;
	emit Clear();

	sceneFileName = path;
	bool res = true;
	clearModels();
	modelLoader->clearPathes(); //清除现有模型

	QElapsedTimer timer;
	timer.start();

	QFile file(path);
	file.open(QFile::ReadOnly);
	QByteArray all = file.readAll();
	file.close(); //读取json文件

	QJsonDocument doc = QJsonDocument::fromJson(all);//转换成文档对象

	if (doc.isObject()) {
		QJsonObject obj = doc.object(); //得到Json对象
		QStringList keys = obj.keys(); //得到所有key
		for (int i = 0; i < keys.size() && res; i++)
		{
			QString key = keys.at(i);
			QJsonValue value = obj[key];

			if (value.isArray()) {
				QJsonArray array = value.toArray();
				for (int j = 0; j < array.size(); j++) {
					if (array[j].isObject()) {
						QJsonObject subobj = array[j].toObject();
						QJsonObject modelMaterial = subobj["material"].toObject();
						QJsonObject transform = subobj["transform"].toObject();
						QString path = subobj["modelPath"].toString();
						QString modelName = subobj["name"].toString();
						QString loadRes;
						if(key=="lights")
							loadRes = addModel(path, modelName, false, true);
						else
							loadRes = addModel(path, modelName);

						if (loadRes.isEmpty()) {
							res = false;
							break;
						}
						models[modelName].prase(transform, modelMaterial);
					}
					else {
						res = false;
						break;
					}
				}
			}

			else if (value.isObject()) {
				QJsonObject subobj = value.toObject();
				if (subobj.contains("x")) {
					camera->prase(subobj);
				} //解析相机
				else if (subobj.contains("+x")) {
					skybox->prase(subobj);
				}
				else {
					res = false;
					break;
				}
				
			}

			else {
				res = false;
				break;
			}

		}
	}
	else {
		res = false;
	}
	if (!res) {
		emit Error("场景加载失败！请检查json格式");
		clearModels();
		modelLoader->clearPathes(); //清除现有模型
		camera->reSet();
		state = NONE;
		return;
	}
	else {
		QString loadSceneTime = "场景加载成功！耗时：" + QString::number(timer.elapsed(), 'f', 2) + "ms";
		emit Info(loadSceneTime);
	}
	state = CREATED;
}

bool SceneManager::saveScene()
{

	if (sceneFileName.isEmpty()) {
		QFileDialog dialog;
		dialog.setWindowIcon(QIcon(":icons/title.ico"));
		QString savePath = dialog.getSaveFileName(nullptr, "选择保存路径", QDir::currentPath(), "场景文件(*.json)");
		if (!savePath.isEmpty())
			sceneFileName = savePath;
		else
			return false;
	} //如果未打开文件,则选择文件名
	
	QJsonDocument doc(toJsonObeject());
	QFile file(sceneFileName);
	file.open(QFile::WriteOnly);
	file.write(doc.toJson());
	file.close(); //写入json
	emit Info("场景保存至" + sceneFileName);
	qDebug() << "场景保存至" + sceneFileName;
	return true;
}

void SceneManager::saveSceneAs(const QString& path)
{
	sceneFileName = path;
	QJsonDocument doc(toJsonObeject());
	QFile file(sceneFileName);
	file.open(QFile::WriteOnly);
	file.write(doc.toJson());
	emit Info("场景保存至" + sceneFileName);
	qDebug() << "场景保存至" + sceneFileName;
	file.close(); //写入json
}

void SceneManager::createScene()
{
	if (!dealDifference()) return;
	emit Clear();
	sceneFileName = ""; //清除保存文件名
	clearModels();
	modelLoader->clearPathes(); //清除现有模型
	camera->reSet();
	emit updateList(&models);
	state = CREATED;
}

void SceneManager::closeScene()
{
	if (!dealDifference()) return;
	emit Clear();
	sceneFileName = ""; //清除保存文件名
	clearModels();
	modelLoader->clearPathes(); //清除现有模型
	camera->reSet();
	emit updateList(&models);
	state = NONE;
}

bool SceneManager::closeApp()
{
	return dealDifference();
}

SceneManager::SceneManager() :
	modelLoader(ModelLoader::GetInstance()),
	camera(Camera::GetInstance()),
	skybox(Skybox::GetInstance()),
	width(0),height(0),
	state(NONE),
	modelToCopy(nullptr)
{
};

QJsonObject SceneManager::toJsonObeject()
{
	QJsonObject json;
	QJsonArray modelsJsons;
	QJsonArray LightsJsons;

	for (auto it = models.begin(); it != models.end(); it++) {
		auto& model = it.value();
		auto modelJson = it.value().toJson();
		modelJson.insert("name", it.key());
		if (model.getType() == NORMAL) {
			modelsJsons.append(modelJson);
		}
		else {
			LightsJsons.append(modelJson);
		}
	}
	json.insert("models", modelsJsons); //保存模型
	json.insert("lights", LightsJsons); //保存灯光

	json.insert("camera", camera->toJson()); //保存相机参数
	json.insert("skybox", skybox->toJson()); //保存天空盒

	return json;
}


bool SceneManager::dealDifference()
{
	bool isDifference = false;
	while (true)
	{
		if (state == NONE) {
			return true;
			break; //刚开始新建不需要检查
		}

		QJsonDocument doc(toJsonObeject());
		QString newJson(doc.toJson());

		if (sceneFileName.isEmpty()) {
			isDifference = true;
			break;
		}

		QFile file(sceneFileName);
		file.open(QFile::ReadOnly);
		QString oldJson(file.readAll());

		if (oldJson == newJson) {
			break;
		}
		else {
			isDifference = true;
			break;
		}
	}
	if (isDifference) {
		QIcon icon(":icons/title.ico");
		QMessageBox msgBox(QMessageBox::Warning, "警告", "当前更改为保存，是否保存？", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		msgBox.setWindowIcon(icon);
		auto res = msgBox.exec();
		if(res == QMessageBox::Yes){
			if (!saveScene()) {
				return false;
			}
		}
		else if(res == QMessageBox::Cancel){
			return false;
		}
		else {
			;
		}
	}
	return true;
}


