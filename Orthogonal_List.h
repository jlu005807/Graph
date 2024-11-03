#pragma once
#include<iostream>
#include<vector>

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
	ArcBox<T>* tlink;

	//��ͷ��ͬ�Ļ�������
	ArcBox<T>* hlink;

	//��Ϣ
	T info;

	ArcBox() = default;

	//Ĭ������Ϣ
	ArcBox(int tail,int head, ArcBox<T>* t=nullptr, ArcBox<T>* h=nullptr):tailvext(tail),headvext(head),tlink(t),hlink(h){ }

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
	ArcBox<T>* firstin;

	//�ö���Ϊ��β�ĵ�һ����
	ArcBox<T>* firstout;

	VexBox() = default;

	VexBox(K d, ArcBox<T>* firstin=nullptr, ArcBox<T>* firstout=nullptr):data(d),firstin(firstin),firstout(firstout){ }

};

//��
template<typename K = int, typename T = int>
class OLGraph
{
public:
	//��ͷ������vector.size()��������,���Բ���Ҫ��¼������
	std::vector<VexNode<K, T>> vertices;

	//��¼����
	int arcnum;

};

