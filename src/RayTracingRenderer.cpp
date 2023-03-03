#include "RayTracingRenderer.h"

bool cmpx(const Triangle& t1, const Triangle& t2) {
	return t1.bound.getCenter().x() < t2.bound.getCenter().x();
}
bool cmpy(const Triangle& t1, const Triangle& t2) {
	return t1.bound.getCenter().y() < t2.bound.getCenter().y();
}
bool cmpz(const Triangle& t1, const Triangle& t2) {
	return t1.bound.getCenter().z() < t2.bound.getCenter().z();
}

QSharedPointer<RayTracingRenderer> RayTracingRenderer::instance = nullptr;


QSharedPointer<RayTracingRenderer>& RayTracingRenderer::GetInstance()
{
	if (instance.isNull())
		instance = QSharedPointer<RayTracingRenderer>(new RayTracingRenderer(), RayTracingRenderer::destory);
	return instance;
}

void RayTracingRenderer::destory(RayTracingRenderer* builder)
{
	delete builder;
}

void RayTracingRenderer::buildBVH() {

	if (models->empty()) return; //没有模型直接返回
	QElapsedTimer timer;
	timer.start();
	emit Info("正在构建BVH...");
	buildTriangles();
	int maxcount = int(log2(triangles.size())) + 1;
	buildBVHHelp(0, triangles.size() - 1, maxcount);
	emit Info("BVH建立完成，耗时" + QString::number(timer.elapsed(), 'f', 2) + "ms");
}

int RayTracingRenderer::buildBVHHelp(int l, int r, int maxCount, int parent, STRATEGY stratgey)
{
	if (l > r) return 0;

	BVHNode node;
	int id = bvh.nodes.size();
	node.parent = parent;
	node.lChild = node.rChild = node.index = node.n = 0;

	bvh.nodes.emplace_back(node); //放入bvh树

	int splitIndex = 0;
	DIM maxdim = AXIS_X;
	float cost = std::numeric_limits<float>::max();
	int i = 0;


	for (i = l; i <= r; i++) {
		auto& triangle = triangles.at(i);
		node.bound.Union(triangle.bound);
	} //计算当前包围盒

	if ((r - l + 1) <= maxCount) {
		bvh.nodes[id].n = r - l + 1;
		bvh.nodes[id].index = l;
		bvh.nodes[id].isLeaf = true;
		bvh.nodes[id].lChild = bvh.nodes[id].rChild = -1;
		bvh.nodes[id].bound = node.bound;
		return id;
	} //当前已经为叶子节点

	for (i = 0; i < 3; i++) {

		if (stratgey == MAXDIM) {
			maxdim = node.bound.getMaxDim(); //获取当前bound最长轴
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
		int newSplitIndex = findSplitAxis(l, r, node, newcost);

		if (stratgey == MAXDIM) {
			splitIndex = newSplitIndex;
			break; //最长轴策略,按照最长轴将包围盒排序 
		}
		else {
			if (newcost < cost) {
				splitIndex = newSplitIndex;
				cost = newcost;
			}
		} //三轴遍历寻找
	}

	int left = buildBVHHelp(l, splitIndex, maxCount, id, stratgey);
	int right = buildBVHHelp(splitIndex + 1, r, maxCount, id, stratgey);

	bvh.nodes[id].lChild = left;
	bvh.nodes[id].rChild = right;
	bvh.nodes[id].isLeaf = false;

	return id;
}

void RayTracingRenderer::buildTriangles()
{
	auto modelCopy = *models; //复制数据，避免冲突
	if (!triangles.isEmpty()) triangles.clear();
	if (!bvh.nodes.isEmpty()) bvh.nodes.clear(); //清空原有数据
	for (auto it = modelCopy.begin(); it != modelCopy.end(); it++) {
		auto& model = it.value();
		if (model.getType() == LIGHT) return; //对于光源不进行处理
		auto modelMatrix = model.transform.getModel();
		QVector<QSharedPointer<Mesh>> meshes;
		if (model.isCopy()) {
			meshes = model.getCopy()->getMeshes();
		}
		else meshes = model.getMeshes();
		for (auto& mesh : meshes) {
			for (int i = 0; i < mesh->indices.size() / 3; i++) {
				Triangle tri(modelMatrix.map(mesh->vertices[mesh->indices.at(i*3)].pos), modelMatrix.map(mesh->vertices[mesh->indices.at(i*3+1)].pos), modelMatrix.map(mesh->vertices[mesh->indices.at(i*3+2)].pos));
				triangles.emplace_back(tri);
			}
		}
	}
}


void RayTracingRenderer::setModels(QMap<QString, Model>* models)
{
	this->models = models;
}

RayTracingRenderer::RayTracingRenderer():models(nullptr)
{

}

int RayTracingRenderer::findSplitAxis(int l, int r, BVHNode& node, float& cost)
{
	int splitIndex = 0;
	float minCost = std::numeric_limits<float>::max(); //最小花费
	cost = 0.0f;

	// 遍历所有可能,寻找最小cost

	QVector<AABB> leftMax(r-l+1);
	QVector<AABB> rightMax(r-l+1); //存放左右两边最大包围盒

	for (qsizetype i = l; i <= r; i++) {
		if (i - l == 0)
			leftMax[i - l].Union(triangles.at(i - l).bound);
		else
			leftMax[i - l].Union(leftMax.at(i - l - 1)).Union(triangles.at(i - l).bound);
	}

	for (qsizetype i = r; i >= l; i--) {
		if (r-i == 0)
			rightMax[i-l].Union(triangles.at(i - l).bound);
		else
			rightMax[i-l].Union(rightMax.at(i - l + 1)).Union(triangles.at(i - l).bound);
	} //利用前缀和优化

	for (qsizetype i = l; i < r; i++) {
		// 建立左右包围盒

		AABB left = leftMax.at(i-l);
		AABB right = rightMax.at(i-l);

		// 获得表面积和数量
		auto leftArea = left.getSurfaceArea();
		auto rightArea = right.getSurfaceArea();
		auto totalArea = node.bound.getSurfaceArea();

		int leftSize = i - l + 1;
		int rightSize = r - i;
		// 计算cost
		cost = leftArea / totalArea * leftSize + rightArea / totalArea * rightSize;

		if (cost < minCost) {
			minCost = cost;
			splitIndex = i;
		}
	}
	return splitIndex;
}

