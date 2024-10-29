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

	VexNode() = default;

	VexNode(std::shared_ptr<ArcNode<T>> arc=nullptr):firstarc(arc){ }

	VexNode(K data, std::shared_ptr<ArcNode<T>> arc = nullptr) :data(data),firstarc(arc) { }

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
	Adj_List(int vexnum, int arcnum, bool direct = false,K datas[]) :arcnum(arcnum), is_direct(direct)
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
			std::shared_ptr<ArcNode<T>> arc(new ArcNode(v));

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
			std::shared_ptr<ArcNode<T>> arc(new ArcNode(v,weights[i]));

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
			return -1��
		}

		//û���ڽӵ�
		if (!list.vertices[v].firsrarc)
		{
			return -1;
		}

		return list.vertices[v].firsrarc.adjvex;
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
	InSertVex(Adj_List<K,T>& list,VexNode<K,T> newvex)
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
	DeleteVex(Adj_List<K, T>& list, int v)
	{
		//��ͼ����v���Ϸ�
		if (list.vertices.size() == 0 || v < 0 || v >= list.vertices.size())
		{
			return -1;
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

		free(list.vertices[v], w);

		//���������ͼ��ɾ���ԳƱ�
		if (!list.is_direct)
		{
			free(list.vertices[w], v);
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

	void do_dfs(Adj_List& list, std::function<void(int&)> address = [](int& u)->void {std::cout << u; })
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


	//�������������BFS���㷨,���Խڵ���д���,vis�����Ƿ����
	void bfs(int start, std::vector<bool>& vis, Adj_List& list, std::function<void(int&)> address = [](int& u)->void {std::cout << u; })
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
			std::shared_ptr<ArcNode<T>> p = list.vertices[u].firstarc;

			while (p)
			{
				vis[p->adjvex] = true;
				q.push(p->adjvex);
				p = p->nextarc;
			}
		}
	}

	// ִ��BFS����
	void do_bfs(Adj_List& list, std::function<void(int&)> address = [](int& u)->void { std::cout << u; }) {
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
	std::vector<int> TopologicalSort(Adj_List& list)
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
				if (indegree[i] == 0)
				{
					zero_in_degree.push(i);
				}

				p = p->nextvarc;
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
	std::vector<int> CriticalPath(Adj_List list)
	{
		// ����ͼ�޷�������������
		if (!list.is_direct)
		{
			std::cerr << "Graph doesn't have direction, CriticalPath not possible." << std::endl;
			return {};
		}


		int number = list.vertices.size();

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
		for (int u = 0; u < Topo.size(); u++)
		{
			std::shared_ptr<ArcNode<T>> p = list.vertices[u].firstarc;
			//����u�����ı�
			while(p)
			{
				int v = p->adjvex;

				VE[v] = std::max(VE[v], VE[u] + p->info);
				edge_num++;

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

			//����u�����ı�
			while (p)
			{
				int v = p->adjvex;

				VL[u] = std::min(VL[u], VL[i] - adj.graph[u][i]);

				p = p->nextarc;
			}
	
		}

		//�����������������翪ʼʱ��ET������������ʼ�㼴��β��VE
		//���㻡i���������ʼʱ��LT���������������㼴��ͷ��VL-weights[i]
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
			}
		}
			
	    std::cout << "��Ŀ���������ʱ��Ϊ��" << VE[number - 1] << std::endl;
		return critical_adjpath;  // ���عؼ�·��
	}

};