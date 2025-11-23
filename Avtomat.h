#include"Hashy.h"
#include<vector>
#include<unordered_map>

#pragma once


class ResWordsAvtomatus {
protected:
	struct Node {
		size_t NodeNum;
		Type LexemType;
	};
	std::vector<std::unordered_map<char, Node>> Perehods;
public:
	ResWordsAvtomatus();
	size_t NewNode();
	void AddNewToken(const HashTable::Token& Lex);
	Type DetectLex(const std::string& S);
};
class IdAvtomatus : public ResWordsAvtomatus {
public:
	IdAvtomatus();
};
class NumAvtomatus : public ResWordsAvtomatus {
public:
	NumAvtomatus();
};

class Avtomat {
public:
	ResWordsAvtomatus FA;
	IdAvtomatus SA;
	NumAvtomatus TA;
};
