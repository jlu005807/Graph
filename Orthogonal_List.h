#pragma once
#include<iostream>
#include<vector>

//十字链表(Orthogonal List) 是**有向图**的另一种链式存储结构
//可以看成是将有向图的邻接表和逆邻接表结合起来得到的一种链表

//弧结点
//尾域(tailvex) 和头域 (headvex)分别指示弧尾和弧头这两个顶点在图中的位置
//链域hlink指向弧头相同的下一条弧，而链域tlink指向弧尾相同的下一条弧
//info域指向该弧的相关信息，例如权值
template<typename T=int>
class ArcBox
{
public:
	//弧尾结点
	int tailvext;
	//弧头结点
	int headvext;

	//弧尾相同的弧的链域
	ArcBox<T>* tlink;

	//弧头相同的弧的链域
	ArcBox<T>* hlink;

	//信息
	T info;

	ArcBox() = default;

	//默认无信息
	ArcBox(int tail,int head, ArcBox<T>* t=nullptr, ArcBox<T>* h=nullptr):tailvext(tail),headvext(head),tlink(t),hlink(h){ }

};


//顶点
//data域存储和顶点相关的信息
//firstin和firstout为两个链域，分别指向以该顶点为弧头或弧尾的第一个弧结点。
template<typename K=int,typename T=int>
class VexBox
{
public:
	//顶点信息，默认为序号
	K data;

	//该顶点为弧头的第一条弧
	ArcBox<T>* firstin;

	//该顶点为弧尾的第一条弧
	ArcBox<T>* firstout;

	VexBox() = default;

	VexBox(K d, ArcBox<T>* firstin=nullptr, ArcBox<T>* firstout=nullptr):data(d),firstin(firstin),firstout(firstout){ }

};

//表
template<typename K = int, typename T = int>
class OLGraph
{
public:
	//表头结点表，此vector.size()即顶点数,所以不需要记录顶点数
	std::vector<VexNode<K, T>> vertices;

	//记录边数
	int arcnum;

};

