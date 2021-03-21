#pragma once
#include <vector>
#include <unordered_set>

class Tree {
public:
    explicit Tree(const std::vector<std::pair<int, int>>& edge_list);

    void Print();

    bool IsTheoremWork();

    std::vector<std::vector<int>> GetChainsFour();

    std::vector<std::vector<std::pair<int, int>>> GetGammaTwo();

    std::unordered_set<std::pair<int, int>> GetEdgeList();

    static Tree ReadTree();

    static Tree GenerateTree(int vertex_num);

private:
    struct VertexInfo {
        std::unordered_set<int> n1, s1, ordinary;

        VertexInfo();

        int GetDegree();
    
        int GetN1();
    
        int GetN2();
    
        int GetS1();
    
        int GetS2();
    
        int GetRemoveFromN1(std::unordered_set<int>& deleted_vertexes);
    
        int GetRemoveFromS1(std::unordered_set<int>& deleted_vertexes);
    
        bool IsTheoremWork();

        void ChangeVertexFromOrdinaryToN1(int u);
    
        void ChangeVertexFromOrdinaryToS1(int u);
    
    private:
        int GetRemoveFrom(std::unordered_set<int>& set, std::unordered_set<int>& deleted_vertexes);
    };

    std::vector<std::vector<int>> adjacency_list_;

    std::vector<VertexInfo> CalculateTreeInfo();
};
