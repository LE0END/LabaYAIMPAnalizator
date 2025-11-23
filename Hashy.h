#pragma once
#include <string>
#include <ostream>
#include <iostream>
#include "Wector.h"
unsigned long long fast_pow(unsigned long long num, unsigned long long deg, unsigned long long mod);
unsigned long long hash(const std::string& S, size_t mod);
enum Type {
	Null,
	id,
	Const,

	RW_PROCEDURE,
	RW_BEGIN,
	RW_END,
	RW_VAR,
	RW_INTEGER,
	RW_CASE,
	RW_OF,

	OP_ASSIGN,
	OP_PLUS,
	OP_MINUS,

	SEMICOLON,
	COMMA,
	COLON,
	LPAREN,
	RPAREN,

	END_OF_FILE,
	Error
};

class HashTable {
private:
	struct Token;
	Wector<Wector<Token>> Table;
	size_t factsz = 0;

public:
	
	struct Token {
		Type type = Type(Null);
		std::string str="NullStr";
		Token(const Type& type,const std::string& S) :type(type), str(S) {}
		Token() = default;
	};
	HashTable();
	~HashTable() = default;
	size_t FullSize();
	size_t size();
	void insert(const Token& tok);
	std::pair<long long, long long> find(const std::string& S);
	void print(std::ostream& out);
	void erase(const std::string& S);
	void erase(std::pair<size_t, size_t> Pos);
};

