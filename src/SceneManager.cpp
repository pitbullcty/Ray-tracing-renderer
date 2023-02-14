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
	sceneManager->clearModels();
	delete sceneManager;
}

void SceneManager::addModel(const QString& path, const QString& modelName)
{
	
	QString newname;
	QString pathSep = path.right(path.size() - path.lastIndexOf('/') - 1);
	QString name = pathSep.left(pathSep.lastIndexOf('.'));
	QString modelLoaded;
	QRegularExpression exp("\\d*$");
	QString maxNum = "-1";

	for (auto it = models.begin(); it != models.end(); it++) {
		QString key = it.key();
		if (key.contains(name)) { //如果包含前缀
			QString num = exp.match(key).captured(0);
			if (num.toInt() > maxNum.toInt()) maxNum = num;
		}
		if (it.value().getPath() == path && modelLoaded.isEmpty()) {
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
	auto loadRes = modelLoader->loadModel(path, models[newname]);
	if (loadRes == RELOADED) {
		models[newname] = models[modelLoaded]; //如果已经加载过则直接复制
		models[newname].transform.reSet(); //重置transform
	}
	else if(loadRes == SUCCESS){
		qDebug() << "模型" + newname + "加载耗时" + QString::number(timer.elapsed(), 'f', 2) + "ms";
		return;
	}
	return;
}

void SceneManager::clearModel(const QString& name)
{
	models[name].destroyTextures();
	models.remove(name);
}

void SceneManager::clearModels()
{
	for (auto it = models.begin(); it != models.end(); it++) {
		it->destroyTextures();
	}
	models.clear();
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

	dealDifference();

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
		for (int i = 0; i < keys.size(); i++)
		{
			QString key = keys.at(i);
			QJsonValue value = obj[key];

			if (value.isArray()) {
				QJsonArray array = value.toArray();
				for (int j = 0; j < array.size(); j++) {
					if (array[j].isObject()) {
						QJsonObject subobj = array[j].toObject();
						if (subobj.contains("modelPath")) {
							QJsonObject transform = subobj["transform"].toObject();
							QString path = subobj["modelPath"].toString();
							QString modelName = subobj["name"].toString();
							addModel(path, modelName);
							models[modelName].prase(transform);
						} //已知为模型类型，解析模型
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
		qDebug() << "场景加载失败！请检查json格式";
	}
	else {
		qDebug() << "场景加载成功！耗时：" + QString::number(timer.elapsed(), 'f', 2) + "ms";
	}
	state = CREATED;
}

bool SceneManager::saveScene()
{

	if (sceneFileName.isEmpty()) {
		QString savePath = QFileDialog::getSaveFileName(nullptr, "选择保存路径", QDir::currentPath(), "场景文件(*.json)");
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
	qDebug() << "场景保存至" + sceneFileName;
	file.close(); //写入json
}

void SceneManager::createScene()
{
	if (!dealDifference()) return;
	sceneFileName = ""; //清除保存文件名
	clearModels();
	modelLoader->clearPathes(); //清除现有模型
	camera->reSet();
	state = CREATED;
}

void SceneManager::closeScene()
{
	if (!dealDifference()) return;
	sceneFileName = ""; //清除保存文件名
	clearModels();
	modelLoader->clearPathes(); //清除现有模型
	camera->reSet();
	state = NONE;
}

bool SceneManager::closeApp()
{
	return dealDifference();
}

SceneManager::SceneManager():
	modelLoader(ModelLoader::GetInstance()),
	camera(Camera::GetInstance()),
	skybox(Skybox::GetInstance()),
	state(NONE)
{
}

QJsonObject SceneManager::toJsonObeject()
{
	QJsonObject json;
	QJsonArray modelsJsons;

	for (auto it = models.begin(); it != models.end(); it++) {
		auto modelJson = it.value().toJson();
		modelJson.insert("name", it.key());
		modelsJsons.append(modelJson);
	}
	json.insert("models", modelsJsons); //保存模型

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
		auto choice = QMessageBox::warning(nullptr, "警告", "当前更改为保存，是否保存？", QMessageBox::Yes | QMessageBox::No);
		if (choice == QMessageBox::Yes) {
			if (!saveScene()) {
				return false;
			}
		}
	}
	return true;
}


