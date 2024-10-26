#include "Edge.h"

// ���������Դ���
int main() {
    Edge_manager manager;

    std::vector<Edge> graph = {
        {0, 1}, {0, 2}, {1, 3}, {1, 4},
        {2, 5}, {3, 6}, {4, 6}, {5, 7},
        {6, 8}, {7, 9},{8,9}
    };

    // �ߵ�Ȩֵ���飬��Ӧ�����ߵĳ���ʱ��
    std::vector<int> weights = { 3, 2, 2, 1, 4, 3, 2, 3, 2, 4 ,5};

    int node_count = 10;  // �ڵ�����Ϊ 10��0 �� 9��


    // ִ����������
    std::vector<int> topo_order = manager.TopologicalSort(graph, node_count);

    // �������������
    if (!topo_order.empty()) {
        std::cout << "������������";
        for (int node : topo_order) {
            std::cout << node << " ";
        }
        std::cout << std::endl;
    }
    else {
        std::cout << "ͼ�д��ڻ����޷�������������" << std::endl;
    }

    std::vector<int> critical_path = manager.CriticalPath(graph, weights, node_count);

    // ����ؼ�·��,��·��Ϊ����Graph��Ӧ�����
    std::cout << "�ؼ�·��Ϊ��";
    for (int node : critical_path) {
        std::cout << node << " ";
    }
    std::cout << std::endl;

    // ִ���������������DFS��
    std::cout << "DFS traversal: ";
    manager.do_dfs(graph, 10); 
    std::cout << std::endl;

    // ִ�й������������BFS��
    std::cout << "BFS traversal: ";
    manager.do_bfs(graph, 10); 
    std::cout << std::endl;

    system("Pause");

    return 0;
}