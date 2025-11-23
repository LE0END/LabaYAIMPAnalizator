#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "Hashy.h"


class Parser {
public:
    Parser(const std::vector<HashTable::Token>& src);
    bool parse();

    void printTree() const;
    void saveTree(const std::string& filename) const;

private:
    std::vector<HashTable::Token> tokens;
    size_t idx = 0;

    std::vector<std::string> tree;
    int depth = 0;
    bool hasError;
    const HashTable::Token& peek(size_t k = 0) const;
    HashTable::Token advance();
    bool accept(Type t);
    bool expect(Type t, const std::string& msg);

    void treePush(const std::string& name);
    void treePop();
    void treeLeaf(const std::string& name);


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