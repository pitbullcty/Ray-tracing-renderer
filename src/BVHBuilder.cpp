#include "BVHBuilder.h"

bool cmpx(const Triangle& t1, const Triangle& t2) {
	return t1.bound.getCenter().x() < t2.bound.getCenter().x();
}
bool cmpy(const Triangle& t1, const Triangle& t2) {
	return t1.bound.getCenter().y() < t2.bound.getCenter().y();
}
bool cmpz(const Triangle& t1, const Triangle& t2) {
	return t1.bound.getCenter().z() < t2.bound.getCenter().z();
}

QSharedPointer<BVHBuilder> BVHBuilder::instance = nullptr;


QSharedPointer<BVHBuilder>& BVHBuilder::GetInstance(QMap<QString, Model>* models)
{
	if (instance.isNull())
		instance = QSharedPointer<BVHBuilder>(new BVHBuilder(models), BVHBuilder::destory);
	return instance;
}

void BVHBuilder::destory(BVHBuilder* builder)
{
	delete builder;
}

void BVHBuilder::buildBVH()
{
	QElapsedTimer timer;
	timer.start();
	for (auto it = models->begin(); it != models->end(); it++) {
		auto& meshes = it.value().getMeshes();
		for (auto& mesh : meshes) {
			buildTriangle(mesh);
			int count = int(log2(mesh.triangles.size())) + 1;
			buildMeshBVH(mesh, 0, mesh.triangles.size() - 1, count);
		}
	}
	Console::Info("BVH建立完成，耗时" + QString::number(timer.elapsed(), 'f', 2) + "ms");
	Console::scroll();
}

int BVHBuilder::buildMeshBVH(Mesh& mesh, int l, int r, int maxCount, int parent, STRATEGY stratgey)
{
	if (l > r) return 0;

	BVHNode node;
	int id = mesh.bvh.nodes.size();
	node.parent = parent;
	node.lChild = node.rChild = node.index = node.n = 0;

	mesh.bvh.nodes.emplace_back(node); //放入bvh树

	int splitIndex = 0;
	DIM maxdim = AXIS_X;
	float cost = std::numeric_limits<float>::max();
	int i = 0;

	for (i = l; i <= r; i++) {
		auto& triangle = mesh.triangles.at(i);
		node.bound.Union(triangle.bound);
	} //计算当前包围盒

	if ((r - l + 1) <= maxCount) {
		mesh.bvh.nodes[id].n = r - l + 1;
		mesh.bvh.nodes[id].index = l;
		mesh.bvh.nodes[id].isLeaf = true;
		mesh.bvh.nodes[id].lChild = mesh.bvh.nodes[id].rChild = -1;
		mesh.bvh.nodes[id].bound = node.bound;
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
			std::sort(mesh.triangles.begin() + l, mesh.triangles.begin() + r + 1, cmpx);
			break;
		case AXIS_Y:
			std::sort(mesh.triangles.begin() + l, mesh.triangles.begin() + r + 1, cmpy);
			break;
		case AXIS_Z:
			std::sort(mesh.triangles.begin() + l, mesh.triangles.begin() + r + 1, cmpz);
			break;
		}
		float newcost = 0.0f;
		int newSplitIndex = findSplitAxis(mesh, l, r, node, newcost);

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

	int left = buildMeshBVH(mesh, l, splitIndex, maxCount,id, stratgey);
	int right = buildMeshBVH(mesh, splitIndex + 1, r, maxCount, id, stratgey);

	mesh.bvh.nodes[id].lChild = left;
	mesh.bvh.nodes[id].rChild = right;
	mesh.bvh.nodes[id].isLeaf = false;

	return id;
}

void BVHBuilder::buildTriangle(Mesh& mesh)
{
	for (auto& index : mesh.indices) {
		if (index < mesh.indices.size() / 3) {
			Triangle tri(mesh.vertices[index / 3].pos, mesh.vertices[index / 3 + 1].pos, mesh.vertices[index + 2].pos);
			mesh.triangles.emplace_back(tri);
		}
	} //计算三角形
}

BVHBuilder::BVHBuilder(QMap<QString, Model>* models)
{
	this->models = models;
}

int BVHBuilder::findSplitAxis(Mesh& mesh, int l, int r, BVHNode& node, float& cost)
{
	int splitIndex = 0;
	float minCost = std::numeric_limits<float>::max(); //最小花费
	cost = 0.0f;
	// 遍历所有可能,寻找最小cost
	for (int index = 0; index <= r - l - 1; index++) {

		// 建立左右包围盒
		AABB left, right;
		for (int i = l; i <= l + index; i++) {
			left.Union(mesh.triangles[i].bound);
		}
		for (int i = l + index + 1; i <= r; i++) {
			right.Union(mesh.triangles[i].bound);
		}

		// 获得表面积和数量
		auto leftArea = left.getSurfaceArea();
		auto rightArea = right.getSurfaceArea();
		auto totalArea = node.bound.getSurfaceArea();

		int leftSize = index + 1;
		int rightSize = r - l - index;
		// 计算cost
		cost = leftArea / totalArea * leftSize + rightArea / totalArea * rightSize;

		if (cost < minCost) {
			minCost = cost;
			splitIndex = l + index;
		}
	}
	return splitIndex;
}

