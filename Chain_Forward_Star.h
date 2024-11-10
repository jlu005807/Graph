#pragma once
#include<iostream>
#include<vector>
#include<queue>

//��ʽǰ���ǣ���ʵ���Ǿ�̬�������ڽӱ��ֿ�����������ʵ�ֵ��ڽӱ�
//������ʽǰ���Ǵ洢���Ǳߵ��ڽӱ������ڽӱ�һ�����ǵ���ڽӱ�

struct edge
{
	//�߽�����
	int to;

	//��ǰ�ߵĺ��,��ʾ������������ͬ����һ���ߵı��
	int next;

	//Ȩֵ
	int info;

	edge(int to,int next,int info):to(to),next(next),info(info){ }
};


//������Ϣ��Ϊint

class Chain_forwardStar
{
public:
	//������
	int vexnum;

	//����
	int arcnum;

	//����
	bool is_direct;

	//�ߵ��ڽӱ�
	std::vector<edge> ChainStar;
	
	//head[i]���飬��ʾ�� i Ϊ�������һ���ߵı��
	std::vector<int> head;

	//���ұ�
	bool find_edge(int u, int v)
	{
		//u,v���Ϸ�
		if (u < 0 || u >= vexnum || v < 0 || v >= vexnum)
		{
			return false;
		}

		//����uΪ�׵ı������,ÿ��ת�Ƶ�������
		for (int i = head[u]; i != -1; i = ChainStar[i].next)
		{
			//�ҵ��˱�
			if (ChainStar[i].to == v)
			{
				return true;
			}
		}

		return false;
	}

	//���ӱ�
	void add(int u, int v,int weight=1)
	{
		//u,v���Ϸ�
		if (u < 0 || u >= vexnum || v < 0 || v >= vexnum)
		{
			return;
		}

		//�Ѿ�����
		if (find_edge(u, v))
		{
			return;
		}

		edge p(v, head[u], weight);

		ChainStar.push_back(p);

		//�ı�headָ��
		head[u] = ChainStar.size()-1;

		//�����,��ӶԳƱ�
		if (!is_direct)
		{
			edge q(u, head[v], weight);

			ChainStar.push_back(q);

			//�ı�headָ��
			head[v] = ChainStar.size()-1;
		}
	}

	//�����������
	void dfs(int u,std::vector<bool>& vis)
	{
		if (vis[u])return;

		//���
		vis[u] = true;

		//����������������Ҫ�����ڴ˴�ʵ��
		std::cout << u << " ";

		for (int i = head[u]; i != -1; i = ChainStar[i].next)
		{
			dfs(ChainStar[i].to, vis);
		}

		return;
	}

	void do_dfs(int u)
	{
		//��ʼ��
		std::vector<bool> vis;
		vis.resize(vexnum, false);

		for (int i = 0; i < vexnum; i++)
		{
			dfs(i, vis);
		}

		return;
	}

	//�����������
	void bfs(int u, std::vector<bool>& vis)
	{
		if (vis[u])return;

		//��ʼ��
		std::queue<int> q;

		q.push(u);

		vis[u] = true;

		while (!q.empty())
		{
			int v = q.front();
			q.pop();

			std::cout << v << " ";

			for (int i = head[v]; i != -1; i = ChainStar[i].next)
			{
				if (!vis[ChainStar[i].to])
				{
					q.push(ChainStar[i].to);
					vis[ChainStar[i].to] = true;
				}
			}
		}

	}

	void do_bfs(int u)
	{
		//��ʼ��
		std::vector<bool> vis;
		vis.resize(vexnum, false);

		for (int i = 0; i < vexnum; i++)
		{
			bfs(i, vis);
		}

		return;
	}

	void Init(int vex, int arc, int edge[][3],bool direct)
	{
		vexnum = vex;

		arcnum = arc;

		is_direct = direct;

		head.resize(vex, -1);

		for (int i = 0; i < arc; i++)
		{
			add(edge[i][0], edge[i][1], edge[i][2]);
		}
	}

	Chain_forwardStar(int vex, int arc, int edge[][3], bool direct = false)
	{
		Init(vex, arc, edge, direct);
	}

	void printGraph() {
		std::cout << "Graph adjacency list (Chain Forward Star representation):" << std::endl;
		for (int i = 0; i < vexnum; i++) {
			std::cout << "Vertex " << i << ":";
			for (int j = head[i]; j != -1; j = ChainStar[j].next) {
				std::cout << " -> (" << ChainStar[j].to << ", weight=" << ChainStar[j].info << ")";
			}
			std::cout << std::endl;
		}
	}

	//��������������

};