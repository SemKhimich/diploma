#include <iostream>
#include <random>
#include <ctime>
#include "VertexInfo.h"
#include "GraphCalculator.h"

namespace std {
    template <> struct hash<std::pair<int, int>> {
        inline size_t operator()(const std::pair<int, int> &v) const {
            std::hash<int> int_hasher;
            return int_hasher(v.first) ^ int_hasher(v.second);
        }
    };
}

std::vector<std::vector<int>> GraphCalculator::GetAdjacencyList(const std::vector<std::pair<int, int>>& edge_list) {
    int max_vertex = 0;
    for (size_t i = 0; i < edge_list.size(); ++i) {
        max_vertex = std::max(max_vertex, edge_list[i].first);
        max_vertex = std::max(max_vertex, edge_list[i].second);
    }
    std::vector<std::vector<int>> adjacency_list(max_vertex + 1);
    for (size_t i = 0; i < edge_list.size(); ++i) {
        adjacency_list[edge_list[i].first].push_back(edge_list[i].second);
        adjacency_list[edge_list[i].second].push_back(edge_list[i].first);
    }
    return adjacency_list;
}

void GraphCalculator::Print(const std::vector<std::vector<int>>& adjacency_list) {
    std::unordered_set<std::pair<int, int>> edge_list(GetEdgeList(adjacency_list));
    std::cout << edge_list.size() + 1 << "\n";
    for (const auto& edge : edge_list) {
        std::cout << edge.first << " " << edge.second << "\n";
    }
}

bool GraphCalculator::IsTheoremWork(const std::vector<std::vector<int>>& adjacency_list) {
    std::vector<VertexInfo> graph_info(CalculateGraphInfo(adjacency_list));
    for (size_t v = 0; v < adjacency_list.size(); ++v) {
        if (graph_info[v].IsTheoremWork()) {
            return true;
        }
    }
    return false;
}

int GraphCalculator::GraphReduction(int v, std::vector<VertexInfo>& graph_info, std::vector<bool>& is_deleted, 
    std::vector<std::vector<int>>& chains_four) {
    while (graph_info[v].GetN1() > 0 && graph_info[v].GetS1() > 0) {
        int u = graph_info[v].GetRemoveFromN1(is_deleted);
        int w = graph_info[v].GetRemoveFromS1(is_deleted);
        int x = graph_info[w].GetRemoveFromN1(is_deleted);
        chains_four.push_back({ x, w, v, u });
    }
    while (graph_info[v].GetN1() > graph_info[v].GetN2()) {
        int u = graph_info[v].GetRemoveFromN1(is_deleted);
    }
    while (graph_info[v].GetDegree() > 1 && graph_info[v].GetS1() > graph_info[v].GetS2()) {
        int u = graph_info[v].GetRemoveFromS1(is_deleted);
        int w = graph_info[v].GetRemoveFromS1(is_deleted);
        int x = graph_info[u].GetRemoveFromN1(is_deleted);
        int y = graph_info[w].GetRemoveFromN1(is_deleted);
        chains_four.push_back({ x, u, v, w });
    }
    if (graph_info[v].GetDegree() == 2 && graph_info[v].GetS1() == 1) {
        int x = *graph_info[v].ordinary.begin();
        graph_info[x].ordinary.erase(v);
        is_deleted[v] = true;
        int y = graph_info[v].GetRemoveFromS1(is_deleted);
        int z = graph_info[y].GetRemoveFromN1(is_deleted);
        chains_four.push_back({ z, y, v, x });
        return x;
    }
    return v;
}

void GraphCalculator::UpdateGraphInfo(int v, std::vector<VertexInfo>& graph_info, std::queue<int>& queue) {
    queue.push(v);
    if (graph_info[v].GetDegree() == 1) {
        if (graph_info[v].GetN1() == 1) {
            int w = *graph_info[v].n1.begin();
            graph_info[w].ChangeVertexToN1(v);
            queue.push(w);
            return;
        }
        if (graph_info[v].GetS1() == 1) {
            int w = *graph_info[v].s1.begin();
            int u = *graph_info[w].n1.begin();
            graph_info[u].ChangeVertexToS1(w);
            graph_info[w].ChangeVertexToN1(v);
            queue.push(w);
            queue.push(u);
            return;
        }
        int w = *graph_info[v].ordinary.begin();
        if (graph_info[w].s1.find(v) != graph_info[w].s1.end() || 
            graph_info[w].n1.find(v) != graph_info[w].n1.end() ) {
            throw std::runtime_error("");
        }
        graph_info[w].ChangeVertexFromOrdinaryToN1(v);
        queue.push(w);
        if (graph_info[w].GetDegree() == 2) {
            int y;
            if (graph_info[w].GetS1() == 1) {
                y = *graph_info[w].s1.begin();
            } else {
                y = *graph_info[w].ordinary.begin();
            }
            graph_info[y].ChangeVertexFromOrdinaryToS1(w);
            queue.push(y);
        }
    } else if (graph_info[v].GetDegree() == 2) {
        if (graph_info[v].GetN1() == 2) {
            int w = *graph_info[v].n1.begin();
            graph_info[w].ChangeVertexToS1(v);
            queue.push(w);
        } else if (graph_info[v].GetN1() == 1) {
            int w;
            if (graph_info[v].GetS1() == 1) {
                w = *graph_info[v].s1.begin();
            } else {
                w = *graph_info[v].ordinary.begin();
            }
            graph_info[w].ChangeVertexToS1(v);
            queue.push(w);
        }
    }
}

std::vector<std::vector<int>> GraphCalculator::GetChainsFourTree(const std::vector<std::vector<int>>& adjacency_list) {
    std::vector<VertexInfo> graph_info(CalculateGraphInfo(adjacency_list));
    std::vector<bool> is_deleted = std::vector<bool>(adjacency_list.size(), false);
    std::vector<std::vector<int>> chains_four;
    std::queue<int> queue;
    for (size_t v = 0; v < adjacency_list.size(); ++v) {
        queue.push(v);
    }
    while (!queue.empty()) {
        int v = queue.front();
        queue.pop();
        if (!graph_info[v].IsTheoremWork() || is_deleted[v]) {
            continue;
        }
        v = GraphReduction(v, graph_info, is_deleted, chains_four);
        UpdateGraphInfo(v, graph_info, queue);
    }
    return chains_four;
}

std::vector<VertexInfo> GraphCalculator::CalculateGraphInfo(const std::vector<std::vector<int>>& adjacency_list) {
    std::vector<VertexInfo> graph_info;
    graph_info.reserve(adjacency_list.size());
    for (size_t v = 0; v < adjacency_list.size(); ++v) {
        VertexInfo vertex_inf;
        for (const auto& u : adjacency_list[v]) {
            if (adjacency_list[u].size() == 1) {
                vertex_inf.n1.insert(u);
            } else if (adjacency_list[u].size() == 2) {
                int w = adjacency_list[u][0];
                if (w == v) {
                    w = adjacency_list[u][1];
                }
                if (adjacency_list[w].size() == 1) {
                    vertex_inf.s1.insert(u);
                } else {
                    vertex_inf.ordinary.insert(u);
                }
            } else {
                vertex_inf.ordinary.insert(u);
            }
        }
        graph_info.push_back(vertex_inf);
    }
    return graph_info;
}

std::unordered_set<std::pair<int, int>> GraphCalculator::GetEdgeList(const std::vector<std::vector<int>>& adjacency_list) {
    std::unordered_set<std::pair<int, int>> edge_list;
    for (size_t v = 0; v < adjacency_list.size(); ++v) {
        for (const auto& u : adjacency_list[v]) {
            if (u < v) {
                edge_list.emplace(u, v);
            }
        }
    }
    return edge_list;
}

std::vector<std::vector<std::pair<int, int>>> GraphCalculator::GetGammaTwoTree(const std::vector<std::vector<int>>& adjacency_list) {
    std::vector<std::vector<int>> chains_four(GetChainsFourTree(adjacency_list));
    std::unordered_set<std::pair<int, int>> edge_list(GetEdgeList(adjacency_list));
    std::vector<std::vector<std::pair<int, int>>> gamma_two;
    for (const auto& chain_four : chains_four) {
        std::vector<std::pair<int, int>> graph;
        for (size_t i = 0; i + 1 < chain_four.size(); ++i) {
            graph.emplace_back(chain_four[i], chain_four[i + 1]);
            if (chain_four[i] < chain_four[i + 1]) {
                edge_list.erase(std::make_pair(chain_four[i], chain_four[i + 1]));
            } else {
                edge_list.erase(std::make_pair(chain_four[i + 1], chain_four[i]));
            }
        }
        gamma_two.push_back(graph);
    }
    while (edge_list.size() > 1) {
        std::vector<std::pair<int, int>> graph;
        std::pair<int, int> edge = *edge_list.begin();
        edge_list.erase(edge);
        graph.push_back(edge);
        edge = *edge_list.begin();
        edge_list.erase(edge);
        graph.push_back(edge);
        gamma_two.push_back(graph);
    }
    if (!edge_list.empty()) {
        gamma_two.push_back(std::vector<std::pair<int, int>>{*edge_list.begin()});
    }
    return gamma_two;
}

std::vector<std::vector<int>> GraphCalculator::ReadTree() {
    int vertex_num;
    std::cin >> vertex_num;
    std::vector<std::pair<int, int>> edge_list;
    for (int i = 0; i + 1 < vertex_num; ++i) {
        int u, v;
        std::cin >> u >> v;
        edge_list.push_back(std::make_pair(u, v));
    }
    return GetAdjacencyList(edge_list);
}

std::vector<std::vector<int>> GraphCalculator::GenerateTree(int vertex_num) {
    std::mt19937 generator;
    generator.seed(time(nullptr));
    std::vector<std::pair<int, int>> edge_list;
    edge_list.reserve(vertex_num - 1);
    for (int i = 0; i + 1 < vertex_num; ++i) {
        std::uniform_int_distribution<> distribution(0, i);
        edge_list.push_back(std::make_pair(i + 1, distribution(generator)));
    }
    return GetAdjacencyList(edge_list);
}
