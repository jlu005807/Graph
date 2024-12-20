#pragma once
#include<iostream>
#include<vector>
#include<queue>
#include<functional>
#include<algorithm>

//使用一个数组来存边，数组中的每个元素都包含一条边的起点与终点（带边权的图还包含边权）。
//（或者使用多个数组分别存起点，终点和边权。）
//此存储结构默认用于有向图，如果为无向图，需要存放对称边
//此存储结构不考虑无边图，即零图

struct Edge
{
	//存储边节点的编号
	//u->v
	int u;
	int v;

	//加入权值
	int weight;

	Edge() = default;

	Edge(int _u, int _v,int weight=1) :u(_u), v(_v),weight(weight)
	{

	}


	bool operator==(const Edge& other)
	{
		return this->u == other.u && this->v == other.v;
	}


	Edge& operator=(const Edge& other)
	{
		//判断自身
		if (this == &other)
		{
			return *this;
		}

		this->u = other.u;
		this->v = other.v;
		this->weight = other.weight;

		return *this;
	}
};


class Edge_manager
{
public:
	
	//尝试找到u,v之间的边,编号从零开始
	bool find_edge(int u, int v, std::vector<Edge> graph)
	{
		for (int i = 0; i < graph.size(); i++)
		{
			if (graph[i].u == u && graph[i].v==v)
			{
				return true;
			}
		}
		return false;
	}

	std::vector<Edge> Init_Graph(int a[][2],int n)
	{
		std::vector<Edge> graph;
		for (int i = 0; i < n; i++)
		{
			Edge one(a[i][0], a[i][1]);
			graph.push_back(one);
		}

		return graph;
	}

	std::vector<Edge> Init_Graph(int a[][2], int n,std::vector<int> weights)
	{
		std::vector<Edge> graph;
		for (int i = 0; i < n; i++)
		{
			Edge one(a[i][0], a[i][1], weights[i]);
			graph.push_back(one);
		}

		return graph;
	}
	

	void Destroy(std::vector<Edge>& graph)
	{
		//空图
		if (graph.empty())
		{
			return;
		}

		graph.clear();

	}

	//返回v的第一个邻接顶点。若v在G中没有邻接顶点，则返回 “-1”。
	int FirstAdjVex(std::vector<Edge>& graph, int u)
	{
		//遍历
		for (int i = 0; i < graph.size(); i++)
		{
			if (graph[i].u == u)return graph[i].v;
		}

		return -1;
	}

	//插入新的边,在相同的后边插入，如没有或者已经存在则在最后插入
	void InsertEdge(std::vector<Edge>& graph, int u, int v)
	{
		Edge newedge(u, v);

		auto it = std::find(graph.begin(), graph.end(), newedge);

		//已存在
		if (it != graph.end())
		{
			return;
		}

		//找到u
		for (it=graph.begin(); it != graph.end(); it++)
		{
			if (it->u == u)
			{
				break;
			}
		}

		//找到了u
		if (it != graph.end())
		{
			graph.insert(it + 1, newedge);
		}
		else 
		{
			graph.insert(it, newedge);
		}
	}
	
	//删除u,v的边，没有则无意义
	void DeleteEdge(std::vector<Edge>& graph, int u, int v)
	{
		//遍历查找
		for (auto it = graph.begin(); it != graph.end(); it++)
		{
			if (it->u == u && it->v == v)
			{
				graph.erase(it);
				return;
			}
		}
	}

	//删除G中顶点v及其相关的弧。
	void DeleteVex(std::vector<Edge>& graph, int v)
	{
		//遍历查找
		for (auto it = graph.begin(); it != graph.end(); it++)
		{
			if (it->u == v||it->v==v)
			{
				it = graph.erase(it);
			}
		}
	}

	//深度优先搜索（DFS）算法,并对节点进行处理,vis保存是否访问
	void dfs(int u, std::vector<bool>& vis, const std::vector<Edge>& graph, std::function<void(int&)> address = [](int& u)->void {std::cout << u; })
	{
		//已经访问过或者u不合法
		if (vis[u]||u<0||u>=vis.size())return;

		//标记访问
		vis[u] = true;

		//处理当前节点
		address(u);

		//找到邻接的点访问
		for (auto i = 0; i < graph.size(); i++)
		{
			if (graph[i].u == u)
			{
				dfs(graph[i].v, vis, graph, address);
			}
		}

	}

	void do_dfs(const std::vector<Edge>& graph, int number, std::function<void(int&)> address = [](int& u)->void {std::cout << u; })
	{
		//空图
		if (number <= 0)
			return;

		std::vector<bool> vis(number, false);

		// 遍历所有节点，确保不遗漏任何不连通的部分
		for (int i = 0; i < number; i++)
		{
			dfs(i, vis, graph, address);
		}

	}

	//深度优先搜索（DFS）算法的非递归形式，类似于广度优先，但这里使用栈
	void dfs_non_recursive(int start, std::vector<bool>& vis, const std::vector<Edge>& graph, std::function<void(int&)> address = [](int& u)->void {std::cout << u; })
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

			// 遍历图，找到所有邻接的节点加入队列
			for (const auto& edge : graph)
			{
				if (edge.u == u && !vis[edge.v])
				{
					vis[edge.v] = true; // 标记为已访问
					q.push_back(edge.v);     // 将邻接节点入栈
				}
			}
		}
	}

	//图深度优先算法的非递归
	void do_dfs_non_recursive(const std::vector<Edge>& graph, int number, std::function<void(int&)> address = [](int& u)->void {std::cout << u; })
	{
		//空图
		if (number <= 0)
			return;

		std::vector<bool> vis(number, false);

		// 遍历所有节点，确保不遗漏任何不连通的部分
		for (int i = 0; i < number; i++)
		{
			dfs_non_recursive(i, vis, graph, address);
		}

	}

	//广度优先搜索（BFS）算法,并对节点进行处理,vis保存是否访问
	void bfs(int start, std::vector<bool>& vis, const std::vector<Edge>& graph, std::function<void(int&)> address = [](int& u)->void {std::cout << u; })
	{
		//已经访问过或者start不合法
		if (vis[start] || start < 0 || start >= vis.size())return;

		std::queue<int> q;//存放当前连通分支节点，并要着start节点一圈圈增加

		q.push(start);//将起始节点入队

		vis[start] = true;// 标记为已访问

		//队列不为空，即当前连通分支还有未访问的节点
		while (!q.empty())
		{
			int u = q.front(); // 获取队列前端的节点

			q.pop();           // 出队

			address(u);       // 处理当前节点

			// 遍历图，找到所有邻接的节点加入队列
			for (const auto& edge : graph)
			{
				if (edge.u == u && !vis[edge.v]) 
				{
					vis[edge.v] = true; // 标记为已访问
					q.push(edge.v);     // 将邻接节点入队
				}
			}
		}

	}

	// 执行BFS遍历
	void do_bfs(const std::vector<Edge>& graph, int number, std::function<void(int&)> address = [](int& u)->void { std::cout << u ; }) {
		// 空图
		if (number <= 0) return;

		std::vector<bool> vis(number, false); // 创建访问标志

		// 遍历所有节点，确保不遗漏任何不连通的部分
		for (int i = 0; i < number; i++) 
		{
			bfs(i, vis, graph, address); // 从未访问的节点开始BFS
		}
	}

	//拓扑排序就是将AOV - 网中所有顶点排成一个线性序列，该序列满足：但是该序列不唯一
	// 若在AOV - 网中由顶点Vi, 到顶点Vj；有一条路径，则在该线性序列中的顶点Vi; 必定在顶点Vj之前。
	//在此函数里默认为有向无环图（DAG)
	std::vector<int> TopologicalSort(const std::vector<Edge>& Graph, int number)
	{
		std::vector<int> Topo;//记录拓扑排序

		std::queue<int> zero_in_degree;//记录入度为0的顶点

		//存放各个顶点的入度
		std::vector<int> indegree(number,0);

		//计算各个顶点的入度
		for (const auto edge : Graph)
		{
			indegree[edge.v]++;
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
		//选择入度为0的节点，输出到结果序列中；
		//删除该节点以及该节点的边；
		//重复执行步骤1和2，直到所有节点输出到结果序列中，完成拓扑排序；
		// 如果最后还存在入度不为0的节点，说明有向图中存在环，无法进行拓扑排序。
		while (!zero_in_degree.empty())
		{
			//取出一个节点处理
			int u = zero_in_degree.front();
			zero_in_degree.pop();
			
			//放入Topo
			Topo.push_back(u);

			//删除相应的边,在indegree中逻辑删除
			for (const auto& edge : Graph)
			{

				if (edge.u == u)
				{
					//相应顶点入度减一
					indegree[edge.v]--;

					//如果为0放入zero_in_degree
					if (indegree[edge.v] == 0)
					{
						zero_in_degree.push(edge.v);
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


	//AOE-网是一个带权的有向无环图
	//其中，顶点表示事件， 弧表示活动，权表示活动持续的时间。
	//要估算整项工程完成的最短时间，就是要找一条从源点到汇点的带权路径长度最长的路径，称为关键路径(Critical Path)。
	//故在正常的情况（无环）下，网中只有一个入度为零的点，称作源点，也只有一个出度为零的点，称作汇点。
	std::vector<int> CriticalPath(const std::vector<Edge>& Graph, int number)
	{
		//求出拓扑排序
		std::vector<int> Topo = TopologicalSort(Graph, number);
		//有环
		if (Topo.empty())
		{
			std::cout << "图中存在环，无法计算关键路径。" << std::endl;
			return {};
		}

		//初始化各顶点的最早发生时间（VE）,对于开始点即拓扑排序第一个点的VT为0
		std::vector<int> VE(number, 0);

		//计算各个顶点的VE
		for (const auto& u : Topo)
		{
			for (int i = 0; i < Graph.size(); i++)
			{
				//更新当前节点的邻接点的VE
				if (Graph[i].u == u)
				{
					int v = Graph[i].v;

					VE[v] = std::max(VE[v], VE[u] + Graph[i].weight);
				}
			}
		}

		//初始化各节点的最晚发生时间（VL）,对于结束点即拓扑排序最后一个点的LT为VE[number - 1]
		std::vector<int> VL(number, VE[number - 1]);


		//逆拓扑排序计算各个顶点VL
		for (auto it = Topo.rbegin(); it != Topo.rend(); it++)
		{
			int u = *it;
			for (int i = 0; i < Graph.size(); i++)
			{
				if (Graph[i].u == u)
				{
					VL[u] = std::min(VL[u], VL[Graph[i].v] - Graph[i].weight);
				}
			}
		}

		//计算各个弧即活动的最早开始时间ET，即各个弧起始点即弧尾的VE
		std::vector<int> ET(Graph.size(), 0);

		for (int i = 0; i < Graph.size(); i++)
		{
			ET[i] = VE[Graph[i].u];
		}

		//计算弧i即活动的最晚开始时间LT，即各个弧结束点即弧头的VL-weights[i]
		std::vector<int> LT(Graph.size(), 0);

		for (int i = 0; i < Graph.size(); i++)
		{
			LT[i] = VL[Graph[i].v] - Graph[i].weight;
		}



		//关键路径即那些ET=LT的活动即弧，这些活动无法拖延，必须同步工程实现
		//如果ET不等于LT,那么ET和LT的差值就是这些活动可以拖延的时间即活动的时间余量
		//识别关键路径并保存关键路径节点序列，如果有多个关键路径只求其中一个并且路径存放弧的编号
		std::vector<int> critical_path;

		for (int i = 0; i < Graph.size(); i++)
		{
			//第一个路径
			if (ET[i] == LT[i] && critical_path.empty())
			{
				critical_path.push_back(i);
			}
			else if(ET[i] == LT[i])//除了第一个路径后接下来的路径必须考虑连续性即头尾相连，避免将多个关键路径返回
			{
				int k = critical_path.back();
				if (Graph[k].v == Graph[i].u)
				{
					critical_path.push_back(i);
				}
			}
		}

		std::cout << "项目的最早完成时间为：" << VE[number-1] << std::endl;
		return critical_path;  // 返回关键路径
		
	}

	//找到无权最短路，图中所有边权值为1，求点v到其他各个点的最短路经和最短路径的长度
	//path[i]记录从 v 到 i 的最短路径上顶点i的前驱结点
	//dist[i]记录从 v 到 i 的最短路径长度
	void ShortestPath(const std::vector<Edge> graph, int v, std::vector<int>& dist, std::vector<int>& path)
	{
		//path和dist初始化由上层完成

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

			//遍历边
			for(int i=0;i<graph.size();i++)
			{
				//找到u的邻接点
				if(graph[i].u==u)
				{
					int k = graph[i].v; //记录当前邻接点

					//dist[v]=-1表示未访问
					if (dist[k] == -1)
					{
						//将u未访问的邻接点v入队
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

	

	//ShortestPath的上层，负责创建，初始化并输出dist和path,对于Edge存放的图没有具体顶点数，需要输入一个
	void SearchShortestPath(const std::vector<Edge> graph,int number, int v)
	{
		std::vector<int> dist, path;

		//初始化dist和path
		dist.resize(number, -1);
		path.resize(number, -1);

		ShortestPath(graph, v, dist, path);

		std::cout << "无权最短路:" << std::endl;
		
		PrintShortestPath(v, dist, path);

		return;
	}

	// 输出从 v 到所有其他点的最短路径和路径长度
	void PrintShortestPath(int v, const std::vector<int>& dist, const std::vector<int>& path)
	{
		int n = dist.size();

		for (int i = 0; i < n; ++i)
		{
			if (i == v) continue;  // 跳过起点自身

			if (dist[i] == -1 || dist[i] == INT_MAX)
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

	//正权最短路径,即图的权值为正实数，求点v到其他各个点的最短路经和最短路径的长度
	//可以用于无权最短路径，即权值为1;
	//找局部最优路，即在已知距离的其他点里找最小距离的点组成局部最优路，再由此点更新其他点的最短距离
	void Dijkstra_ShortestPath(const std::vector<Edge>& graph, int number,int v, std::vector<bool>& vis,std::vector<int>& dist, std::vector<int>& path)
	{
		//path和dist,vis初始化由上层完成
		
		//自身路径为0
		dist[v] = 0;

		//标记访问
		vis[v] = true;
		
		int u = v;
		
		//求v到其他各个顶点的最短路径
		//访问除了起始点以外所有点
		for (int j = 0; j < number-1; j++)
		{
			//从目前最小路径的点出发更新未访问点的最小路径
			for (int i = 0; i < graph.size(); i++)
			{
				auto p = graph[i];
				int info = graph[i].weight;

				if(p.u==u)
				{
					int k = p.v;

					//未访问过并且有更小路径
					if (!vis[k] && dist[u] + info < dist[k])
					{
						//更新最短路径和前驱结点
						dist[k] = dist[u] + info;
						path[k] = u;
					}
				}

			}

			//寻找当前未访问过并且有最小路径的点
			int shortest = INT_MAX;

			for (int i = 0; i < number; i++)
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

	//利用优先队列维护最短路长度最小的结点，适用于稀疏图
	void Dijkstra_ShortestPath_optimize(const std::vector<Edge>& graph, int number, int v, std::vector<bool>& vis, std::vector<int>& dist, std::vector<int>& path)
	{
		//创建优先队列，利用pair<T,int>分别存放距离，节点,默认T有greater
		std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;

		//初始化
		dist[v] = 0;

		//放入队列
		pq.push({ 0,v });

		//直到队列为空则完成
		while (!pq.empty())
		{
			//取当前最短路长度最小的结点
			std::pair<int, int> node = pq.top();
			pq.pop();

			//如果此点已经找到最小路径
			int u = node.second;
			if (vis[u])continue;

			////如果有已经更新的最短路径,放弃这个记录
			//if (node.first > dist[u])continue;
			//此操作和上面判断相同

			//标记
			vis[u] = true;

			//更新从此点出发的其他点最短路径
			for(int i=0;i<graph.size();i++)
			{
				if (graph[i].u != u)continue;

				int adjvex = graph[i].v;
				int w = graph[i].weight;

				//更小则更新
				if (dist[u] + w < dist[adjvex])
				{
					//更新路径和前驱并放入队列
					dist[adjvex] = dist[u] + w;
					path[adjvex] = u;
					pq.push({ dist[adjvex] ,adjvex });
				}
			}
		}
	}

	//Dijkstra_ShortestPath的上层，负责创建，初始化并输出dist和path
	void Dijkstra_SearchShortestPath(const std::vector<Edge>& graph,int number, int v)
	{
		std::vector<int> dist, path;

		//vis不仅是访问标识，更重要的是说明该点已找到最短路径
		//并且可以通过该点作为其他点的最短路径中的中转点，即找到局部最优路
		std::vector<bool> vis;


		//初始化dist和path,vis
		dist.resize(number, INT_MAX);//无边为无穷大
		path.resize(number, -1);

		vis.resize(number, false);

		Dijkstra_ShortestPath(graph,number, v, vis,dist, path);

		std::cout << "正权最短路:" << std::endl;

		PrintShortestPath(v, dist, path);

		return;
	}

	//对于每一对顶点之间的最短路径
	//可以以每个顶点分别调用Dijkstra算法

	//连通分量数
	//dfs执行次数即连通分量数
	int Connected_Component(const std::vector<Edge>& graph,int number)
	{
		// 空图
		if (number<=0) return 0;

		//连通分量数
		int count = 0;

		std::vector<bool> vis(number, false); // 创建访问标志

		// 遍历所有节点，确保不遗漏任何不连通的部分
		for (int i = 0; i <number; i++)
		{
			if (!vis[i])
			{
				count++;
				bfs(i, vis, graph, [](int& u)->void {}); // 从未访问的节点开始BFS
			}
		}

		return count;

	}
    

	//普里姆 (Prim) 算法，又称为加点法,适用于稠密图
	//在已选顶点的集合里（起始时只有起始点）找到未选点与已选点的权值最小的边
	//将其连接点加入树中，重新更新未选点的权值最小边
	//默认此图为连通图，并返回一个图,此方法适用于邻接矩阵，此处不考虑实现


	//克鲁斯卡尔 (Kruskal)算法，可称为“加边法”，适用于稀疏图
	//每次选出权值最小并且无法使现有的树形成环的边加入最小支撑树,返回一个图
	std::vector<Edge> MiniSpanTree_Kruskal(std::vector<Edge> graph,int number)
	{
		//非连通图
		if (Connected_Component(graph,number) != 1)
		{
			return{};
		}

		std::vector<Edge> MiniSpanTree;

		//排序graph的边，使其从以权值从小到大排序，利用冒泡排序
		//bool flag = true;
		//while(flag)
		//{
		//	flag = false;
		//	for (int i = 0; i < graph.size() - 1; i++)
		//	{
		//		if (weights[i] > weights[i + 1])
		//		{
		//			auto p = graph[i];
		//			graph[i + 1] = graph[i];
		//			graph[i] = p;

		//			auto q = weights[i];
		//			weights[i] = weights[i + 1];
		//			weights[i + 1] = q;

		//			flag = true;
		//		}
		//	}
		//}
		//在Edge增加了weight成员存储权值，可以直接用sort排序
		std::sort(graph.begin(), graph.end(), [](const Edge& a, const Edge& b)->bool {return a.weight < b.weight; });

		//辅助数组Vexset,标识各个顶点所属的连通分量,类似于并查集
		std::vector<int> Vexset;
		Vexset.resize(number,0);
		//初始化,表示各顶点自成一个连通分址
		for (int i = 0; i < number; i++)
		{
			Vexset[i] = i;
		}

		//开始创建最小支撑树
		for (int i = 0; i < graph.size(); i++)
		{
			int v1 = graph[i].u;
			int v2 = graph[i].v;
			int vs1 = Vexset[v1];
			int vs2 = Vexset[v2];

			//边的两个顶点不在同一连通分量
			if (vs1 != vs2)
			{
				//加入此边
				MiniSpanTree.push_back(Edge(v1, v2));

				//合并vs1和vs2两个分量，即两个集合统一编号
				for (int j = 0; j < number; j++)
				{
					if (Vexset[j] == vs2)Vexset[j] = vs1;
				}
			}
		}

		//返回
		return MiniSpanTree;
	}

	//打印函数
	void PrintGraph(const std::vector<Edge>& graph)
	{
		for (const auto& edge : graph) {
			std::cout << edge.u << " -> " << edge.v << " weight:" << edge.weight << std::endl;
		};
	}
};