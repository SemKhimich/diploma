#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include "Tree.h"

void PrintChainsFour(const std::vector<std::vector<int>>& chains_four) {
    std::cout << "Chains four\n";
    for (const auto& chain_four : chains_four) {
        std::cout << "(";
        for (const auto& vertex : chain_four) {
            std::cout << " " << vertex;
        }
        std::cout << " )\n";
    }
}

void PrintGammaTwo(const std::vector<std::vector<std::pair<int, int>>>& gamma_two) {
    std::cout << "Gamma two\n";
    for (const auto& edges : gamma_two) {
        for (const auto& edge : edges) {
            std::cout << "(" << edge.first << ", " << edge.second << ") ";
        }
        std::cout << "\n";
    }
}

int main() {
    srand(time(nullptr));
    do {
        std::cout << "Press 'g' to generate graph\n";
        std::cout << "Press 'e' to enter graph\n";
        std::cout << "Press 'q' to exit\n";
        Tree tree({});
        std::string command;
        std::getline(std::cin, command);
        if (command == "q") {
            break;
        } else if (command == "g") {
            int vertex_num = rand() % 10 + 10;
            tree = Tree::GenerateTree(vertex_num);
            tree.Print();
        } else if (command == "e") {
            tree = Tree::ReadTree();
        } else {
            continue;
        }
        std::cout << "**************************\n";
        if (tree.IsTheoremWork()) {
            std::cout << "theorem works\n";
        } else {
            std::cout << "theorem doesn't work\n";
        }
        std::cout << "**************************\n";
        PrintChainsFour(tree.GetChainsFour());
        std::cout << "**************************\n";
        PrintGammaTwo(tree.GetGammaTwo());
        std::cout << "**************************\n";
    } while (true);
    system("pause");
    return 0;
}
