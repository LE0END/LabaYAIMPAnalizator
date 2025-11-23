#include "Hashy.h"

unsigned long long fast_pow(unsigned long long num, unsigned long long deg, unsigned long long mod)
{
	unsigned long long res = 1;
	while (deg) {
		if (deg % 2 == 0) {
			deg /= 2;
			num *= num;
			num %= mod;
		}
		else {
			deg--;
			res *= num;
			res %= mod;
		}
	}
	return (res%mod);
}

unsigned long long hash(const std::string& S, size_t mod)
{
	const int mod1 = 1000000007;
	const int p = 257;
	unsigned long long res = 0;
	for (int i = 0; i < S.size(); i++) {
		res += (S[i] * fast_pow(p, i+1, mod1)) % mod1;
		res %= mod;
	}
	return (res % mod);
}

HashTable::HashTable() {
	Table.resize(512);
}

size_t HashTable::FullSize() {
	size_t anw = 0;
	for (int i = 0; i < Table.size(); i++) {
		anw += Table[i].size();
	}
	return anw;
}
size_t HashTable::size() {
	return Table.size();
}

void HashTable::insert(const Token& tok) {
	if (factsz >= Table.size()) {
		Wector<Wector<Token>> Tmp(Table.size() * 1.4 + 5);
		for (int i = 0; i < Table.size(); i++) {
			for (int j = 0; j < Table[i].size(); j++) {
				size_t h = hash(Table[i][j].str,Tmp.size());
				Tmp[h].push_back(Table[i][j]);
			}
		}
		std::swap(Tmp, Table);
	}
	++factsz;
	size_t h = hash(tok.str, Table.size());
	for (int i = 0; i < Table[h].size(); i++) {
		return;	
	}
	Table[h].push_back(tok);

	

}

std::pair<long long, long long> HashTable::find(const std::string& S) {
	size_t h = hash(S, Table.size());
	for (int i = 0; i < Table[h].size(); i++) {
		if (Table[h][i].str == S) {
			return std::pair<size_t, size_t>(h, i);
		}
	}
	return std::pair<size_t, size_t>(-1, -1);
}

void HashTable::print(std::ostream& out) {
	for (int i = 0; i < Table.size(); i++) {
		for (int j = 0; j < Table[i].size(); j++) {
			out << i << ' ' << j << " | " << Table[i][j].str << " | ";
			const Token& token = Table[i][j];

			if (token.type == Null) out << "Null";
			else if (token.type == id) out << "id";
			else if (token.type == Const) out << "Const";

			else if (token.type == RW_PROCEDURE) out << "RW_PROCEDURE";
			else if (token.type == RW_BEGIN) out << "RW_BEGIN";
			else if (token.type == RW_END) out << "RW_END";
			else if (token.type == RW_VAR) out << "RW_VAR";
			else if (token.type == RW_INTEGER) out << "RW_INTEGER";
			else if (token.type == RW_CASE) out << "RW_CASE";
			else if (token.type == RW_OF) out << "RW_OF";

			else if (token.type == OP_ASSIGN) out << "OP_ASSIGN";
			else if (token.type == OP_PLUS) out << "OP_PLUS";
			else if (token.type == OP_MINUS) out << "OP_MINUS";

			else if (token.type == SEMICOLON) out << "SEMICOLON";
			else if (token.type == COMMA) out << "COMMA";
			else if (token.type == COLON) out << "COLON";
			else if (token.type == LPAREN) out << "LPAREN";
			else if (token.type == RPAREN) out << "RPAREN";

			else if (token.type == END_OF_FILE) out << "END_OF_FILE";
			else if (token.type == Error) out << "Error";

			else out << "Unknown";

			out << '\n';
			
		}
	}
}

void HashTable::erase(const std::string& S) {
	std::pair<long long, long long> Tmp = find(S);
	if (Tmp.first != -1 && Tmp.second != -1) {
		erase(Tmp);
	}
}

void HashTable::erase(std::pair<size_t, size_t> Pos) {
	Table[Pos.first].erase(Pos.second);
}

