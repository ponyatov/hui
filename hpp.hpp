#ifndef _H_HUI
#define _H_HUI

#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>
using namespace std;

struct Sym {
	string tag,val;
	Sym(string,string); Sym(string);
	vector<Sym*> nest; void push(Sym*);
	virtual string tagval(); string tagstr(); string pad(int);
	virtual string dump(int=0);
};

extern map<string,Sym*> env;
extern void env_init();

struct List: Sym { List(); };

struct Op: Sym { Op(string); Sym*eval(); };

extern int yylex();
extern int yylineno;
extern char* yytext;
#define TOC(C,X) { yylval.o = new C(yytext); return X; }
extern int yyparse();
extern void yyerror(string);
#include "ypp.tab.hpp"

#endif // _H_HUI
