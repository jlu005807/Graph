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


//小技巧：对于有向无向图图，并且规定没有弧为0，有弧为1，则矩阵A^n[i][j]的值表示从顶点i到顶点j中路径长度为n的数目

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

	//构造n个顶点，m条边的图
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
			return false;
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
			return false;
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

	bool find_edge(const Adj_Matrix& adj, int v, int w)
	{
		//空图
		if (adj.graph.empty())
		{
			return false;
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
	void dfs(int u, std::vector<bool>& vis, const Adj_Matrix& adj, std::function<void(int&)> address = [](int& u)->void {std::cout << u; })
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

	void do_dfs(const Adj_Matrix& adj, std::function<void(int&)> address = [](int& u)->void {std::cout << u; })
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

	//深度优先搜索（DFS）算法的非递归形式，类似于广度优先，但这里使用栈
	void dfs_non_recursive(int start, std::vector<bool>& vis, const Adj_Matrix& adj, std::function<void(int&)> address = [](int& u)->void {std::cout << u; })
	{
		//已经访问过或者start不合法
		if (vis[start] || start < 0 || start >= vis.size())return;

		std::vector<int> q;//存放当前连通分支节点

		q.push_back(start);//将起始节点入栈

		vis[start] = true;// 标记为已访问

		//栈不为空，即当前连通分支还有未访问的节点
		while (!q.empty())
		{
			int u = q.back(); // 获取栈顶的节点

			q.pop_back();           // 出栈

			address(u);       // 处理当前节点

			// 遍历当前顶点，找到所有邻接并且未访问的节点加入队列
			for (int i = 0; i < adj.graph.size(); i++)
			{
				if (adj.graph[u][i] != INT_MAX && !vis[i])
				{
					vis[i] = true;// 标记为已访问
					q.push_back(i);     // 将邻接节点入栈
				}
			}
			
		}
	}

	//图深度优先算法的非递归
	void do_dfs_non_recursive(const Adj_Matrix& adj, std::function<void(int&)> address = [](int& u)->void {std::cout << u; })
	{
		int number = adj.graph.size();

		//空图
		if (number <= 0)
			return;

		std::vector<bool> vis(number, false);

		// 遍历所有节点，确保不遗漏任何不连通的部分
		for (int i = 0; i < number; i++)
		{
			dfs_non_recursive(i, vis, adj, address);
		}

	}


	//广度优先搜索（BFS）算法,并对节点进行处理,vis保存是否访问
	void bfs(int start, std::vector<bool>& vis, const Adj_Matrix& adj, std::function<void(int&)> address = [](int& u)->void {std::cout << u; })
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
	void do_bfs(const Adj_Matrix& adj,  std::function<void(int&)> address = [](int& u)->void { std::cout << u; }) {
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
	std::vector<int> TopologicalSort(const Adj_Matrix& adj)
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
	std::vector<int> CriticalPath(const Adj_Matrix& adj)
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

	//找到无权最短路，图中所有边权值为1，求点v到其他各个点的最短路经和最短路径的长度
	//path[i]记录从 v 到 i 的最短路径上顶点i的前驱结点
	//dist[i]记录从 v 到 i 的最短路径长度
	void ShortestPath(const Adj_Matrix& adj, int v, std::vector<int>& dist, std::vector<int>& path)
	{
		//path和dist初始化由上层完成

		int number = dist.size();
		//创建一个队列
		std::queue<int> Q;
		Q.push(v); //将开始结点入队

		//自身路径为0
		dist[v] = 0;

		//求v到其他各个顶点的最短路径，类似于图的BFS
		while (!Q.empty())
		{
			//处理队头顶点
			int u = Q.front();
			Q.pop();

			//遍历当前点的所有边
			for (int i = 0; i < number; i++)
			{
				//存在边
				if (adj.graph[u][i] != INT_MAX)
				{
					int k = i;

					//dist[k]=-1表示未访问
					if (dist[k] == -1)
					{
						//将u未访问的邻接点k入队
						Q.push(k);

						//距离增加为u的距离加一
						dist[k] = dist[u] + 1;

						//更新k的在最短路的前驱结点
						path[k] = u;
					}
				}
			}
		}
	}

	// 输出从 v 到所有其他点的最短路径和路径长度
	void PrintShortestPath(int v, const std::vector<int>& dist, const std::vector<int>& path)
	{
		int n = dist.size();

		for (int i = 0; i < n; ++i)
		{
			if (i == v) continue;  // 跳过起点自身

			if (dist[i] == -1)
			{
				// 如果 dist[i] 仍为 -1，说明从 v 到 i 不可达
				std::cout << "No path from " << v << " to " << i << std::endl;
			}
			else
			{
				// 输出路径长度
				std::cout << "Shortest path length from " << v << " to " << i << ": " << dist[i] << std::endl;

				// 输出路径
				std::cout << "Path: ";
				std::vector<int> path_vec;
				for (int u = i; u != -1; u = path[u])
				{
					path_vec.push_back(u);
				}
				std::reverse(path_vec.begin(), path_vec.end());

				for (size_t j = 0; j < path_vec.size(); ++j)
				{
					std::cout << path_vec[j];
					if (j < path_vec.size() - 1)
						std::cout << " -> ";
				}
				std::cout << std::endl;
			}
		}
	}

	//ShortestPath的上层，负责创建，初始化并输出dist和path
	void SearchShortestPath(const Adj_Matrix& adj, int v)
	{
		std::vector<int> dist, path;

		int number = adj.graph.size();

		//初始化dist和path
		dist.resize(number, -1);
		path.resize(number, -1);

		ShortestPath(adj, v, dist, path);

		std::cout << "无权最短路:" << std::endl;

		PrintShortestPath(v, dist, path);

		return;
	}

	//正权最短路径,即图的权值为正实数，求点v到其他各个点的最短路经和最短路径的长度
	//可以用于无权最短路径，即权值为1;
	//找局部最优路，即在已知距离的其他点里找最小距离的点组成局部最优路，再由此点更新其他点的最短距离
	void Dijkstra_ShortestPath(const Adj_Matrix& adj, int v, std::vector<bool> vis, std::vector<int>& dist, std::vector<int>& path)
	{
		//path和dist,vis初始化由上层完成

		//自身路径为0
		dist[v] = 0;

		//标记访问
		vis[v] = true;

		int u = v;
		
		//求v到其他各个顶点的最短路径
		//访问除了起始点以外所有点
		for (int j = 0; j < adj.graph.size() - 1; j++)
		{
			//更新从u出发到未访问的所有顶点的最短路径长度
			for (int i = 0; i < adj.graph.size(); i++)
			{
				int k = i;

				//未访问过并且有更小路径
				if (!vis[k] && adj.graph[u][k]!=INT_MAX && dist[u] + adj.graph[u][k] < dist[k])
				{
					//更新k最短路径和前驱结点
					dist[k] = dist[u] + adj.graph[u][k];
					path[k] = u;
				}
			}

			//寻找当前未访问过并且有最小路径的点
			int shortest = INT_MAX;

			for (int i = 0; i < adj.graph.size(); i++)
			{
				if (dist[i] < shortest && !vis[i])
				{
					u = i;
					shortest = dist[i];
				}
			}

			//访问新顶点
			vis[u] = true;
		}
	}

	//Dijkstra_ShortestPath的上层，负责创建，初始化并输出dist和path
	void Dijkstra_SearchShortestPath(const Adj_Matrix& adj, int v)
	{
		std::vector<int> dist, path;

		//vis不仅是访问标识，更重要的是说明该点已找到最短路径
		//并且可以通过该点作为其他点的最短路径中的中转点，即找到局部最优路
		std::vector<bool> vis;

		int number = adj.graph.size();

		//初始化dist和path,vis
		vis.resize(number, false);
		dist.resize(number, 0);
		path.resize(number, -1);

		for (int i = 0; i < number; i++)
		{
			//将i到各个终点的最短路径长度初始化为弧上的权值
			dist[i] = adj.graph[v][i];

			//如果i和v之间有弧， 则将i的前驱置为v
			if (dist[i] < INT_MAX)path[i] = v;
		}
		

		Dijkstra_ShortestPath(adj, v, vis, dist, path);

		std::cout << "正权最短路:" << std::endl;

		PrintShortestPath(v, dist, path);

		return;
	}

	//对于每一对顶点之间的最短路径
	//可以以每个顶点分别调用Dijkstra算法
	//但是对于邻接矩阵，可以使用Floyd算法实现，但是时间复杂度都是O(n^3)
	void Floyd_ShortestPath(const Adj_Matrix& adj)
	{
		//将Vi到Vj的最短路径长度初始化， 即dist[i][j]= graph[i][j]
		std::vector<std::vector<int>> dist, path;

		//初始化为同邻接矩阵大小
		dist.resize(adj.graph.size(), std::vector<int>(adj.graph.size(), 0));
		path.resize(adj.graph.size(), std::vector<int>(adj.graph.size(), -1));

		//遍历图
		for (int i = 0; i < adj.graph.size(); i++)
		{
			for (int j = 0; j < adj.graph.size(); j++)
			{
				dist[i][j] = adj.graph[i][j];

				//如果i和j之间有弧，则将j的前驱置为l
				if (dist[i][j] < INT_MAX)
					path[i][j] = i; 
			}
		}

		//n次比较和更新
		for (int k = 0; k < adj.graph.size(); k++)
		{
			//每次遍历图，在Vi和Vj现有最优路径里加入顶点Vo。
			//比较 现路径 和 加入v0后的路径 长度,取较短者更新最短路径
			for (int i = 0; i < adj.graph.size(); i++)
			{
				for (int j = 0; j < adj.graph.size(); j++)
				{
					//如果更小则更新路径
					if (dist[i][k] + dist[k][j] < dist[i][j])
					{
						//更新长度和前驱
						dist[i][j] = dist[i][k] + dist[k][j];
						path[i][j] = path[k][j];
					}

				}
			}
		}

		//打印类似于n次打印Dijkstra的结果,不考虑实现
	}

	//在一个连通网的所有生成树中，各边的代价之和最小的那棵生成树
	//称为该连通网的最小代价生成树(MinimumCost Spanning Tree), 简称为最小生成树。

	//在n个顶点的网络N中构建最小支撑树
	//此最小支撑树为n-l条边和图的n个顶点构成的具有最小代价一个连通图
	//一个网络可能存在不只一棵最小支撑树

	//连通分量数
	//dfs执行次数即连通分量数
	int Connected_Component(const Adj_Matrix& adj)
	{
		// 空图
		if (adj.graph.empty()) return 0;

		//连通分量数
		int count = 0;

		std::vector<bool> vis(adj.graph.size(), false); // 创建访问标志

		// 遍历所有节点，确保不遗漏任何不连通的部分
		for (int i = 0; i < adj.graph.size(); i++)
		{
			if(!vis[i])
			{
				count++;
				dfs(i, vis, adj, [](int& u)->void {}); // 从未访问的节点开始BFS
			}
		}

		return count;

	}

	//普里姆 (Prim) 算法，又称为加点法,适用于稠密图
	//在已选顶点的集合里（起始时只有起始点）找到未选点与已选点的权值最小的边
	//将其连接点加入树中，重新更新未选点的权值最小边
	//默认此图为连通图，并返回一个图
	Adj_Matrix MiniSpanTree_Prim(const Adj_Matrix& adj,int u=0/*起始点*/)
	{
		//非连通图，返回空图
		if (Connected_Component(adj) != 1)
		{
			return Adj_Matrix(0);
		}

		//初始化一个图
		Adj_Matrix MiniSpanTree(adj.graph.size(),adj.is_direct);


		//辅助数组closedge,以记录从U到V-U具有最小权值的边
		//first存储最小边的权值，second存储最小边在 U 中的那个顶点
		std::vector<std::pair<int, int>> close_edge;
		//标记是否访问
		std::vector<bool> vis;

		vis.resize(adj.graph.size(), false);
		close_edge.resize(adj.graph.size());

		//初始化cloes_edge
		for (int i = 0; i < adj.graph.size(); i++)
		{
			if (i != u) 
			{
				close_edge[i].first = adj.graph[u][i];
				close_edge[i].second = u;
			}
		}

		//标记起始点
		vis[u] = true;

		//找到最小权值的边
		auto Min = [&]()->int {
			int minindex = -1;
			int min = INT_MAX;
			for (int i = 0; i < close_edge.size(); i++)
			{
				const auto& vex = close_edge[i];

				//未访问并且有更小权值的边
				if (!vis[i] && vex.first < min)
				{
					minindex = i;
					min = vex.first;
				}
			}
			return minindex;
		};

		//选择其余n-1个顶点，生成n-1 条边(n=G.vexnum)
		for (int j = 1; j < adj.graph.size(); j++)
		{
			//求出最小权值的边
			int u0 = Min();
			
			// 无连接点，非连通情况
			if (u == -1)
			{
				break;
			}

			//为MiniSpanTree增加边
			int v0 = close_edge[u0].second;
			MiniSpanTree.graph[u0][v0] = close_edge[u0].first;
			
			// 无向图，则则增加对称边
			if(!MiniSpanTree.is_direct)
			{
				MiniSpanTree.graph[v0][u0] = close_edge[u0].first;
			}

			//标记u0，并更新close_edge
			vis[u0] = true;

			for (int j = 0; j < close_edge.size(); j++)
			{
				if (adj.graph[u0][j] < close_edge[j].first)
				{
					close_edge[j].first = adj.graph[u0][j];
					close_edge[j].second = j;
				}
			}

		}

		//返回
		return MiniSpanTree;
	}


	//克鲁斯卡尔 (Kruskal)算法，可称为“加边法”，适用于稀疏图
	//每次选出权值最小并且无法使现有的树形成环的边加入最小支撑树
	//适合以Edge存储的树，此处不考虑实现，不过可以考虑转换


	//打印矩阵
	void PrintAdjMatrix(const Adj_Matrix& graph) {
		std::cout << "邻接矩阵：" << std::endl;
		for (const auto& row : graph.graph) {
			for (int val : row) {
				if (val == INT_MAX) {
					std::cout << "∞" << "\t";  // 用 ∞ 表示无穷大（无边）
				}
				else {
					std::cout << val << "\t";
				}
			}
			std::cout << std::endl;
		}
	}
};