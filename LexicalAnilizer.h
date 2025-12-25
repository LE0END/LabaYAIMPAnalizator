#include"Hashy.h"
#include"Avtomat.h"
#include<ostream>
#include<string>
#include <fstream>
#pragma once
class LexicalAnilizer {
private:
	Avtomat Avt;
	std::fstream Input;
	HashTable T;
public:
	LexicalAnilizer();
	void openInputFile(const std::string& InputFile);
	bool Analize(std::vector<HashTable::Token>& Tokens);
	void PrintToFile(const std::string& Out);
};

