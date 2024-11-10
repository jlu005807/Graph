#pragma once
#include<iostream>
#include<vector>
#include<queue>
#include<functional>
#include<climits>

//ʹ��һ����ά���� adj ����ߣ����� adj[u][v] Ϊ 1 ��ʾ���� u �� v �ıߣ�Ϊ 0 ��ʾ�����ڡ�
//����Ǵ���Ȩ��ͼ�������� adj[u][v] �д洢 u �� v �ıߵı�Ȩ��
//Ĭ�϶��㵽����û�б�
//���Ҷ�ά�������Ըı�ṹ���Բ��������Ӻ�ɾ���ڵ㵫�ǿ�����ɾ�ߣ����Ҷ�ά����ռ��������ϡ��ͼ
//��ʵ����ͼΪ�Գƾ������ѹ��Ϊһά�����ţ�����ʵ���߼����Ʋ����Ƿ���  


//С���ɣ�������������ͼͼ�����ҹ涨û�л�Ϊ0���л�Ϊ1�������A^n[i][j]��ֵ��ʾ�Ӷ���i������j��·������Ϊn����Ŀ

class Adj_Matrix
{
public:
	std::vector<std::vector<int>> graph;

	//��־�Ƿ�Ϊ����ͼ
	bool is_direct;

	Adj_Matrix() = default;

	//��������number,���ɴ��㿪ʼ,��weightĬ��ʱ��ʾ��Ȩͼ
	Adj_Matrix(int number,bool direct=false, int weight = INT_MAX):is_direct(direct)
	{
		graph.resize(number, std::vector<int>(number,weight));
	}
};


class AdjMatrix_manager
{
public:

	//����n�����㣬m���ߵ�ͼ
	Adj_Matrix CreateGraph(int n, int m, int edge[][2],bool direct)
	{
		//��ʼ���ڽӾ���
		Adj_Matrix one(n,direct);

		//�����
		for (int i = 0; i < m; i++)
		{
			int u = edge[i][0];
			int v = edge[i][1];

			one.graph[u][v] = 1;

			//�����������ͼ����ԳƵ�ͬ����ֵ
			if (!one.is_direct)
			{
				one.graph[v][u] = 1;
			}
		}

		return one;

	}

	//����n�����㣬m���ߵ���,����Ȩͼ
	Adj_Matrix CreateGraph(int n, int m, int edge[][2], bool direct,std::vector<int> weights)
	{
		//��ʼ���ڽӾ���,��int���ֵ��ʼ��
		Adj_Matrix one(n,direct);

		//�����
		for (int i = 0; i < m; i++)
		{
			int u = edge[i][0];
			int v = edge[i][1];

			//����Ȩֵ
			one.graph[u][v] = weights[i];

			//�����������ͼ����ԳƵ�ͬ����ֵ
			if (!one.is_direct)
			{
				one.graph[v][u] = weights[i];
			}
		}

		return one;

	}

	//��ͼG���ڣ�����ͼG��
	void DestroyGraph(Adj_Matrix& adj)
	{
		//��ͼ
		if (adj.graph.empty())
		{
			return;
		}
		
		adj.graph.clear();
		return;
	}

	//��ͼG���ڣ�v��G��ĳ�����㷵��v�ĵ�һ���ڽӶ��㡣
	//��v��G��û���ڽӶ��㣬�򷵻� ���ա���-1��
	int FirstAdjVex(Adj_Matrix& adj, int v)
	{
		//��ͼ
		if (adj.graph.empty())
		{
			return -1;
		}

		//������v
		if (v < 0 || v >= adj.graph.size())
		{
			return -1;
		}

		//����Graph[v]������v���ڽ����У�
		for (int i = 0; i < adj.graph.size(); i++)
		{
			//�ҵ��ڽӵ�,����
			if (adj.graph[v][i] != INT_MAX)
			{
				return i;
			}
		}

		//û���ڽӵ�
		return -1;

	}

	int NextAdjVex(Adj_Matrix& adj, int v, int w)
	{
		//��ͼ
		if (adj.graph.empty())
		{
			return -1;
		}

		//������v
		if (v < 0 || v >= adj.graph.size())
		{
			return -1;
		}

	    //v��w���ڽ�
		if (adj.graph[v][w] != INT_MAX )
		{
			return -1;
		}

		for (int i = w+1; i < adj.graph.size(); i++)
		{
			if (adj.graph[v][i] != INT_MAX)return i;
		}

		//û����һ���ڽӵ����wΪ���һ��
		return -1;
	}

	//��ͼG���ڣ�v��w��G���������㡣
	//��G��������߸��»�<v, w>, ��G������ͼ��������Գƻ�<w, v>��
	bool InSertArc(Adj_Matrix& adj, int v, int w,int weight=1)
	{
		//��ͼ
		if (adj.graph.empty())
		{
			return false;
		}

		//�����������ĵ�
		if (v < 0 || v >= adj.graph.size() || w < 0 || w >= adj.graph.size())
		{
			return false;
		}

		//���ӻ���±�
		adj.graph[v][w] = weight;
		//���Ϊ����ͼ
		if (!adj.is_direct)
		{
			adj.graph[w][v] = weight;
		}

		return true;
	}

	//��G��ɾ����<v, w>, ��G������ͼ����ɾ���Գƻ�<w, v>
	bool DeleteArc(Adj_Matrix& adj, int v, int w)
	{
		//��ͼ
		if (adj.graph.empty())
		{
			return false;
		}

		//�����������ĵ�
		if (v < 0 || v >= adj.graph.size() || w < 0 || w >= adj.graph.size())
		{
			return false;
		}

		//ɾ����
		adj.graph[v][w] = INT_MAX;
		//���Ϊ����ͼ
		if (!adj.is_direct)
		{
			adj.graph[w][v] = INT_MAX;
		}

		return true;

	}

	bool find_edge(const Adj_Matrix& adj, int v, int w)
	{
		//��ͼ
		if (adj.graph.empty())
		{
			return false;
		}

		//�����������ĵ�
		if (v < 0 || v >= adj.graph.size() || w < 0 || w >= adj.graph.size())
		{
			return false;
		}

		//���ڱ�
		if (adj.graph[v][w] != INT_MAX)
		{
			return true;
		}
		
		//�����ڱ�
		return false;
	}

	//�������������DFS���㷨,���Խڵ���д���,vis�����Ƿ����
	void dfs(int u, std::vector<bool>& vis, const Adj_Matrix& adj, std::function<void(int&)> address = [](int& u)->void {std::cout << u; })
	{
		//���ʹ�����u���Ϸ�
		if (u<0||u>=vis.size()||vis[u])
		{
			return;
		}

		//����ѷ���
		vis[u] = true;

		address(u);

		//�ݹ�����
		for (int i = 0; i < vis.size(); i++)
		{
			//�����ڽӵ�
			if (adj.graph[u][i] != INT_MAX)
			{
				dfs(i, vis,adj,address);
			}
		}

	}

	void do_dfs(const Adj_Matrix& adj, std::function<void(int&)> address = [](int& u)->void {std::cout << u; })
	{
		//��ͼ
		if (adj.graph.empty())
		{
			return;
		}

		//��ʼ��vis
		std::vector<bool> vis(adj.graph.size(), false);

		// �������нڵ㣬ȷ������©�κβ���ͨ�Ĳ���
		for (int i = 0; i < adj.graph.size(); i++)
		{
			dfs(i, vis, adj, address);
		}
	}

	//�������������DFS���㷨�ķǵݹ���ʽ�������ڹ�����ȣ�������ʹ��ջ
	void dfs_non_recursive(int start, std::vector<bool>& vis, const Adj_Matrix& adj, std::function<void(int&)> address = [](int& u)->void {std::cout << u; })
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

			// ������ǰ���㣬�ҵ������ڽӲ���δ���ʵĽڵ�������
			for (int i = 0; i < adj.graph.size(); i++)
			{
				if (adj.graph[u][i] != INT_MAX && !vis[i])
				{
					vis[i] = true;// ���Ϊ�ѷ���
					q.push_back(i);     // ���ڽӽڵ���ջ
				}
			}
			
		}
	}

	//ͼ��������㷨�ķǵݹ�
	void do_dfs_non_recursive(const Adj_Matrix& adj, std::function<void(int&)> address = [](int& u)->void {std::cout << u; })
	{
		int number = adj.graph.size();

		//��ͼ
		if (number <= 0)
			return;

		std::vector<bool> vis(number, false);

		// �������нڵ㣬ȷ������©�κβ���ͨ�Ĳ���
		for (int i = 0; i < number; i++)
		{
			dfs_non_recursive(i, vis, adj, address);
		}

	}


	//�������������BFS���㷨,���Խڵ���д���,vis�����Ƿ����
	void bfs(int start, std::vector<bool>& vis, const Adj_Matrix& adj, std::function<void(int&)> address = [](int& u)->void {std::cout << u; })
	{
		//�Ѿ����ʹ�����start���Ϸ�
		if (vis[start] || start < 0 || start >= vis.size())return;

		std::queue<int> q;//��ŵ�ǰ��ͨ��֧�ڵ㣬��Ҫ��start�ڵ�һȦȦ����

		vis[start] = true;//��ǳ�ʼ�����

		q.push(start);//����ʼ�ڵ����

		//���в�Ϊ�գ�����ǰ��ͨ��֧����Ϊ���ʵĽڵ�
		while (!q.empty())
		{
			int u = q.front(); // ��ȡ����ǰ�˵Ľڵ�

			q.pop();           // ����

			address(u);       // ����ǰ�ڵ�

			// ������ǰ���㣬�ҵ������ڽӲ���δ���ʵĽڵ�������
			for (int i=0;i<adj.graph.size();i++)
			{
				if (adj.graph[u][i] != INT_MAX && !vis[i])
				{
					vis[i] = true;// ���Ϊ�ѷ���
					q.push(i);     // ���ڽӽڵ����
				}
			}
		}
	}

	// ִ��BFS����
	void do_bfs(const Adj_Matrix& adj,  std::function<void(int&)> address = [](int& u)->void { std::cout << u; }) {
		// ��ͼ
		if (adj.graph.empty()) return;

		std::vector<bool> vis(adj.graph.size(), false); // �������ʱ�־

		// �������нڵ㣬ȷ������©�κβ���ͨ�Ĳ���
		for (int i = 0; i < adj.graph.size(); i++)
		{
			bfs(i, vis, adj, address); // ��δ���ʵĽڵ㿪ʼBFS
		}
	}

	//������������
	std::vector<int> TopologicalSort(const Adj_Matrix& adj)
	{
		// ����ͼ�޷�������������
		if (!adj.is_direct)
		{
			std::cerr << "Graph doesn't have direction, topological sorting not possible." << std::endl;
			return {};
		}

		int number = adj.graph.size();

		std::vector<int> Topo;//��¼��������

		std::queue<int> zero_in_degree;//��¼���Ϊ0�Ķ���

		//��Ÿ�����������
		std::vector<int> indegree(number, 0);

		//�������������
		for (int u = 0; u < number; u++)
		{
			for (int v = 0; v < number; v++)
			{
				//���ڱ�u->v
				if (adj.graph[u][v] != INT_MAX)
				{
					indegree[v]++;
				}
			}
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
		while (!zero_in_degree.empty())
		{
			//ȡ��һ���ڵ㴦��
			int u = zero_in_degree.front();
			zero_in_degree.pop();

			//����Topo
			Topo.push_back(u);

			//ɾ����Ӧ�ı�,��indegree���߼�ɾ��
			for (int i=0;i<number;i++)
			{
				//��Ӧ������ȼ�һ
				if (adj.graph[u][i] != INT_MAX)
				{
					indegree[i]--;

					//���Ϊ0����zero_in_degree
					if (indegree[i] == 0)
					{
						zero_in_degree.push(i);
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

	//�ؼ�·��
	std::vector<int> CriticalPath(const Adj_Matrix& adj)
	{
		// ����ͼ�޷�������������
		if (!adj.is_direct)
		{
			std::cerr << "Graph doesn't have direction, CriticalPath not possible." << std::endl;
			return {};
		}


		int number = adj.graph.size();

		//��������
		std::vector<int> Topo = TopologicalSort(adj);

		//�л�
		if (Topo.empty())
		{
			std::cout << "ͼ�д��ڻ����޷�����ؼ�·����" << std::endl;
			return {};
		}

		//�ߵ�����
		int edge_num = 0;

		//��ʼ������������緢��ʱ�䣨VE��,���ڿ�ʼ�㼴���������һ�����VTΪ0
		std::vector<int> VE(number, 0);

		//������������VE
		for (int u=0;u<Topo.size();u++)
		{
			for (int v = 0; v < number; v++)
			{
				//���µ�ǰ�ڵ���ڽӵ��VE
				if (adj.graph[u][v] != INT_MAX)
				{
					VE[v] = std::max(VE[v], VE[u] + adj.graph[u][v]);
					edge_num++;
				}
			}
		}

		//��ʼ�����ڵ��������ʱ�䣨VL��,���ڽ����㼴�����������һ�����LTΪVE[number - 1]
		std::vector<int> VL(number, VE[number - 1]);


		//��������������������VL
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

		//�����������������翪ʼʱ��ET������������ʼ�㼴��β��VE
		//���㻡i���������ʼʱ��LT���������������㼴��ͷ��VL-weights[i]
		//ͬʱ����ؼ�·���Ľڵ㣨��ʱPath��Ϊ������ǻ���
		std::vector<int> critical_adjpath;

		for (int u = 0; u < Topo.size(); u++)
		{
			for (int v = 0; v < number; v++)
			{
				//���µ�ǰ�ڵ��Ϊ��β�Ļ��ET��LT
				if (adj.graph[u][v] != INT_MAX)
				{
					int ET = VE[u];
					int LT = VL[v] - adj.graph[u][v];
					//�ж��Ƿ�Ϊ�ؼ�·��
					if (ET == LT)
					{
						//���Ϊ��,������β
						if (critical_adjpath.empty())
						{
							critical_adjpath.push_back(u);
							critical_adjpath.push_back(v);
						}
						else if(critical_adjpath.back()==u)//�ж���β�Ƿ����
						{
							critical_adjpath.push_back(v);//ֻ����β�ڵ�
						}
					}
				}
			}
		}

		std::cout << "��Ŀ���������ʱ��Ϊ��" << VE[number - 1] << std::endl;
		return critical_adjpath;  // ���عؼ�·��
	}

	//�ҵ���Ȩ���·��ͼ�����б�ȨֵΪ1�����v����������������·�������·���ĳ���
	//path[i]��¼�� v �� i �����·���϶���i��ǰ�����
	//dist[i]��¼�� v �� i �����·������
	void ShortestPath(const Adj_Matrix& adj, int v, std::vector<int>& dist, std::vector<int>& path)
	{
		//path��dist��ʼ�����ϲ����

		int number = dist.size();
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

			//������ǰ������б�
			for (int i = 0; i < number; i++)
			{
				//���ڱ�
				if (adj.graph[u][i] != INT_MAX)
				{
					int k = i;

					//dist[k]=-1��ʾδ����
					if (dist[k] == -1)
					{
						//��uδ���ʵ��ڽӵ�k���
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

	//ShortestPath���ϲ㣬���𴴽�����ʼ�������dist��path
	void SearchShortestPath(const Adj_Matrix& adj, int v)
	{
		std::vector<int> dist, path;

		int number = adj.graph.size();

		//��ʼ��dist��path
		dist.resize(number, -1);
		path.resize(number, -1);

		ShortestPath(adj, v, dist, path);

		std::cout << "��Ȩ���·:" << std::endl;

		PrintShortestPath(v, dist, path);

		return;
	}

	//��Ȩ���·��,��ͼ��ȨֵΪ��ʵ�������v����������������·�������·���ĳ���
	//����������Ȩ���·������ȨֵΪ1;
	//�Ҿֲ�����·��������֪�����������������С����ĵ���ɾֲ�����·�����ɴ˵�������������̾���
	void Dijkstra_ShortestPath(const Adj_Matrix& adj, int v, std::vector<bool> vis, std::vector<int>& dist, std::vector<int>& path)
	{
		//path��dist,vis��ʼ�����ϲ����

		//����·��Ϊ0
		dist[v] = 0;

		//��Ƿ���
		vis[v] = true;

		int u = v;
		
		//��v������������������·��
		//���ʳ�����ʼ���������е�
		for (int j = 0; j < adj.graph.size() - 1; j++)
		{
			//���´�u������δ���ʵ����ж�������·������
			for (int i = 0; i < adj.graph.size(); i++)
			{
				int k = i;

				//δ���ʹ������и�С·��
				if (!vis[k] && adj.graph[u][k]!=INT_MAX && dist[u] + adj.graph[u][k] < dist[k])
				{
					//����k���·����ǰ�����
					dist[k] = dist[u] + adj.graph[u][k];
					path[k] = u;
				}
			}

			//Ѱ�ҵ�ǰδ���ʹ���������С·���ĵ�
			int shortest = INT_MAX;

			for (int i = 0; i < adj.graph.size(); i++)
			{
				if (dist[i] < shortest && !vis[i])
				{
					u = i;
					shortest = dist[i];
				}
			}

			//�����¶���
			vis[u] = true;
		}
	}

	//Dijkstra_ShortestPath���ϲ㣬���𴴽�����ʼ�������dist��path
	void Dijkstra_SearchShortestPath(const Adj_Matrix& adj, int v)
	{
		std::vector<int> dist, path;

		//vis�����Ƿ��ʱ�ʶ������Ҫ����˵���õ����ҵ����·��
		//���ҿ���ͨ���õ���Ϊ����������·���е���ת�㣬���ҵ��ֲ�����·
		std::vector<bool> vis;

		int number = adj.graph.size();

		//��ʼ��dist��path,vis
		vis.resize(number, false);
		dist.resize(number, 0);
		path.resize(number, -1);

		for (int i = 0; i < number; i++)
		{
			//��i�������յ�����·�����ȳ�ʼ��Ϊ���ϵ�Ȩֵ
			dist[i] = adj.graph[v][i];

			//���i��v֮���л��� ��i��ǰ����Ϊv
			if (dist[i] < INT_MAX)path[i] = v;
		}
		

		Dijkstra_ShortestPath(adj, v, vis, dist, path);

		std::cout << "��Ȩ���·:" << std::endl;

		PrintShortestPath(v, dist, path);

		return;
	}

	//����ÿһ�Զ���֮������·��
	//������ÿ������ֱ����Dijkstra�㷨
	//���Ƕ����ڽӾ��󣬿���ʹ��Floyd�㷨ʵ�֣�����ʱ�临�Ӷȶ���O(n^3)
	void Floyd_ShortestPath(const Adj_Matrix& adj)
	{
		//��Vi��Vj�����·�����ȳ�ʼ���� ��dist[i][j]= graph[i][j]
		std::vector<std::vector<int>> dist, path;

		//��ʼ��Ϊͬ�ڽӾ����С
		dist.resize(adj.graph.size(), std::vector<int>(adj.graph.size(), 0));
		path.resize(adj.graph.size(), std::vector<int>(adj.graph.size(), -1));

		//����ͼ
		for (int i = 0; i < adj.graph.size(); i++)
		{
			for (int j = 0; j < adj.graph.size(); j++)
			{
				dist[i][j] = adj.graph[i][j];

				//���i��j֮���л�����j��ǰ����Ϊl
				if (dist[i][j] < INT_MAX)
					path[i][j] = i; 
			}
		}

		//n�αȽϺ͸���
		for (int k = 0; k < adj.graph.size(); k++)
		{
			//ÿ�α���ͼ����Vi��Vj��������·������붥��Vo��
			//�Ƚ� ��·�� �� ����v0���·�� ����,ȡ�϶��߸������·��
			for (int i = 0; i < adj.graph.size(); i++)
			{
				for (int j = 0; j < adj.graph.size(); j++)
				{
					//�����С�����·��
					if (dist[i][k] + dist[k][j] < dist[i][j])
					{
						//���³��Ⱥ�ǰ��
						dist[i][j] = dist[i][k] + dist[k][j];
						path[i][j] = path[k][j];
					}

				}
			}
		}

		//��ӡ������n�δ�ӡDijkstra�Ľ��,������ʵ��
	}

	//��һ����ͨ���������������У����ߵĴ���֮����С���ǿ�������
	//��Ϊ����ͨ������С����������(MinimumCost Spanning Tree), ���Ϊ��С��������

	//��n�����������N�й�����С֧����
	//����С֧����Ϊn-l���ߺ�ͼ��n�����㹹�ɵľ�����С����һ����ͨͼ
	//һ��������ܴ��ڲ�ֻһ����С֧����

	//��ͨ������
	//dfsִ�д�������ͨ������
	int Connected_Component(const Adj_Matrix& adj)
	{
		// ��ͼ
		if (adj.graph.empty()) return 0;

		//��ͨ������
		int count = 0;

		std::vector<bool> vis(adj.graph.size(), false); // �������ʱ�־

		// �������нڵ㣬ȷ������©�κβ���ͨ�Ĳ���
		for (int i = 0; i < adj.graph.size(); i++)
		{
			if(!vis[i])
			{
				count++;
				dfs(i, vis, adj, [](int& u)->void {}); // ��δ���ʵĽڵ㿪ʼBFS
			}
		}

		return count;

	}

	//����ķ (Prim) �㷨���ֳ�Ϊ�ӵ㷨,�����ڳ���ͼ
	//����ѡ����ļ������ʼʱֻ����ʼ�㣩�ҵ�δѡ������ѡ���Ȩֵ��С�ı�
	//�������ӵ�������У����¸���δѡ���Ȩֵ��С��
	//Ĭ�ϴ�ͼΪ��ͨͼ��������һ��ͼ
	Adj_Matrix MiniSpanTree_Prim(const Adj_Matrix& adj,int u=0/*��ʼ��*/)
	{
		//����ͨͼ�����ؿ�ͼ
		if (Connected_Component(adj) != 1)
		{
			return Adj_Matrix(0);
		}

		//��ʼ��һ��ͼ
		Adj_Matrix MiniSpanTree(adj.graph.size(),adj.is_direct);


		//��������closedge,�Լ�¼��U��V-U������СȨֵ�ı�
		//first�洢��С�ߵ�Ȩֵ��second�洢��С���� U �е��Ǹ�����
		std::vector<std::pair<int, int>> close_edge;
		//����Ƿ����
		std::vector<bool> vis;

		vis.resize(adj.graph.size(), false);
		close_edge.resize(adj.graph.size());

		//��ʼ��cloes_edge
		for (int i = 0; i < adj.graph.size(); i++)
		{
			if (i != u) 
			{
				close_edge[i].first = adj.graph[u][i];
				close_edge[i].second = u;
			}
		}

		//�����ʼ��
		vis[u] = true;

		//�ҵ���СȨֵ�ı�
		auto Min = [&]()->int {
			int minindex = -1;
			int min = INT_MAX;
			for (int i = 0; i < close_edge.size(); i++)
			{
				const auto& vex = close_edge[i];

				//δ���ʲ����и�СȨֵ�ı�
				if (!vis[i] && vex.first < min)
				{
					minindex = i;
					min = vex.first;
				}
			}
			return minindex;
		};

		//ѡ������n-1�����㣬����n-1 ����(n=G.vexnum)
		for (int j = 1; j < adj.graph.size(); j++)
		{
			//�����СȨֵ�ı�
			int u0 = Min();
			
			// �����ӵ㣬����ͨ���
			if (u == -1)
			{
				break;
			}

			//ΪMiniSpanTree���ӱ�
			int v0 = close_edge[u0].second;
			MiniSpanTree.graph[u0][v0] = close_edge[u0].first;
			
			// ����ͼ���������ӶԳƱ�
			if(!MiniSpanTree.is_direct)
			{
				MiniSpanTree.graph[v0][u0] = close_edge[u0].first;
			}

			//���u0��������close_edge
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

		//����
		return MiniSpanTree;
	}


	//��³˹���� (Kruskal)�㷨���ɳ�Ϊ���ӱ߷�����������ϡ��ͼ
	//ÿ��ѡ��Ȩֵ��С�����޷�ʹ���е����γɻ��ı߼�����С֧����
	//�ʺ���Edge�洢�������˴�������ʵ�֣��������Կ���ת��


	//��ӡ����
	void PrintAdjMatrix(const Adj_Matrix& graph) {
		std::cout << "�ڽӾ���" << std::endl;
		for (const auto& row : graph.graph) {
			for (int val : row) {
				if (val == INT_MAX) {
					std::cout << "��" << "\t";  // �� �� ��ʾ������ޱߣ�
				}
				else {
					std::cout << val << "\t";
				}
			}
			std::cout << std::endl;
		}
	}
};