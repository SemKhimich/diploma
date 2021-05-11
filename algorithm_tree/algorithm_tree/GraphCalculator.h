#pragma once
#include "VertexInfo.h"
#include <vector>
#include <unordered_set>
#include <queue>


class GraphCalculator {
public:
    static std::vector<std::vector<std::pair<int, int>>> GetGammaTwoTree(const std::vector<std::vector<int>>& adjacency_list);

    static std::vector<std::vector<int>> GetChainsFourTree(const std::vector<std::vector<int>>& adjacency_list);

    static void Print(const std::vector<std::vector<int>>& adjacency_list);

    static bool IsTheoremWork(const std::vector<std::vector<int>>& adjacency_list);

    static std::vector<std::vector<int>> ReadTree();

    static std::vector<std::vector<int>> GenerateTree(int vertex_num);

private:
    static std::vector<VertexInfo> CalculateGraphInfo(const std::vector<std::vector<int>>& adjacency_list);

    static std::unordered_set<std::pair<int, int>> GetEdgeList(const std::vector<std::vector<int>>& adjacency_list);

    static std::vector<std::vector<int>> GetAdjacencyList(const std::vector<std::pair<int, int>>& edge_list);

    static int GraphReduction(int v, std::vector<VertexInfo>& tree_info, std::vector<bool>& is_deleted, std::vector<std::vector<int>>& chains_four);

    static void UpdateGraphInfo(int v, std::vector<VertexInfo>& tree_info, std::queue<int>& queue);
};
