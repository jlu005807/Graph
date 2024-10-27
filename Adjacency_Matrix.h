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

	//����n�����㣬m���ߵ�ͼ,
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
			return -1;
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
			return -1;
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

	bool find_edge(Adj_Matrix& adj, int v, int w)
	{
		//��ͼ
		if (adj.graph.empty())
		{
			return -1;
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
	void dfs(int u, std::vector<bool>& vis, Adj_Matrix& adj, std::function<void(int&)> address = [](int& u)->void {std::cout << u; })
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

	void do_dfs(Adj_Matrix& adj, std::function<void(int&)> address = [](int& u)->void {std::cout << u; })
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


	//�������������BFS���㷨,���Խڵ���д���,vis�����Ƿ����
	void bfs(int start, std::vector<bool>& vis, Adj_Matrix& adj, std::function<void(int&)> address = [](int& u)->void {std::cout << u; })
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

			// ����ͼ���ҵ������ڽӲ���δ���ʵĽڵ�������
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
	void do_bfs(Adj_Matrix& adj,  std::function<void(int&)> address = [](int& u)->void { std::cout << u; }) {
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
	std::vector<int> TopologicalSort(Adj_Matrix adj)
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
	std::vector<int> CriticalPath(Adj_Matrix adj)
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
};