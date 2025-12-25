#pragma once
#include <string>
#include <stack>
#include <vector>
#include <map>
#include <unordered_set>
#include <fstream>
#include "Parser.h"

class SematixAnilizer {
private:
    HashTable DeclVariables;
    
    std::vector<std::string> Postfix;
    std::vector<std::string> Errors;

    std::stack<std::string> St;
    std::string AnwTmp;
    
    int GenMet = 0;
    std::string Gen();
    void Declaration(Node* DescrList);
    void Operators(Node* Operators);
    void Op(Node* Op);
    void Preobr(Node* Expr);
    void ExprToPostfix(Node* Expr);
    void Case(Node* Op);
public:
    bool analyze(Node* root);
    void saveResultsToFile(const std::string& filename);
};



