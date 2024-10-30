#include "Edge.h"
#include"Adjacency_Matrix.h"
#include"Adjacency_List.h"


void TestEdge()
{
    Edge_manager manager;

    std::vector<Edge> graph = {
        {0, 1}, {0, 2}, {1, 3}, {1, 4},
        {2, 5}, {3, 6}, {4, 6}, {5, 7},
        {6, 8}, {7, 9}, {8, 9}
    };

    // �ߵ�Ȩֵ���飬��Ӧ�����ߵĳ���ʱ��
    std::vector<int> weights = { 3, 2, 2, 1, 4, 3, 2, 3, 2, 4 ,5 };

    int node_count = 10;  // �ڵ�����Ϊ 10��0 �� 9��

    // ������б�
    std::cout << "��ʼͼ�ıߣ�" << std::endl;
    for (const auto& edge : graph) {
        std::cout << edge.u << " -> " << edge.v << std::endl;
    }

    // ���Բ����
    manager.InsertEdge(graph, 4, 7);
    std::cout << "\n����� 4 -> 7 ���ͼ��" << std::endl;
    for (const auto& edge : graph) {
        std::cout << edge.u << " -> " << edge.v << std::endl;
    }

    // ����ɾ����
    manager.DeleteEdge(graph, 4, 7);
    std::cout << "\nɾ���� 4 -> 7 ���ͼ��" << std::endl;
    for (const auto& edge : graph) {
        std::cout << edge.u << " -> " << edge.v << std::endl;
    }

    // ִ���������������DFS��
    std::cout << "DFS traversal: ";
    manager.do_dfs(graph, 10);
    std::cout << std::endl;

    // ִ�й������������BFS��
    std::cout << "BFS traversal: ";
    manager.do_bfs(graph, 10);
    std::cout << std::endl;

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
}

//��ӡ����
void PrintAdjMatrix(const Adj_Matrix& graph) {
    std::cout << "�ڽӾ���" << std::endl;
    for (const auto& row : graph.graph) {
        for (int val : row) {
            if (val == INT_MAX) {
                std::cout << "��" << "\t";  // �� �� ��ʾ������ޱߣ�
            }
            else {
                std::cout << val << "\t";
            }
        }
        std::cout << std::endl;
    }
}


void TestAdjMatrix() {
    // ����ͼ�Ķ��������ͱ���Ϣ
    int vertices = 10;
    const int numEdges = 11;

    int edges[numEdges][2] = {
        {0, 1}, {0, 2}, {1, 3}, {1, 4},
        {2, 5}, {3, 6}, {4, 6}, {5, 7},
        {6, 8}, {7, 9}, {8, 9}
    };

    // ����Ȩֵ
    std::vector<int> weights = { 3, 2, 2, 1, 4, 3, 2, 3, 2, 4, 5 };
    bool is_directed = true;  // ����ͼ

    // ����ͼ����������
    AdjMatrix_manager manager;
    Adj_Matrix graph = manager.CreateGraph(vertices, numEdges, edges, is_directed, weights);

    PrintAdjMatrix(graph);

    // ����DFS����
    std::cout << "������ȱ�����DFS�������" << std::endl;
    manager.do_dfs(graph, [](int& u) { std::cout << u << " "; });
    std::cout << std::endl;

    // ����BFS����
    std::cout << "������ȱ�����BFS�������" << std::endl;
    manager.do_bfs(graph, [](int& u) { std::cout << u << " "; });
    std::cout << std::endl;

    // ������������
    std::cout << "������������" << std::endl;
    std::vector<int> topo_order = manager.TopologicalSort(graph);
    for (int v : topo_order) {
        std::cout << v << " ";
    }
    std::cout << std::endl;

    // ���Թؼ�·��
    std::cout << "�ؼ�·����" << std::endl;
    std::vector<int> critical_path = manager.CriticalPath(graph);
    for (int v : critical_path) {
        std::cout << v << " ";
    }
    std::cout << std::endl;

}

// �����ڽӱ�
void TestAdjList() {
    // ����ͼ�Ķ��������ͱ���Ϣ
    int vertices = 10;  // ������Ϊ10
    const int numEdges = 11;  // ����Ϊ11

    // ����ߵ���Ϣ
    int edges[numEdges][2] = {
        {0, 1}, {0, 2}, {1, 3}, {1, 4},
        {2, 5}, {3, 6}, {4, 6}, {5, 7},
        {6, 8}, {7, 9}, {8, 9}
    };

    // ����ÿ���ߵ�Ȩֵ
    std::vector<int> weights = { 3, 2, 2, 1, 4, 3, 2, 3, 2, 4, 5 };
    bool is_directed = true;  // ���Ϊ����ͼ

    // ����ͼ����������
    AdjList_manager<int,int> manager;

    // ʹ��ͼ�����������ڽӱ�ͼ
    auto graph = manager.CreateGraph(vertices,numEdges, edges, weights, is_directed);

    // ��ӡ�ڽӱ�
    std::cout << "�ڽӱ�" << std::endl;
    manager.PrintGraph(graph);

    // ���Աߵ���ɾ
    std::cout << "��ӱ� (2, 3)��" << std::endl;
    manager.InSertArc(graph, 2, 3);
    manager.PrintGraph(graph);

    std::cout << "ɾ���� (2, 3)��" << std::endl;
    manager.DeleteArc(graph, 2, 3);
    manager.PrintGraph(graph);

    // ����������ȱ���
    std::cout << "������ȱ�����DFS�������" << std::endl;
    manager.do_dfs(graph);
    std::cout << std::endl;

    // ���Թ�����ȱ���
    std::cout << "������ȱ�����BFS�������" << std::endl;
    manager.do_bfs(graph);
    std::cout << std::endl;

    // ������������
    std::cout << "������������" << std::endl;
    std::vector<int> topo_order = manager.TopologicalSort(graph);
    for (int v : topo_order) {
        std::cout << v << " ";
    }
    std::cout << std::endl;

    // ���Թؼ�·��
    std::cout << "�ؼ�·����" << std::endl;
    std::vector<int> critical_path = manager.CriticalPath(graph);
    for (int v : critical_path) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
}



// ���������Դ���
int main() {
   
    TestEdge();

    system("Pause");

    TestAdjMatrix();

    system("Pause");

    TestAdjList();

    system("Pause");



    return 0;
}