#include "Edge.h"

// 主函数测试代码
int main() {
    Edge_manager manager;

    // 初始化图的边
    int edges[][2] = {
        {0, 1}, {0, 2}, {1, 3}, {1, 4}, {2, 5}, {3, 4}, {5, 0}, // 连通分支 1
        {6, 7}, {7, 8}, {8, 9}, {9, 6}, // 连通分支 2
    };

    // 创建图
    std::vector<Edge> graph = manager.Init_Graph(edges, sizeof(edges) / sizeof(edges[0]));

    // 执行深度优先搜索（DFS）
    std::cout << "DFS traversal: ";
    manager.do_dfs(graph, 10); // 假设节点编号从0到14，共15个节点
    std::cout << std::endl;

    // 执行广度优先搜索（BFS）
    std::cout << "BFS traversal: ";
    manager.do_bfs(graph, 10); // 假设节点编号从0到14，共15个节点
    std::cout << std::endl;

    system("Pause");

    return 0;
}