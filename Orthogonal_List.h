#pragma once
#include<iostream>
#include<vector>
#include<queue>
#include<memory>
#include<functional>

//ʮ������(Orthogonal List) ��**����ͼ**����һ����ʽ�洢�ṹ
//���Կ����ǽ�����ͼ���ڽӱ�����ڽӱ��������õ���һ������

//�����
//β��(tailvex) ��ͷ�� (headvex)�ֱ�ָʾ��β�ͻ�ͷ������������ͼ�е�λ��
//����hlinkָ��ͷ��ͬ����һ������������tlinkָ��β��ͬ����һ����
//info��ָ��û��������Ϣ������Ȩֵ
template<typename T=int>
class ArcBox
{
public:
	//��β���
	int tailvext;
	//��ͷ���
	int headvext;
	
	//��β��ͬ�Ļ�������
    std::shared_ptr<ArcBox<T>> tlink;

	//��ͷ��ͬ�Ļ�������
    std::shared_ptr<ArcBox<T>> hlink;

	//��Ϣ
	T info;

	ArcBox() = default;

	//Ĭ������Ϣ
	ArcBox(int tail,int head, std::shared_ptr<ArcBox<T>> t=nullptr, ArcBox<T>* h=nullptr):tailvext(tail),headvext(head),tlink(t),hlink(h){ }

};


//����
//data��洢�Ͷ�����ص���Ϣ
//firstin��firstoutΪ�������򣬷ֱ�ָ���Ըö���Ϊ��ͷ��β�ĵ�һ������㡣
template<typename K=int,typename T=int>
class VexBox
{
public:
	//������Ϣ��Ĭ��Ϊ���
	K data;

	//�ö���Ϊ��ͷ�ĵ�һ����
    std::shared_ptr<ArcBox<T>> firstin;

	//�ö���Ϊ��β�ĵ�һ����
    std::shared_ptr<ArcBox<T>> firstout;

	VexBox() = default;

	VexBox(K d, std::shared_ptr<ArcBox<T>> firstin=nullptr, std::shared_ptr<ArcBox<T>> firstout=nullptr):data(d),firstin(firstin),firstout(firstout){ }

	//�������,����β
	void Add_outArc(std::shared_ptr<ArcBox<T>> arc)
	{
		//����arc��firstarc
		std::shared_ptr<ArcNode<T>> p = firstout;

		firstout = arc;

		arc->tlink = p;
	}

	//������ߣ�����ͷ
	void Add_inArc(std::shared_ptr<ArcBox<T>> arc)
	{
		//����arc��firstarc
		std::shared_ptr<ArcNode<T>> p = firstin;

		firstin = arc;

		arc->hlink = p;
	}
};

//��
template<typename K = int, typename T = int>
class OLGraph
{
public:
	//��ͷ������vector.size()��������,���Բ���Ҫ��¼������
	std::vector<VexBox<K, T>> vertices;

	//��¼����
	int arcnum;

	//��data�Ķ���,Ĭ��dataΪ���
	OLGraph(int vexnum)
	{
		//��ʼ��arcnum
		vertices.resize(vexnum);
		for (int i = 0; i < vexnum; i++)
		{
			vertices[i].data = i;
		}
	}

	//��Ҫdata�Ķ���
	OLGraph(int vexnum, K datas[]) 
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
class OLGraph_manager
{
public:
	//����n�����㣬m���ߵ�ͼ
	OLGraph<K, T> CreateGraph(int n, int m, int edge[][2])
	{
		//��ʼ��
		Adj_List<K, T> list(n);

		//����ߵ���Ϣ���˴�ʹ��ͷ�巨������edge�е�λ����List�е��ȶ���
		for (int i = 0; i < m; i++)
		{
			//ȷ����
			int u = edge[i][0];
			int v = edge[i][1];

			//����arc
			std::shared_ptr<ArcBox<T>> arc(new ArcBox<T>(u, v));
			
			list.vertices[u].Add_outArc(arc);


			list.vertices[v].Add_inArc(arcn);
		}

		return list;
	}

	//����n�����㣬m���ߵĴ�Ȩֵ��ͼ
	OLGraph<K, T> CreateGraph(int n, int m, int edge[][2], std::vector<int> weights)
	{
		//��ʼ��
		Adj_List<K, T> list(n);

		//����ߵ���Ϣ���˴�ʹ��ͷ�巨������edge�е�λ����List�е��ȶ���
		for (int i = 0; i < m; i++)
		{
			//ȷ����
			int u = edge[i][0];
			int v = edge[i][1];

			//����arc
			std::shared_ptr<ArcBox<T>> arc(new ArcBox<T>(u, v));

			arc->info = weights[i];

			list.vertices[u].Add_outArc(arc);

			list.vertices[v].Add_inArc(arc);
		}

		return list;
	}

	//��ͼG���ڣ�����ͼG��
	void DestroyGraph(OLGraph<K,T>& graph)
	{
		//��ͼ
		if (graph.vertices.size() == 0)
		{
			return;
		}

		for (const auto& vex : graph.vertices)
		{
			std::shared_ptr<ArcBox<T>> arc = vex.firstout;

			//�����ͷ�
			while (vex.tlink!= nullptr)
			{
				std::shared_ptr<ArcBox<T>> arc = vex.tlink;

				vex.tlink = arc->nextarc;

				arc.reset();
			}
		}

		graph.vertices.clear();

		graph.arcnum = 0;

		return;
	}

	//����v�ĵ�һ���ڽӶ������š���v��G��û���ڽӶ��㣬�򷵻� ���ա���
	int FirstAdjVex(OLGraph<K, T>& graph, int v)
	{
		//��ͼ����v���Ϸ�
		if (graph.vertices.size() == 0 || v < 0 || v >= graph.vertices.size())
		{
			return -1;
		}

		//û���ڽӵ�
		if (!graph.vertices[v].firstout)
		{
			return -1;
		}

		return graph.vertices[v].firstout.headvext;
	}

	//����v�ģ����ǧw�ģ���һ���ڽӶ��㡣��w��v�����һ���ڽӵ㣬�򷵻� ���ա���
	int NextAdjVex(OLGraph<K, T>& graph, int v, int w)
	{
		//��ͼ����v���Ϸ�����w���Ϸ�
		if (graph.vertices.size() == 0 || v < 0 || v >= graph.vertices.size() || w < 0 || w >= graph.vertices.size())
		{
			return -1;
		}

		std::shared_ptr<ArcBox<T>> arc = graph.vertices[v].firstout;

		//�ҵ�w
		while (arc->tlink && arc->headvext != w)
			arc = arc->tlink;

		if (arc)
		{
			return arc->headvext;
		}
		else
		{
			return -1;
		}

	}

	//��������ɾ������


	//��G��ɾ����<v, w>
	void DeleteArc(OLGraph<K, T>& graph, int v, int w)
	{
		//��ͼ����v���Ϸ�����w���Ϸ�
		if (graph.vertices.size() == 0 || v < 0 || v >= graph.vertices.size() || w < 0 || w >= graph.vertices.size())
		{
			return;
		}

		auto freeout = [&](std::shared_ptr<ArcBox<T>> arc, int k)->void {

			while (arc->tlink && arc->tlink->headvext != k)
			{
				arc = arc->tlink;
			}

			//�ҵ���
			if (arc->tlink)
			{
				std::shared_ptr<ArcBox<T>> p;

				p = arc->tlink;

				arc->tlink = p->tlink;
			}

		};

		auto freein = [&](std::shared_ptr<ArcBox<T>> arc, int k)->void {

			while (arc->hlink && arc->hlink->tailvext != k)
			{
				arc = arc->=hlink;
			}

			//�ҵ���
			if (arc->hlink)
			{
				std::shared_ptr<ArcBox<T>> p;

				p = arc->hlink;

				arc->hlink = p->hlink;

				p.reset();
			}

		};

		//���
		freeout(graph.vertices[v].firstout, w);
		//�������ͷ�
		freein(graph.vertices[w].firstin, v);

	}

	//��ͼ��Ѱ��v��w֮��Ļ�
	bool Find_Arc(OLGraph<K, T>& graph, int v, int w)
	{
		//��ͼ����v���Ϸ�����w���Ϸ�
		if (graph.vertices.size() == 0 || v < 0 || v >= graph.vertices.size() || w < 0 || w >= graph.vertices.size())
		{
			return false;
		}

		std::shared_ptr<ArcBox<T>> arc = graph.vertices[v].firstout;

		while (arc && arc->headvext != w)
		{
			arc = arc->tlink;
		}

		//�ҵ�
		if (arc)
		{
			return true;
		}

		return false;
	}

	//��G������<v, w>
	void InSertArc(OLGraph<K, T>& graph, int v, int w)
	{
		//��ͼ����v���Ϸ�����w���Ϸ�
		if (graph.vertices.size() == 0 || v < 0 || v >= graph.vertices.size() || w < 0 || w >= graph.vertices.size())
		{
			return;
		}

		auto addout = [](std::shared_ptr<ArcBox<T>> arc, int k) -> void {
			//���������
			while (arc->tlink)
			{
				arc = arc->tlink;
			}

			std::shared_ptr<ArcBox<T>> p(new ArcBox<T>(k));

			//����
			arc->tlink = p;

		};

		auto addin = [](std::shared_ptr<ArcBox<T>> arc, int k) -> void {
			//���������
			while (arc->hlink)
			{
				arc = arc->hlink;
			}

			std::shared_ptr<ArcBox<T>> p(new ArcBox<T>(k));

			//����
			arc->hlink = p;

		};

		//ͼ���޴˻�
		if (!Find_Arc(graph, v, w))
		{
			std::shared_ptr<ArcBox<T>> p = list.vertices[v].firstout;

			std::shared_ptr<ArcBox<T>> q = list.vertices[w].firstin;

			addout(p, w);

			addin(q, v);
		}

		return;
	}

	//�������������DFS���㷨,���Խڵ���д���,vis�����Ƿ����
	void dfs(int u, std::vector<bool>& vis, OLGraph<K, T>& graph, std::function<void(VexBox<K, T>&)> address = [](VexBox<K, T>& node)->void {std::cout << node.data; })
	{
		//���ʹ�����u���Ϸ�
		if (u < 0 || u >= vis.size() || vis[u])
		{
			return;
		}

		//����ѷ���
		vis[u] = true;

		address(graph.vertices[u]);

		//�ݹ�����
		std::shared_ptr<ArcBox<T>> p = graph.vertices[u].firstout;

		while (p)
		{
			dfs(p->headvext, vis, graph, address);

			p = p->tlink;
		}

	}

	void do_dfs(OLGraph<K, T>& graph, std::function<void(VexBox<K, T>&)> address = [](VexBox<K, T>& node)->void {std::cout << node.data; })
	{
		//��ͼ
		if (graph.vertices.empty())
		{
			return;
		}

		//��ʼ��vis
		std::vector<bool> vis(graph.vertices.size(), false);

		// �������нڵ㣬ȷ������©�κβ���ͨ�Ĳ���
		for (int i = 0; i < graph.vertices.size(); i++)
		{
			dfs(i, vis, graph, address);
		}
	}

	//�������������BFS���㷨,���Խڵ���д���,vis�����Ƿ����
	void bfs(int start, std::vector<bool>& vis, OLGraph<K, T>& graph, std::function<void(VexBox<K, T>&)> address = [](VexBox<K, T>& node)->void {std::cout << node.data; })
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

			address(graph.vertices[u]);       // ����ǰ�ڵ�

			// ������ǰ���㣬�ҵ������ڽӲ���δ���ʵĽڵ�������
			std::shared_ptr<ArcBox<T>> p = graph.vertices[u].firstout;

			while (p)
			{
				//δ����
				if (!vis[p->headvext])
				{
					vis[p->headvext] = true;
					q.push(p->headvext);
				}

				p = p->tlink;
			}
		}
	}

	// ִ��BFS����
	void do_bfs(OLGraph<K, T>& graph, std::function<void(VexBox<K, T>&)> address = [](VexBox<K, T>& node)->void {std::cout << node.data; })
	{
		// ��ͼ
		if (graph.vertices.empty()) return;

		std::vector<bool> vis(graph.vertices.size(), false); // �������ʱ�־

		// �������нڵ㣬ȷ������©�κβ���ͨ�Ĳ���
		for (int i = 0; i < graph.vertices.size(); i++)
		{
			bfs(i, vis, graph, address); // ��δ���ʵĽڵ㿪ʼBFS
		}
	}

	//���ڽӱ����ƣ���������������͹ؼ�·����
};