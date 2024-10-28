#pragma once
#include<iostream>
#include<memory>//����ʹ��shared_ptr
#include<vector>
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

	ArcNode(int vex) :adjvex(vex), nextarc(nullptr){ }

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


};