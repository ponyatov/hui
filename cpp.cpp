#include "hpp.hpp"
#define YYERR "\n\n"<<yylineno<<":"<<msg<<"["<<yytext<<"]\n\n"
void yyerror(string msg) { cout<<YYERR; cerr<<YYERR; exit(-1); }
int main() { env_init(); return yyparse(); }

Sym::Sym(string T,string V) { tag=T; val=V; }
Sym::Sym(string V):Sym("sym",V) {}
void Sym::push(Sym*o) { nest.push_back(o); }

string Sym::pad(int n) { string S; for (int i=0;i<n;i++) S+='\t'; return S; }
string Sym::tagval() { return tag+":"+val; }
string Sym::dump(int depth) { string S = "\n"+pad(depth)+tagval();
//	for (auto pr=pars.begin(),e=pars.end();pr!=e;pr++)
//		S += ","+*pr;
	for (auto it=nest.begin(),e=nest.end();it!=e;it++)
		S += (*it)->dump(depth+1);
	return S; }

Sym* Sym::eval() {
	Sym *E = env[val]; if (E) return E; // lookup
	for (auto it=nest.begin(),e=nest.end();it!=e;it++)
		(*it) = (*it)->eval();
	return this;
}

Sym* Sym::eq(Sym*o) { env[val]=o;
	if (o->tag=="list" && o->val=="[]") o->val=val; return this; }
Sym* Sym::colon(Sym*o) { return new Sym(val,o->val); }

Str::Str(string V):Sym("str",V) {}
string Str::tagval() { return tagstr(); }
string Sym::tagstr() { string S = tag+":'";
	for (int i=0,e=val.size();i<e;i++)
		switch (val[i]) {
			case '\t': S += "\\t"; break;
			case '\n': S += "\\n"; break;
			default: S += val[i];
		}
	return S+"'"; }

List::List():Sym("list","[]") {}

Op::Op(string V):Sym("op",V) {}
Sym* Op::eval() {
	if (val=="~") return nest[0]; else Sym::eval();
	if (val=="=") return nest[0]->eq(nest[1]);
//	if (val=="@") return nest[0]->at(nest[1]);
//	if (val=="+") return nest[0]->add(nest[1]);
	if (val==":") return nest[0]->colon(nest[1]);
	return this;
}

Fn::Fn(string V, FN F):Sym("fn",V) { fn=F; }
Sym* Fn::at(Sym*o) { return fn(o); }

File::File(string V):Sym("file",V) {}
Sym* File::file(Sym*o) { return new File(o->val); }
Sym* File::eq(Sym*o) { push(o); return o; }

map<string,Sym*> env;
void env_init() {
	env["file"] = new Fn("file",File::file);
}
