#include "DataBuilder.h"

bool cmpx(const Triangle& t1, const Triangle& t2) {
	return t1.getCenter().x() < t2.getCenter().x();
}
bool cmpy(const Triangle& t1, const Triangle& t2) {
	return t1.getCenter().y() < t2.getCenter().y();
}
bool cmpz(const Triangle& t1, const Triangle& t2) {
	return t1.getCenter().z() < t2.getCenter().z();
}  //按照三角形中心排序

QSharedPointer<DataBuilder> DataBuilder::instance = nullptr;


QSharedPointer<DataBuilder>& DataBuilder::GetInstance()
{
	if (instance.isNull())
		instance = QSharedPointer<DataBuilder>(new DataBuilder(), DataBuilder::destory);
	return instance;
}

void DataBuilder::destory(DataBuilder* builder)
{
	delete builder;
}

void DataBuilder::buildData(bool needTips) {

	if (models->empty()) {
		data.clear();
		emit sendDataDone();
		return;
	} //模型为空则直接清空
	QElapsedTimer timer;
	timer.start();
	if(needTips) emit Info("正在构建BVH...");
	flattenAndBuildData(); //展平数据并且构建BVH
	int maxcount = int(log2(triangles.size())) + 1;
	buildBVHHelp(0, triangles.size() - 1, maxcount, -2, MAXDIM);
	encodeData();
	emit sendDataDone();
	if (needTips) emit Info("BVH建立完成，耗时" + QString::number(timer.elapsed(), 'f', 2) + "ms");
}

int DataBuilder::buildBVHHelp(int l, int r, int maxCount, int parent, STRATEGY stratgey)
{
	if (l > r) return 0;

	BVHNode node;
	bvh.emplace_back(node); //放入bvh树
	int id = bvh.size()-1;
	bvh[id].parent = parent;
	bvh[id].lChild = bvh[id].rChild = bvh[id].index = bvh[id].n = 0;

	int splitIndex = 0;
	DIM maxdim = AXIS_X;
	DIM best = AXIS_X;
	float cost = std::numeric_limits<float>::max();
	int i = 0;

	for (i = l; i <= r; i++) {
		auto& triangle = triangles.at(i);
		bvh[id].bound.Union(triangle.bound);
	} //计算当前包围盒
	

	if ((r - l + 1) <= maxCount) {
		bvh[id].n = r - l + 1;
		bvh[id].index = l;
		bvh[id].isLeaf = true;
		bvh[id].lChild = bvh[id].rChild = -1;
		return id;
	} //当前已经为叶子节点

	for (i = 0; i < 3; i++) {

		if (stratgey == MAXDIM) {
			maxdim = bvh[id].bound.getMaxDim(); //获取当前bound最长轴
		}
		else {
			maxdim = (DIM)i;
		}
		switch (maxdim) {
		case AXIS_X:
			std::sort(triangles.begin() + l, triangles.begin() + r + 1, cmpx);
			break;
		case AXIS_Y:
			std::sort(triangles.begin() + l, triangles.begin() + r + 1, cmpy);
			break;
		case AXIS_Z:
			std::sort(triangles.begin() + l, triangles.begin() + r + 1, cmpz);
			break;
		}
		float newcost = 0.0f;
		int newSplitIndex = findSplitAxis(l, r, newcost);

		if (stratgey == MAXDIM) {
			splitIndex = newSplitIndex;
			break; //最长轴策略,按照最长轴将包围盒排序 
		}
		else {
			if (newcost < cost) {
				splitIndex = newSplitIndex;
				cost = newcost;
				best = maxdim;
			}
		} //三轴遍历寻找
	}
	
	if (stratgey == ALLAXIS) {
		if (best == AXIS_X) std::sort(triangles.begin() + l, triangles.begin() + r + 1, cmpx);
		if (best == AXIS_Y) std::sort(triangles.begin() + l, triangles.begin() + r + 1, cmpy);
		if (best == AXIS_Z) std::sort(triangles.begin() + l, triangles.begin() + r + 1, cmpz);
	}


	int left = buildBVHHelp(l, splitIndex, maxCount, id, stratgey);
	int right = buildBVHHelp(splitIndex + 1, r, maxCount, id, stratgey);

	bvh[id].lChild = left;
	bvh[id].rChild = right;
	bvh[id].isLeaf = false;

	return id;
}

void DataBuilder::flattenAndBuildData()
{
	int meshindex = 0;
	int modelIndex = 0;
	if (!triangles.isEmpty()) triangles.clear();
	if (!bvh.isEmpty()) bvh.clear();
	bvh.emplace_back(BVHNode());
	if (!texInfo.isEmpty()) texInfo.clear();
	if (!materialsFlatten.isEmpty()) materialsFlatten.clear(); //清空原有数据
	for (auto it = models->begin(); it != models->end(); it++) {
		auto& model = it.value();
		auto modelMatrix = model.transform.getModel();
		if (model.isLight()) {
			auto& bound = model.getDectionBound();
			auto& maxpos = bound.maxpos;
			auto& minpos = bound.minpos;
			LightEncoded light;
			light.position = modelMatrix.map(model.getCenter());
			light.emission = model.lightMaterial.emissive;
			light.u = maxpos;
			light.v = minpos;
			if (model.getType() == SPHERELIGHT) {
				float radius = (maxpos.x() - minpos.x()) / 2;
				light.param.setX(1);
				light.param.setY(radius);
			}
			else {
				light.param.setX(0);
			}
			data.encodedLight.push_back(light);
			continue;
		} //对光源单独编码
		QVector<QSharedPointer<Mesh>> meshes;
		if (model.isCopy()) {
			meshes = model.getCopy()->getMeshes();
		}
		else meshes = model.getMeshes();
		for (auto& mesh : meshes) {
			QVector<QPair<QString, int>> temp;
			for (int i = 0; i < mesh->indices.size() / 3; i++) {
				auto a = mesh->vertices[mesh->indices[i * 3]];
				auto b = mesh->vertices[mesh->indices[i * 3 + 1]];
				auto c = mesh->vertices[mesh->indices[i * 3 + 2]];
				Triangle tri(a.map(modelMatrix), b.map(modelMatrix),c.map(modelMatrix),
					meshindex, modelIndex);
				triangles.emplace_back(tri);
			}
			for (auto& texture : mesh->textures) {
				if (!model.isCopy()) textureImageFlatten.emplace_back(QImage(texture.path).convertToFormat(QImage::Format_RGBA8888)); //不为复制则直接复制路径便于加载,同时转换格式
				temp.emplace_back(std::make_pair(texture.type, texture.id));
			}
			texInfo.emplace_back(temp);
			meshindex++; //meshID增加
		}
		materialsFlatten.emplace_back(model.modelMaterial);
		modelIndex++;
	}
}

void DataBuilder::setModels(QMap<QString, Model>* models)
{
	this->models = models;
}

RenderData& DataBuilder::getData()
{
	return data;
}

void DataBuilder::encodeData()
{
	data.clear();
	for (auto& triangle : triangles) {
		TriangleEncoded tri;

		int meshIndex = triangle.meshIndex; //获取纹理索引
		int modelIndex = triangle.modelIndex; //获取顶点索引

		tri.a = triangle.a.pos;
		tri.b = triangle.b.pos;
		tri.c = triangle.c.pos;
		
		tri.n1 = triangle.a.normal;
		tri.n2 = triangle.b.normal;
		tri.n3 = triangle.c.normal;

		data.encodedTriangles.emplace_back(tri);

		auto& material = materialsFlatten.at(modelIndex); //获取材质
		auto& info = texInfo.at(meshIndex);
		auto encoded = material.encode();

		for (auto& texture : info) {
			if (texture.first == "texture_diffuse") encoded.param4.setY(texture.second);
			if (texture.first == "texture_metalness") encoded.param4.setZ(texture.second);
			if (texture.first == "texture_normal") encoded.param5.setX(texture.second);
			if (texture.first == "texture_emissive") encoded.param5.setY(texture.second);
		} //设置材质类型
		data.encodedMaterials.emplace_back(encoded);
	}  //编码材质，三角形

	int texBytes = Texturesize * Texturesize * 4; //贴图大小
	data.encodedTexture.resize(texBytes * textureImageFlatten.size());
	int i = 0;
	for (auto& texture : textureImageFlatten)
	{
		int texWidth = texture.width();
		int texHeight = texture.height();
		if (texWidth != Texturesize || texHeight != Texturesize)
		{
			texture = texture.scaled(Texturesize, Texturesize); //改变图片大小成缓冲区大小
		}
		std::copy(texture.constBits(), texture.constBits() + texBytes, &data.encodedTexture[i * texBytes]);
		i++;
	}

	for (auto& node:bvh) {
		BVHNodeEncoded encodeNode;
		encodeNode.childs = QVector3D(node.lChild, node.rChild, 0);
		encodeNode.leafInfo = QVector3D(node.n, node.index, 0);
		encodeNode.AA = node.bound.minpos;
		encodeNode.BB = node.bound.maxpos; //AA为小点
		data.encodedBVH.emplace_back(encodeNode);
	} //编码

	textureImageFlatten.clear();
	texInfo.clear();
	materialsFlatten.clear();
	bvh.clear(); //释放临时内存
}

DataBuilder::DataBuilder():models(nullptr)
{
	;
}

int DataBuilder::findSplitAxis(int l, int r, float& cost)
{
	int splitIndex = 0;
	float minCost = std::numeric_limits<float>::max(); //最小花费
	cost = 0.0f;
	// 遍历所有可能,寻找最小cost

	QVector<AABB> leftMax(r-l+1);
	QVector<AABB> rightMax(r-l+1); //存放左右两边最大包围盒

	for (qsizetype i = l; i <= r; i++) {
		if (i - l == 0)
			leftMax[i - l].Union(triangles.at(i).bound);
		else
			leftMax[i - l].Union(leftMax.at(i - l - 1)).Union(triangles.at(i).bound);
	}
	for (qsizetype i = r; i >= l; i--) {
		if (r-i == 0)
			rightMax[i-l].Union(triangles.at(i).bound);
		else
			rightMax[i-l].Union(rightMax.at(i - l + 1)).Union(triangles.at(i).bound);
		auto tri = triangles.at(i);
	} //利用前缀和优化

	for (qsizetype i = l; i < r; i++) {
		// 建立左右包围盒

		AABB left = leftMax.at(i-l);
		AABB right = rightMax.at(i+1-l);

		// 获得表面积和数量
		auto leftArea = left.getSurfaceArea();
		auto rightArea = right.getSurfaceArea();

		int leftSize = i - l + 1;
		int rightSize = r - i;
		// 计算cost
		cost = leftArea  * leftSize + rightArea  * rightSize;

		if (cost < minCost) {
			minCost = cost;
			splitIndex = i;
		}
	}
	cost = minCost;
	return splitIndex;
}

