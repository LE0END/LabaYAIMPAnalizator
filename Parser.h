#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <stack>
#include "Hashy.h"

struct Node {
    std::vector<Node*> Offsprings;
    std::string Val1;
    std::string Val2;
    Node* find2(const std::string& Val1, const std::string& Val2);
    Node* find1(const std::string& Val1);
    Node(const std::string& Val1="", const std::string& Val2="") :Val1(Val1), Val2(Val2) {};
    void NewChild(Node* Child);
};
class  Parser{
public:
    Parser(const std::vector<HashTable::Token>& src);
    bool parse();
    Node* root=nullptr;
    
    void printNodeTree(std::ostream& out, const Node* node, int indent = 0);
private:
    std::vector<HashTable::Token> tokens;
    size_t idx = 0;

    std::stack<Node*> nodeStack;
    std::vector<std::string> tree;
    int depth = 0;
    bool hasError;
    const HashTable::Token& peek(size_t k = 0) const;
    HashTable::Token advance();
    bool accept(Type t);
    bool expect(Type t, const std::string& msg);

    void pushNode(const std::string& v1, const std::string& v2="");
    void popNode();
    

    // грамматика
    bool Procedure();
    bool BeginBlock();
    bool EndBlock();
    bool Descriptions();
    bool DescrList();
    bool Descr();
    bool VarList();
    bool TypeRule();
    bool Operators();
    bool Op();
    bool Options();
    bool Expr();
    bool SimpleExpr();
    bool IdRule();
    bool ConstRule();
};