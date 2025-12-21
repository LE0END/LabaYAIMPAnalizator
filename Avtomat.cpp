#include "Avtomat.h"

ResWordsAvtomatus::ResWordsAvtomatus() {
	NewNode();
}

size_t ResWordsAvtomatus::NewNode() {
	Perehods.push_back({});
	return Perehods.size()-1;
}

void ResWordsAvtomatus::AddNewToken(const HashTable::Token& Lex) {
	/*
	Linkin Park - Numb

	I'm tired of being what you want me to be
	Feeling so faithless, lost under the surface
	Don't know what you're expecting of me
	Put under the pressure of walking in your shoes
	Every step that I take is another mistake to you
	(Caught in the undertow, just caught in the undertow)
	I've become so numb, I can't feel you there
	Become so tired, so much more aware
	I'm becoming this, all I want to do
	Is be more like me and be less like you
	Can't you see that you're smothering me?
	Holding too tightly, afraid to lose control
	'Cause everything that you thought I would be
	Has fallen apart right in front of you
	Every step that I take is another mistake to you
	(Caught in the undertow, just caught in the undertow)
	And every second I waste is more than I can take
	I've become so numb, I can't feel you there
	Become so tired, so much more aware
	I'm becoming this, all I want to do
	Is be more like me and be less like you
	And I know
	I may end up failing too
	But I know
	You were just like me, with someone disappointed in you
	I've become so numb, I can't feel you there
	Become so tired, so much more aware
	I'm becoming this, all I want to do
	Is be more like me and be less like you
	I've become so numb, I can't feel you there
	I'm tired of being what you want me to be
	I've become so numb, I can't feel you there
	I'm tired of being what you want me to be
	*/
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
