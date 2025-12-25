#include "SematixAnilizer.h"

std::string SematixAnilizer::Gen() {
	return "#M"+std::to_string(GenMet++);
}

void SematixAnilizer::Declaration(Node* DescrList) {
	for (int j = 0;j < DescrList->Offsprings.size();j++) {
		Node* VarList = DescrList->Offsprings[j]->Offsprings[0];
		for (int i = 0; i < VarList->Offsprings.size(); i++) {
			if (VarList->Offsprings[i]->Val1 == "Id") {
				if (DeclVariables.find(VarList->Offsprings[i]->Offsprings[0]->Val2) == std::pair<long long, long long>(-1, -1)) {
					HashTable::Token Tmp(Type::id, VarList->Offsprings[i]->Offsprings[0]->Val2);
					DeclVariables.insert(Tmp);
					Postfix.push_back(Tmp.str+" ");
				}
				else {
					Errors.push_back("Повторное объявление, ФУ: " + VarList->Offsprings[i]->Offsprings[0]->Val2 + '\n');
				}

			}
		}
	}
	
}

void SematixAnilizer::Operators(Node* Operators) {
	for (int i = 0; i < Operators->Offsprings.size(); i++) {
		Op(Operators->Offsprings[i]);
	}
}

void SematixAnilizer::Op(Node* Op) {
	if (Op->Offsprings[0]->Val1 == "Id") {
		std::string var = Op->Offsprings[0]->Offsprings[0]->Val2;

		if (DeclVariables.find(var) == std::pair<long long, long long>(-1, -1)) {
			Errors.push_back("Что-то не то выдумал, такого нет ~_~: " + var + "\n");
		}
		Postfix.push_back(var + " ");
		ExprToPostfix(Op->Offsprings[2]);
		Postfix.push_back(":= ");
		Postfix.push_back("\n");
	}
	else {
		Case(Op);
	}
}

void SematixAnilizer::Preobr(Node* ExprOrSE) {

	if (ExprOrSE->Offsprings.size() == 0) {

		if (ExprOrSE->Val1 == "CONST" || ExprOrSE->Val1 == "ID") {
			if (ExprOrSE->Val1 == "ID") {
				if (DeclVariables.find(ExprOrSE->Val2) == std::pair<long long, long long>(-1, -1)) {
					Errors.push_back("Что-то не то выдумал, такого нет ~_~: " + ExprOrSE->Val2 + "\n");
				}
			}
			AnwTmp.append(ExprOrSE->Val2 + " ");
		}
		else if (ExprOrSE->Val2 == "+" || ExprOrSE->Val2 == "-") {
			while (!St.empty() && St.top() != "(") {
				AnwTmp.append(St.top() + ' ');
				St.pop();
			}

			St.push(ExprOrSE->Val2);

		}
		else if (ExprOrSE->Val2 == "(") {

			St.push(ExprOrSE->Val2);
		}else if (ExprOrSE->Val2 == ")") {
			while (St.top() != "(") {
				AnwTmp.append(St.top() + ' ');
				St.pop();
			}
			St.pop();
		}
		
	}
	for (int i = 0; i < ExprOrSE->Offsprings.size(); i++) {
		Preobr(ExprOrSE->Offsprings[i]);
	}
}

void SematixAnilizer::ExprToPostfix(Node* Expr) {

	AnwTmp.clear();
	Preobr(Expr);
	while (!St.empty()) {
		AnwTmp.append(St.top()+' ');
		St.pop();
	}
	Postfix.push_back(AnwTmp);
	AnwTmp.clear();

}

void SematixAnilizer::Case(Node* Op) {

	ExprToPostfix(Op->Offsprings[1]);

	std::string Expr = Postfix[Postfix.size() - 1];
	Postfix.pop_back();

	Node* opts = Op->Offsprings[3];

	for (int i = 0; i < opts->Offsprings.size(); i += 3) {

		std::string c =
			opts->Offsprings[i]->Offsprings[0]->Val2;

		Postfix.push_back(Expr);
		Postfix.push_back(c+" = ");
		std::string Metka = Gen();
		Postfix.push_back(Metka + " BF ");
		Operators(opts->Offsprings[i + 2]);
		Postfix.push_back(Metka + " DEFL\n");
	}

	//Postfix.push_back("ENDCASE \n");
}




bool SematixAnilizer::analyze(Node* root) {
	Postfix.push_back("integer ");
	Declaration(root->find1("DescrList"));
	
	if (DeclVariables.Empty()) {
		Postfix.pop_back();
	}
	else {
		Postfix.push_back(std::to_string(DeclVariables.FullSize() + 1) + " DECL\n");
	}
	
	Operators(root->find1("Operators"));
	


	return Errors.empty();
}

void SematixAnilizer::saveResultsToFile(const std::string& filename) {
	std::ofstream out;
	out.open(filename);
	for (int i = 0; i < Postfix.size(); i++) {
		//std::cout << Postfix[i];
		out << Postfix[i];
	}
	if (Errors.size() > 0) {
		//std::cout << "ERRORS:\n";
		out << "ERRORS:\n";
		for (int i = 0; i < Errors.size(); i++) {
			//std::cout << Errors[i];
			out << Errors[i];
		}
	}
	
	out.close();
}
