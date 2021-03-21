#include <iostream>
#include <deque>
#include <random>
#include <ctime>
#include "Tree.h"

namespace std {
    template <> struct hash<std::pair<int, int>> {
        inline size_t operator()(const std::pair<int, int> &v) const {
            std::hash<int> int_hasher;
            return int_hasher(v.first) ^ int_hasher(v.second);
        }
    };
}

Tree::Tree(const std::vector<std::pair<int, int>>& edge_list) : adjacency_list_(edge_list.size() + 1) {
    for (size_t i = 0; i < edge_list.size(); ++i) {
        adjacency_list_[edge_list[i].first].push_back(edge_list[i].second);
        adjacency_list_[edge_list[i].second].push_back(edge_list[i].first);
    }
}

void Tree::Print() {
    std::unordered_set<std::pair<int, int>> edge_list(GetEdgeList());
    std::cout << edge_list.size() + 1 << "\n";
    for (const auto& edge : edge_list) {
        std::cout << edge.first << " " << edge.second << "\n";
    }
}

bool Tree::IsTheoremWork() {
    std::vector<VertexInfo> tree_info(CalculateTreeInfo());
    for (size_t v = 0; v < adjacency_list_.size(); ++v) {
        if (tree_info[v].IsTheoremWork()) {
            return true;
        }
    }
    return false;
}

std::vector<std::vector<int>> Tree::GetChainsFour() {
    if (adjacency_list_.size() < 4) {
        return std::vector<std::vector<int>>();
    }
    std::vector<VertexInfo> tree_info(CalculateTreeInfo());
    std::unordered_set<int> deleted_vertexes;
    std::vector<std::vector<int>> chains_four;
    std::deque<int> vertexes_to_process;
    for (size_t v = 0; v < adjacency_list_.size(); ++v) {
        if (tree_info[v].IsTheoremWork()) {
            vertexes_to_process.push_back(v);
        }
    }
    while (true) {
        if (vertexes_to_process.empty()) {
            throw std::invalid_argument("");
        }
        int v = vertexes_to_process.front();
        vertexes_to_process.pop_front();
        if (deleted_vertexes.find(v) != deleted_vertexes.end()) {
            continue;
        }
        while (tree_info[v].GetN1() != 0 && tree_info[v].GetS1() != 0) {
            int u = tree_info[v].GetRemoveFromN1(deleted_vertexes);
            int w = tree_info[v].GetRemoveFromS1(deleted_vertexes);
            int x = tree_info[w].GetRemoveFromN1(deleted_vertexes);
            chains_four.push_back({ x, w, v, u });
        }
        while (tree_info[v].GetDegree() != 1 && tree_info[v].GetN1() > tree_info[v].GetN2()) {
            int u = tree_info[v].GetRemoveFromN1(deleted_vertexes);
        }
        while (tree_info[v].GetDegree() != 1 && tree_info[v].GetS1() > tree_info[v].GetS2()) {
            int u = tree_info[v].GetRemoveFromS1(deleted_vertexes);
            int w = tree_info[v].GetRemoveFromS1(deleted_vertexes);
            int x = tree_info[u].GetRemoveFromN1(deleted_vertexes);
            int y = tree_info[w].GetRemoveFromN1(deleted_vertexes);
            chains_four.push_back({ x, u, v, w });
        }
        if (tree_info[v].GetDegree() == 2 && tree_info[v].GetS1() == 1) {
            int x = *tree_info[v].ordinary.begin();
            tree_info[x].ordinary.erase(v);
            deleted_vertexes.insert(v);
            int y = tree_info[v].GetRemoveFromS1(deleted_vertexes);
            int z = tree_info[y].GetRemoveFromN1(deleted_vertexes);
            chains_four.push_back({ z, y, v, x });
            vertexes_to_process.push_front(x);
            continue;
        }
        if (tree_info[v].GetDegree() == 0) {
            break;
        }
        if (tree_info[v].GetDegree() == 1) {
            if (tree_info[v].GetN1() == 1 || tree_info[v].GetS1() == 1) {
                break;
            }
            int x = *tree_info[v].ordinary.begin();
            if (tree_info[x].GetDegree() != 2) {
                if (tree_info[x].n1.find(v) == tree_info[x].n1.end()) {
                    tree_info[x].ChangeVertexFromOrdinaryToN1(v);
                    if (tree_info[x].IsTheoremWork()) {
                        vertexes_to_process.push_back(x);
                    }
                }
            } else {
                if (tree_info[x].n1.find(v) == tree_info[x].n1.end()) {
                    tree_info[x].ChangeVertexFromOrdinaryToN1(v);
                    if (tree_info[x].IsTheoremWork()) {
                        vertexes_to_process.push_back(x);
                    }
                    int y;
                    if (tree_info[x].GetS1() == 1) {
                        y = *tree_info[x].s1.begin();
                    } else {
                        y = *tree_info[x].ordinary.begin();
                    }
                    tree_info[y].ChangeVertexFromOrdinaryToS1(x);
                    if (tree_info[y].IsTheoremWork()) {
                        vertexes_to_process.push_back(y);
                    }
                }
            }
        }
        if (tree_info[v].GetDegree() == 2 && tree_info[v].ordinary.size() != 2) {
            if (tree_info[v].GetN1() == 2) {
                break;
            }
            int y = *tree_info[v].ordinary.begin();
            tree_info[y].ChangeVertexFromOrdinaryToS1(v);
            if (tree_info[y].IsTheoremWork()) {
                vertexes_to_process.push_back(y);
            }
        }
    }
    return chains_four;
}

std::vector<Tree::VertexInfo> Tree::CalculateTreeInfo() {
    std::vector<VertexInfo> tree_info;
    tree_info.reserve(adjacency_list_.size());
    for (size_t v = 0; v < adjacency_list_.size(); ++v) {
        VertexInfo vertex_inf;
        for (const auto& u : adjacency_list_[v]) {
            if (adjacency_list_[u].size() == 1) {
                vertex_inf.n1.insert(u);
            } else if (adjacency_list_[u].size() == 2) {
                int w = adjacency_list_[u][0];
                if (w == v) {
                    w = adjacency_list_[u][1];
                }
                if (adjacency_list_[w].size() == 1) {
                    vertex_inf.s1.insert(u);
                } else {
                    vertex_inf.ordinary.insert(u);
                }
            } else {
                vertex_inf.ordinary.insert(u);
            }
        }
        tree_info.push_back(vertex_inf);
    }
    return tree_info;
}

std::unordered_set<std::pair<int, int>> Tree::GetEdgeList() {
    std::unordered_set<std::pair<int, int>> edge_list;
    for (size_t v = 0; v < adjacency_list_.size(); ++v) {
        for (const auto& u : adjacency_list_[v]) {
            if (u < v) {
                edge_list.emplace(u, v);
            }
        }
    }
    return edge_list;
}

std::vector<std::vector<std::pair<int, int>>> Tree::GetGammaTwo() {
    std::vector<std::vector<int>> chains_four(GetChainsFour());
    std::unordered_set<std::pair<int, int>> edge_list(GetEdgeList());
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

Tree Tree::ReadTree() {
    int vertex_num;
    std::cin >> vertex_num;
    std::vector<std::pair<int, int>> edge_list;
    for (int i = 0; i + 1 < vertex_num; ++i) {
        int u, v;
        std::cin >> u >> v;
        edge_list.push_back(std::make_pair(u, v));
    }
    return Tree(edge_list);
}

Tree Tree::GenerateTree(int vertex_num) {
    std::mt19937 generator;
    generator.seed(time(nullptr));
    std::vector<std::pair<int, int>> edge_list;
    edge_list.reserve(vertex_num - 1);
    for (int i = 0; i + 1 < vertex_num; ++i) {
        std::uniform_int_distribution<> distribution(0, i);
        edge_list.push_back(std::make_pair(i + 1, distribution(generator)));
    }
    return Tree(edge_list);
}

Tree::VertexInfo::VertexInfo() : n1(), s1(), ordinary() {
}
    
int Tree::VertexInfo::GetDegree() {
    return n1.size() + s1.size() + ordinary.size();
}
    
int Tree::VertexInfo::GetN1() {
    return n1.size();
}
    
int Tree::VertexInfo::GetN2() {
    return s1.size() + ordinary.size();
}
    
int Tree::VertexInfo::GetS1() {
    return s1.size();
}
    
int Tree::VertexInfo::GetS2() {
    return n1.size() + ordinary.size();
}
    
int Tree::VertexInfo::GetRemoveFromN1(std::unordered_set<int>& deleted_vertexes) {
    return GetRemoveFrom(n1, deleted_vertexes);
}
    
int Tree::VertexInfo::GetRemoveFromS1(std::unordered_set<int>& deleted_vertexes) {
    return GetRemoveFrom(s1, deleted_vertexes);
}
    
bool Tree::VertexInfo::IsTheoremWork() {
    return GetN1() > GetN2() || GetS1() > GetS2() || ((GetN1() > 0 || GetDegree() == 2) && GetS1() > 0);
}
    
void Tree::VertexInfo::ChangeVertexFromOrdinaryToN1(int u) {
    ordinary.erase(u);
    n1.insert(u);
}
    
void Tree::VertexInfo::ChangeVertexFromOrdinaryToS1(int u) {
    ordinary.erase(u);
    s1.insert(u);
}
        
int Tree::VertexInfo::GetRemoveFrom(std::unordered_set<int>& set, std::unordered_set<int>& deleted_vertexes) {
    int u = *set.begin();
    set.erase(u);
    deleted_vertexes.insert(u);
    return u;
}
