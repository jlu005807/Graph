#pragma once
#include<iostream>
#include<memory>//便于使用shared_ptr
#include<vector>
#include<queue>
#include<functional>

//邻接表(Adjacency List) 是图的一种链式存储结构
//在邻接表中，对图中每个顶点V;建立一个单链表，把与V; 相邻接的顶点放在这个链表中。
//邻接表中每个单链表的第一个结点存放有关顶点的信息， 把这一结点看成链表的表头，其余结点存放有关边的信息
//这样邻接表便由两部分组成：表头结点表和边表。

//边结点
//边链表中边结点包括邻接点域(adjvex)、数据域(info) 和链域(nextarc) 三部分
//邻接点域指示与顶点V; 邻接的点在图中的位置；数据域存储和边相关的信息， 如权值等；链域指示与顶点v邻接的下一条边的结点。
template<class T=int>//默认为int
class ArcNode
{
public:
	//该边所指向的顶点位置，如果为有向边则为弧头
	int adjvex;

	//指向下一条边的指针
	std::shared_ptr<ArcNode<T>> nextarc;

	//边信息，如权值,默认无信息
	T info;

	ArcNode() = default;

	ArcNode(int vex) :adjvex(vex),info(1),nextarc(nullptr){ }

	ArcNode(int vex,T info) :adjvex(vex),info(info), nextarc(nullptr) { }

};

//顶点结点
//表头结点包括数据域 (data) 和链域 (firstarc) 两部分
//数据域用于存储顶点V; 的名称或其他有关信息；链域用千指向链表中第一个结点（即与顶点V; 邻接的第一个邻接点）。
template<class K=int,class T = int>
class VexNode
{
public:
	//顶点信息，默认是顶点编号则不需要信息
	K data;

	//指向第一条依附该顶点的边的指针
	std::shared_ptr<ArcNode<T>> firstarc;

	VexNode() = default;

	VexNode(std::shared_ptr<ArcNode<T>> arc=nullptr):firstarc(arc){ }

	VexNode(K data, std::shared_ptr<ArcNode<T>> arc = nullptr) :data(data),firstarc(arc) { }

	//加入边
	void Add_Arc(std::shared_ptr<ArcNode<T>> arc)
	{
		//插入arc到firstarc
		std::shared_ptr<ArcNode<T>> p = firstarc;

		firstarc = arc;

		arc->nextarc = p;
	}


};

template<class K = int, class T = int>
class Adj_List
{
public:
	//表头结点表，此vector.size()即顶点数,所以不需要记录顶点数
	std::vector<VexNode<K, T>> vertices;

	//记录边数
	int arcnum;

	//是否为有向图
	bool is_direct;

	//无data的顶点,默认data为序号
	Adj_List(int vexnum, int arcnum, bool direct = false) :arcnum(arcnum), is_direct(direct)
	{
		//初始化arcnum
		vertices.resize(vexnum);
		for (int i = 0; i < vexnum; i++)
		{
			vertices[i].data = i;
		}
	}

	//需要data的顶点
	Adj_List(int vexnum, int arcnum, bool direct = false,K datas[]) :arcnum(arcnum), is_direct(direct)
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
class AdjList_manager
{
public:
	
	//构造n个顶点，m条边的图
	Adj_List<K,T> CreateGraph(int n, int m, int edge[][2], bool direct=false)
	{
		//初始化
		Adj_List<K,T> list(n, m, direct);

		//加入边的信息，此处使用头插法不考虑edge中的位置在List中的稳定性
		for (int i = 0; i < m; i++)
		{
			//确定弧
			int u = edge[i][0];
			int v = edge[i][1];

			//构建arc
			std::shared_ptr<ArcNode<T>> arc(new ArcNode(v));

			list.vertices[u].Add_Arc(arc);

			//如果是无向图，则增加对称边
			if (!list.is_direct)
			{
				list.vertices[v].Add_Arc(arc);
			}
		}

		return list;
	}

	//构造n个顶点，m条边的带权值的图
	Adj_List<K,T> CreateGraph(int n, int m, int edge[][2],std::vector<int> weights, bool direct = false)
	{
		//初始化
		Adj_List<K, T> list(n, m, direct);

		//加入边的信息，此处使用头插法不考虑edge中的位置在List中的稳定性
		for (int i = 0; i < m; i++)
		{
			//确定弧
			int u = edge[i][0];
			int v = edge[i][1];

			//构建arc
			std::shared_ptr<ArcNode<T>> arc(new ArcNode(v,weights[i]));

			list.vertices[u].Add_Arc(arc);

			//如果是无向图，则增加对称边
			if (!list.is_direct)
			{
				list.vertices[v].Add_Arc(arc);
			}
		}

		return list;
	}

	//若图G存在，销毁图G。
	void DestroyGraph(Adj_List<K,T>& list)
	{
		//空图
		if (list.vertices.size()==0)
		{
			return;
		}

		for (const auto& vex:list.vertices)
		{	
			std::shared_ptr<ArcNode<T>> arc = vex.firstarc;

			//遍历释放
			while (vex.firstarc != nullptr)
			{
				std::shared_ptr<ArcNode<T>> arc = vex.firstarc;

				vex.firstarc = arc->nextarc;

				arc.reset();
			}
		}

		list.vertices.clear();

		list.arcnum = 0;

		return;
	}

	//返回v的第一个邻接顶点的序号。若v在G中没有邻接顶点，则返回 “空”。
	int FirstAdjVex(Adj_List<K,T>& list, int v)
	{
		//空图或者v不合法
		if (list.vertices.size() == 0 || v < 0 || v >= list.vertices.size())
		{
			return -1；
		}

		//没有邻接点
		if (!list.vertices[v].firsrarc)
		{
			return -1;
		}

		return list.vertices[v].firsrarc.adjvex;
	}

	//返回v的（相对千w的）下一个邻接顶点。若w是v的最后一个邻接点，则返回 “空”。
	int NextAdjVex(Adj_List<K,T>& list, int v, int w)
	{
		//空图或者v不合法或者w不合法
		if (list.vertices.size() == 0 || v < 0 || v >= list.vertices.size() || w < 0 || w >= list.vertices.size())
		{
			return -1;
		}

		std::shared_ptr<ArcNode<T>> arc = list.vertices[v].firstarc;

		//找到w
		while (arc->nextarc && arc->adjvex != w)
			arc = arc->nextarc;

		if (arc->nextarc)
		{
			return arc->nextarc->adjvex;
		}
		else
		{
			return -1;
		}

	}

	//在图G中增添新顶点v。
	InSertVex(Adj_List<K,T>& list,VexNode<K,T> newvex)
	{
		auto it = std::find(list.vertices.begin(), list.vertices.end(), newvex);

		//没找到
		if (it == list.vertices.end())
		{
			list.vertices.push_back(newvex);
		}
		else
		{
			return;
		}

	}

	//删除G中顶点v及其相关的弧。
	DeleteVex(Adj_List<K, T>& list, int v)
	{
		//空图或者v不合法
		if (list.vertices.size() == 0 || v < 0 || v >= list.vertices.size())
		{
			return -1;
		}

		auto it = list.vertices.begin();

		it += v;

		//记录
		VexNode<K, T> vex = *it;


		std::shared_ptr<ArcNode<T>> arc = vex.firstarc;

		while (arc)
		{
			DeleteArc(list, v, arc->adjvex);
		}
	}

	//在G中删除弧<v, w>, 若G是无向图，则还删除对称弧<w, v>。
	void DeleteArc(Adj_List<K, T>& list, int v, int w)
	{
		//空图或者v不合法或者w不合法
		if (list.vertices.size() == 0 || v < 0 || v >= list.vertices.size() || w < 0 || w >= list.vertices.size())
		{
			return;
		}

		auto free = [&](std::shared_ptr<ArcNode<T>> arc, int k)->void {
			while (arc->nextarc && arc->nextarc->adjvex != k)
			{
				arc = arc->nextarc;
			}

			//找到弧
			if (arc->nextarc)
			{
				std::shared_ptr<ArcNode<T>> p;

				p = arc->nextarc;

				arc->nextarc = p->nextarc;

				//主动释放
				p.reset();
			}

		};

		free(list.vertices[v], w);

		//如果是无向图，删除对称边
		if (!list.is_direct)
		{
			free(list.vertices[w], v);
		}

	}

	//在图中寻找v和w之间的弧
	bool Find_Arc(Adj_List<K, T>& list, int v, int w)
	{
		//空图或者v不合法或者w不合法
		if (list.vertices.size() == 0 || v < 0 || v >= list.vertices.size() || w < 0 || w >= list.vertices.size())
		{
			return false;
		}

		std::shared_ptr<ArcNode<T>> arc = list.vertices[v].firstarc;

		while (arc && arc->adjvex != w)
		{
			arc = arc->nextarc;
		}

		//找到
		if (arc)
		{
			return true;
		}

		return false;


	}

	//在G中增添弧<v, w>, 若G是无向图，则还增添对称弧<w, v>。
	void InSertArc(Adj_List<K, T>& list, int v, int w)
	{
		//空图或者v不合法或者w不合法
		if (list.vertices.size() == 0 || v < 0 || v >= list.vertices.size() || w < 0 || w >= list.vertices.size())
		{
			return;
		}

		auto add = [](std::shared_ptr<ArcNode<T>> arc, int k) -> void {
			//遍历到最后
			while (arc->nextarc)
			{
				arc = arc->nextarc;
			}

			std::shared_ptr<ArcNode<T>> p(new ArcNode<T>(k));

			//加入
			arc->nextarc = p;

		};

		//图中无此弧
		if (!Find_Arc(list, v, w))
		{
			std::shared_ptr<ArcNode<T>> p = list.vertices[v].firstarc;

			add(p, w);

			//如果是无向图，则增加对称边
			if (!list.is_direct)
			{
				p = list.vertices[w].firstarc;
				add(p, v);
			}
		}
		return;
	}

	//深度优先搜索（DFS）算法,并对节点进行处理,vis保存是否访问
	void dfs(int u, std::vector<bool>& vis, Adj_List<K,T>& list, std::function<void(VexNode<K, T>&)> address = [](VexNode<K, T>& node)->void {std::cout << node.data; })
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
		std::shared_ptr<ArcNode<T>> p = list.vertices[u].firstarc;

		while (p)
		{
			dfs(p->adjvex, vis, list, address);

			p = p->nextarc;
		}

	}

	void do_dfs(Adj_List& list, std::function<void(int&)> address = [](int& u)->void {std::cout << u; })
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
	void bfs(int start, std::vector<bool>& vis, Adj_List& list, std::function<void(int&)> address = [](int& u)->void {std::cout << u; })
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

			address(u);       // 处理当前节点

			// 遍历当前顶点，找到所有邻接并且未访问的节点加入队列
			std::shared_ptr<ArcNode<T>> p = list.vertices[u].firstarc;

			while (p)
			{
				vis[p->adjvex] = true;
				q.push(p->adjvex);
				p = p->nextarc;
			}
		}
	}

	// 执行BFS遍历
	void do_bfs(Adj_List& list, std::function<void(int&)> address = [](int& u)->void { std::cout << u; }) {
		// 空图
		if (list.vertices.empty()) return;

		std::vector<bool> vis(list.vertices.size(), false); // 创建访问标志

		// 遍历所有节点，确保不遗漏任何不连通的部分
		for (int i = 0; i < list.vertices.size(); i++)
		{
			bfs(i, vis, list, address); // 从未访问的节点开始BFS
		}
	}

	//生成拓扑排序
	std::vector<int> TopologicalSort(Adj_List& list)
	{
		// 无向图无法进行拓扑排序
		if (!list.is_direct)
		{
			std::cerr << "Graph doesn't have direction, topological sorting not possible." << std::endl;
			return {};
		}

		int number = list.vertices.size();

		std::vector<int> Topo;//记录拓扑排序

		std::queue<int> zero_in_degree;//记录入度为0的顶点

		//存放各个顶点的入度
		std::vector<int> indegree(number, 0);

		//计算各个点的入度
		for (int u = 0; u < number; u++)
		{
			std::shared_ptr<ArcNode<T>> p = list.vertices[u].firstarc;

			while (p)
			{
				indegree[p->adjvex]++;

				p = p->nextarc;
			}

		}

		//将入度为0的点放入zero_in_degree中
		for (int i = 0; i < number; i++)
		{
			if (indegree[i] == 0)
			{
				zero_in_degree.push(i);
			}
		}

		//Kahn算法
		while (!zero_in_degree.empty())
		{
			//取出一个节点处理
			int u = zero_in_degree.front();
			zero_in_degree.pop();

			//放入Topo
			Topo.push_back(u);

			//删除相应的边,在indegree中逻辑删除
			std::shared_ptr<ArcNode<T>> p = list.vertices[u].firstarc;

			while (p)
			{
				//相应顶点入度减一
				indegree[p->adjvex]--;

				//如果为0放入zero_in_degree
				if (indegree[i] == 0)
				{
					zero_in_degree.push(i);
				}

				p = p->nextvarc;
			}

		}

		//检查是否存在环
		if (Topo.size() != number)
		{
			std::cerr << "Graph has a cycle, topological sorting not possible." << std::endl;
			return {};
		}

		//返回Topo
		return Topo;
	}

	//关键路径
	std::vector<int> CriticalPath(Adj_List list)
	{
		// 无向图无法进行拓扑排序
		if (!list.is_direct)
		{
			std::cerr << "Graph doesn't have direction, CriticalPath not possible." << std::endl;
			return {};
		}


		int number = list.vertices.size();

		//拓扑排序
		std::vector<int> Topo = TopologicalSort(adj);

		//有环
		if (Topo.empty())
		{
			std::cout << "图中存在环，无法计算关键路径。" << std::endl;
			return {};
		}

		//边的数量
		int edge_num = 0;

		//初始化各顶点的最早发生时间（VE）,对于开始点即拓扑排序第一个点的VT为0
		std::vector<int> VE(number, 0);

		//计算各个顶点的VE
		for (int u = 0; u < Topo.size(); u++)
		{
			std::shared_ptr<ArcNode<T>> p = list.vertices[u].firstarc;
			//遍历u相连的边
			while(p)
			{
				int v = p->adjvex;

				VE[v] = std::max(VE[v], VE[u] + p->info);
				edge_num++;

				p = p->nextarc;
			}
		}

		//初始化各节点的最晚发生时间（VL）,对于结束点即拓扑排序最后一个点的LT为VE[number - 1]
		std::vector<int> VL(number, VE[number - 1]);


		//逆拓扑排序计算各个顶点VL
		for (auto it = Topo.rbegin(); it != Topo.rend(); it++)
		{
			int u = *it;

			std::shared_ptr<ArcNode<T>> p = list.vertices[u].firstarc;

			//遍历u相连的边
			while (p)
			{
				int v = p->adjvex;

				VL[u] = std::min(VL[u], VL[i] - adj.graph[u][i]);

				p = p->nextarc;
			}
	
		}

		//计算各个弧即活动的最早开始时间ET，即各个弧起始点即弧尾的VE
		//计算弧i即活动的最晚开始时间LT，即各个弧结束点即弧头的VL-weights[i]
		//同时计算关键路径的节点（此时Path内为顶点而非弧）
		std::vector<int> critical_adjpath;

		for (int u = 0; u < Topo.size(); u++)
		{
			std::shared_ptr<ArcNode<T>> p = list.vertices[u].firstarc;
			while(p)
			{
				//更新当前节点的为弧尾的活动的ET和LT
				int v = p->adjvex;

				int ET = VE[u];
				int LT = VL[v] - p->info;
				//判断是否为关键路径
				if (ET == LT)
				{
				//如果为空,放入首尾
					if (critical_adjpath.empty())
					{
						critical_adjpath.push_back(u);
						critical_adjpath.push_back(v);
					}
					else if (critical_adjpath.back() == u)//判断首尾是否相接
					{
							critical_adjpath.push_back(v);//只放入尾节点
					}
				}
			}
		}
			
	    std::cout << "项目的最早完成时间为：" << VE[number - 1] << std::endl;
		return critical_adjpath;  // 返回关键路径
	}

};