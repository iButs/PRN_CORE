// tempp.cpp: определ€ет точку входа дл€ консольного приложени€.
#include "stdafx.h"
#include<iostream>
#include<stack>
using namespace std;

class calcCore
{
	struct Node{
		char* function;
		char term;
		double variable;
	public:
		Node(char* c): function(c), term(0){}
		Node(char c): term(c), variable(-1){}
		Node(double v): variable(v), term(0){}
		double retData(){ return variable; }
		char retTerm(){ return term; }
	};
	Node* tmp;
	char* expression;
	double result;
	stack<Node> out;
	stack<char> terms;
public:
	calcCore(char* c): expression(c){}
	double getResult(){ return result; }
	void toRPN();
	int getPrioritet(char);
	void toCalculate();
	bool isItFunc(char c){ if (( c =='s')||(c == 'c')||(c =='t')||(c =='e')||(c =='l')) return 1; else return 0; }
	bool isItOperator(char c){ if ((c == '+')||(c == '-')||(c =='*')||(c == '/')||(c =='^')) return 1; else return 0; }
	void toPrint(){ while (!out.empty()) { if (out.top().retData() > (-1)) cout<<out.top().retData()<<' '; else cout<<out.top().retTerm()<<' '; out.pop();} }
};

int calcCore::getPrioritet(char c){
	if (c == '^') return 4;
	if ((c == '*')||(c == '/')) return 3;
	if ((c == '+')||(c =='-')) return 2;
	if ((c == ')')||(c == '(')) return 1;
	return 0;
}

void calcCore::toRPN(){
	int i = 0;
	double arg;
	while (expression[i]){
		if (((int)expression[i] > 47)&&((int)expression[i] < 58)||(expression[i] =='.')||(expression[i]=='(')){
			arg = 0;
			double lildec = 10;
			double dec = 10;
			bool point = 0;

			if (expression[i] == '('){
			terms.push(expression[i]); i++;
				if (expression[i] == '-')
					arg*=-1, i++;
			}
			while ((!isItFunc(expression[i]))&&(!isItOperator(expression[i]))&&(expression[i]!='(')&&(expression[i]!=')')&&(expression[i])){
				if (expression[i] == '.'){
					point = 1;
					i++;
				}
				if (point){	
					arg+=(((int)expression[i]-48)/lildec);
					lildec*=10;
				}
				else{
					arg*=dec;
					arg+=((int)expression[i]-48);
				}
				i++;
			}
			out.push(arg);
		}			
		if (isItFunc(expression[i]))	//symbol of function
			terms.push(expression[i]);
		if (expression[i] == ')'){	
			while(terms.top()!='('){
				tmp = new Node(terms.top());
				out.push(*tmp);
				terms.pop(); 
				if (terms.empty()){ 
					cout<<"error";
					break;			//ошибочка вишла
				}
				if (isItFunc(terms.top())){
					out.push(terms.top());
					terms.pop();
				}
			}
			terms.pop();			// delete (
		}
		if ((isItOperator(expression[i]))){
			while((!terms.empty())&&(getPrioritet(expression[i])<=getPrioritet(terms.top()))){
				tmp = new Node(terms.top());
				out.push(*tmp);
				terms.pop();
			}
			terms.push(expression[i]);
		}
		i++;
	}
	while(!terms.empty()){
			tmp = new Node(terms.top());
			out.push(*tmp);
			terms.pop();
	}
}

void calcCore::toCalculate(){
	char c;
	stack<Node> ut;
	stack<double> sResult;
	double argm1 = 0;
	double argm2 = 0;
	while (!out.empty()){
		ut.push(out.top());
		out.pop();
	}
	while(!ut.empty()){
		Node nd = ut.top();
		ut.pop();
		if (nd.retData()>=0)
			sResult.push(nd.retData());
		else{
			switch(c = nd.retTerm()){
			case '+': 
				sResult.push(sResult.top()+=nd.retData());
				break;
			case '-': 
						if (!ut.top().retData())
							sResult.push(sResult.top()=nd.retData()*(-1)), ut.pop();
				else
					sResult.push(sResult.top()-=nd.retData());
				break;
			case '*': 
				sResult.push(sResult.top()*=nd.retData());
				break;
			case '/':
				sResult.push(sResult.top()/=nd.retData());
				break;
			case '^':
				sResult.push(sResult.top()+=nd.retData());
				break;
			}
		}
	}
	result = sResult.top();
}

int _tmain(int argc,_TCHAR* argv[])
{
	calcCore one("-38+24*(56-25)-3");	//не реализировал со знаком 
	one.toRPN();
	//one.toCalculate();
	one.toPrint();
	//for (int i = 0; i<256; i++)
	//	cout<<(char)i<<" = "<<i<<endl;
	return 0;
}

