#pragma once
#include<iostream>
#include<vector>
#include<queue>
#include<memory>
#include<functional>

//十字链表(Orthogonal List) 是**有向图**的另一种链式存储结构
//可以看成是将有向图的邻接表和逆邻接表结合起来得到的一种链表

//弧结点
//尾域(tailvex) 和头域 (headvex)分别指示弧尾和弧头这两个顶点在图中的位置
//链域hlink指向弧头相同的下一条弧，而链域tlink指向弧尾相同的下一条弧
//info域指向该弧的相关信息，例如权值
template<typename T=int>
class ArcBox
{
public:
	//弧尾结点
	int tailvext;
	//弧头结点
	int headvext;
	
	//弧尾相同的弧的链域
    std::shared_ptr<ArcBox<T>> tlink;

	//弧头相同的弧的链域
    std::shared_ptr<ArcBox<T>> hlink;

	//信息
	T info;

	ArcBox() = default;

	//默认无信息
	ArcBox(int tail,int head, std::shared_ptr<ArcBox<T>> t=nullptr, ArcBox<T>* h=nullptr):tailvext(tail),headvext(head),tlink(t),hlink(h){ }

};


//顶点
//data域存储和顶点相关的信息
//firstin和firstout为两个链域，分别指向以该顶点为弧头或弧尾的第一个弧结点。
template<typename K=int,typename T=int>
class VexBox
{
public:
	//顶点信息，默认为序号
	K data;

	//该顶点为弧头的第一条弧
    std::shared_ptr<ArcBox<T>> firstin;

	//该顶点为弧尾的第一条弧
    std::shared_ptr<ArcBox<T>> firstout;

	VexBox() = default;

	VexBox(K d, std::shared_ptr<ArcBox<T>> firstin=nullptr, std::shared_ptr<ArcBox<T>> firstout=nullptr):data(d),firstin(firstin),firstout(firstout){ }

	//加入出边,即弧尾
	void Add_outArc(std::shared_ptr<ArcBox<T>> arc)
	{
		//插入arc到firstarc
		std::shared_ptr<ArcNode<T>> p = firstout;

		firstout = arc;

		arc->tlink = p;
	}

	//加入入边，即弧头
	void Add_inArc(std::shared_ptr<ArcBox<T>> arc)
	{
		//插入arc到firstarc
		std::shared_ptr<ArcNode<T>> p = firstin;

		firstin = arc;

		arc->hlink = p;
	}
};

//表
template<typename K = int, typename T = int>
class OLGraph
{
public:
	//表头结点表，此vector.size()即顶点数,所以不需要记录顶点数
	std::vector<VexBox<K, T>> vertices;

	//记录边数
	int arcnum;

	//无data的顶点,默认data为序号
	OLGraph(int vexnum)
	{
		//初始化arcnum
		vertices.resize(vexnum);
		for (int i = 0; i < vexnum; i++)
		{
			vertices[i].data = i;
		}
	}

	//需要data的顶点
	OLGraph(int vexnum, K datas[]) 
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
class OLGraph_manager
{
public:
	//构造n个顶点，m条边的图
	OLGraph<K, T> CreateGraph(int n, int m, int edge[][2])
	{
		//初始化
		Adj_List<K, T> list(n);

		//加入边的信息，此处使用头插法不考虑edge中的位置在List中的稳定性
		for (int i = 0; i < m; i++)
		{
			//确定弧
			int u = edge[i][0];
			int v = edge[i][1];

			//构建arc
			std::shared_ptr<ArcBox<T>> arc(new ArcBox<T>(u, v));
			
			list.vertices[u].Add_outArc(arc);


			list.vertices[v].Add_inArc(arcn);
		}

		return list;
	}

	//构造n个顶点，m条边的带权值的图
	OLGraph<K, T> CreateGraph(int n, int m, int edge[][2], std::vector<int> weights)
	{
		//初始化
		Adj_List<K, T> list(n);

		//加入边的信息，此处使用头插法不考虑edge中的位置在List中的稳定性
		for (int i = 0; i < m; i++)
		{
			//确定弧
			int u = edge[i][0];
			int v = edge[i][1];

			//构建arc
			std::shared_ptr<ArcBox<T>> arc(new ArcBox<T>(u, v));

			arc->info = weights[i];

			list.vertices[u].Add_outArc(arc);

			list.vertices[v].Add_inArc(arc);
		}

		return list;
	}

	//若图G存在，销毁图G。
	void DestroyGraph(OLGraph<K,T>& graph)
	{
		//空图
		if (graph.vertices.size() == 0)
		{
			return;
		}

		for (const auto& vex : graph.vertices)
		{
			std::shared_ptr<ArcBox<T>> arc = vex.firstout;

			//遍历释放
			while (vex.tlink!= nullptr)
			{
				std::shared_ptr<ArcBox<T>> arc = vex.tlink;

				vex.tlink = arc->nextarc;

				arc.reset();
			}
		}

		graph.vertices.clear();

		graph.arcnum = 0;

		return;
	}

	//返回v的第一个邻接顶点的序号。若v在G中没有邻接顶点，则返回 “空”。
	int FirstAdjVex(OLGraph<K, T>& graph, int v)
	{
		//空图或者v不合法
		if (graph.vertices.size() == 0 || v < 0 || v >= graph.vertices.size())
		{
			return -1;
		}

		//没有邻接点
		if (!graph.vertices[v].firstout)
		{
			return -1;
		}

		return graph.vertices[v].firstout.headvext;
	}

	//返回v的（相对千w的）下一个邻接顶点。若w是v的最后一个邻接点，则返回 “空”。
	int NextAdjVex(OLGraph<K, T>& graph, int v, int w)
	{
		//空图或者v不合法或者w不合法
		if (graph.vertices.size() == 0 || v < 0 || v >= graph.vertices.size() || w < 0 || w >= graph.vertices.size())
		{
			return -1;
		}

		std::shared_ptr<ArcBox<T>> arc = graph.vertices[v].firstout;

		//找到w
		while (arc->tlink && arc->headvext != w)
			arc = arc->tlink;

		if (arc)
		{
			return arc->headvext;
		}
		else
		{
			return -1;
		}

	}

	//不考虑增删顶点了


	//在G中删除弧<v, w>
	void DeleteArc(OLGraph<K, T>& graph, int v, int w)
	{
		//空图或者v不合法或者w不合法
		if (graph.vertices.size() == 0 || v < 0 || v >= graph.vertices.size() || w < 0 || w >= graph.vertices.size())
		{
			return;
		}

		auto freeout = [&](std::shared_ptr<ArcBox<T>> arc, int k)->void {

			while (arc->tlink && arc->tlink->headvext != k)
			{
				arc = arc->tlink;
			}

			//找到弧
			if (arc->tlink)
			{
				std::shared_ptr<ArcBox<T>> p;

				p = arc->tlink;

				arc->tlink = p->tlink;
			}

		};

		auto freein = [&](std::shared_ptr<ArcBox<T>> arc, int k)->void {

			while (arc->hlink && arc->hlink->tailvext != k)
			{
				arc = arc->=hlink;
			}

			//找到弧
			if (arc->hlink)
			{
				std::shared_ptr<ArcBox<T>> p;

				p = arc->hlink;

				arc->hlink = p->hlink;

				p.reset();
			}

		};

		//入边
		freeout(graph.vertices[v].firstout, w);
		//出边再释放
		freein(graph.vertices[w].firstin, v);

	}

	//在图中寻找v和w之间的弧
	bool Find_Arc(OLGraph<K, T>& graph, int v, int w)
	{
		//空图或者v不合法或者w不合法
		if (graph.vertices.size() == 0 || v < 0 || v >= graph.vertices.size() || w < 0 || w >= graph.vertices.size())
		{
			return false;
		}

		std::shared_ptr<ArcBox<T>> arc = graph.vertices[v].firstout;

		while (arc && arc->headvext != w)
		{
			arc = arc->tlink;
		}

		//找到
		if (arc)
		{
			return true;
		}

		return false;
	}

	//在G中增添弧<v, w>
	void InSertArc(OLGraph<K, T>& graph, int v, int w)
	{
		//空图或者v不合法或者w不合法
		if (graph.vertices.size() == 0 || v < 0 || v >= graph.vertices.size() || w < 0 || w >= graph.vertices.size())
		{
			return;
		}

		auto addout = [](std::shared_ptr<ArcBox<T>> arc, int k) -> void {
			//遍历到最后
			while (arc->tlink)
			{
				arc = arc->tlink;
			}

			std::shared_ptr<ArcBox<T>> p(new ArcBox<T>(k));

			//加入
			arc->tlink = p;

		};

		auto addin = [](std::shared_ptr<ArcBox<T>> arc, int k) -> void {
			//遍历到最后
			while (arc->hlink)
			{
				arc = arc->hlink;
			}

			std::shared_ptr<ArcBox<T>> p(new ArcBox<T>(k));

			//加入
			arc->hlink = p;

		};

		//图中无此弧
		if (!Find_Arc(graph, v, w))
		{
			std::shared_ptr<ArcBox<T>> p = list.vertices[v].firstout;

			std::shared_ptr<ArcBox<T>> q = list.vertices[w].firstin;

			addout(p, w);

			addin(q, v);
		}

		return;
	}

	//深度优先搜索（DFS）算法,并对节点进行处理,vis保存是否访问
	void dfs(int u, std::vector<bool>& vis, OLGraph<K, T>& graph, std::function<void(VexBox<K, T>&)> address = [](VexBox<K, T>& node)->void {std::cout << node.data; })
	{
		//访问过或者u不合法
		if (u < 0 || u >= vis.size() || vis[u])
		{
			return;
		}

		//标记已访问
		vis[u] = true;

		address(graph.vertices[u]);

		//递归搜索
		std::shared_ptr<ArcBox<T>> p = graph.vertices[u].firstout;

		while (p)
		{
			dfs(p->headvext, vis, graph, address);

			p = p->tlink;
		}

	}

	void do_dfs(OLGraph<K, T>& graph, std::function<void(VexBox<K, T>&)> address = [](VexBox<K, T>& node)->void {std::cout << node.data; })
	{
		//空图
		if (graph.vertices.empty())
		{
			return;
		}

		//初始化vis
		std::vector<bool> vis(graph.vertices.size(), false);

		// 遍历所有节点，确保不遗漏任何不连通的部分
		for (int i = 0; i < graph.vertices.size(); i++)
		{
			dfs(i, vis, graph, address);
		}
	}

	//广度优先搜索（BFS）算法,并对节点进行处理,vis保存是否访问
	void bfs(int start, std::vector<bool>& vis, OLGraph<K, T>& graph, std::function<void(VexBox<K, T>&)> address = [](VexBox<K, T>& node)->void {std::cout << node.data; })
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

			address(graph.vertices[u]);       // 处理当前节点

			// 遍历当前顶点，找到所有邻接并且未访问的节点加入队列
			std::shared_ptr<ArcBox<T>> p = graph.vertices[u].firstout;

			while (p)
			{
				//未访问
				if (!vis[p->headvext])
				{
					vis[p->headvext] = true;
					q.push(p->headvext);
				}

				p = p->tlink;
			}
		}
	}

	// 执行BFS遍历
	void do_bfs(OLGraph<K, T>& graph, std::function<void(VexBox<K, T>&)> address = [](VexBox<K, T>& node)->void {std::cout << node.data; })
	{
		// 空图
		if (graph.vertices.empty()) return;

		std::vector<bool> vis(graph.vertices.size(), false); // 创建访问标志

		// 遍历所有节点，确保不遗漏任何不连通的部分
		for (int i = 0; i < graph.vertices.size(); i++)
		{
			bfs(i, vis, graph, address); // 从未访问的节点开始BFS
		}
	}

	//与邻接表相似，不考虑拓扑排序和关键路径了
};