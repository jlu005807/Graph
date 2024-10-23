#pragma once
#include<iostream>
#include<vector>
#include<queue>
#include<functional>
#include<algorithm>

//ʹ��һ����������ߣ������е�ÿ��Ԫ�ض�����һ���ߵ�������յ㣨����Ȩ��ͼ��������Ȩ����
//������ʹ�ö������ֱ����㣬�յ�ͱ�Ȩ����
//�˴洢�ṹ��������ͼ

struct Edge
{
	//�洢�߽ڵ�ı��
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
	
	//�����ҵ�u,v֮��ı�,��Ŵ��㿪ʼ
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
		//��ͼ
		if (graph.empty())
		{
			return;
		}

		graph.clear();

	}

	//����v�ĵ�һ���ڽӶ��㡣��v��G��û���ڽӶ��㣬�򷵻� ��-1����
	int FirstAdjVex(std::vector<Edge>& graph, int u)
	{
		//����
		for (int i = 0; i < graph.size(); i++)
		{
			if (graph[i].u == u)return graph[i].v;
		}

		return -1;
	}

	//�����µı�,����ͬ�ĺ�߲��룬��û�л����Ѿ���������������
	void InsertEdge(std::vector<Edge>& graph, int u, int v)
	{
		Edge newedge(u, v);

		auto it = std::find(graph.begin(), graph.end(), newedge);

		//�Ѵ���
		if (it == graph.end())
		{
			return;
		}

		//�ҵ�u
		for (it=graph.begin(); it != graph.end(); it++)
		{
			if (it->u == u)
			{
				break;
			}
		}

		//�ҵ���u
		if (it != graph.end())
		{
			graph.insert(it + 1, newedge);
		}
		else 
		{
			graph.insert(it, newedge);
		}
	}
	
	//ɾ��u,v�ıߣ�û����������
	void DeleteEdge(std::vector<Edge>& graph, int u, int v)
	{
		//��������
		for (auto it = graph.begin(); it != graph.end(); it++)
		{
			if (it->u == u && it->v == v)
			{
				graph.erase(it);
				return;
			}
		}
	}

	//ɾ��G�ж���v������صĻ���
	void DeleteVex(std::vector<Edge>& graph, int v)
	{
		//��������
		for (auto it = graph.begin(); it != graph.end(); it++)
		{
			if (it->u == v||it->v==v)
			{
				it = graph.erase(it);
			}
		}
	}

	//�������������DFS���㷨,���Խڵ���д���,vis�����Ƿ����
	void dfs(int u, std::vector<bool>& vis, std::vector<Edge> graph, std::function<void(int&)> address = [](int& u)->void {std::cout << u; })
	{
		//�Ѿ����ʹ�
		if (vis[u])return;

		//��Ƿ���
		vis[u] = true;

		//����ǰ�ڵ�
		address(u);

		//�ҵ��ڽӵĵ����
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
		//��ͼ
		if (number <= 0)
			return;

		std::vector<bool> vis(number, false);

		// �������нڵ㣬ȷ������©�κβ���ͨ�Ĳ���
		for (int i = 0; i < number; i++)
		{
			dfs(i, vis, graph, address);
		}

	}

	//�������������BFS���㷨,���Խڵ���д���,vis�����Ƿ����
	void bfs(int start, std::vector<bool>& vis, std::vector<Edge> graph, std::function<void(int&)> address = [](int& u)->void {std::cout << u; })
	{
		//�Ѿ�����
		if (vis[start])
		{
			return;
		}

		std::queue<int> q;//��ŵ�ǰ��ͨ��֧�ڵ㣬��Ҫ��start�ڵ�һȦȦ����

		q.push(start);//����ʼ�ڵ����

		vis[start] = true;// ���Ϊ�ѷ���

		//���в�Ϊ�գ�����ǰ��ͨ��֧����Ϊ���ʵĽڵ�
		while (!q.empty())
		{
			int u = q.front(); // ��ȡ����ǰ�˵Ľڵ�

			q.pop();           // ����

			address(u);       // ����ǰ�ڵ�

			// ����ͼ���ҵ������ڽӵĽڵ�������
			for (const auto& edge : graph)
			{
				if (edge.u == u && !vis[edge.v]) 
				{
					vis[edge.v] = true; // ���Ϊ�ѷ���
					q.push(edge.v);     // ���ڽӽڵ����
				}
			}
		}

	}

	// ִ��BFS����
	void do_bfs(std::vector<Edge> graph, int number, std::function<void(int&)> address = [](int& u)->void { std::cout << u ; }) {
		// ��ͼ
		if (number <= 0) return;

		std::vector<bool> vis(number, false); // �������ʱ�־

		// �������нڵ㣬ȷ������©�κβ���ͨ�Ĳ���
		for (int i = 0; i < number; i++) 
		{
			bfs(i, vis, graph, address); // ��δ���ʵĽڵ㿪ʼBFS
		}
	}

};