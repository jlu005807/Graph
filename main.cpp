#include "Edge.h"

// 主函数测试代码
int main() {
    Edge_manager manager;

    std::vector<Edge> graph = {
        {0, 1}, {0, 2}, {1, 3}, {1, 4},
        {2, 5}, {3, 6}, {4, 6}, {5, 7},
        {6, 8}, {7, 9},{8,9}
    };

    // 边的权值数组，对应上述边的持续时间
    std::vector<int> weights = { 3, 2, 2, 1, 4, 3, 2, 3, 2, 4 ,5};

    int node_count = 10;  // 节点数量为 10（0 到 9）


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

    // 执行深度优先搜索（DFS）
    std::cout << "DFS traversal: ";
    manager.do_dfs(graph, 10); 
    std::cout << std::endl;

    // 执行广度优先搜索（BFS）
    std::cout << "BFS traversal: ";
    manager.do_bfs(graph, 10); 
    std::cout << std::endl;

    system("Pause");

    return 0;
}