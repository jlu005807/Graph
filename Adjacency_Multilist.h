#pragma once
#include<iostream>
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
	EdgeBox<T>* ilink;
	EdgeBox<T>* jlink;

	//����Ϣ
	T info;

	EdgeBox() = default;

	//Ĭ������Ϣ
	EdgeBox(int i,int j, EdgeBox<T>* ilink, EdgeBox<T>* jlink):mark(false),ivex(i),jvex(j),ilink(ilink),jlink(jlink){ }
};


//����
//data��洢�͸ö�����ص���Ϣ
//firstedge��ָʾ��һ�������ڸö���ı�
template<typename K=int,typename T=int>
class Vex
{
public:
	//������Ϣ��Ĭ��Ϊ��������
	K data;

	//ָ���һ�������ö���ı�
	EdgeBox<T>* firstedge;

	Vex() = default;

	Vex(K d, EdgeBox<T>* first):data(d),firstedge(first){ }

};


//ͼ�ı�
template<typename K=int ,typename T=int>
class AMLGraph
{
public:
	//��ͷ������vector.size()��������,���Բ���Ҫ��¼������
	std::vector<VexNode<K, T>> vertices;

	//��¼����
	int arcnum;

};