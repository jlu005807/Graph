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
	//u->v
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
		if (it != graph.end())
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
		//�Ѿ����ʹ�����u���Ϸ�
		if (vis[u]||u<0||u>=vis.size())return;

		//��Ƿ���
		vis[u] = true;

		//����ǰ�ڵ�
		address(u);

		//�ҵ��ڽӵĵ����
		for (auto i = 0; i < graph.size(); i++)
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

	//�������������DFS���㷨�ķǵݹ���ʽ�������ڹ�����ȣ�������ʹ��ջ
	void dfs_non_recursive(int start, std::vector<bool>& vis, std::vector<Edge> graph, std::function<void(int&)> address = [](int& u)->void {std::cout << u; })
	{
		//�Ѿ����ʹ�����start���Ϸ�
		if (vis[start] || start < 0 || start >= vis.size())return;

		std::vector<int> q;//��ŵ�ǰ��ͨ��֧�ڵ�

		q.push_back(start);//����ʼ�ڵ���ջ

		vis[start] = true;// ���Ϊ�ѷ���

		//ջ��Ϊ�գ�����ǰ��ͨ��֧����δ���ʵĽڵ�
		while (!q.empty())
		{
			int u = q.back(); // ��ȡջ���Ľڵ�

			q.pop_back();           // ��ջ

			address(u);       // ����ǰ�ڵ�

			// ����ͼ���ҵ������ڽӵĽڵ�������
			for (const auto& edge : graph)
			{
				if (edge.u == u && !vis[edge.v])
				{
					vis[edge.v] = true; // ���Ϊ�ѷ���
					q.push_back(edge.v);     // ���ڽӽڵ���ջ
				}
			}
		}
	}

	//ͼ��������㷨�ķǵݹ�
	void do_dfs_non_recursive(std::vector<Edge> graph, int number, std::function<void(int&)> address = [](int& u)->void {std::cout << u; })
	{
		//��ͼ
		if (number <= 0)
			return;

		std::vector<bool> vis(number, false);

		// �������нڵ㣬ȷ������©�κβ���ͨ�Ĳ���
		for (int i = 0; i < number; i++)
		{
			dfs_non_recursive(i, vis, graph, address);
		}

	}

	//�������������BFS���㷨,���Խڵ���д���,vis�����Ƿ����
	void bfs(int start, std::vector<bool>& vis, std::vector<Edge> graph, std::function<void(int&)> address = [](int& u)->void {std::cout << u; })
	{
		//�Ѿ����ʹ�����start���Ϸ�
		if (vis[start] || start < 0 || start >= vis.size())return;

		std::queue<int> q;//��ŵ�ǰ��ͨ��֧�ڵ㣬��Ҫ��start�ڵ�һȦȦ����

		q.push(start);//����ʼ�ڵ����

		vis[start] = true;// ���Ϊ�ѷ���

		//���в�Ϊ�գ�����ǰ��ͨ��֧����δ���ʵĽڵ�
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

	//����������ǽ�AOV - �������ж����ų�һ���������У����������㣺���Ǹ����в�Ψһ
	// ����AOV - �����ɶ���Vi, ������Vj����һ��·�������ڸ����������еĶ���Vi; �ض��ڶ���Vj֮ǰ��
	//�ڴ˺�����Ĭ��Ϊ�����޻�ͼ��DAG)
	std::vector<int> TopologicalSort(std::vector<Edge> Graph, int number)
	{
		std::vector<int> Topo;//��¼��������

		std::queue<int> zero_in_degree;//��¼���Ϊ0�Ķ���

		//��Ÿ�����������
		std::vector<int> indegree(number,0);

		//���������������
		for (const auto edge : Graph)
		{
			indegree[edge.v]++;
		}

		//�����Ϊ0�ĵ����zero_in_degree��
		for (int i = 0; i < number; i++)
		{
			if (indegree[i] == 0)
			{
				zero_in_degree.push(i);
			}
		}

		//Kahn�㷨
		//ѡ�����Ϊ0�Ľڵ㣬�������������У�
		//ɾ���ýڵ��Լ��ýڵ�ıߣ�
		//�ظ�ִ�в���1��2��ֱ�����нڵ��������������У������������
		// �����󻹴�����Ȳ�Ϊ0�Ľڵ㣬˵������ͼ�д��ڻ����޷�������������
		while (!zero_in_degree.empty())
		{
			//ȡ��һ���ڵ㴦��
			int u = zero_in_degree.front();
			zero_in_degree.pop();
			
			//����Topo
			Topo.push_back(u);

			//ɾ����Ӧ�ı�,��indegree���߼�ɾ��
			for (const auto& edge : Graph)
			{

				if (edge.u == u)
				{
					//��Ӧ������ȼ�һ
					indegree[edge.v]--;

					//���Ϊ0����zero_in_degree
					if (indegree[edge.v] == 0)
					{
						zero_in_degree.push(edge.v);
					}
				}
			}

		}

		//����Ƿ���ڻ�
		if (Topo.size() != number)
		{
			std::cerr << "Graph has a cycle, topological sorting not possible." << std::endl;
			return {};
		}

		//����Topo
		return Topo;

	}


	//AOE-����һ����Ȩ�������޻�ͼ
	//���У������ʾ�¼��� ����ʾ���Ȩ��ʾ�������ʱ�䡣
	//Ҫ�����������ɵ����ʱ�䣬����Ҫ��һ����Դ�㵽���Ĵ�Ȩ·���������·������Ϊ�ؼ�·��(Critical Path)��
	//����������������޻����£�����ֻ��һ�����Ϊ��ĵ㣬����Դ�㣬Ҳֻ��һ������Ϊ��ĵ㣬������㡣
	std::vector<int> CriticalPath(std::vector<Edge> Graph, std::vector<int>& weights, int number)
	{
		//�����������
		std::vector<int> Topo = TopologicalSort(Graph, number);
		//�л�
		if (Topo.empty())
		{
			std::cout << "ͼ�д��ڻ����޷�����ؼ�·����" << std::endl;
			return {};
		}

		//��ʼ������������緢��ʱ�䣨VE��,���ڿ�ʼ�㼴���������һ�����VTΪ0
		std::vector<int> VE(number, 0);

		//������������VE
		for (const auto& u : Topo)
		{
			for (int i = 0; i < Graph.size(); i++)
			{
				//���µ�ǰ�ڵ���ڽӵ��VE
				if (Graph[i].u == u)
				{
					int v = Graph[i].v;

					VE[v] = std::max(VE[v], VE[u] + weights[i]);
				}
			}
		}

		//��ʼ�����ڵ��������ʱ�䣨VL��,���ڽ����㼴�����������һ�����LTΪVE[number - 1]
		std::vector<int> VL(number, VE[number - 1]);


		//��������������������VL
		for (auto it = Topo.rbegin(); it != Topo.rend(); it++)
		{
			int u = *it;
			for (int i = 0; i < Graph.size(); i++)
			{
				if (Graph[i].u == u)
				{
					VL[u] = std::min(VL[u], VL[Graph[i].v] - weights[i]);
				}
			}
		}

		//�����������������翪ʼʱ��ET������������ʼ�㼴��β��VE
		std::vector<int> ET(Graph.size(), 0);

		for (int i = 0; i < Graph.size(); i++)
		{
			ET[i] = VE[Graph[i].u];
		}

		//���㻡i���������ʼʱ��LT���������������㼴��ͷ��VL-weights[i]
		std::vector<int> LT(Graph.size(), 0);

		for (int i = 0; i < Graph.size(); i++)
		{
			LT[i] = VL[Graph[i].v] - weights[i];
		}



		//�ؼ�·������ЩET=LT�Ļ��������Щ��޷����ӣ�����ͬ������ʵ��
		//���ET������LT,��ôET��LT�Ĳ�ֵ������Щ��������ӵ�ʱ�伴���ʱ������
		//ʶ��ؼ�·��������ؼ�·���ڵ����У�����ж���ؼ�·��ֻ������һ������·����Ż��ı��
		std::vector<int> critical_path;

		for (int i = 0; i < Graph.size(); i++)
		{
			//��һ��·��
			if (ET[i] == LT[i] && critical_path.empty())
			{
				critical_path.push_back(i);
			}
			else if(ET[i] == LT[i])//���˵�һ��·�����������·�����뿼�������Լ�ͷβ���������⽫����ؼ�·������
			{
				int k = critical_path.back();
				if (Graph[k].v == Graph[i].u)
				{
					critical_path.push_back(i);
				}
			}
		}

		std::cout << "��Ŀ���������ʱ��Ϊ��" << VE[number-1] << std::endl;
		return critical_path;  // ���عؼ�·��
		
	}

	//�ҵ���Ȩ���·��ͼ�����б�ȨֵΪ1�����v����������������·�������·���ĳ���
	//path[i]��¼�� v �� i �����·���϶���i��ǰ�����
	//dist[i]��¼�� v �� i �����·������
	void ShortestPath(const std::vector<Edge> graph, int v, std::vector<int>& dist, std::vector<int>& path)
	{
		//path��dist��ʼ�����ϲ����

		//����һ������
		std::queue<int> Q;
		Q.push(v); //����ʼ������

		//����·��Ϊ0
		dist[v] = 0;

		//��v������������������·����������ͼ��BFS
		while (!Q.empty())
		{
			//�����ͷ����
			int u = Q.front();
			Q.pop();

			//������
			for(int i=0;i<graph.size();i++)
			{
				//�ҵ�u���ڽӵ�
				if(graph[i].u==u)
				{
					int k = graph[i].v; //��¼��ǰ�ڽӵ�

					//dist[v]=-1��ʾδ����
					if (dist[k] == -1)
					{
						//��uδ���ʵ��ڽӵ�v���
						Q.push(k);

						//��������Ϊu�ľ����һ
						dist[k] = dist[u] + 1;

						//����k�������·��ǰ�����
						path[k] = u;
					}
				}
			}
		}
	}

	// ����� v ����������������·����·������
	void PrintShortestPath(int v, const std::vector<int>& dist, const std::vector<int>& path)
	{
		int n = dist.size();

		for (int i = 0; i < n; ++i)
		{
			if (i == v) continue;  // �����������

			if (dist[i] == -1)
			{
				// ��� dist[i] ��Ϊ -1��˵���� v �� i ���ɴ�
				std::cout << "No path from " << v << " to " << i << std::endl;
			}
			else
			{
				// ���·������
				std::cout << "Shortest path length from " << v << " to " << i << ": " << dist[i] << std::endl;

				// ���·��
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

	//ShortestPath���ϲ㣬���𴴽�����ʼ�������dist��path,����Edge��ŵ�ͼû�о��嶥��������Ҫ����һ��
	void SearchShortestPath(const std::vector<Edge> graph,int number, int v)
	{
		std::vector<int> dist, path;

		//��ʼ��dist��path
		dist.resize(number, -1);
		path.resize(number, -1);

		ShortestPath(graph, v, dist, path);

		std::cout << "��Ȩ���·:" << std::endl;
		
		PrintShortestPath(v, dist, path);

		return;
	}


};