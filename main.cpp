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

    // 边的权值数组，对应上述边的持续时间
    std::vector<int> weights = { 3, 2, 2, 1, 4, 3, 2, 3, 2, 4 ,5 };

    int node_count = 10;  // 节点数量为 10（0 到 9）

    // 输出所有边
    std::cout << "初始图的边：" << std::endl;
    for (const auto& edge : graph) {
        std::cout << edge.u << " -> " << edge.v << std::endl;
    }

    // 测试插入边
    manager.InsertEdge(graph, 4, 7);
    std::cout << "\n插入边 4 -> 7 后的图：" << std::endl;
    for (const auto& edge : graph) {
        std::cout << edge.u << " -> " << edge.v << std::endl;
    }

    // 测试删除边
    manager.DeleteEdge(graph, 4, 7);
    std::cout << "\n删除边 4 -> 7 后的图：" << std::endl;
    for (const auto& edge : graph) {
        std::cout << edge.u << " -> " << edge.v << std::endl;
    }

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

    std::vector<int> critical_path = manager.CriticalPath(graph, weights, node_count);

    // 输出关键路径,此路径为边在Graph对应的序号
    std::cout << "关键路径为：";
    for (int node : critical_path) {
        std::cout << node << " ";
    }
    std::cout << std::endl;
}

//打印矩阵
void PrintAdjMatrix(const Adj_Matrix& graph) {
    std::cout << "邻接矩阵：" << std::endl;
    for (const auto& row : graph.graph) {
        for (int val : row) {
            if (val == INT_MAX) {
                std::cout << "∞" << "\t";  // 用 ∞ 表示无穷大（无边）
            }
            else {
                std::cout << val << "\t";
            }
        }
        std::cout << std::endl;
    }
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

    PrintAdjMatrix(graph);

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
}



// 主函数测试代码
int main() {
   
    TestEdge();

    system("Pause");

    TestAdjMatrix();

    system("Pause");

    TestAdjList();

    system("Pause");



    return 0;
}