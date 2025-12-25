#include "Avtomat.h"

ResWordsAvtomatus::ResWordsAvtomatus() {
	NewNode();
}

size_t ResWordsAvtomatus::NewNode() {
	Perehods.push_back({});
	return Perehods.size()-1;
}

void ResWordsAvtomatus::AddNewToken(const HashTable::Token& Lex) {
	int NodeNumb=0;
	Node Tmp;
	for (int i = 0; i < Lex.str.size(); i++) {
		if (Perehods[NodeNumb].find(Lex.str[i]) == Perehods[NodeNumb].end()) {
			Tmp.NodeNum = NewNode();
			if (i == Lex.str.size() - 1) {
				Tmp.LexemType = Lex.type;
			}
			else {
				Tmp.LexemType = id;
			}
			Perehods[NodeNumb].insert({ Lex.str[i],Tmp });
		}
		NodeNumb = Perehods[NodeNumb][Lex.str[i]].NodeNum;
	}
}

Type ResWordsAvtomatus::DetectLex(const std::string& S) {
	int NodeNumb = 0;
	Type anw = Error;
	for (int i = 0; i < S.size(); i++) {
		if (Perehods[NodeNumb].find(S[i]) == Perehods[NodeNumb].end())
			return Error;
		anw = Perehods[NodeNumb][S[i]].LexemType;
		NodeNumb = Perehods[NodeNumb][S[i]].NodeNum;
	}
	return anw;
}

IdAvtomatus::IdAvtomatus() {
	NewNode();
	Node tmp;
	tmp.LexemType = id;
	tmp.NodeNum = 0;
	for (char i = 'A'; i <= 'Z'; ++i)
		Perehods[0].insert({ i,tmp });
	for (char i = 'a'; i <= 'z'; ++i)
		Perehods[0].insert({ i,tmp });
}
NumAvtomatus::NumAvtomatus() {
	NewNode();
	NewNode(); // состояние для 0
	NewNode();
	Node tmp;
	tmp.NodeNum = 1;
	tmp.LexemType = Const;
	Perehods[0].insert({ '0',tmp });
	tmp.NodeNum = 2;
	for (int i = '1'; i <= '9'; ++i)
		Perehods[0].insert({ i,tmp });
	for (int i = '0'; i <= '9'; ++i)
		Perehods[2].insert({ i,tmp });
}
