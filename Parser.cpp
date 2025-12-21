#include "Parser.h"

const std::string typeToString(Type t) {
    switch (t) {
    case Type::Null: return "Null";
    case Type::id: return "ID";
    case Type::Const: return "CONST";
    case Type::RW_PROCEDURE: return "PROCEDURE";
    case Type::RW_BEGIN: return "BEGIN";
    case Type::RW_END: return "END";
    case Type::RW_VAR: return "VAR";
    case Type::RW_INTEGER: return "INTEGER";
    case Type::RW_CASE: return "CASE";
    case Type::RW_OF: return "OF";
    case Type::OP_ASSIGN: return "ASSIGN";
    case Type::OP_PLUS: return "PLUS";
    case Type::OP_MINUS: return "MINUS";
    case Type::SEMICOLON: return "SEMICOLON";
    case Type::COMMA: return "COMMA";
    case Type::COLON: return "COLON";
    case Type::LPAREN: return "LPAREN";
    case Type::RPAREN: return "RPAREN";
    case Type::END_OF_FILE: return "EOF";
    case Type::Error: return "ERROR";
    default: return "UNKNOWN";
    }
}

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
        pushNode(typeToString(t), peek().str);
        advance();
        popNode();
        return true;
    }
    return false;
}


bool Parser::expect(Type t, const std::string& msg) {

    if (peek().type == t) {
        pushNode(typeToString(t), peek().str);
        advance();
        popNode();
        return true;
    }

    hasError = true;
    std::cout << "Ошибка: ожидалось " << msg
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

void Parser::pushNode(const std::string& v1, const std::string& v2) {
    Node* n = new Node(v1, v2);

    if (!nodeStack.empty())
        nodeStack.top()->NewChild(n);
    else
        root = n;   // первый узел — корень

    nodeStack.push(n);
}

void Parser::popNode() {
    if (!nodeStack.empty())
        nodeStack.pop();
}



bool Parser::parse() {

    hasError = false;


    Procedure();


    expect(Type::END_OF_FILE, "EOF");

    if (hasError) {
        root = nullptr;
        std::cout << "Parse FAILED: обнаружены ошибки\n";
        return false;
    }

    std::ofstream f("parse_tree.txt");
    for (auto& s : tree) f << s;
    f.close();

    return true;
}


bool Parser::Procedure() {
    pushNode("Procedure");

    BeginBlock();
    Descriptions();
    Operators();
    EndBlock();

    popNode();
    return true;
}

bool Parser::BeginBlock() {
    pushNode("BeginBlock");

    expect(Type::RW_PROCEDURE, "procedure");
    IdRule();
    expect(Type::SEMICOLON, ";");
    expect(Type::RW_BEGIN, "begin");

    popNode();
    return true;
}

bool Parser::EndBlock() {
    pushNode("EndBlock");

    expect(Type::RW_END, "end");

    popNode();
    return true;
}

bool Parser::Descriptions() {
    pushNode("Descriptions");


    if (accept(Type::RW_VAR)) {
        DescrList();
        return true;
    }



    popNode();
    return true;
}

bool Parser::DescrList() {

    pushNode("DescrList");

    Descr();

    while (peek().type == Type::id &&
        (peek(1).type == Type::COMMA || peek(1).type == Type::COLON))
    {
        Descr();
    }


    popNode();
    return true;
}

bool Parser::Descr() {
    pushNode("Descr");

    VarList();
    expect(Type::COLON, ":");
    TypeRule();
    expect(Type::SEMICOLON, ";");

    popNode();
    return true;
}

bool Parser::VarList() {
    pushNode("VarList");

    IdRule();
    while (accept(Type::COMMA))
        IdRule();


    popNode();
    return true;
}

bool Parser::TypeRule() {
    pushNode("Type");

    expect(Type::RW_INTEGER, "integer");

    popNode();
    return true;
}

bool Parser::Operators() {
    pushNode("Operators");


    Op();
    while (peek().type == Type::id || peek().type == Type::RW_CASE)
        Op();

    popNode();
    return true;
}

bool Parser::Op() {

    pushNode("Op");
    if (peek().type == Type::id) {
        IdRule();
        expect(Type::OP_ASSIGN, ":=");
        Expr();
        expect(Type::SEMICOLON, ";");

        popNode();
        return true;
    }

    if (peek().type == Type::RW_CASE) {
        accept(Type::RW_CASE);
        Expr();
        expect(Type::RW_OF, "of");
        Options();
        expect(Type::RW_END, "end");
        expect(Type::SEMICOLON, ";");

        popNode();
        return true;
    }

    hasError = true;  
    std::cout << "Ошибка: ожидался оператор, но найдено '" << peek().str << "'\n";

    while (peek().type != Type::SEMICOLON &&
        peek().type != Type::END_OF_FILE)
        advance();

    accept(Type::SEMICOLON); // съедим все ошибки ХЕХЕ;


    popNode();
    return true;
}

bool Parser::Options() {

    pushNode("Options");
    ConstRule();
    expect(Type::COLON, ":");
    Operators();

    while (peek().type == Type::Const) {
        ConstRule();
        expect(Type::COLON, ":");
        Operators();
    }


    popNode();
    return true;
}

bool Parser::Expr() {

    pushNode("Expr");
    SimpleExpr();

    if (accept(Type::OP_PLUS))  Expr();
    else if (accept(Type::OP_MINUS)) Expr();
    popNode();

    return true;
}

bool Parser::SimpleExpr() {

    pushNode("SimpleExpr");
    if (peek().type == Type::id) {
        IdRule();

        popNode();
        return true;
    }

    if (peek().type == Type::Const) {
        ConstRule();

        popNode();
        return true;
    }

    if (accept(Type::LPAREN)) {
        Expr();
        expect(Type::RPAREN, ")");

        popNode();
        return true;
    }

    hasError = true;
    std::cout << "Ошибка: неверное выражение '" << peek().str << "'\n";

    // едим до конца выражения
    while (peek().type != Type::SEMICOLON &&
        peek().type != Type::RPAREN &&
        peek().type != Type::END_OF_FILE)
        advance();


    popNode();
    return true;
}

bool Parser::IdRule() {

    pushNode("Id");
    expect(Type::id, "идентификатор");

    popNode();
    return true;
}

bool Parser::ConstRule() {

    pushNode("Const");
    expect(Type::Const, "константа");

    popNode();
    return true;
}

Node* Node::find2(const std::string& Val1, const std::string& Val2) {
    if (this->Val1 == Val1 && this->Val2 == Val2) return this;

    for (int i = 0; i < this->Offsprings.size(); i++) {
        if (this->Offsprings[i]->Val1 == Val1 && this->Offsprings[i]->Val2 == Val2) {
            return this->Offsprings[i];
        }

        Node* found = this->Offsprings[i]->find2(Val1, Val2);
        if (found != nullptr) {
            return found;
        }
    }
    return nullptr;
}

Node* Node::find1(const std::string& Val1)
{
    if (this->Val1 == Val1) return this;

    for (int i = 0; i < this->Offsprings.size(); i++) {
        if (this->Offsprings[i]->Val1 == Val1) {
            return this->Offsprings[i];
        }

        Node* found = this->Offsprings[i]->find1(Val1);
        if (found != nullptr) {
            return found;
        }
    }
    return nullptr;
}

void Node::NewChild(Node* Child) {
    Offsprings.push_back(Child);
}

void Parser::printNodeTree(std::ostream& out,const Node* node, int indent) {
    if (!node) return;

    for (int i = 0; i < indent; ++i)
        out << "  ";

    out << node->Val1;
    if (!node->Val2.empty())
        out << " : " << node->Val2;
    out << "\n";

    for (const Node* child : node->Offsprings)
        printNodeTree(out,child, indent + 1);
}
