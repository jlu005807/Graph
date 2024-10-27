#pragma once
#include<iostream>
#include<vector>
#include<queue>
#include<functional>
#include<climits>

//使用一个二维数组 adj 来存边，其中 adj[u][v] 为 1 表示存在 u 到 v 的边，为 0 表示不存在。
//如果是带边权的图，可以在 adj[u][v] 中存储 u 到 v 的边的边权。
//默认顶点到自身没有边
//并且二维数组难以改变结构所以不考虑增加和删除节点但是可以增删边，并且二维数组空间大不适用于稀疏图
//其实无向图为对称矩阵可以压缩为一维数组存放，但是实现逻辑相似不考虑分离  

class Adj_Matrix
{
public:
	std::vector<std::vector<int>> graph;

	//标志是否为有向图
	bool is_direct;

	Adj_Matrix() = default;

	//顶点数量number,依旧从零开始,当weight默认时表示无权图
	Adj_Matrix(int number,bool direct=false, int weight = INT_MAX):is_direct(direct)
	{
		graph.resize(number, std::vector<int>(number,weight));
	}
};


class AdjMatrix_manager
{
public:

	//构造n个顶点，m条边的图,
	Adj_Matrix CreateGraph(int n, int m, int edge[][2],bool direct)
	{
		//初始化邻接矩阵
		Adj_Matrix one(n,direct);

		//加入边
		for (int i = 0; i < m; i++)
		{
			int u = edge[i][0];
			int v = edge[i][1];

			one.graph[u][v] = 1;

			//如果不是有向图，则对称点同样赋值
			if (!one.is_direct)
			{
				one.graph[v][u] = 1;
			}
		}

		return one;

	}

	//构造n个顶点，m条边的网,即带权图
	Adj_Matrix CreateGraph(int n, int m, int edge[][2], bool direct,std::vector<int> weights)
	{
		//初始化邻接矩阵,以int最大值初始化
		Adj_Matrix one(n,direct);

		//加入边
		for (int i = 0; i < m; i++)
		{
			int u = edge[i][0];
			int v = edge[i][1];

			//赋予权值
			one.graph[u][v] = weights[i];

			//如果不是有向图，则对称点同样赋值
			if (!one.is_direct)
			{
				one.graph[v][u] = weights[i];
			}
		}

		return one;

	}

	//若图G存在，销毁图G。
	void DestroyGraph(Adj_Matrix& adj)
	{
		//空图
		if (adj.graph.empty())
		{
			return;
		}
		
		adj.graph.clear();
		return;
	}

	//若图G存在，v是G的某个顶点返回v的第一个邻接顶点。
	//若v在G中没有邻接顶点，则返回 “空”（-1）
	int FirstAdjVex(Adj_Matrix& adj, int v)
	{
		//空图
		if (adj.graph.empty())
		{
			return -1;
		}

		//不存在v
		if (v < 0 || v >= adj.graph.size())
		{
			return -1;
		}

		//遍历Graph[v]即顶点v的邻接序列，
		for (int i = 0; i < adj.graph.size(); i++)
		{
			//找到邻接点,返回
			if (adj.graph[v][i] != INT_MAX)
			{
				return i;
			}
		}

		//没有邻接点
		return -1;

	}

	int NextAdjVex(Adj_Matrix& adj, int v, int w)
	{
		//空图
		if (adj.graph.empty())
		{
			return -1;
		}

		//不存在v
		if (v < 0 || v >= adj.graph.size())
		{
			return -1;
		}

	    //v和w不邻接
		if (adj.graph[v][w] != INT_MAX )
		{
			return -1;
		}

		for (int i = w+1; i < adj.graph.size(); i++)
		{
			if (adj.graph[v][i] != INT_MAX)return i;
		}

		//没有下一个邻接点或者w为最后一个
		return -1;
	}

	//若图G存在，v和w是G中两个顶点。
	//在G中增添或者更新弧<v, w>, 若G是无向图，则还增添对称弧<w, v>。
	bool InSertArc(Adj_Matrix& adj, int v, int w,int weight=1)
	{
		//空图
		if (adj.graph.empty())
		{
			return -1;
		}

		//不存在这样的点
		if (v < 0 || v >= adj.graph.size() || w < 0 || w >= adj.graph.size())
		{
			return false;
		}

		//增加或更新边
		adj.graph[v][w] = weight;
		//如果为无向图
		if (!adj.is_direct)
		{
			adj.graph[w][v] = weight;
		}

		return true;
	}

	//在G中删除弧<v, w>, 若G是无向图，则还删除对称弧<w, v>
	bool DeleteArc(Adj_Matrix& adj, int v, int w)
	{
		//空图
		if (adj.graph.empty())
		{
			return -1;
		}

		//不存在这样的点
		if (v < 0 || v >= adj.graph.size() || w < 0 || w >= adj.graph.size())
		{
			return false;
		}

		//删除边
		adj.graph[v][w] = INT_MAX;
		//如果为无向图
		if (!adj.is_direct)
		{
			adj.graph[w][v] = INT_MAX;
		}

		return true;

	}

	bool find_edge(Adj_Matrix& adj, int v, int w)
	{
		//空图
		if (adj.graph.empty())
		{
			return -1;
		}

		//不存在这样的点
		if (v < 0 || v >= adj.graph.size() || w < 0 || w >= adj.graph.size())
		{
			return false;
		}

		//存在边
		if (adj.graph[v][w] != INT_MAX)
		{
			return true;
		}
		
		//不存在边
		return false;
	}

	//深度优先搜索（DFS）算法,并对节点进行处理,vis保存是否访问
	void dfs(int u, std::vector<bool>& vis, Adj_Matrix& adj, std::function<void(int&)> address = [](int& u)->void {std::cout << u; })
	{
		//访问过或者u不合法
		if (u<0||u>=vis.size()||vis[u])
		{
			return;
		}

		//标记已访问
		vis[u] = true;

		address(u);

		//递归搜索
		for (int i = 0; i < vis.size(); i++)
		{
			//存在邻接点
			if (adj.graph[u][i] != INT_MAX)
			{
				dfs(i, vis,adj,address);
			}
		}

	}

	void do_dfs(Adj_Matrix& adj, std::function<void(int&)> address = [](int& u)->void {std::cout << u; })
	{
		//空图
		if (adj.graph.empty())
		{
			return;
		}

		//初始化vis
		std::vector<bool> vis(adj.graph.size(), false);

		// 遍历所有节点，确保不遗漏任何不连通的部分
		for (int i = 0; i < adj.graph.size(); i++)
		{
			dfs(i, vis, adj, address);
		}
	}


	//广度优先搜索（BFS）算法,并对节点进行处理,vis保存是否访问
	void bfs(int start, std::vector<bool>& vis, Adj_Matrix& adj, std::function<void(int&)> address = [](int& u)->void {std::cout << u; })
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

			// 遍历图，找到所有邻接并且未访问的节点加入队列
			for (int i=0;i<adj.graph.size();i++)
			{
				if (adj.graph[u][i] != INT_MAX && !vis[i])
				{
					vis[i] = true;// 标记为已访问
					q.push(i);     // 将邻接节点入队
				}
			}
		}
	}

	// 执行BFS遍历
	void do_bfs(Adj_Matrix& adj,  std::function<void(int&)> address = [](int& u)->void { std::cout << u; }) {
		// 空图
		if (adj.graph.empty()) return;

		std::vector<bool> vis(adj.graph.size(), false); // 创建访问标志

		// 遍历所有节点，确保不遗漏任何不连通的部分
		for (int i = 0; i < adj.graph.size(); i++)
		{
			bfs(i, vis, adj, address); // 从未访问的节点开始BFS
		}
	}

	//生成拓扑排序
	std::vector<int> TopologicalSort(Adj_Matrix adj)
	{
		// 无向图无法进行拓扑排序
		if (!adj.is_direct)
		{
			std::cerr << "Graph doesn't have direction, topological sorting not possible." << std::endl;
			return {};
		}

		int number = adj.graph.size();

		std::vector<int> Topo;//记录拓扑排序

		std::queue<int> zero_in_degree;//记录入度为0的顶点

		//存放各个顶点的入度
		std::vector<int> indegree(number, 0);

		//计算各个点的入度
		for (int u = 0; u < number; u++)
		{
			for (int v = 0; v < number; v++)
			{
				//存在边u->v
				if (adj.graph[u][v] != INT_MAX)
				{
					indegree[v]++;
				}
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
			for (int i=0;i<number;i++)
			{
				//相应顶点入度减一
				if (adj.graph[u][i] != INT_MAX)
				{
					indegree[i]--;

					//如果为0放入zero_in_degree
					if (indegree[i] == 0)
					{
						zero_in_degree.push(i);
					}
				}
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
	std::vector<int> CriticalPath(Adj_Matrix adj)
	{
		// 无向图无法进行拓扑排序
		if (!adj.is_direct)
		{
			std::cerr << "Graph doesn't have direction, CriticalPath not possible." << std::endl;
			return {};
		}


		int number = adj.graph.size();

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
		for (int u=0;u<Topo.size();u++)
		{
			for (int v = 0; v < number; v++)
			{
				//更新当前节点的邻接点的VE
				if (adj.graph[u][v] != INT_MAX)
				{
					VE[v] = std::max(VE[v], VE[u] + adj.graph[u][v]);
					edge_num++;
				}
			}
		}

		//初始化各节点的最晚发生时间（VL）,对于结束点即拓扑排序最后一个点的LT为VE[number - 1]
		std::vector<int> VL(number, VE[number - 1]);


		//逆拓扑排序计算各个顶点VL
		for (auto it = Topo.rbegin(); it != Topo.rend(); it++)
		{
			int u = *it;
			for (int i = 0; i < number; i++)
			{
				if (adj.graph[u][i] != INT_MAX)
				{
					VL[u] = std::min(VL[u], VL[i] - adj.graph[u][i]);
				}
			}
		}

		//计算各个弧即活动的最早开始时间ET，即各个弧起始点即弧尾的VE
		//计算弧i即活动的最晚开始时间LT，即各个弧结束点即弧头的VL-weights[i]
		//同时计算关键路径的节点（此时Path内为顶点而非弧）
		std::vector<int> critical_adjpath;

		for (int u = 0; u < Topo.size(); u++)
		{
			for (int v = 0; v < number; v++)
			{
				//更新当前节点的为弧尾的活动的ET和LT
				if (adj.graph[u][v] != INT_MAX)
				{
					int ET = VE[u];
					int LT = VL[v] - adj.graph[u][v];
					//判断是否为关键路径
					if (ET == LT)
					{
						//如果为空,放入首尾
						if (critical_adjpath.empty())
						{
							critical_adjpath.push_back(u);
							critical_adjpath.push_back(v);
						}
						else if(critical_adjpath.back()==u)//判断首尾是否相接
						{
							critical_adjpath.push_back(v);//只放入尾节点
						}
					}
				}
			}
		}

		std::cout << "项目的最早完成时间为：" << VE[number - 1] << std::endl;
		return critical_adjpath;  // 返回关键路径


	}
};