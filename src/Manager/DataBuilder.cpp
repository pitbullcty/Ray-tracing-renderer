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

void DataBuilder::buildData(bool needTips, bool needRebuild, bool needSend) {

	if (models->empty()) {
		data.clear();
		emit sendDataDone(false);
		return;
	} //模型为空则直接清空

	QElapsedTimer timer;
	timer.start();
	if (needTips) emit Info("正在构建BVH...");

	flattenAndBuildData(needRebuild);

	if (needRebuild) {
		int maxcount = int(log2(triangles.size())) + 1;
		buildBVHHelp(0, triangles.size() - 1, maxcount, -2, MAXDIM);
	}  //展平数据并且构建BVH

	encodeData(needRebuild);

	emit sendDataDone(needSend);
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

void DataBuilder::flattenAndBuildData(bool needRebuild)
{
	int meshindex = 0;
	int modelIndex = 0;

	if (needRebuild) {
		triangles.clear();
		bvh.clear();
		bvh.emplace_back(BVHNode());
		texInfo.clear();
		textureIndex.clear();
		modelMaterialsFlatten.clear();
		data.clear(); //清空相关数据
	}
	modelMaterialsFlatten.clear();

	if (models->empty()) return;

	for (auto it = models->begin(); it != models->end(); it++) {
		auto& model = it.value();
		
		if (needRebuild) {
			auto modelMatrix = model.transform.getModel();
			QVector<QSharedPointer<Mesh>> meshes;
			if (model.isCopy()) {
				meshes = model.getCopy()->getMeshes();
			}
			else meshes = model.getMeshes();
			for (auto& mesh : meshes) {
				for (int i = 0; i < mesh->indices.size() / 3; i++) {
					auto a = mesh->vertices[mesh->indices[i * 3]];
					auto b = mesh->vertices[mesh->indices[i * 3 + 1]];
					auto c = mesh->vertices[mesh->indices[i * 3 + 2]];
					Triangle tri(a.map(modelMatrix), b.map(modelMatrix), c.map(modelMatrix),
						meshindex, modelIndex);
					triangles.emplace_back(tri);
				} //建立三角形
				QVector<QPair<QString, int>> temp;
				for (auto& texture : mesh->textures) {
					if (!model.isCopy()) {
						QImage tex(texture.path);
						tex = tex.convertToFormat(QImage::Format_RGBA8888);
						int texWidth = tex.width();
						int texHeight = tex.height();
						if (texWidth != Texturesize || texHeight != Texturesize) {
							tex = tex.scaled(Texturesize, Texturesize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation); //改变图片大小
						}
						data.encodedTexture.emplace_back(tex);
						int index = data.encodedTexture.size() - 1;
						textureIndex.insert(texture.id, index);
						temp.emplace_back(qMakePair<>(texture.type, index));
					}
					else {
						int index = textureIndex[texture.id];
						temp.emplace_back(qMakePair<>(texture.type, index));
					}

				}
				texInfo.insert(meshindex, temp);
				meshindex++; //meshID增加
			}
		} //重建所有数据
		
		if (model.isLight()) modelMaterialsFlatten.emplace_back(model.lightMaterial.toModelMaterial());
		else modelMaterialsFlatten.emplace_back(model.modelMaterial);
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

void DataBuilder::encodeData(bool needRebuild)
{
	if (!needRebuild) data.encodedMaterials.clear();

	for (auto& triangle : triangles) {
		TriangleEncoded tri;

		int meshIndex = triangle.meshIndex; //获取纹理索引
		int modelIndex = triangle.modelIndex; //获取顶点索引

		auto& material = modelMaterialsFlatten.at(modelIndex); //获取材质
		auto& infos = texInfo[meshIndex];
		auto encoded = material.encode();

		for (auto& info : infos) {
			if (info.first == "texture_diffuse") encoded.param4.setY(info.second);
			if (info.first == "texture_metalness") encoded.param4.setZ(info.second);
			if (info.first == "texture_normal") encoded.param5.setX(info.second);
			if (info.first == "texture_emissive") encoded.param5.setY(info.second);
		} //设置材质类型
		data.encodedMaterials.emplace_back(encoded);

		if (needRebuild) {
			tri.a = triangle.a.pos;
			tri.b = triangle.b.pos;
			tri.c = triangle.c.pos;

			tri.n1 = triangle.a.normal;
			tri.n2 = triangle.b.normal;
			tri.n3 = triangle.c.normal;

			tri.texcoord1 = { triangle.a.texCoords, -1.0f };
			tri.texcoord2 = { triangle.b.texCoords, -1.0f };
			tri.texcoord3 = { triangle.c.texCoords, -1.0f };

			data.encodedTriangles.emplace_back(tri);
		}

	}  //编码材质，三角形

	if (!needRebuild) return; //不需要重建则直接返回

	for (auto& node:bvh) {
		BVHNodeEncoded encodeNode;
		encodeNode.childs = QVector3D(node.lChild, node.rChild, 0);
		encodeNode.leafInfo = QVector3D(node.n, node.index, 0);
		encodeNode.AA = node.bound.minpos;
		encodeNode.BB = node.bound.maxpos; //AA为小点
		data.encodedBVH.emplace_back(encodeNode);
	} //编码

}

DataBuilder::DataBuilder():
	models(nullptr)
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

