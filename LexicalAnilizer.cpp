#include "LexicalAnilizer.h"


LexicalAnilizer::LexicalAnilizer() {
	Avt.FA.AddNewToken(HashTable::Token(RW_PROCEDURE, "procedure"));
	Avt.FA.AddNewToken(HashTable::Token(RW_BEGIN, "begin"));
	Avt.FA.AddNewToken(HashTable::Token(RW_END, "end"));
	Avt.FA.AddNewToken(HashTable::Token(RW_VAR, "var"));
	Avt.FA.AddNewToken(HashTable::Token(RW_INTEGER, "integer"));
	Avt.FA.AddNewToken(HashTable::Token(RW_CASE, "case"));
	Avt.FA.AddNewToken(HashTable::Token(RW_OF, "of"));
}

void LexicalAnilizer::openInputFile(const std::string& InputFile) {
	Input.open(InputFile);
}

void LexicalAnilizer::Analize(std::vector<HashTable::Token>& Tokens) {
	std::string Tmp;
	Tmp.reserve(24);
	char t;
	while (!Input.eof()) {
		t = Input.get();
		
		if ((t == ';' || t == ' ' || t == '\n' || t == '\t' || t == '+' || t == '-' || t == '=' || t == '(' || t == ')' || t == EOF || t == ',' || t == ':')) {
			if (!Tmp.empty()) {
				Type temp = Avt.FA.DetectLex(Tmp);
				if (temp != Error) {
					Tokens.push_back(HashTable::Token(temp, Tmp));
					T.insert(HashTable::Token(temp, Tmp));
				}
				else {
					temp = Avt.SA.DetectLex(Tmp);
					if (temp != Error) {
						Tokens.push_back(HashTable::Token(temp, Tmp));
						T.insert(HashTable::Token(temp, Tmp));
					}
					else {
						temp = Avt.TA.DetectLex(Tmp);
						if (temp != Error) {
							Tokens.push_back(HashTable::Token(temp, Tmp));
							T.insert(HashTable::Token(temp, Tmp));
						}
						else {
							Tokens.push_back(HashTable::Token(Error, Tmp));
							T.insert(HashTable::Token(Error, Tmp));
						}
					}
				}
				Tmp.clear();
				Tmp.reserve(24);
			}
			if (t == ':') {
				char next = Input.get();
				if (next == '=') {
					Tokens.push_back(HashTable::Token(OP_ASSIGN, ":="));
					T.insert(HashTable::Token(OP_ASSIGN, ":="));
				}
				else {
					Tokens.push_back(HashTable::Token(COLON, ":"));
					T.insert(HashTable::Token(COLON, ":"));
					Input.unget();
				}
				continue;
			}
			if (t == ';') {
				Tokens.push_back(HashTable::Token(SEMICOLON, ";"));
				T.insert(HashTable::Token(SEMICOLON, ";"));
			}
			else if (t == '+') {
				Tokens.push_back(HashTable::Token(OP_PLUS, "+"));
				T.insert(HashTable::Token(OP_PLUS, "+"));
			}
			else if (t == '-') {
				Tokens.push_back(HashTable::Token(OP_MINUS, "-"));
				T.insert(HashTable::Token(OP_MINUS, "-"));
			}
			else if (t == '(') {
				Tokens.push_back(HashTable::Token(LPAREN, "("));
				T.insert(HashTable::Token(LPAREN, "("));
			}
			else if (t == ')') {
				Tokens.push_back(HashTable::Token(RPAREN, ")"));
				T.insert(HashTable::Token(RPAREN, ")"));
			}
			else if (t == ',') {
				Tokens.push_back(HashTable::Token(COMMA, ","));
				T.insert(HashTable::Token(COMMA, ","));
			}
		}
		if (!(t == ';' || t == ' ' || t == '\n' || t == '\t' || t == '+' || t == '-' || t == '=' || t == '(' || t == ')' || t == EOF || t == ',' || t == ':'))
			Tmp.push_back(t);
		
	}

	if (!Tmp.empty()) {
		Type temp = Avt.FA.DetectLex(Tmp);
		if (temp != Error) {
			Tokens.push_back(HashTable::Token(temp, Tmp));
			T.insert(HashTable::Token(temp, Tmp));
		}
		else {
			temp = Avt.SA.DetectLex(Tmp);
			if (temp != Error) {
				Tokens.push_back(HashTable::Token(temp, Tmp));
				T.insert(HashTable::Token(temp, Tmp));
			}
			else {
				temp = Avt.TA.DetectLex(Tmp);
				if (temp != Error) {
					Tokens.push_back(HashTable::Token(temp, Tmp));
					T.insert(HashTable::Token(temp, Tmp));
				}
				else {
					Tokens.push_back(HashTable::Token(Error, Tmp));
					T.insert(HashTable::Token(Error, Tmp));
				}
			}
		}
	}
	Tmp.clear();
}

void LexicalAnilizer::PrintToFile(const std::string& Out) {
	std::ofstream out;
	out.open(Out);
	T.print(out);
	T.print(std::cout);
	out.close();
}