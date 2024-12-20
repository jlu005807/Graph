#include "Edge.h"
#include"Adjacency_Matrix.h"
#include"Adjacency_List.h"
#include"Orthogonal_List.h"
#include"Adjacency_Multilist.h"
#include"Chain_Forward_Star.h"

void TestEdge()
{
    Edge_manager manager;

    int graphedge[][2] = {
        {0, 1}, {0, 2}, {1, 3}, {1, 4},
        {2, 5}, {3, 6}, {4, 6}, {5, 7},
        {6, 8}, {7, 9}, {8, 9}
    };

    // 边的权值数组，对应上述边的持续时间
    std::vector<int> weights = { 3, 2, 2, 1, 4, 3, 2, 3, 2, 4 ,5 };

    int node_count = 10;  // 节点数量为 10（0 到 9）

    std::vector<Edge> graph = manager.Init_Graph(graphedge, node_count, weights);

    // 输出所有边
    std::cout << "初始图的边：" << std::endl;
    manager.PrintGraph(graph);

    // 测试插入边
    manager.InsertEdge(graph, 4, 7);
    std::cout << "\n插入边 4 -> 7 后的图：" << std::endl;
    manager.PrintGraph(graph);

    // 测试删除边
    manager.DeleteEdge(graph, 4, 7);
    std::cout << "\n删除边 4 -> 7 后的图：" << std::endl;
    manager.PrintGraph(graph);

    // 执行深度优先搜索（DFS）
    std::cout << "DFS traversal: ";
    manager.do_dfs(graph, 10);
    std::cout << std::endl;

    // 执行广度优先搜索（BFS）
    std::cout << "BFS traversal: ";
    manager.do_bfs(graph, 10);
    std::cout << std::endl;

    // 执行拓扑排序
    std::vector<int> topo_order = manager.TopologicalSort(graph, node_count);

    // 输出拓扑排序结果
    if (!topo_order.empty()) {
        std::cout << "拓扑排序结果：";
        for (int node : topo_order) {
            std::cout << node << " ";
        }
        std::cout << std::endl;
    }
    else {
        std::cout << "图中存在环，无法进行拓扑排序。" << std::endl;
    }

    std::vector<int> critical_path = manager.CriticalPath(graph, node_count);

    // 输出关键路径,此路径为边在Graph对应的序号
    std::cout << "关键路径为：";
    for (int node : critical_path) {
        std::cout << node << " ";
    }
    std::cout << std::endl;

    //测试无权最短路径
    manager.SearchShortestPath(graph,10,0);
    std::cout << std::endl;

    // Define edges and weights
   // 定义边和权重
    int graph1edge[][2] = {
        {0, 1}, {0, 2}, {1, 3}, {1, 4}, {2, 4}, {2, 5},
        {3, 6}, {4, 6}, {4, 7}, {5, 7}, {6, 7}, {3, 5}
    };

    std::vector<int> weights1 = { 3, 10, 5, 11, 7, 4, 2, 9, 6, 3, 1, 8 };

    std::vector<Edge> graph1 = manager.Init_Graph(graph1edge, 8, weights1);

    // Define start and target vertices for Dijkstra's algorithm
    int start = 0;

    // Run Dijkstra's algorithm
    manager.Dijkstra_SearchShortestPath(graph1, 8, start);
    std::cout << std::endl;
}




void TestAdjMatrix() {
    // 定义图的顶点数量和边信息
    int vertices = 10;
    const int numEdges = 11;

    int edges[numEdges][2] = {
        {0, 1}, {0, 2}, {1, 3}, {1, 4},
        {2, 5}, {3, 6}, {4, 6}, {5, 7},
        {6, 8}, {7, 9}, {8, 9}
    };

    // 定义权值
    std::vector<int> weights = { 3, 2, 2, 1, 4, 3, 2, 3, 2, 4, 5 };
    bool is_directed = true;  // 有向图

    // 创建图管理器对象
    AdjMatrix_manager manager;
    Adj_Matrix graph = manager.CreateGraph(vertices, numEdges, edges, is_directed, weights);

    manager.PrintAdjMatrix(graph);

    // 测试DFS遍历
    std::cout << "深度优先遍历（DFS）结果：" << std::endl;
    manager.do_dfs(graph, [](int& u) { std::cout << u << " "; });
    std::cout << std::endl;

    // 测试BFS遍历
    std::cout << "广度优先遍历（BFS）结果：" << std::endl;
    manager.do_bfs(graph, [](int& u) { std::cout << u << " "; });
    std::cout << std::endl;

    // 测试拓扑排序
    std::cout << "拓扑排序结果：" << std::endl;
    std::vector<int> topo_order = manager.TopologicalSort(graph);
    for (int v : topo_order) {
        std::cout << v << " ";
    }
    std::cout << std::endl;

    // 测试关键路径
    std::cout << "关键路径：" << std::endl;
    std::vector<int> critical_path = manager.CriticalPath(graph);
    for (int v : critical_path) {
        std::cout << v << " ";
    }
    std::cout << std::endl;

    //测试无权最短路径
    manager.SearchShortestPath(graph, 0);
    std::cout << std::endl;

    // Define edges and weights
     // 定义边和权重
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

// 测试邻接表
void TestAdjList() {
    // 定义图的顶点数量和边信息
    int vertices = 10;  // 顶点数为10
    const int numEdges = 11;  // 边数为11

    // 定义边的信息
    int edges[numEdges][2] = {
        {0, 1}, {0, 2}, {1, 3}, {1, 4},
        {2, 5}, {3, 6}, {4, 6}, {5, 7},
        {6, 8}, {7, 9}, {8, 9}
    };

    // 定义每条边的权值
    std::vector<int> weights = { 3, 2, 2, 1, 4, 3, 2, 3, 2, 4, 5 };
    bool is_directed = true;  // 标记为有向图

    // 创建图管理器对象
    AdjList_manager<int,int> manager;

    // 使用图管理器创建邻接表图
    auto graph = manager.CreateGraph(vertices,numEdges, edges, weights, is_directed);

    // 打印邻接表
    std::cout << "邻接表：" << std::endl;
    manager.PrintGraph(graph);

    // 测试边的增删
    std::cout << "添加边 (2, 3)：" << std::endl;
    manager.InSertArc(graph, 2, 3);
    manager.PrintGraph(graph);

    std::cout << "删除边 (2, 3)：" << std::endl;
    manager.DeleteArc(graph, 2, 3);
    manager.PrintGraph(graph);

    // 测试深度优先遍历
    std::cout << "深度优先遍历（DFS）结果：" << std::endl;
    manager.do_dfs(graph);
    std::cout << std::endl;

    // 测试广度优先遍历
    std::cout << "广度优先遍历（BFS）结果：" << std::endl;
    manager.do_bfs(graph);
    std::cout << std::endl;

    // 测试拓扑排序
    std::cout << "拓扑排序结果：" << std::endl;
    std::vector<int> topo_order = manager.TopologicalSort(graph);
    for (int v : topo_order) {
        std::cout << v << " ";
    }
    std::cout << std::endl;

    // 测试关键路径
    std::cout << "关键路径：" << std::endl;
    std::vector<int> critical_path = manager.CriticalPath(graph);
    for (int v : critical_path) {
        std::cout << v << " ";
    }
    std::cout << std::endl;

    //测试无权最短路径
    manager.SearchShortestPath(graph, 0);
    std::cout << std::endl;

    // Define edges and weights
    // 定义边和权重
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

//测试十字链表
void TestOLGraph()
{
    // 创建管理类实例
    OLGraph_manager<int, int> manager;

    // 定义图的顶点数和边数
    int vertices = 10;  // 顶点数为10
    const int numEdges = 11;  // 边数为11

    // 定义边的信息
    int edges[numEdges][2] = {
        {0, 1}, {0, 2}, {1, 3}, {1, 4},
        {2, 5}, {3, 6}, {4, 6}, {5, 7},
        {6, 8}, {7, 9}, {8, 9}
    };

    // 创建图
    OLGraph<int, int> graph = manager.CreateGraph(vertices, numEdges, edges);

    manager.PrintGraph(graph);

    // 测试DFS遍历
    std::cout << "DFS遍历结果: ";
    manager.do_dfs(graph, [](VexBox<int, int>& node) { std::cout << node.data << " "; });
    std::cout << std::endl;

    // 测试BFS遍历
    std::cout << "BFS遍历结果: ";
    manager.do_bfs(graph, [](VexBox<int, int>& node) { std::cout << node.data << " "; });
    std::cout << std::endl;

    // 插入一条新边 (2, 8)
    manager.InSertArc(graph, 2, 8);
    std::cout << "插入边 (2, 8) 后的BFS遍历结果: ";
    manager.do_bfs(graph, [](VexBox<int, int>& node) { std::cout << node.data << " "; });
    std::cout << std::endl;

    // 删除边 (1, 4)
    manager.DeleteArc(graph, 1, 4);
    std::cout << "删除边 (1, 4) 后的DFS遍历结果: ";
    manager.do_dfs(graph, [](VexBox<int, int>& node) { std::cout << node.data << " "; });
    std::cout << std::endl;

    // 查找是否存在边 (6, 8)
    bool found = manager.Find_Arc(graph, 6, 8);
    std::cout << "边 (6, 8) " << (found ? "存在" : "不存在") << std::endl;

    // 销毁图
    manager.DestroyGraph(graph);
    std::cout << "图已销毁" << std::endl;

    return;
}

//测试多重邻接表
void TestAdjMulList()
{
    // 顶点数和边数
    int vertices = 10;
    const int numEdges = 11;

    // 边的信息
    int edges[numEdges][2] = {
        {0, 1}, {0, 2}, {1, 3}, {1, 4},
        {2, 5}, {3, 6}, {4, 6}, {5, 7},
        {6, 8}, {7, 9}, {8, 9}
    };

    // 创建管理器对象
    AdjMulList_manager<int, int> manager;

    // 生成无权图
    AdjMul_List<int, int> graph = manager.CreateGraph(vertices, numEdges, edges);

    // 打印图结构
    manager.PrintGraph(graph);

    // 测试 DFS 遍历
    std::cout << "\nDFS Traversal:" << std::endl;
    manager.do_dfs(graph, [](Vexnode<int, int>& node) {
        std::cout << "Visited Vertex: " << node.data << std::endl;
    });

    // 测试 BFS 遍历
    std::cout << "\nBFS Traversal:" << std::endl;
    manager.do_bfs(graph, [](Vexnode<int, int>& node) {
        std::cout << "Visited Vertex: " << node.data << std::endl;
    });

    // 测试删除边 (例如删除 {6, 8} 边)
    std::cout << "\nDeleting edge (6, 8)" << std::endl;
    manager.DeleteArc(graph, 6, 8);

    // 打印删除边后的图结构
    manager.PrintGraph(graph);

    return;
}

//测试链式前向星
void TestChain_forwardStar()
{
    // 顶点数
    int vexnum = 5;

    // 边数
    int arcnum = 6;

    // 边的信息，格式为 {起点, 终点, 权值}
    int edges[][3] = {
        {0, 1, 2},
        {0, 2, 3},
        {1, 3, 4},
        {2, 3, 5},
        {3, 4, 1},
        {4, 0, 7}
    };

    // 创建有向图
    Chain_forwardStar directedGraph(vexnum, arcnum, edges, true);
    std::cout << "Directed Graph:" << std::endl;
    directedGraph.printGraph();
    std::cout << "\n";
    std::cout << "Directed Graph DFS: ";
    directedGraph.do_dfs(0);
    std::cout << "\nDirected Graph BFS: ";
    directedGraph.do_bfs(0);
    std::cout << "\n";



    // 创建无向图
    Chain_forwardStar undirectedGraph(vexnum, arcnum, edges, false);
    std::cout << "Undirected Graph:" << std::endl;
    undirectedGraph.printGraph();
    std::cout << "\n";
    std::cout << "Undirected Graph DFS: ";
    undirectedGraph.do_dfs(0);
    std::cout << "\nUndirected Graph BFS: ";
    undirectedGraph.do_bfs(0);
    std::cout << "\n";

    // 查找边
    std::cout << "\nTesting find_edge function:\n";
    std::cout << "Edge 0 -> 1 in directedGraph: "
        << (directedGraph.find_edge(0, 1) ? "Found" : "Not Found") << "\n";
    std::cout << "Edge 1 -> 0 in directedGraph: "
        << (directedGraph.find_edge(1, 0) ? "Found" : "Not Found") << "\n";
    std::cout << "Edge 0 -> 1 in undirectedGraph: "
        << (undirectedGraph.find_edge(0, 1) ? "Found" : "Not Found") << "\n";
    std::cout << "Edge 1 -> 0 in undirectedGraph: "
        << (undirectedGraph.find_edge(1, 0) ? "Found" : "Not Found") << "\n";

}

// 主函数测试代码
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

    TestChain_forwardStar();

    system("Pause");

    return 0;
}