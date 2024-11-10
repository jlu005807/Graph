#pragma once
#include<iostream>
#include<memory>//����ʹ��shared_ptr
#include<vector>
#include<queue>
#include<functional>

//�ڽӱ�(Adjacency List) ��ͼ��һ����ʽ�洢�ṹ
//���ڽӱ��У���ͼ��ÿ������V;����һ������������V; ���ڽӵĶ��������������С�
//�ڽӱ���ÿ��������ĵ�һ��������йض������Ϣ�� ����һ��㿴������ı�ͷ�����������йرߵ���Ϣ
//�����ڽӱ������������ɣ���ͷ����ͱ߱�

//�߽��
//�������б߽������ڽӵ���(adjvex)��������(info) ������(nextarc) ������
//�ڽӵ���ָʾ�붥��V; �ڽӵĵ���ͼ�е�λ�ã�������洢�ͱ���ص���Ϣ�� ��Ȩֵ�ȣ�����ָʾ�붥��v�ڽӵ���һ���ߵĽ�㡣
template<class T=int>//Ĭ��Ϊint
class ArcNode
{
public:
	//�ñ���ָ��Ķ���λ�ã����Ϊ�������Ϊ��ͷ
	int adjvex;

	//ָ����һ���ߵ�ָ��
	std::shared_ptr<ArcNode<T>> nextarc;

	//����Ϣ����Ȩֵ,Ĭ������Ϣ
	T info;

	ArcNode() = default;

	ArcNode(int vex) :adjvex(vex),info(1),nextarc(nullptr){ }

	ArcNode(int vex,T info) :adjvex(vex),info(info), nextarc(nullptr) { }

};

//������
//��ͷ������������ (data) ������ (firstarc) ������
//���������ڴ洢����V; �����ƻ������й���Ϣ��������ǧָ�������е�һ����㣨���붥��V; �ڽӵĵ�һ���ڽӵ㣩��
template<class K=int,class T = int>
class VexNode
{
public:
	//������Ϣ��Ĭ���Ƕ���������Ҫ��Ϣ
	K data;

	//ָ���һ�������ö���ıߵ�ָ��
	std::shared_ptr<ArcNode<T>> firstarc;

	VexNode(std::shared_ptr<ArcNode<T>> arc=nullptr):firstarc(arc){ }

	VexNode(K data, std::shared_ptr<ArcNode<T>> arc) :data(data),firstarc(arc) { }

	//�����
	void Add_Arc(std::shared_ptr<ArcNode<T>> arc)
	{
		//����arc��firstarc
		std::shared_ptr<ArcNode<T>> p = firstarc;

		firstarc = arc;

		arc->nextarc = p;
	}


};

template<class K = int, class T = int>
class Adj_List
{
public:
	//��ͷ������vector.size()��������,���Բ���Ҫ��¼������
	std::vector<VexNode<K, T>> vertices;

	//��¼����
	int arcnum;

	//�Ƿ�Ϊ����ͼ
	bool is_direct;


	//��data�Ķ���,Ĭ��dataΪ���
	Adj_List(int vexnum, int arcnum, bool direct = false) :arcnum(arcnum), is_direct(direct)
	{
		//��ʼ��arcnum
		vertices.resize(vexnum);
		for (int i = 0; i < vexnum; i++)
		{
			vertices[i].data = i;
		}
	}

	//��Ҫdata�Ķ���
	Adj_List(int vexnum, int arcnum, K datas[],bool direct = false) :arcnum(arcnum), is_direct(direct)
	{
		//��ʼ��arcnum
		vertices.resize(vexnum);
		for (int i = 0; i < vexnum; i++)
		{
			vertices[i].data = datas[i];
		}
	}

};

//������
template<class K = int, class T = int>
class AdjList_manager
{
public:
	
	//����n�����㣬m���ߵ�ͼ
	Adj_List<K,T> CreateGraph(int n, int m, int edge[][2], bool direct=false)
	{
		//��ʼ��
		Adj_List<K,T> list(n, m, direct);

		//����ߵ���Ϣ���˴�ʹ��ͷ�巨������edge�е�λ����List�е��ȶ���
		for (int i = 0; i < m; i++)
		{
			//ȷ����
			int u = edge[i][0];
			int v = edge[i][1];

			//����arc
			std::shared_ptr<ArcNode<T>> arc(new ArcNode<T>(v));

			list.vertices[u].Add_Arc(arc);

			//���������ͼ�������ӶԳƱ�
			if (!list.is_direct)
			{
				list.vertices[v].Add_Arc(arc);
			}
		}

		return list;
	}

	//����n�����㣬m���ߵĴ�Ȩֵ��ͼ
	Adj_List<K,T> CreateGraph(int n, int m, int edge[][2],std::vector<int> weights, bool direct = false)
	{
		//��ʼ��
		Adj_List<K, T> list(n, m, direct);

		//����ߵ���Ϣ���˴�ʹ��ͷ�巨������edge�е�λ����List�е��ȶ���
		for (int i = 0; i < m; i++)
		{
			//ȷ����
			int u = edge[i][0];
			int v = edge[i][1];

			//����arc
			std::shared_ptr<ArcNode<T>> arc(new ArcNode<T>(v,weights[i]));

			list.vertices[u].Add_Arc(arc);

			//���������ͼ�������ӶԳƱ�
			if (!list.is_direct)
			{
				list.vertices[v].Add_Arc(arc);
			}
		}

		return list;
	}

	//��ͼG���ڣ�����ͼG��
	void DestroyGraph(Adj_List<K,T>& list)
	{
		//��ͼ
		if (list.vertices.size()==0)
		{
			return;
		}

		for (const auto& vex:list.vertices)
		{	
			std::shared_ptr<ArcNode<T>> arc = vex.firstarc;

			//�����ͷ�
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

	//����v�ĵ�һ���ڽӶ������š���v��G��û���ڽӶ��㣬�򷵻� ���ա���
	int FirstAdjVex(Adj_List<K,T>& list, int v)
	{
		//��ͼ����v���Ϸ�
		if (list.vertices.size() == 0 || v < 0 || v >= list.vertices.size())
		{
			return -1;
		}

		//û���ڽӵ�
		if (!list.vertices[v].firstarc)
		{
			return -1;
		}

		return list.vertices[v].firstarc.adjvex;
	}

	//����v�ģ����ǧw�ģ���һ���ڽӶ��㡣��w��v�����һ���ڽӵ㣬�򷵻� ���ա���
	int NextAdjVex(Adj_List<K,T>& list, int v, int w)
	{
		//��ͼ����v���Ϸ�����w���Ϸ�
		if (list.vertices.size() == 0 || v < 0 || v >= list.vertices.size() || w < 0 || w >= list.vertices.size())
		{
			return -1;
		}

		std::shared_ptr<ArcNode<T>> arc = list.vertices[v].firstarc;

		//�ҵ�w
		while (arc->nextarc && arc->adjvex != w)
			arc = arc->nextarc;

		if (arc->nextarc)
		{
			return arc->nextarc->adjvex;
		}
		else
		{
			return -1;
		}

	}

	//��ͼG�������¶���v��
	void InSertVex(Adj_List<K,T>& list,VexNode<K,T> newvex)
	{
		auto it = std::find(list.vertices.begin(), list.vertices.end(), newvex);

		//û�ҵ�
		if (it == list.vertices.end())
		{
			list.vertices.push_back(newvex);
		}
		else
		{
			return;
		}

	}

	//ɾ��G�ж���v������صĻ���
	bool DeleteVex(Adj_List<K, T>& list, int v)
	{
		//��ͼ����v���Ϸ�
		if (list.vertices.size() == 0 || v < 0 || v >= list.vertices.size())
		{
			return false;
		}

		auto it = list.vertices.begin();

		it += v;

		//��¼
		VexNode<K, T> vex = *it;


		std::shared_ptr<ArcNode<T>> arc = vex.firstarc;

		while (arc)
		{
			DeleteArc(list, v, arc->adjvex);
		}

		return true;
	}

	//��G��ɾ����<v, w>, ��G������ͼ����ɾ���Գƻ�<w, v>��
	void DeleteArc(Adj_List<K, T>& list, int v, int w)
	{
		//��ͼ����v���Ϸ�����w���Ϸ�
		if (list.vertices.size() == 0 || v < 0 || v >= list.vertices.size() || w < 0 || w >= list.vertices.size())
		{
			return;
		}

		auto free = [&](std::shared_ptr<ArcNode<T>> arc, int k)->void {
			
			while (arc->nextarc && arc->nextarc->adjvex != k)
			{
				arc = arc->nextarc;
			}

			//�ҵ���
			if (arc->nextarc)
			{
				std::shared_ptr<ArcNode<T>> p;

				p = arc->nextarc;

				arc->nextarc = p->nextarc;

				//�����ͷ�
				p.reset();
			}

		};

		free(list.vertices[v].firstarc, w);

		//���������ͼ��ɾ���ԳƱ�
		if (!list.is_direct)
		{
			free(list.vertices[w].firstarc, v);
		}

	}

	//��ͼ��Ѱ��v��w֮��Ļ�
	bool Find_Arc(Adj_List<K, T>& list, int v, int w)
	{
		//��ͼ����v���Ϸ�����w���Ϸ�
		if (list.vertices.size() == 0 || v < 0 || v >= list.vertices.size() || w < 0 || w >= list.vertices.size())
		{
			return false;
		}

		std::shared_ptr<ArcNode<T>> arc = list.vertices[v].firstarc;

		while (arc && arc->adjvex != w)
		{
			arc = arc->nextarc;
		}

		//�ҵ�
		if (arc)
		{
			return true;
		}

		return false;
	}

	//��G������<v, w>, ��G������ͼ��������Գƻ�<w, v>��
	void InSertArc(Adj_List<K, T>& list, int v, int w)
	{
		//��ͼ����v���Ϸ�����w���Ϸ�
		if (list.vertices.size() == 0 || v < 0 || v >= list.vertices.size() || w < 0 || w >= list.vertices.size())
		{
			return;
		}

		auto add = [](std::shared_ptr<ArcNode<T>> arc, int k) -> void {
			//���������
			while (arc->nextarc)
			{
				arc = arc->nextarc;
			}

			std::shared_ptr<ArcNode<T>> p(new ArcNode<T>(k));

			//����
			arc->nextarc = p;

		};

		//ͼ���޴˻�
		if (!Find_Arc(list, v, w))
		{
			std::shared_ptr<ArcNode<T>> p = list.vertices[v].firstarc;

			add(p, w);

			//���������ͼ�������ӶԳƱ�
			if (!list.is_direct)
			{
				p = list.vertices[w].firstarc;
				add(p, v);
			}
		}
		return;
	}

	//�������������DFS���㷨,���Խڵ���д���,vis�����Ƿ����
	void dfs(int u, std::vector<bool>& vis, Adj_List<K,T>& list, std::function<void(VexNode<K, T>&)> address = [](VexNode<K, T>& node)->void {std::cout << node.data; })
	{
		//���ʹ�����u���Ϸ�
		if (u < 0 || u >= vis.size() || vis[u])
		{
			return;
		}

		//����ѷ���
		vis[u] = true;

		address(list.vertices[u]);

		//�ݹ�����
		std::shared_ptr<ArcNode<T>> p = list.vertices[u].firstarc;

		while (p)
		{
			dfs(p->adjvex, vis, list, address);

			p = p->nextarc;
		}

	}

	void do_dfs(Adj_List<K,T>& list, std::function<void(VexNode<K, T>&)> address = [](VexNode<K, T>& node)->void {std::cout << node.data; })
	{
		//��ͼ
		if (list.vertices.empty())
		{
			return;
		}

		//��ʼ��vis
		std::vector<bool> vis(list.vertices.size(), false);

		// �������нڵ㣬ȷ������©�κβ���ͨ�Ĳ���
		for (int i = 0; i < list.vertices.size(); i++)
		{
			dfs(i, vis, list, address);
		}
	}

	//�������������DFS���㷨�ķǵݹ���ʽ�������ڹ�����ȣ�������ʹ��ջ
	void dfs_non_recursive(int start, std::vector<bool>& vis, Adj_List<K, T>& list, std::function<void(VexNode<K, T>&)> address = [](VexNode<K, T>& node)->void {std::cout << node.data; })
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
			auto p = list.vertices[u].firstarc;
			while (p)
			{
				vis[p->adjvex] = true;
				q.push_back(p->adjvex);

				p = p->nextarc;
			}

		}
	}

	//ͼ��������㷨�ķǵݹ�
	void do_dfs_non_recursive(Adj_List<K, T>& list, std::function<void(VexNode<K, T>&)> address = [](VexNode<K, T>& node)->void {std::cout << node.data; })
	{
		int number = list.vertices.size();

		//��ͼ
		if (number <= 0)
			return;

		std::vector<bool> vis(number, false);

		// �������нڵ㣬ȷ������©�κβ���ͨ�Ĳ���
		for (int i = 0; i < number; i++)
		{
			dfs_non_recursive(i, vis, list, address);
		}

	}


	//�������������BFS���㷨,���Խڵ���д���,vis�����Ƿ����
	void bfs(int start, std::vector<bool>& vis, Adj_List<K,T>& list, std::function<void(VexNode<K, T>&)> address = [](VexNode<K, T>& node)->void {std::cout << node.data; })
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

			address(list.vertices[u]);       // ����ǰ�ڵ�

			// ������ǰ���㣬�ҵ������ڽӲ���δ���ʵĽڵ�������
			std::shared_ptr<ArcNode<T>> p = list.vertices[u].firstarc;

			while (p)
			{
				//δ����
				if(!vis[p->adjvex])
				{
					vis[p->adjvex] = true;
					q.push(p->adjvex);
				}

				p = p->nextarc;
			}
		}
	}

	// ִ��BFS����
	void do_bfs(Adj_List<K,T>& list, std::function<void(VexNode<K, T>&)> address = [](VexNode<K, T>& node)->void {std::cout << node.data; }) 
	{
		// ��ͼ
		if (list.vertices.empty()) return;

		std::vector<bool> vis(list.vertices.size(), false); // �������ʱ�־

		// �������нڵ㣬ȷ������©�κβ���ͨ�Ĳ���
		for (int i = 0; i < list.vertices.size(); i++)
		{
			bfs(i, vis, list, address); // ��δ���ʵĽڵ㿪ʼBFS
		}
	}

	//������������
	std::vector<int> TopologicalSort(Adj_List<K,T>& list)
	{
		// ����ͼ�޷�������������
		if (!list.is_direct)
		{
			std::cerr << "Graph doesn't have direction, topological sorting not possible." << std::endl;
			return {};
		}

		int number = list.vertices.size();

		std::vector<int> Topo;//��¼��������

		std::queue<int> zero_in_degree;//��¼���Ϊ0�Ķ���

		//��Ÿ�����������
		std::vector<int> indegree(number, 0);

		//�������������
		for (int u = 0; u < number; u++)
		{
			std::shared_ptr<ArcNode<T>> p = list.vertices[u].firstarc;

			while (p)
			{
				indegree[p->adjvex]++;

				p = p->nextarc;
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
			std::shared_ptr<ArcNode<T>> p = list.vertices[u].firstarc;

			while (p)
			{
				//��Ӧ������ȼ�һ
				indegree[p->adjvex]--;

				//���Ϊ0����zero_in_degree
				if (indegree[p->adjvex] == 0)
				{
					zero_in_degree.push(p->adjvex);
				}

				p = p->nextarc;
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
	std::vector<int> CriticalPath(Adj_List<K,T> list)
	{
		// ����ͼ�޷�������������
		if (!list.is_direct)
		{
			std::cerr << "Graph doesn't have direction, CriticalPath not possible." << std::endl;
			return {};
		}


		int number = list.vertices.size();

		//��������
		std::vector<int> Topo = TopologicalSort(list);

		//�л�
		if (Topo.empty())
		{
			std::cout << "ͼ�д��ڻ����޷�����ؼ�·����" << std::endl;
			return {};
		}

		//��ʼ������������緢��ʱ�䣨VE��,���ڿ�ʼ�㼴���������һ�����VTΪ0
		std::vector<int> VE(number, 0);

		//������������VE
		for (int u = 0; u < Topo.size(); u++)
		{
			std::shared_ptr<ArcNode<T>> p = list.vertices[u].firstarc;
			//����u�����ı�
			while(p)
			{
				int v = p->adjvex;

				VE[v] = std::max(VE[v], VE[u] + p->info);
			
				p = p->nextarc;
			}
		}

		//��ʼ�����ڵ��������ʱ�䣨VL��,���ڽ����㼴�����������һ�����LTΪVE[number - 1]
		std::vector<int> VL(number, VE[number - 1]);


		//��������������������VL
		for (auto it = Topo.rbegin(); it != Topo.rend(); it++)
		{
			int u = *it;

			std::shared_ptr<ArcNode<T>> p = list.vertices[u].firstarc;

			//������u�����ı�
			while (p)
			{
				int v = p->adjvex;

				VL[u] = std::min(VL[u], VL[v] - p->info);

				p = p->nextarc;
			}
	
		}

		//ͬʱ����ؼ�·���Ľڵ㣨��ʱPath��Ϊ������ǻ���
		std::vector<int> critical_adjpath;

		for (int u = 0; u < Topo.size(); u++)
		{
			std::shared_ptr<ArcNode<T>> p = list.vertices[u].firstarc;
			
			while(p)
			{
				//���µ�ǰ�ڵ��Ϊ��β�Ļ��ET��LT
				int v = p->adjvex;

				int ET = VE[u];

				int LT = VL[v] - p->info;

				//�ж��Ƿ�Ϊ�ؼ�·��
				if (ET == LT)
				{
				//���Ϊ��,������β
					if (critical_adjpath.empty())
					{
						critical_adjpath.push_back(u);
						critical_adjpath.push_back(v);
					}
					else if (critical_adjpath.back() == u)//�ж���β�Ƿ����
					{
							critical_adjpath.push_back(v);//ֻ����β�ڵ�
					}
				}

				//����
				p = p->nextarc;
			}
		}
			
	    std::cout << "��Ŀ���������ʱ��Ϊ��" << VE[number - 1] << std::endl;
		return critical_adjpath;  // ���عؼ�·��
	}

	//�ҵ���Ȩ���·��ͼ�����б�ȨֵΪ1�����v����������������·�������·���ĳ���
	//path[i]��¼�� v �� i �����·���϶���i��ǰ�����
	//dist[i]��¼�� v �� i �����·������
	void ShortestPath(const Adj_List<K, T>& list, int v, std::vector<int>& dist, std::vector<int>& path)
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

			auto p = list.vertices[u].firstarc;
			while (p)
			{
				int k = p->adjvex; //��¼��ǰ�ڽӵ�

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

				//������һ���ڽӵ�
				p = p->nextarc;
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
	void SearchShortestPath(const Adj_List<K, T>& list, int v)
	{
		std::vector<int> dist, path;

		int number = list.vertices.size();

		//��ʼ��dist��path
		dist.resize(number, -1);
		path.resize(number, -1);

		ShortestPath(list, v, dist, path);

		std::cout << "��Ȩ���·:" << std::endl;

		PrintShortestPath(v, dist, path);

		return;
	}

	//��ӡ�ڽӱ�
	void PrintGraph(const Adj_List<K,T>& list) {
		std::cout << "Graph adjacency list representation:" << std::endl;
		for (const auto& node : list.vertices) {
			std::cout << "Node " << node.data << ": ";
			auto edge = node.firstarc;
			while (edge) {
				std::cout << "-> (" << edge->adjvex << ", weight: " << edge->info << ") ";
				edge = edge->nextarc;
			}
			std::cout << std::endl;
		}
	}

	//��Ȩ���·��,��ͼ��ȨֵΪ��ʵ�������v����������������·�������·���ĳ���
	//����������Ȩ���·������ȨֵΪ1;
	//�Ҿֲ�����·��������֪�����������������С����ĵ���ɾֲ�����·�����ɴ˵�������������̾���
	void Dijkstra_ShortestPath(const Adj_List<K, T>& list, int v, std::vector<bool> vis, std::vector<int>& dist, std::vector<int>& path)
	{
		//path��dist,vis��ʼ�����ϲ����

		//����·��Ϊ0
		dist[v] = 0;

		//��Ƿ���
		vis[v] = true;

		int u = v;
		auto p = list.vertices[v].firstarc;

		//��v������������������·��
		//���ʳ�����ʼ���������е�
		for (int i = 0; i < list.vertices.size() - 1; i++)
		{
			//��Ŀǰ��С·���ĵ��������δ���ʵ����С·��
			while (p)
			{
				int k = p->adjvex;

				//δ���ʹ������и�С·��
				if (!vis[k] && dist[u] + p->info < dist[k])
				{
					//�������·����ǰ�����
					dist[k] = dist[u] + p->info;
					path[k] = u;
				}

				p = p->nextarc;
			}

			//Ѱ�ҵ�ǰδ���ʹ���������С·���ĵ�
			int shortest = INT_MAX;

			for (int i = 0; i < list.vertices.size(); i++)
			{
				if (dist[i] < shortest && !vis[i])
				{
					u = i;
					shortest = dist[i];
				}
			}

			//�����¶����p
			vis[u] = true;
			p = list.vertices[u].firstarc;
		}
	}

	//Dijkstra_ShortestPath���ϲ㣬���𴴽�����ʼ�������dist��path
	void Dijkstra_SearchShortestPath(const Adj_List<K, T>& list, int v)
	{
		std::vector<int> dist, path;

		//vis�����Ƿ��ʱ�ʶ������Ҫ����˵���õ����ҵ����·��
		//���ҿ���ͨ���õ���Ϊ����������·���е���ת�㣬���ҵ��ֲ�����·
		std::vector<bool> vis;

		int number = list.vertices.size();

		//��ʼ��dist��path,vis
		dist.resize(number, INT_MAX);//�ޱ�Ϊ�����
		path.resize(number, -1);
		vis.resize(number, false);

		Dijkstra_ShortestPath(list, v, vis, dist, path);

		std::cout << "��Ȩ���·:" << std::endl;

		PrintShortestPath(v, dist, path);

		return;
	}


	//����ÿһ�Զ���֮������·��
	//������ÿ������ֱ����Dijkstra�㷨


	//��ͨ������
	//dfsִ�д�������ͨ������
	int Connected_Component(const Adj_List<K, T>& list)
	{
		// ��ͼ
		if (list.vertices.empty()) return 0;

		//��ͨ������
		int count = 0;

		std::vector<bool> vis(list.vertices.size(), false); // �������ʱ�־

		// �������нڵ㣬ȷ������©�κβ���ͨ�Ĳ���
		for (int i = 0; i < list.vertices.size(); i++)
		{
			if (!vis[i])
			{
				count++;
				dfs(i, vis, list, [](int& u)->void {}); // ��δ���ʵĽڵ㿪ʼBFS
			}
		}

		return count;

	}

	//����ķ (Prim) �㷨���ֳ�Ϊ�ӵ㷨,�����ڳ���ͼ
	//����ѡ����ļ������ʼʱֻ����ʼ�㣩�ҵ�δѡ������ѡ���Ȩֵ��С�ı�
	//�������ӵ�������У����¸���δѡ���Ȩֵ��С��
	//Ĭ�ϴ�ͼΪ��ͨͼ��������һ��ͼ
	Adj_List<K, T> MiniSpanTree_Prim(const Adj_List<K, T>& list, int u = 0/*��ʼ��*/)
	{
		//����ͨͼ�����ؿ�ͼ
		if (Connected_Component(list) != 1)
		{
			return Adj_List<K, T>(0,0);
		}

		int vexnum = list.vertices.size();

		//��ʼ��һ��ͼ
		Adj_List<K, T> MiniSpanTree(vexnum, list.arcnum, list.is_direct);


		//��������closedge,�Լ�¼��U��V-U������СȨֵ�ı�
		//first�洢��С�ߵ�Ȩֵ��second�洢��С���� U �е��Ǹ�����
		std::vector<std::pair<T, int>> close_edge;
		//����Ƿ����
		std::vector<bool> vis;

		vis.resize(vexnum, false);
		close_edge.resize(vexnum,std::make_pair<int,int>(INT_MAX,u));

		//��ʼ��cloes_edge,����ʼ�������������С�߸���
		auto p = list.vertices[u].firstarc;
		while (p)
		{
			close_edge[p->adjvex].first = p->info;
			p = p->nextarc;
		}

		//�����ʼ��
		vis[u] = true;

		//�ҵ���СȨֵ�ı�
		auto Min = [&]()->int {
			int minindex = -1;
			T min = INT_MAX;
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
		for (int j = 1; j < vexnum; j++)
		{
			//�����СȨֵ�ı�
			int u0 = Min();

			if (u0 == -1) break;  // �����ӵ㣬����ͨ���

			//ΪMiniSpanTree���ӱ�
			int v0 = close_edge[u0].second;
			std::shared_ptr<ArcNode<T>> arc(new ArcNode<T>(v0));
			MiniSpanTree.vertices[u0].Add_arc(arc,close_edge[u0].first);

			//����ͼ�������ӶԳƱ�
			if (!MiniSpanTree.is_direct)
			{
				MiniSpanTree.vertices[v0].Add_arc(arc, close_edge[u0].first);
			}

			//���u0��������close_edge
			vis[u0] = true;

			auto p = list.vertices[u0].firstarc;
			while (p)
			{
				if (p->info < close_edge[p->adjvex].first)
				{
					close_edge[j].first = p->info;
					close_edge[j].second = p->adjvex;
				}
				p = p->nextarc;
			}

		}

		//����
		return MiniSpanTree;
	}
};