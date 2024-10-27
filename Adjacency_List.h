#pragma once
#include<iostream>
#include<memory>//便于使用shared_ptr
#include<vector>

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

	ArcNode(int vex) :adjvex(vex), nextarc(nullptr){ }

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

	Adj_List(int vexnum, int arcnum, bool direct = false) :arcnum(arcnum), is_direct(direct)
	{
		//初始化arcnum
		vertices.resize(vexnum);
	}

};

//管理类
template<class K = int, class T = int>
class AdjList_manager
{
public:
	
	//构造n个顶点，m条边的图
	Adj_List CreateGraph(int n, int m, int edge[][2], bool direct=false)
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
	Adj_List CreateGraph(int n, int m, int edge[][2],std::vector<int> weights, bool direct = false)
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
	void DestroyGraph(Adj_List& list)
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
};