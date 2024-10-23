#include "Edge.h"

// ���������Դ���
int main() {
    Edge_manager manager;

    // ��ʼ��ͼ�ı�
    int edges[][2] = {
        {0, 1}, {0, 2}, {1, 3}, {1, 4}, {2, 5}, {3, 4}, {5, 0}, // ��ͨ��֧ 1
        {6, 7}, {7, 8}, {8, 9}, {9, 6}, // ��ͨ��֧ 2
    };

    // ����ͼ
    std::vector<Edge> graph = manager.Init_Graph(edges, sizeof(edges) / sizeof(edges[0]));

    // ִ���������������DFS��
    std::cout << "DFS traversal: ";
    manager.do_dfs(graph, 10); // ����ڵ��Ŵ�0��14����15���ڵ�
    std::cout << std::endl;

    // ִ�й������������BFS��
    std::cout << "BFS traversal: ";
    manager.do_bfs(graph, 10); // ����ڵ��Ŵ�0��14����15���ڵ�
    std::cout << std::endl;

    system("Pause");

    return 0;
}