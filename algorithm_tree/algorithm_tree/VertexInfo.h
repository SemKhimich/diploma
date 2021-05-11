#pragma once
#include <vector>
#include <unordered_set>
#include <queue>

struct VertexInfo {
    std::unordered_set<int> n1, s1, ordinary;

    VertexInfo();

    int GetDegree();
    
    int GetN1();
    
    int GetN2();
    
    int GetS1();
    
    int GetS2();
    
    int GetRemoveFromN1(std::vector<bool>& is_deleted);
    
    int GetRemoveFromS1(std::vector<bool>& is_deleted);
    
    bool IsTheoremWork();

    void ChangeVertexFromOrdinaryToN1(int u);
    
    void ChangeVertexFromOrdinaryToS1(int u);

    void ChangeVertexToN1(int u);

    void ChangeVertexToS1(int u);
    
private:
    int GetRemoveFrom(std::unordered_set<int>& set, std::vector<bool>& is_deleted);
};
