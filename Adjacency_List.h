#pragma once
#include<iostream>
#include<memory>//����ʹ��shared_ptr
#include<vector>

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

	Adj_List(int vexnum, int arcnum, bool direct = false) :arcnum(arcnum), is_direct(direct)
	{
		//��ʼ��arcnum
		vertices.resize(vexnum);
	}

};

//������
template<class K = int, class T = int>
class AdjList_manager
{
public:
	
	//����n�����㣬m���ߵ�ͼ
	Adj_List CreateGraph(int n, int m, int edge[][2], bool direct=false)
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
	Adj_List CreateGraph(int n, int m, int edge[][2],std::vector<int> weights, bool direct = false)
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
	void DestroyGraph(Adj_List& list)
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
};