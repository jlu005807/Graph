#pragma once
#include<iostream>
#include<vector>
#include<queue>
#include<functional>
#include<algorithm>

//使用一个数组来存边，数组中的每个元素都包含一条边的起点与终点（带边权的图还包含边权）。
//（或者使用多个数组分别存起点，终点和边权。）
//此存储结构用于有向图

struct Edge
{
	//存储边节点的编号
	int u;
	int v;

	Edge() = default;

	Edge(int _u, int _v) :u(_u), v(_v)
	{

	}

	bool operator==(const Edge& other)
	{
		return this->u == other.u && this->v == other.v;
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
		if (it == graph.end())
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
	void dfs(int u, std::vector<bool>& vis, std::vector<Edge> graph, std::function<void(int&)> address = [](int& u)->void {std::cout << u; })
	{
		//已经访问过
		if (vis[u])return;

		//标记访问
		vis[u] = true;

		//处理当前节点
		address(u);

		//找到邻接的点访问
		for (auto i = 0; i < vis.size(); i++)
		{
			if (graph[i].u == u)
			{
				dfs(graph[i].v, vis, graph, address);
			}
		}



	}

	void do_dfs(std::vector<Edge> graph, int number, std::function<void(int&)> address = [](int& u)->void {std::cout << u; })
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

	//广度优先搜索（BFS）算法,并对节点进行处理,vis保存是否访问
	void bfs(int start, std::vector<bool>& vis, std::vector<Edge> graph, std::function<void(int&)> address = [](int& u)->void {std::cout << u; })
	{
		//已经访问
		if (vis[start])
		{
			return;
		}

		std::queue<int> q;//存放当前连通分支节点，并要着start节点一圈圈增加

		q.push(start);//将起始节点入队

		vis[start] = true;// 标记为已访问

		//队列不为空，即当前连通分支还有为访问的节点
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
	void do_bfs(std::vector<Edge> graph, int number, std::function<void(int&)> address = [](int& u)->void { std::cout << u ; }) {
		// 空图
		if (number <= 0) return;

		std::vector<bool> vis(number, false); // 创建访问标志

		// 遍历所有节点，确保不遗漏任何不连通的部分
		for (int i = 0; i < number; i++) 
		{
			bfs(i, vis, graph, address); // 从未访问的节点开始BFS
		}
	}

};