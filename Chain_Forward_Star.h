#pragma once
#include<iostream>
#include<vector>
#include<queue>

//链式前向星，其实就是静态建立的邻接表，又可以是用链表实现的邻接表
//不过链式前向星存储的是边的邻接表，而非邻接表一般存的是点的邻接表

struct edge
{
	//边结束点
	int to;

	//当前边的后继,表示与这个边起点相同的下一条边的编号
	int next;

	//权值
	int info;

	edge(int to,int next,int info):to(to),next(next),info(info){ }
};


//所有信息均为int

class Chain_forwardStar
{
public:
	//顶点数
	int vexnum;

	//边数
	int arcnum;

	//有向
	bool is_direct;

	//边的邻接表
	std::vector<edge> ChainStar;
	
	//head[i]数组，表示以 i 为起点的最后一条边的编号
	std::vector<int> head;

	//查找边
	bool find_edge(int u, int v)
	{
		//u,v不合法
		if (u < 0 || u >= vexnum || v < 0 || v >= vexnum)
		{
			return false;
		}

		//在以u为首的边里查找,每次转移到下条边
		for (int i = head[u]; i != -1; i = ChainStar[i].next)
		{
			//找到了边
			if (ChainStar[i].to == v)
			{
				return true;
			}
		}

		return false;
	}

	//增加边
	void add(int u, int v,int weight=1)
	{
		//u,v不合法
		if (u < 0 || u >= vexnum || v < 0 || v >= vexnum)
		{
			return;
		}

		//已经存在
		if (find_edge(u, v))
		{
			return;
		}

		edge p(v, head[u], weight);

		ChainStar.push_back(p);

		//改变head指向
		head[u] = ChainStar.size()-1;

		//无向边,添加对称边
		if (!is_direct)
		{
			edge q(u, head[v], weight);

			ChainStar.push_back(q);

			//改变head指向
			head[v] = ChainStar.size()-1;
		}
	}

	//深度优先搜索
	void dfs(int u,std::vector<bool>& vis)
	{
		if (vis[u])return;

		//标记
		vis[u] = true;

		//不考虑其他处理，需要可以在此处实现
		std::cout << u << " ";

		for (int i = head[u]; i != -1; i = ChainStar[i].next)
		{
			dfs(ChainStar[i].to, vis);
		}

		return;
	}

	void do_dfs(int u)
	{
		//初始化
		std::vector<bool> vis;
		vis.resize(vexnum, false);

		for (int i = 0; i < vexnum; i++)
		{
			dfs(i, vis);
		}

		return;
	}

	//广度优先搜索
	void bfs(int u, std::vector<bool>& vis)
	{
		if (vis[u])return;

		//初始化
		std::queue<int> q;

		q.push(u);

		vis[u] = true;

		while (!q.empty())
		{
			int v = q.front();
			q.pop();

			std::cout << v << " ";

			for (int i = head[v]; i != -1; i = ChainStar[i].next)
			{
				if (!vis[ChainStar[i].to])
				{
					q.push(ChainStar[i].to);
					vis[ChainStar[i].to] = true;
				}
			}
		}

	}

	void do_bfs(int u)
	{
		//初始化
		std::vector<bool> vis;
		vis.resize(vexnum, false);

		for (int i = 0; i < vexnum; i++)
		{
			bfs(i, vis);
		}

		return;
	}

	void Init(int vex, int arc, int edge[][3],bool direct)
	{
		vexnum = vex;

		arcnum = arc;

		is_direct = direct;

		head.resize(vex, -1);

		for (int i = 0; i < arc; i++)
		{
			add(edge[i][0], edge[i][1], edge[i][2]);
		}
	}

	Chain_forwardStar(int vex, int arc, int edge[][3], bool direct = false)
	{
		Init(vex, arc, edge, direct);
	}

	void printGraph() {
		std::cout << "Graph adjacency list (Chain Forward Star representation):" << std::endl;
		for (int i = 0; i < vexnum; i++) {
			std::cout << "Vertex " << i << ":";
			for (int j = head[i]; j != -1; j = ChainStar[j].next) {
				std::cout << " -> (" << ChainStar[j].to << ", weight=" << ChainStar[j].info << ")";
			}
			std::cout << std::endl;
		}
	}

	//不考虑其他功能

};