#include "Parser.h"

Parser::Parser(const std::vector<HashTable::Token>& src) {
    tokens = src;
    tokens.push_back(HashTable::Token(Type::END_OF_FILE, "EOF"));
    idx = 0;
    hasError = false; 
}



const HashTable::Token& Parser::peek(size_t k) const {
    if (idx + k >= tokens.size()) return tokens.back();
    return tokens[idx + k];
}

HashTable::Token Parser::advance() {
    if (idx < tokens.size()) return tokens[idx++];
    return tokens.back();
}

bool Parser::accept(Type t) {
    if (peek().type == t) {
        treeLeaf(std::to_string((int)t) + "(" + peek().str + ")");
        advance();
        return true;
    }
    return false;
}


bool Parser::expect(Type t, const std::string& msg) {

    if (peek().type == t) {
        treeLeaf(std::to_string((int)t) + "(" + peek().str + ")");
        advance();
        return true;
    }

    hasError = true;
    std::cerr << "Ошибка: ожидалось " << msg
        << ", встретилось '" << peek().str << "'\n";
    // едим до конца выражения
    while (peek().type != Type::SEMICOLON &&
        peek().type != Type::RW_END &&
        peek().type != Type::RW_OF &&
        peek().type != Type::END_OF_FILE)
    {
        advance();
    }

    return true;
}


void Parser::treePush(const std::string& name) {
    std::string tmp(depth * 2, ' ');
    tree.push_back(tmp + name + "\n");
    depth++;
}

void Parser::treePop() {
    depth--;
}

void Parser::treeLeaf(const std::string& name) {
    std::string tmp(depth * 2, ' ');
    tree.push_back(tmp + name + "\n");
}


bool Parser::parse() {

    tree.clear();
    depth = 0;
    hasError = false;

    treePush("Procedure");
    Procedure();
    treePop();

    expect(Type::END_OF_FILE, "EOF");

    if (hasError) {
        std::cerr << "Parse FAILED: обнаружены ошибки\n";
        return false;
    }

    std::ofstream f("parse_tree.txt");
    for (auto& s : tree) f << s;
    f.close();

    return true;
}

void Parser::printTree() const {
    for (auto& s : tree) std::cout << s;
}

void Parser::saveTree(const std::string& filename) const {
    std::ofstream f(filename);
    for (auto& s : tree) f << s;
    f.close();
}


bool Parser::Procedure() {
    return BeginBlock() && Descriptions() && Operators() && EndBlock();
}

bool Parser::BeginBlock() {
    treePush("BeginBlock");
    expect(Type::RW_PROCEDURE, "procedure");
    IdRule();
    expect(Type::SEMICOLON, ";");
    expect(Type::RW_BEGIN, "begin");
    treePop();
    return true;
}

bool Parser::EndBlock() {
    treePush("EndBlock");
    expect(Type::RW_END, "end");
    treePop();
    return true;
}

bool Parser::Descriptions() {
    treePush("Descriptions");

    if (accept(Type::RW_VAR)) {
        DescrList();
        treePop();
        return true;
    }

    treeLeaf("(empty)");
    treePop();
    return true;
}

bool Parser::DescrList() {
    treePush("DescrList");

    Descr();

    while (peek().type == Type::id &&
        (peek(1).type == Type::COMMA || peek(1).type == Type::COLON))
    {
        Descr();
    }

    treePop();
    return true;
}

bool Parser::Descr() {
    treePush("Descr");
    VarList();
    expect(Type::COLON, ":");
    TypeRule();
    expect(Type::SEMICOLON, ";");
    treePop();
    return true;
}

bool Parser::VarList() {
    treePush("VarList");

    IdRule();
    while (accept(Type::COMMA))
        IdRule();

    treePop();
    return true;
}

bool Parser::TypeRule() {
    treePush("Type");
    expect(Type::RW_INTEGER, "integer");
    treePop();
    return true;
}

bool Parser::Operators() {
    treePush("Operators");

    Op();
    while (peek().type == Type::id || peek().type == Type::RW_CASE)
        Op();

    treePop();
    return true;
}

bool Parser::Op() {
    treePush("Op");

    if (peek().type == Type::id) {
        IdRule();
        expect(Type::OP_ASSIGN, ":=");
        Expr();
        expect(Type::SEMICOLON, ";");
        treePop();
        return true;
    }

    if (peek().type == Type::RW_CASE) {
        accept(Type::RW_CASE);
        Expr();
        expect(Type::RW_OF, "of");
        Options();
        expect(Type::RW_END, "end");
        expect(Type::SEMICOLON, ";");
        treePop();
        return true;
    }

    hasError = true;  
    std::cerr << "Ошибка: ожидался оператор, но найдено '" << peek().str << "'\n";

    while (peek().type != Type::SEMICOLON &&
        peek().type != Type::END_OF_FILE)
        advance();

    accept(Type::SEMICOLON); // съедим ;

    treePop();
    return true;
}

bool Parser::Options() {
    treePush("Options");

    ConstRule();
    expect(Type::COLON, ":");
    Operators();

    while (peek().type == Type::Const) {
        ConstRule();
        expect(Type::COLON, ":");
        Operators();
    }

    treePop();
    return true;
}

bool Parser::Expr() {
    treePush("Expr");

    SimpleExpr();

    if (accept(Type::OP_PLUS))  Expr();
    else if (accept(Type::OP_MINUS)) Expr();

    treePop();
    return true;
}

bool Parser::SimpleExpr() {
    treePush("SimpleExpr");

    if (peek().type == Type::id) {
        IdRule();
        treePop();
        return true;
    }

    if (peek().type == Type::Const) {
        ConstRule();
        treePop();
        return true;
    }

    if (accept(Type::LPAREN)) {
        Expr();
        expect(Type::RPAREN, ")");
        treePop();
        return true;
    }

    hasError = true;
    std::cerr << "Ошибка: неверное выражение '" << peek().str << "'\n";

    // едим до конца выражения
    while (peek().type != Type::SEMICOLON &&
        peek().type != Type::RPAREN &&
        peek().type != Type::END_OF_FILE)
        advance();

    treePop();
    return true;
}

bool Parser::IdRule() {
    treePush("Id");
    expect(Type::id, "идентификатор");
    treePop();
    return true;
}

bool Parser::ConstRule() {
    treePush("Const");
    expect(Type::Const, "константа");
    treePop();
    return true;
}
