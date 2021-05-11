#include "VertexInfo.h"

VertexInfo::VertexInfo() : n1(), s1(), ordinary() {
}
    
int VertexInfo::GetDegree() {
    return n1.size() + s1.size() + ordinary.size();
}
    
int VertexInfo::GetN1() {
    return n1.size();
}
    
int VertexInfo::GetN2() {
    return s1.size() + ordinary.size();
}
    
int VertexInfo::GetS1() {
    return s1.size();
}
    
int VertexInfo::GetS2() {
    return n1.size() + ordinary.size();
}
    
int VertexInfo::GetRemoveFromN1(std::vector<bool>& is_deleted) {
    return GetRemoveFrom(n1, is_deleted);
}
    
int VertexInfo::GetRemoveFromS1(std::vector<bool>& is_deleted) {
    return GetRemoveFrom(s1, is_deleted);
}
    
bool VertexInfo::IsTheoremWork() {
    return GetN1() > GetN2() || GetS1() > GetS2() || ((GetN1() > 0 || GetDegree() == 2) && GetS1() > 0);
}
    
void VertexInfo::ChangeVertexFromOrdinaryToN1(int u) {
    ordinary.erase(u);
    n1.insert(u);
}

void VertexInfo::ChangeVertexToN1(int u) {
    ordinary.erase(u);
    s1.erase(u);
    n1.insert(u);
}

void VertexInfo::ChangeVertexToS1(int u) {
    ordinary.erase(u);
    n1.erase(u);
    s1.insert(u);
}
    
void VertexInfo::ChangeVertexFromOrdinaryToS1(int u) {
    ordinary.erase(u);
    s1.insert(u);
}
        
int VertexInfo::GetRemoveFrom(std::unordered_set<int>& set, std::vector<bool>& is_deleted) {
    int u = *set.begin();
    set.erase(u);
    is_deleted[u] = true;
    return u;
}
