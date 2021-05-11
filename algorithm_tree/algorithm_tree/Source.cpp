#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include "GraphCalculator.h"

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
        std::string command;
        std::getline(std::cin, command);
        std::vector<std::vector<int>> adj_list;
        if (command == "q") {
            break;
        } else if (command == "g") {
            std::cout << "Enter vertex_num ";
            int vertex_num;
            std::cin >> vertex_num;
            adj_list = GraphCalculator::GenerateTree(vertex_num);
            GraphCalculator::Print(adj_list);
        } else if (command == "e") {
            adj_list = GraphCalculator::ReadTree();
        } else {
            continue;
        }
        std::cout << "**************************\n";
        PrintChainsFour(GraphCalculator::GetChainsFourTree(adj_list));
        std::cout << "**************************\n";
        PrintGammaTwo(GraphCalculator::GetGammaTwoTree(adj_list));
        std::cout << "**************************\n";
    } while (true);
    return 0;
}
