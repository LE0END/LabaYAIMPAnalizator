#include <iostream>
#include <vector>
#include<string>
#include"Wector.h"
#include"Hashy.h"
#include "LexicalAnilizer.h"
#include"Parser.h"

int main()
{
    LexicalAnilizer A;
    A.openInputFile("Input.txt");
    std::vector<HashTable::Token> Tokens;
    A.Analize(Tokens); // Magic
    A.PrintToFile("Out.txt");// Print Magic
    std::setlocale(LC_ALL, "");
   /* for (int i = 0; i < Tokens.size(); i++) {
        std::cout << Tokens[i].str << ' ';
    }*/
    Parser P(Tokens);

    if (P.parse()) {
        std::cout << "Parse OK\n";
        P.printTree();
        P.saveTree("parse_tree.txt");
    }
    else {
        std::cout << "Parse FAILED\n";
    }
}

