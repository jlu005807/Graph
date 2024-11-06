#pragma once
#include<iostream>
#include<memory>
#include<functional>
#include<queue>
#include<vector>

//邻接多重表 (Adjacency Multilist) 是**无向图**的另一种链式存储结构。
//在邻接多重表中，每一条边用一个结点表示

//边结点
//mark为标志域，可用以标记该条边是否被搜索过
//ivex和jvex为该边依附的两个顶点在图中的位置
//ilink指向下一条依附顶点ivex的边，jlink指向下一条依附顶点jvex的边
//info为指向和边相关的各种信息的指针域
template<class T>
class EdgeBox
{
public:
	//访问标记
	bool mark;

	//该边依附的两个顶点的位置
	int ivex;
	int jvex;

	//指向依附这两个顶点的下一条边
	std::shared_ptr<EdgeBox<T>> ilink;

	std::shared_ptr<EdgeBox<T>> jlink;

	//边信息
	T info;

	EdgeBox() = default;

	//默认无信息
	EdgeBox(int i,int j, std::shared_ptr<EdgeBox<T>> ilink=nullptr, std::shared_ptr<EdgeBox<T>> jlink=nullptr):mark(false),ivex(i),jvex(j),ilink(ilink),jlink(jlink){ }
};


//顶点
//data域存储和该顶点相关的信息
//firstedge域指示第一条依附于该顶点的边
template<typename K=int,typename T=int>
class Vexnode
{
public:
	//顶点信息，默认为表里的序号
	K data;

	//指向第一条依附该顶点的边
	std::shared_ptr<EdgeBox<T>> firstedge;

	Vexnode() = default;

	Vexnode(K d, std::shared_ptr<EdgeBox<T>> first=nullptr):data(d),firstedge(first){ }


	void Add_iEdge(std::shared_ptr<EdgeBox<T>> edge)
	{
		std::shared_ptr<EdgeBox<T>> p = firstedge;

		firstedge = edge;

		firstedge->ilink = p;
	}

	void Add_jEdge(std::shared_ptr<EdgeBox<T>> edge)
	{
		std::shared_ptr<EdgeBox<T>> p = firstedge;

		firstedge = edge;

		firstedge->jlink = p;
	}
};


//图的表
template<typename K=int ,typename T=int>
class AdjMul_List
{
public:
	//表头结点表，此vector.size()即顶点数,所以不需要记录顶点数
	std::vector<Vexnode<K, T>> vertices;

	//记录边数
	int arcnum;

	//无data的顶点,默认data为序号
	AdjMul_List(int vexnum, int arcnum) :arcnum(arcnum)
	{
		//初始化arcnum
		vertices.resize(vexnum);
		for (int i = 0; i < vexnum; i++)
		{
			vertices[i].data = i;
		}
	}

	//需要data的顶点
	AdjMul_List(int vexnum, int arcnum, K datas[]) :arcnum(arcnum)
	{
		//初始化arcnum
		vertices.resize(vexnum);
		for (int i = 0; i < vexnum; i++)
		{
			vertices[i].data = datas[i];
		}
	}

};


//管理类
template<class K = int, class T = int>
class AdjMulList_manager
{
public:
	//构造n个顶点，m条边的图
	AdjMul_List<K, T> CreateGraph(int n, int m, int edge[][2])
	{
		//初始化
		AdjMul_List<K, T> list(n, m);

		//加入边的信息，此处使用头插法不考虑edge中的位置在List中的稳定性
		for (int i = 0; i < m; i++)
		{
			//确定弧
			int u = edge[i][0];
			int v = edge[i][1];

			//构建arc
			std::shared_ptr<EdgeBox<T>> arc(new EdgeBox<T>(u,v));

			//添加i顶点的边
			list.vertices[u].Add_iEdge(arc);

			//添加j顶点的边
			list.vertices[v].Add_jEdge(arc);

		}

		return list;
	}

	//构造n个顶点，m条边的带权图
	AdjMul_List<K, T> CreateGraph(int n, int m, int edge[][2],std::vector<int> weights)
	{
		//初始化
		AdjMul_List<K, T> list(n, m);

		//加入边的信息，此处使用头插法不考虑edge中的位置在List中的稳定性
		for (int i = 0; i < m; i++)
		{
			//确定弧
			int u = edge[i][0];
			int v = edge[i][1];

			//构建arc
			std::shared_ptr<EdgeBox<T>> arc(new EdgeBox<T>(u, v));

			arc->info = weights[i];

			//添加i顶点的边
			list.vertices[u].Add_iEdge(arc);

			//添加j顶点的边
			list.vertices[v].Add_jEdge(arc);

		}

		return list;
	}

	//若图G存在，销毁图G。
	void DestroyGraph(AdjMul_List<K, T>& list)
	{
		//空图
		if (list.vertices.size() == 0)
		{
			return;
		}

		for (auto& vex : list.vertices)
		{
			// 遍历释放依附于当前顶点的所有边
			std::shared_ptr<EdgeBox<T>> edge = vex.firstedge;

			while (edge != nullptr)
			{
				// 保留下一个指针
				std::shared_ptr<EdgeBox<T>> nextEdge = (edge->ivex == vex.data) ? edge->ilink : edge->jlink;

				// 重置当前边指针，释放内存
				edge.reset();

				// 移动到下一条边
				edge = nextEdge;
			}

			// 清除顶点的 firstedge 指针
			vex.firstedge.reset();
		}

		//清理顶点表
		list.vertices.clear();

		list.arcnum = 0;

		return;
	}

	//返回v的第一个邻接顶点的序号。若v在G中没有邻接顶点，则返回 “空”。
	int FirstAdjVex(AdjMul_List<K, T>& list, int v)
	{
		//空图或者v不合法
		if (list.vertices.size() == 0 || v < 0 || v >= list.vertices.size())
		{
			return -1;
		}

		//没有邻接点
		if (!list.vertices[v].firstedge)
		{
			return -1;
		}

		return list.vertices[v].firstedge.jvex;
	}

	//返回v的（相对千w的）下一个邻接顶点。若w是v的最后一个邻接点，则返回 “空”。
	int NextAdjVex(AdjMul_List<K, T>& list, int v, int w)
	{
		//空图或者v不合法或者w不合法
		if (list.vertices.size() == 0 || v < 0 || v >= list.vertices.size() || w < 0 || w >= list.vertices.size())
		{
			return -1;
		}

		std::shared_ptr<EdgeBox<T>> arc = list.vertices[v].firstedge;

		//找到w
		while (arc->ilink&& arc->jvex != w)
		{
			arc = arc->ilink;
		}


		if (arc)
		{
			return arc->ilink->jvex;
		}
		else
		{
			return -1;
		}

	}

	//不考虑增删顶点


	//在G中删除弧<v, w>
	void DeleteArc(AdjMul_List<K, T>& list, int v, int w)
	{
		//空图或者v不合法或者w不合法
		if (list.vertices.size() == 0 || v < 0 || v >= list.vertices.size() || w < 0 || w >= list.vertices.size())
		{
			return;
		}

		
		// 释放依附顶点v的边
		auto freeivex = [&](std::shared_ptr<EdgeBox<T>>& firstEdge, int k) -> void {
			// 如果首边就是要删除的边
			if (firstEdge && firstEdge->jvex == k) {
				firstEdge = firstEdge->ilink;  // 更新首边指针
				return;
			}

			// 遍历寻找并删除边
			std::shared_ptr<EdgeBox<T>> arc = firstEdge;
			while (arc && arc->ilink) {
				if (arc->ilink->jvex == k) {
					arc->ilink = arc->ilink->ilink;  // 删除中间节点
					return;
				}
				arc = arc->ilink;
			}
		};

		// 释放依附顶点w的边
		auto freejvex = [&](std::shared_ptr<EdgeBox<T>>& firstEdge, int k) -> void {
			// 如果首边就是要删除的边
			if (firstEdge && firstEdge->ivex == k) {
				firstEdge = firstEdge->jlink;  // 更新首边指针
				return;
			}

			// 遍历寻找并删除边
			std::shared_ptr<EdgeBox<T>> arc = firstEdge;
			while (arc && arc->jlink) {
				if (arc->jlink->ivex == k) {
					arc->jlink = arc->jlink->jlink;  // 删除中间节点
					return;
				}
				arc = arc->jlink;
			}
		};

		freeivex(list.vertices[v].firstedge, w);

		freejvex(list.vertices[w].firstedge, v);

	}

	//在图中寻找v和w之间的弧
	bool Find_Arc(AdjMul_List<K, T>& list, int v, int w)
	{
		//空图或者v不合法或者w不合法
		if (list.vertices.size() == 0 || v < 0 || v >= list.vertices.size() || w < 0 || w >= list.vertices.size())
		{
			return false;
		}

		std::shared_ptr<EdgeBox<T>> arc = list.vertices[v].firstedge;

		while (arc && arc->jvex != w)
		{
			arc = arc->jlink;
		}

		//找到
		if (arc)
		{
			return true;
		}

		return false;
	}

	//在G中增添弧<v, w>, 若G是无向图，则还增添对称弧<w, v>。
	void InSertArc(AdjMul_List<K, T>& list, int v, int w)
	{
		//空图或者v不合法或者w不合法
		if (list.vertices.size() == 0 || v < 0 || v >= list.vertices.size() || w < 0 || w >= list.vertices.size())
		{
			return;
		}


		//图中无此弧
		if (!Find_Arc(list, v, w))
		{
			std::shared_ptr<EdgeBox<T>> newedge = new EdgeBox<T>(v, w);

			list.vertices[v].Add_iEdge(newedge);

			list.vertices[w].Add_jEdge(newedge);
		}

		return;
	}

	//深度优先搜索（DFS）算法,并对节点进行处理,vis保存是否访问
	void dfs(int u, std::vector<bool>& vis, AdjMul_List<K, T>& list, std::function<void(Vexnode<K, T>&)> address = [](Vexnode<K, T>& node)->void {std::cout << node.data; })
	{
		//访问过或者u不合法
		if (u < 0 || u >= vis.size() || vis[u])
		{
			return;
		}

		//标记已访问
		vis[u] = true;

		address(list.vertices[u]);

		//递归搜索
		std::shared_ptr<EdgeBox<T>> p = list.vertices[u].firstedge;

		while (p)
		{
			dfs(p->jvex, vis, list, address);

			p = p->ilink;
		}

	}

	void do_dfs(AdjMul_List<K, T>& list, std::function<void(Vexnode<K, T>&)> address = [](Vexnode<K, T>& node)->void {std::cout << node.data; })
	{
		//空图
		if (list.vertices.empty())
		{
			return;
		}

		//初始化vis
		std::vector<bool> vis(list.vertices.size(), false);

		// 遍历所有节点，确保不遗漏任何不连通的部分
		for (int i = 0; i < list.vertices.size(); i++)
		{
			dfs(i, vis, list, address);
		}
	}

	//广度优先搜索（BFS）算法,并对节点进行处理,vis保存是否访问
	void bfs(int start, std::vector<bool>& vis, AdjMul_List<K, T>& list, std::function<void(Vexnode<K, T>&)> address = [](Vexnode<K, T>& node)->void {std::cout << node.data; })
	{
		//已经访问过或者start不合法
		if (vis[start] || start < 0 || start >= vis.size())return;

		std::queue<int> q;//存放当前连通分支节点，并要着start节点一圈圈增加

		vis[start] = true;//标记初始点访问

		q.push(start);//将起始节点入队

		//队列不为空，即当前连通分支还有为访问的节点
		while (!q.empty())
		{
			int u = q.front(); // 获取队列前端的节点

			q.pop();           // 出队

			address(list.vertices[u]);       // 处理当前节点

			// 遍历当前顶点，找到所有邻接并且未访问的节点加入队列
			std::shared_ptr<EdgeBox<T>> p = list.vertices[u].firstedge;

			while (p)
			{
				//未访问
				if (!vis[p->jvex])
				{
					vis[p->jvex] = true;
					q.push(p->jvex);
				}

				p = p->ilink;
			}
		}
	}

	// 执行BFS遍历
	void do_bfs(AdjMul_List<K, T>& list, std::function<void(Vexnode<K, T>&)> address = [](Vexnode<K, T>& node)->void {std::cout << node.data; })
	{
		// 空图
		if (list.vertices.empty()) return;

		std::vector<bool> vis(list.vertices.size(), false); // 创建访问标志

		// 遍历所有节点，确保不遗漏任何不连通的部分
		for (int i = 0; i < list.vertices.size(); i++)
		{
			bfs(i, vis, list, address); // 从未访问的节点开始BFS
		}
	}

	//因类似与邻接表，不考虑拓扑排序和关键路径


	void PrintGraph(const AdjMul_List<K, T>& graph) {
		std::cout << "Graph with " << graph.vertices.size() << " vertices and "
			<< graph.arcnum << " edges:" << std::endl;

		// 遍历每个顶点，打印与之相连的边
		for (const auto& vertex : graph.vertices) {
			std::cout << "Vertex " << vertex.data << ":";

			// 获取每个顶点的第一条边
			auto edge = vertex.firstedge;

			if (!edge) {
				std::cout << " No edges" << std::endl;
				continue;
			}

			// 遍历每条边
			while (edge) {
				std::cout << " -> Edge (" << edge->ivex << ", " << edge->jvex
					<< "), Weight: " << edge->info
					<< ", Marked: " << (edge->mark ? "Yes" : "No");

				// 根据顶点是 ivex 还是 jvex 来选择下一个边
				edge = (edge->ivex == vertex.data) ? edge->ilink : edge->jlink;
			}
			std::cout << std::endl;
		}
	}

};
