#pragma once
#include<iostream>
#include<vector>

//邻接多重表 (Adjacency Multilist) 是**无向图**的另一种链式存储结构。
//在邻接多重表中，每一条边用一个结点表示

//边结点
//mark为标志域，可用以标记该条边是否被搜索过
//ivex和jvex为该边依附的两个顶点在图中的位置
//ilink指向下一条依附顶点ivex的边，jlink指向下一条依附顶点jvex的边
//info为指向和边相关的各种信息的指针域
template<class T>
class EdgeBox
{
public:
	//访问标记
	bool mark;

	//该边依附的两个顶点的位置
	int ivex;
	int jvex;

	//指向依附这两个顶点的下一条边
	EdgeBox<T>* ilink;
	EdgeBox<T>* jlink;

	//边信息
	T info;

	EdgeBox() = default;

	//默认无信息
	EdgeBox(int i,int j, EdgeBox<T>* ilink, EdgeBox<T>* jlink):mark(false),ivex(i),jvex(j),ilink(ilink),jlink(jlink){ }
};


//顶点
//data域存储和该顶点相关的信息
//firstedge域指示第一条依附于该顶点的边
template<typename K=int,typename T=int>
class Vex
{
public:
	//顶点信息，默认为表里的序号
	K data;

	//指向第一条依附该顶点的边
	EdgeBox<T>* firstedge;

	Vex() = default;

	Vex(K d, EdgeBox<T>* first):data(d),firstedge(first){ }

};


//图的表
template<typename K=int ,typename T=int>
class AMLGraph
{
public:
	//表头结点表，此vector.size()即顶点数,所以不需要记录顶点数
	std::vector<VexNode<K, T>> vertices;

	//记录边数
	int arcnum;

};