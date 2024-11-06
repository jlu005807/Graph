#pragma once
#include<iostream>
#include<memory>
#include<functional>
#include<queue>
#include<vector>

//�ڽӶ��ر� (Adjacency Multilist) ��**����ͼ**����һ����ʽ�洢�ṹ��
//���ڽӶ��ر��У�ÿһ������һ������ʾ

//�߽��
//markΪ��־�򣬿����Ա�Ǹ������Ƿ�������
//ivex��jvexΪ�ñ�����������������ͼ�е�λ��
//ilinkָ����һ����������ivex�ıߣ�jlinkָ����һ����������jvex�ı�
//infoΪָ��ͱ���صĸ�����Ϣ��ָ����
template<class T>
class EdgeBox
{
public:
	//���ʱ��
	bool mark;

	//�ñ����������������λ��
	int ivex;
	int jvex;

	//ָ�������������������һ����
	std::shared_ptr<EdgeBox<T>> ilink;

	std::shared_ptr<EdgeBox<T>> jlink;

	//����Ϣ
	T info;

	EdgeBox() = default;

	//Ĭ������Ϣ
	EdgeBox(int i,int j, std::shared_ptr<EdgeBox<T>> ilink=nullptr, std::shared_ptr<EdgeBox<T>> jlink=nullptr):mark(false),ivex(i),jvex(j),ilink(ilink),jlink(jlink){ }
};


//����
//data��洢�͸ö�����ص���Ϣ
//firstedge��ָʾ��һ�������ڸö���ı�
template<typename K=int,typename T=int>
class Vexnode
{
public:
	//������Ϣ��Ĭ��Ϊ��������
	K data;

	//ָ���һ�������ö���ı�
	std::shared_ptr<EdgeBox<T>> firstedge;

	Vexnode() = default;

	Vexnode(K d, std::shared_ptr<EdgeBox<T>> first=nullptr):data(d),firstedge(first){ }


	void Add_iEdge(std::shared_ptr<EdgeBox<T>> edge)
	{
		std::shared_ptr<EdgeBox<T>> p = firstedge;

		firstedge = edge;

		firstedge->ilink = p;
	}

	void Add_jEdge(std::shared_ptr<EdgeBox<T>> edge)
	{
		std::shared_ptr<EdgeBox<T>> p = firstedge;

		firstedge = edge;

		firstedge->jlink = p;
	}
};


//ͼ�ı�
template<typename K=int ,typename T=int>
class AdjMul_List
{
public:
	//��ͷ������vector.size()��������,���Բ���Ҫ��¼������
	std::vector<Vexnode<K, T>> vertices;

	//��¼����
	int arcnum;

	//��data�Ķ���,Ĭ��dataΪ���
	AdjMul_List(int vexnum, int arcnum) :arcnum(arcnum)
	{
		//��ʼ��arcnum
		vertices.resize(vexnum);
		for (int i = 0; i < vexnum; i++)
		{
			vertices[i].data = i;
		}
	}

	//��Ҫdata�Ķ���
	AdjMul_List(int vexnum, int arcnum, K datas[]) :arcnum(arcnum)
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
class AdjMulList_manager
{
public:
	//����n�����㣬m���ߵ�ͼ
	AdjMul_List<K, T> CreateGraph(int n, int m, int edge[][2])
	{
		//��ʼ��
		AdjMul_List<K, T> list(n, m);

		//����ߵ���Ϣ���˴�ʹ��ͷ�巨������edge�е�λ����List�е��ȶ���
		for (int i = 0; i < m; i++)
		{
			//ȷ����
			int u = edge[i][0];
			int v = edge[i][1];

			//����arc
			std::shared_ptr<EdgeBox<T>> arc(new EdgeBox<T>(u,v));

			//���i����ı�
			list.vertices[u].Add_iEdge(arc);

			//���j����ı�
			list.vertices[v].Add_jEdge(arc);

		}

		return list;
	}

	//����n�����㣬m���ߵĴ�Ȩͼ
	AdjMul_List<K, T> CreateGraph(int n, int m, int edge[][2],std::vector<int> weights)
	{
		//��ʼ��
		AdjMul_List<K, T> list(n, m);

		//����ߵ���Ϣ���˴�ʹ��ͷ�巨������edge�е�λ����List�е��ȶ���
		for (int i = 0; i < m; i++)
		{
			//ȷ����
			int u = edge[i][0];
			int v = edge[i][1];

			//����arc
			std::shared_ptr<EdgeBox<T>> arc(new EdgeBox<T>(u, v));

			arc->info = weights[i];

			//���i����ı�
			list.vertices[u].Add_iEdge(arc);

			//���j����ı�
			list.vertices[v].Add_jEdge(arc);

		}

		return list;
	}

	//��ͼG���ڣ�����ͼG��
	void DestroyGraph(AdjMul_List<K, T>& list)
	{
		//��ͼ
		if (list.vertices.size() == 0)
		{
			return;
		}

		for (auto& vex : list.vertices)
		{
			// �����ͷ������ڵ�ǰ��������б�
			std::shared_ptr<EdgeBox<T>> edge = vex.firstedge;

			while (edge != nullptr)
			{
				// ������һ��ָ��
				std::shared_ptr<EdgeBox<T>> nextEdge = (edge->ivex == vex.data) ? edge->ilink : edge->jlink;

				// ���õ�ǰ��ָ�룬�ͷ��ڴ�
				edge.reset();

				// �ƶ�����һ����
				edge = nextEdge;
			}

			// �������� firstedge ָ��
			vex.firstedge.reset();
		}

		//�������
		list.vertices.clear();

		list.arcnum = 0;

		return;
	}

	//����v�ĵ�һ���ڽӶ������š���v��G��û���ڽӶ��㣬�򷵻� ���ա���
	int FirstAdjVex(AdjMul_List<K, T>& list, int v)
	{
		//��ͼ����v���Ϸ�
		if (list.vertices.size() == 0 || v < 0 || v >= list.vertices.size())
		{
			return -1;
		}

		//û���ڽӵ�
		if (!list.vertices[v].firstedge)
		{
			return -1;
		}

		return list.vertices[v].firstedge.jvex;
	}

	//����v�ģ����ǧw�ģ���һ���ڽӶ��㡣��w��v�����һ���ڽӵ㣬�򷵻� ���ա���
	int NextAdjVex(AdjMul_List<K, T>& list, int v, int w)
	{
		//��ͼ����v���Ϸ�����w���Ϸ�
		if (list.vertices.size() == 0 || v < 0 || v >= list.vertices.size() || w < 0 || w >= list.vertices.size())
		{
			return -1;
		}

		std::shared_ptr<EdgeBox<T>> arc = list.vertices[v].firstedge;

		//�ҵ�w
		while (arc->ilink&& arc->jvex != w)
		{
			arc = arc->ilink;
		}


		if (arc)
		{
			return arc->ilink->jvex;
		}
		else
		{
			return -1;
		}

	}

	//��������ɾ����


	//��G��ɾ����<v, w>
	void DeleteArc(AdjMul_List<K, T>& list, int v, int w)
	{
		//��ͼ����v���Ϸ�����w���Ϸ�
		if (list.vertices.size() == 0 || v < 0 || v >= list.vertices.size() || w < 0 || w >= list.vertices.size())
		{
			return;
		}

		
		// �ͷ���������v�ı�
		auto freeivex = [&](std::shared_ptr<EdgeBox<T>>& firstEdge, int k) -> void {
			// ����ױ߾���Ҫɾ���ı�
			if (firstEdge && firstEdge->jvex == k) {
				firstEdge = firstEdge->ilink;  // �����ױ�ָ��
				return;
			}

			// ����Ѱ�Ҳ�ɾ����
			std::shared_ptr<EdgeBox<T>> arc = firstEdge;
			while (arc && arc->ilink) {
				if (arc->ilink->jvex == k) {
					arc->ilink = arc->ilink->ilink;  // ɾ���м�ڵ�
					return;
				}
				arc = arc->ilink;
			}
		};

		// �ͷ���������w�ı�
		auto freejvex = [&](std::shared_ptr<EdgeBox<T>>& firstEdge, int k) -> void {
			// ����ױ߾���Ҫɾ���ı�
			if (firstEdge && firstEdge->ivex == k) {
				firstEdge = firstEdge->jlink;  // �����ױ�ָ��
				return;
			}

			// ����Ѱ�Ҳ�ɾ����
			std::shared_ptr<EdgeBox<T>> arc = firstEdge;
			while (arc && arc->jlink) {
				if (arc->jlink->ivex == k) {
					arc->jlink = arc->jlink->jlink;  // ɾ���м�ڵ�
					return;
				}
				arc = arc->jlink;
			}
		};

		freeivex(list.vertices[v].firstedge, w);

		freejvex(list.vertices[w].firstedge, v);

	}

	//��ͼ��Ѱ��v��w֮��Ļ�
	bool Find_Arc(AdjMul_List<K, T>& list, int v, int w)
	{
		//��ͼ����v���Ϸ�����w���Ϸ�
		if (list.vertices.size() == 0 || v < 0 || v >= list.vertices.size() || w < 0 || w >= list.vertices.size())
		{
			return false;
		}

		std::shared_ptr<EdgeBox<T>> arc = list.vertices[v].firstedge;

		while (arc && arc->jvex != w)
		{
			arc = arc->jlink;
		}

		//�ҵ�
		if (arc)
		{
			return true;
		}

		return false;
	}

	//��G������<v, w>, ��G������ͼ��������Գƻ�<w, v>��
	void InSertArc(AdjMul_List<K, T>& list, int v, int w)
	{
		//��ͼ����v���Ϸ�����w���Ϸ�
		if (list.vertices.size() == 0 || v < 0 || v >= list.vertices.size() || w < 0 || w >= list.vertices.size())
		{
			return;
		}


		//ͼ���޴˻�
		if (!Find_Arc(list, v, w))
		{
			std::shared_ptr<EdgeBox<T>> newedge = new EdgeBox<T>(v, w);

			list.vertices[v].Add_iEdge(newedge);

			list.vertices[w].Add_jEdge(newedge);
		}

		return;
	}

	//�������������DFS���㷨,���Խڵ���д���,vis�����Ƿ����
	void dfs(int u, std::vector<bool>& vis, AdjMul_List<K, T>& list, std::function<void(Vexnode<K, T>&)> address = [](Vexnode<K, T>& node)->void {std::cout << node.data; })
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
		std::shared_ptr<EdgeBox<T>> p = list.vertices[u].firstedge;

		while (p)
		{
			dfs(p->jvex, vis, list, address);

			p = p->ilink;
		}

	}

	void do_dfs(AdjMul_List<K, T>& list, std::function<void(Vexnode<K, T>&)> address = [](Vexnode<K, T>& node)->void {std::cout << node.data; })
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
	void bfs(int start, std::vector<bool>& vis, AdjMul_List<K, T>& list, std::function<void(Vexnode<K, T>&)> address = [](Vexnode<K, T>& node)->void {std::cout << node.data; })
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
			std::shared_ptr<EdgeBox<T>> p = list.vertices[u].firstedge;

			while (p)
			{
				//δ����
				if (!vis[p->jvex])
				{
					vis[p->jvex] = true;
					q.push(p->jvex);
				}

				p = p->ilink;
			}
		}
	}

	// ִ��BFS����
	void do_bfs(AdjMul_List<K, T>& list, std::function<void(Vexnode<K, T>&)> address = [](Vexnode<K, T>& node)->void {std::cout << node.data; })
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

	//���������ڽӱ���������������͹ؼ�·��


	void PrintGraph(const AdjMul_List<K, T>& graph) {
		std::cout << "Graph with " << graph.vertices.size() << " vertices and "
			<< graph.arcnum << " edges:" << std::endl;

		// ����ÿ�����㣬��ӡ��֮�����ı�
		for (const auto& vertex : graph.vertices) {
			std::cout << "Vertex " << vertex.data << ":";

			// ��ȡÿ������ĵ�һ����
			auto edge = vertex.firstedge;

			if (!edge) {
				std::cout << " No edges" << std::endl;
				continue;
			}

			// ����ÿ����
			while (edge) {
				std::cout << " -> Edge (" << edge->ivex << ", " << edge->jvex
					<< "), Weight: " << edge->info
					<< ", Marked: " << (edge->mark ? "Yes" : "No");

				// ���ݶ����� ivex ���� jvex ��ѡ����һ����
				edge = (edge->ivex == vertex.data) ? edge->ilink : edge->jlink;
			}
			std::cout << std::endl;
		}
	}

};
