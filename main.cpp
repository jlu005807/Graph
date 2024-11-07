#include "Edge.h"
#include"Adjacency_Matrix.h"
#include"Adjacency_List.h"
#include"Orthogonal_List.h"
#include"Adjacency_Multilist.h"


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

    //������Ȩ���·��
    manager.SearchShortestPath(graph,10,0);
    std::cout << std::endl;

    // Define edges and weights
   // ����ߺ�Ȩ��
    std::vector<Edge> graph1 = {
        {0, 1}, {0, 2}, {1, 3}, {1, 4}, {2, 4}, {2, 5},
        {3, 6}, {4, 6}, {4, 7}, {5, 7}, {6, 7}, {3, 5}
    };

    std::vector<int> weights1 = { 3, 10, 5, 11, 7, 4, 2, 9, 6, 3, 1, 8 };


    // Define start and target vertices for Dijkstra's algorithm
    int start = 0;

    // Run Dijkstra's algorithm
    manager.Dijkstra_SearchShortestPath(graph1, 8, weights1, start);
    std::cout << std::endl;
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

    manager.PrintAdjMatrix(graph);

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

    //������Ȩ���·��
    manager.SearchShortestPath(graph, 0);
    std::cout << std::endl;

    // Define edges and weights
     // ����ߺ�Ȩ��
    int edges1[12][2] = {
        {0, 1}, {0, 2}, {1, 3}, {1, 4}, {2, 4}, {2, 5},
        {3, 6}, {4, 6}, {4, 7}, {5, 7}, {6, 7}, {3, 5}
    };
    std::vector<int> weights1 = { 3, 10, 5, 11, 7, 4, 2, 9, 6, 3, 1, 8 };

    // Create a directed graph with weighted edges
    Adj_Matrix graph1 = manager.CreateGraph(8, 12, edges1, true, weights1);

    // Define start and target vertices for Dijkstra's algorithm
    int start = 0;

    // Run Dijkstra's algorithm
    manager.Dijkstra_SearchShortestPath(graph1, start);
  
   
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

    //������Ȩ���·��
    manager.SearchShortestPath(graph, 0);
    std::cout << std::endl;

    // Define edges and weights
    // ����ߺ�Ȩ��
    int edges1[12][2] = {
        {0, 1}, {0, 2}, {1, 3}, {1, 4}, {2, 4}, {2, 5},
        {3, 6}, {4, 6}, {4, 7}, {5, 7}, {6, 7}, {3, 5}
    };
    std::vector<int> weights1 = { 3, 10, 5, 11, 7, 4, 2, 9, 6, 3, 1, 8 };

    // Create a directed graph with weighted edges
    Adj_List<int, int> graph1 = manager.CreateGraph(8, 12, edges1,  weights1 ,true);

    // Define start and target vertices for Dijkstra's algorithm
    int start = 0;

    // Run Dijkstra's algorithm
    manager.Dijkstra_SearchShortestPath(graph1, start);
}

//����ʮ������
void TestOLGraph()
{
    // ����������ʵ��
    OLGraph_manager<int, int> manager;

    // ����ͼ�Ķ������ͱ���
    int vertices = 10;  // ������Ϊ10
    const int numEdges = 11;  // ����Ϊ11

    // ����ߵ���Ϣ
    int edges[numEdges][2] = {
        {0, 1}, {0, 2}, {1, 3}, {1, 4},
        {2, 5}, {3, 6}, {4, 6}, {5, 7},
        {6, 8}, {7, 9}, {8, 9}
    };

    // ����ͼ
    OLGraph<int, int> graph = manager.CreateGraph(vertices, numEdges, edges);

    manager.PrintGraph(graph);

    // ����DFS����
    std::cout << "DFS�������: ";
    manager.do_dfs(graph, [](VexBox<int, int>& node) { std::cout << node.data << " "; });
    std::cout << std::endl;

    // ����BFS����
    std::cout << "BFS�������: ";
    manager.do_bfs(graph, [](VexBox<int, int>& node) { std::cout << node.data << " "; });
    std::cout << std::endl;

    // ����һ���±� (2, 8)
    manager.InSertArc(graph, 2, 8);
    std::cout << "����� (2, 8) ���BFS�������: ";
    manager.do_bfs(graph, [](VexBox<int, int>& node) { std::cout << node.data << " "; });
    std::cout << std::endl;

    // ɾ���� (1, 4)
    manager.DeleteArc(graph, 1, 4);
    std::cout << "ɾ���� (1, 4) ���DFS�������: ";
    manager.do_dfs(graph, [](VexBox<int, int>& node) { std::cout << node.data << " "; });
    std::cout << std::endl;

    // �����Ƿ���ڱ� (6, 8)
    bool found = manager.Find_Arc(graph, 6, 8);
    std::cout << "�� (6, 8) " << (found ? "����" : "������") << std::endl;

    // ����ͼ
    manager.DestroyGraph(graph);
    std::cout << "ͼ������" << std::endl;

    return;
}

//���Զ����ڽӱ�
void TestAdjMulList()
{
    // �������ͱ���
    int vertices = 10;
    const int numEdges = 11;

    // �ߵ���Ϣ
    int edges[numEdges][2] = {
        {0, 1}, {0, 2}, {1, 3}, {1, 4},
        {2, 5}, {3, 6}, {4, 6}, {5, 7},
        {6, 8}, {7, 9}, {8, 9}
    };

    // ��������������
    AdjMulList_manager<int, int> manager;

    // ������Ȩͼ
    AdjMul_List<int, int> graph = manager.CreateGraph(vertices, numEdges, edges);

    // ��ӡͼ�ṹ
    manager.PrintGraph(graph);

    // ���� DFS ����
    std::cout << "\nDFS Traversal:" << std::endl;
    manager.do_dfs(graph, [](Vexnode<int, int>& node) {
        std::cout << "Visited Vertex: " << node.data << std::endl;
    });

    // ���� BFS ����
    std::cout << "\nBFS Traversal:" << std::endl;
    manager.do_bfs(graph, [](Vexnode<int, int>& node) {
        std::cout << "Visited Vertex: " << node.data << std::endl;
    });

    // ����ɾ���� (����ɾ�� {6, 8} ��)
    std::cout << "\nDeleting edge (6, 8)" << std::endl;
    manager.DeleteArc(graph, 6, 8);

    // ��ӡɾ���ߺ��ͼ�ṹ
    manager.PrintGraph(graph);

    return;
}

// ���������Դ���
int main() {
   
    TestEdge();

    system("Pause");

    TestAdjMatrix();

    system("Pause");

    TestAdjList();

    system("Pause");

    TestOLGraph();

    system("Pause");

    TestAdjMulList();

    system("Pause");

    return 0;
}