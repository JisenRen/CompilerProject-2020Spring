#include <json/json.h>
#include <string>
#include <fstream>
#include <vector>
#include <stack>
#include <cstring>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
#include <map>
#include <set>
using namespace std;

string myitoa(int n) {
	int temp;
	string res;
	do {
		temp = n % 10;
		res += temp + '0';
		n /= 10;
	} while (n);
	reverse(res.begin(), res.end());
	return res;
}
int myatoi(string n) {
	int res = 0;
	int len = n.length();
	for (int i = 0; i<len; i++) {
		res *= 10;
		res += n[i] - '0';
	}
	return res;
}

enum Type {
	FloatV, IntV, Id,
	OpHigh, OpLow, Equal,
	Lpar, Rpar, Langle, Rangle, Lbracket, Rbracket,
	Comma, Semicolon,
	RHS, LHS, IdExpr, Expr,
	TRef, SRef,
	CList, AList
};

string Typename(Type t) {
	switch (t) {
	case FloatV: {
		return "FloatV";
		break;
	}
	case IntV: {
		return "IntV";
		break;
	}
	case Id: {
		return "Id";
		break;
	}
	case OpHigh: {
		return "OpHigh";
		break;
	}
	case OpLow: {
		return "OpLow";
		break;
	}
	case Equal: {
		return "Equal";
		break;
	}
	case Lpar: {
		return "Lpar";
		break;
	}
	case Rpar: {
		return "Rpar";
		break;
	}
	case Langle: {
		return "Langle";
		break;
	}
	case Rangle: {
		return "Rangle";
		break;
	}
	case Lbracket: {
		return "Lbracket";
		break;
	}
	case Rbracket: {
		return "Rbracket";
		break;
	}
	case Comma: {
		return "Comma";
		break;
	}
	case Semicolon: {
		return "Semicolon";
		break;
	}
	case RHS: {
		return "RHS";
		break;
	}
	case LHS: {
		return "LHS";
		break;
	}
	case IdExpr: {
		return "IdExpr";
		break;
	}
	case Expr: {
		return "IdExpr";
		break;
	}
	case TRef: {
		return "TRef";
		break;
	}
	case SRef: {
		return "SRef";
		break;
	}
	case CList: {
		return "CList";
		break;
	}
	case AList: {
		return "AList";
		break;
	}
	default: return "";
	}
}

struct Token {
	Type type;
	string name;
	Token() {}
	Token(Type _type, string _name) : type(_type), name(_name) {}
};
string Pp;
stringstream PP;
vector<Token> tokens;
void read_CList() {
	char p;
	string value;
	while ((p = PP.peek())) {
		if (p >= '0' && p <= '9') {
			PP >> p;
			value += p;
		}
		else if (p == ',') {
			PP >> p;
			tokens.push_back(Token(IntV, value));
			tokens.push_back(Token(Comma, ","));
			value = "";
			//read_CList();
		}
		else if (p == '>') {
			tokens.push_back(Token(IntV, value));
			break;
		}
		else {
			PP >> p;
			value += p;
		}
	}
}
void read_Num() {
	char p;
	Type t = IntV;
	string num;
	while ((p = PP.peek())) {
		if (p == '.') {
			t = FloatV;
			PP >> p;
			num += p;
		}
		else if (p >= '0' && p <= '9') {
			PP >> p;
			num += p;
		}
		else {
			break;
		}
	}
	tokens.push_back(Token(t, num));
}
void read_IDExpr() {
	char p;
	string id;
	while ((p = PP.peek())) {
		if (p == '+') {
			if (id.length() == 0) break;
			tokens.push_back(Token(Id, id));
			id = "";
			PP >> p;
			tokens.push_back(Token(OpLow, "+"));
			read_IDExpr();
		}
		else if (p == '-') {
			if (id.length() == 0) break;
			tokens.push_back(Token(Id, id));
			id = "";
			PP >> p;
			tokens.push_back(Token(OpLow, "-"));
			read_IDExpr();
		}
		else if (p == '*') {
			if (id.length() == 0) break;
			tokens.push_back(Token(Id, id));
			id = "";
			PP >> p;
			tokens.push_back(Token(OpHigh, "*"));
			read_IDExpr();
		}
		else if (p == '%') {
			if (id.length() == 0) break;
			tokens.push_back(Token(Id, id));
			id = "";
			PP >> p;
			tokens.push_back(Token(OpHigh, "%"));
			read_IDExpr();
		}
		else if (p == '/') {
			if (id.length() == 0) break;
			tokens.push_back(Token(Id, id));
			id = "";
			PP >> p;
			PP >> p;
			tokens.push_back(Token(OpHigh, "//"));
			read_IDExpr();
		}
		else if (p == '(') {
			PP >> p;
			tokens.push_back(Token(Lpar, "("));
			read_IDExpr();
			PP >> p;
			tokens.push_back(Token(Rpar, ")"));
		}
		else if (p == ']' || p == ')') {
			if (id.length() == 0) break;
			tokens.push_back(Token(Id, id));
			id = "";
			break;
		}
		else if (p == ',') {
			if (id.length() == 0) break;
			tokens.push_back(Token(Id, id));
			id = "";
			break;
		}
		else if (p >= '0' && p <= '9') {
			read_Num();
		}
		else {
			PP >> p;
			id += p;
		}
	}
}
void read_AList() {
	char p;
	while ((p = PP.peek())) {
		if (p == ']') {
			break;
		}
		else if (p == ',') {
			PP >> p;
			tokens.push_back(Token(Comma, ","));
			read_IDExpr();
		}
		else {
			read_IDExpr();
		}
	}
}
void read_TSRef() {
	char p;
	string token;
	while ((p = PP.peek())) {
		if (p == '=' || p == ';' || p == '+' || p == '-' || p == '*' || p == '/' || p == '%')  break;
		else if (p == '<') {
			tokens.push_back(Token(Id, token));
			token = "";
			PP >> p;  // to be revised: PP -> sstream
			tokens.push_back(Token(Langle, "<"));
			read_CList();
			PP >> p;
			tokens.push_back(Token(Rangle, ">"));
		}
		else if (p == '[') {
			PP >> p;
			tokens.push_back(Token(Lbracket, "["));
			read_AList();
			PP >> p;
			tokens.push_back(Token(Rbracket, "]"));
		}
		else if (p == ')') {
			break;
		}
		else {
			PP >> p;
			token += p;
		}
	}
}
void read_RHS() {
	char p;
	while ((p = PP.peek())) {
		if (p >= '0' && p <= '9') {
			read_Num();
		}
		else if (p == ';') {
			break;
		}
		else if (p == '(') {
			PP >> p;
			tokens.push_back(Token(Lpar, "("));
			read_RHS();
			PP >> p;
			tokens.push_back(Token(Rpar, ")"));
			if ((p = PP.peek()) == ')') {

			}
		}
		else if (p == ')') {
			break;
		}
		else if (p == '+') {
			PP >> p;
			tokens.push_back(Token(OpLow, "+"));
			read_RHS();
		}
		else if (p == '-') {
			PP >> p;
			tokens.push_back(Token(OpLow, "-"));
			read_RHS();
		}
		else if (p == '*') {
			PP >> p;
			tokens.push_back(Token(OpHigh, "*"));
			read_RHS();
		}
		else if (p == '%') {
			PP >> p;
			tokens.push_back(Token(OpHigh, "%"));
			read_RHS();
		}
		else if (p == '/') {
			PP >> p;
			if ((p = PP.peek()) == '/') {
				PP >> p;
				tokens.push_back(Token(OpHigh, "//"));
			}
			else {
				tokens.push_back(Token(OpHigh, "/"));
			}
			read_RHS();
		}
		else {
			read_TSRef();
		}
	}
}
void read_S() {
	char p;
	while ((p = PP.peek())) {
		if (p == ';') {
			break;
		}
		else if (p == ' ') {
			PP >> p;
		}
		else if (p == '=') {
			PP >> p;
			tokens.push_back(Token(Equal, "="));
			read_RHS();
		}
		else {
			read_TSRef();
		}
	}
}
void read_P() {
	char p;
	while ((p = PP.peek())) {
		if (p == '?') break;
		else if (p == ' ') {
			PP >> p;
		}
		else if (p == ';') {
			PP >> p;
			tokens.push_back(Token(Semicolon, ";"));
		}
		else {
			read_S();
		}
	}
}

vector <string> in_id, out_id, grad_id;
string datatype, function_name;
struct Node {
	Type type;
	Node() {}
	Node(Type x) : type(x) {}
	virtual ~Node() {}
	virtual void getinfo(Type &t, string &v, vector<Node *> &cl) {
		t = type;
		v = "";
		cl.clear();
	}
	virtual void gettype(Type &t) {
		t = type;
	}
	virtual void getchild(vector<Node *> &cl) {
		cl.clear();
	}
	virtual void getvalue(string &v) {
		v = "";
	}
	virtual bool isTerminal() {
		return false;
	}
};

Node *Left_root, *Right_root, *Left_out, *Right_out;
int current;
vector<pair<Node*, Node*> > Kernel;
stack<int> States;
stack<Node*> Symbols;
struct Terminal : Node {
	string value;
	Terminal() {}
	~Terminal() {}
	Terminal(const Token& T) {
		type = T.type;
		value = T.name;
	}
	virtual void getinfo(Type &t, string &v, vector<Node *> &cl) {
		t = type;
		v = value;
		cl.clear();
	}
	virtual void gettype(Type &t) {
		t = type;
	}
	virtual void getchild(vector<Node *> &cl) {
		cl.clear();
	}
	virtual void getvalue(string &v) {
		v = value;
	}
	virtual bool isTerminal() {
		return true;
	}
};
struct nTerminal : Node {
	vector<Node*> child;
	nTerminal() {}
	nTerminal(Type x) {
		Node::type = x;
	}
	virtual ~nTerminal() {
		child.clear();
	}
	void Reduce(int n)
	{
		int k;
		child.resize(n);
		for (k = n - 1; k >= 0; k--)
		{
			Node* t = Symbols.top();
			Symbols.pop();
			child[k] = t;
			States.pop();
		}
		current = States.top();
		States.pop();
	}
	virtual void getinfo(Type &t, string &v, vector<Node *> &cl) {
		t = type;
		v = "";
		cl = child;
	}
	virtual void gettype(Type &t) {
		t = type;
	}
	virtual void getchild(vector<Node *> &cl) {
		cl = child;
	}
	virtual void getvalue(string &v) {
		v = "";
	}
	virtual bool isTerminal() {
		return false;
	}
};

struct Expr_Constraint : nTerminal {
	vector<pair<string, string> > conds;
	Expr_Constraint() {}
	Expr_Constraint(Type x) {
		Node::type = x;
	}
	Expr_Constraint(const nTerminal& n)
	{
		Node::type = n.type;
		nTerminal::child = n.child;
		conds.clear();
	}
	~Expr_Constraint() {};
	virtual void getinfo(Type &t, string &v, vector<Node *> &cl) {
		t = type;
		v = "";
		cl = child;
	}
	virtual void gettype(Type &t) {
		t = type;
	}
	virtual void getchild(vector<Node *> &cl) {
		cl = child;
	}
	virtual void getvalue(string &v) {
		v = "";
	}
	virtual bool isTerminal() {
		return false;
	}
};
struct Func;
struct Sent;
struct TSRef;

void buildExpr(Node *n, string &s);
void getNodeInfo(Node *n, Type &t, string &v, vector<Node *> &cl);
void getIdxInfo(Node *n, vector<string> &idxname, vector<string> &bound, int cmd);
void getParIdx(Node *n, vector<string> &idxname, int cmd);
void getRefName(Node *n, string &name);
void getRefInfo(Node *n, vector<TSRef> &Ref_vec);

struct TSRef {
	string name;
	vector<string> paridx;
	vector<pair<string, string> > idx;
	vector<pair<string, string> > conds;
	TSRef() {}
	void printinfo() {
		cout << "ref name: " << name << endl;
		vector<string>::iterator it;
		vector<pair<string, string> >::iterator itp;
		cout << "-------paridx:" << endl;
		for (it = paridx.begin(); it != paridx.end(); it++) {
			cout << "  paridx:" << *it << endl;
		}
		cout << "-------idx:" << endl;
		for (itp = idx.begin(); itp != idx.end(); itp++) {
			cout << "  idx name:" << itp->first << " bound:" << itp->second << endl;
		}
		cout << endl;
	}
};

bool isExpr(string idxs) {
	int len = idxs.length();
	for (int i = 0; i<len; i++) {
		if (idxs[i] == '+' || idxs[i] == '/' || idxs[i] == '*' || idxs[i] == '%' || idxs[i] == '-') {
			return true;
		}
	}
	return false;
}

struct Sent {

	pair<Node*, Node*> kernel;//input

	Func * Master;// c
	string cal;// n c
	string res;// n

	vector<TSRef> refvec;// c 
	vector<string> paridx;// c
	vector<pair<string, string> > idx;// c
	vector<string> ifsent;// n
	vector<string> forsent;// n
	vector<pair<string, string> > lut;// input

	Sent() {}
	~Sent() {
		refvec.clear();
		paridx.clear();
		idx.clear();
		ifsent.clear();
		forsent.clear();
		lut.clear();
	}

	void Init_Sent() {
		vector<TSRef>::iterator itr;
		vector<TSRef> tempTSRefvec;
		getRefInfo(kernel.first, tempTSRefvec);
		getRefInfo(kernel.second, tempTSRefvec);
		for (itr = tempTSRefvec.begin(); itr != tempTSRefvec.end(); itr++) {
			refvec.push_back(*itr);
		}

		vector<TSRef>::iterator itrs;
		vector<string>::iterator its;
		vector<pair<string, string> >::iterator itp;
		for (itrs = refvec.begin(); itrs != refvec.end(); itrs++) {
			for (its = itrs->paridx.begin(); its != itrs->paridx.end(); its++) {
				paridx.push_back(*its);
			}
			for (itp = itrs->idx.begin(); itp != itrs->idx.end(); itp++) {
				idx.push_back(*itp);
			}
		}
	}
	void build_if() {
		string ifs;
		vector<pair<string, string> >::iterator itp;
		for (itp = idx.begin(); itp != idx.end(); itp++) {
			ifs = "";
			if (isExpr(itp->first)) {
				ifs = "if(";
				ifs += itp->first;
				ifs += '<';
				ifs += itp->second;
				ifs += "){";
				ifsent.push_back(ifs);
			}
		}
	}
	void build_for() {
		string fors;
		set<string>::iterator its;
		set<string> parset(paridx.begin(), paridx.end());
		for (its = parset.begin(); its != parset.end(); its++) {
			string bd = "";
			vector<pair<string, string> >::iterator itp;
			for (itp = lut.begin(); itp != lut.end(); itp++) {
				if (itp->first.compare(*its) == 0) {
					if (bd == "") {
						bd = itp->second;
					}
					else {
						if (myatoi(bd)>myatoi(itp->second)) {
							bd = itp->second;
						}
					}

				}
			}
			fors = "";
			fors += "for(int ";
			fors += *its;
			fors += "=0;";
			fors += *its;
			fors += "<";
			fors += bd;
			fors += ";";
			fors += *its;
			fors += "++){";
			forsent.push_back(fors);
		}
	}
	string build_idsymbol(TSRef &ts) {
		string sym;
		sym = ts.name;
		vector<pair<string, string> >::iterator itp;
		for (itp = ts.idx.begin(); itp != ts.idx.end(); itp++) {
			sym += '[';
			sym += itp->first;
			sym += ']';
		}
		if (ts.conds.empty())
			return sym;
		else {
			int nconds = ts.conds.size(), j;
			string condexpr = "";
			for (j = 0; j < nconds; j++)
			{
				if (j)
					condexpr += "&&";
				int len = ts.conds[j].second.length();
				for (int i = 1; i < len; i++)
					if (ts.conds[j].second[i - 1] == '/' && ts.conds[j].second[i] == '/')
						ts.conds[j].second[i] = ' ';
				condexpr += ts.conds[j].first + "==" + ts.conds[j].second;
			}
			return "(" + condexpr + "?" + sym + ":0)";
		}
	}
	void build_Expr(Node *n, int &cnt, string &res) {
		Type t;
		string v;
		vector<Node *> cl;
		vector<Node *>::iterator it;
		getNodeInfo(n, t, v, cl);
		if (t == TRef || t == SRef) {
			int sz = refvec.size();
			if (cnt<sz) {
				res += build_idsymbol(refvec[cnt]);
			}
			cnt++;
		}
		else if (t == OpLow || t == OpHigh) {
			res += v;
		}
		else if (t == CList || t == AList) {
			return;
		}
		else if (t == IntV || t == FloatV) {
			res += v;
		}
		else if (t == Lpar || t == Rpar) {
			res += v;
		}
		else {
			for (it = cl.begin(); it != cl.end(); it++) {
				build_Expr(*it, cnt, res);
			}
		}
	}
	void build_cal() {
		int cnt = 0;
		build_Expr(kernel.first, cnt, cal);
		cal += " += ";
		build_Expr(kernel.second, cnt, cal);
		cal += ";";
	}
	void build_Sent() {
		build_cal();
		build_if();
		build_for();
		int cnt = 0;
		vector<string>::iterator itv;
		for (itv = forsent.begin(); itv != forsent.end(); itv++) {
			for (int i = 0; i <= cnt; i++) {
				res += "    ";
			}
			res += *itv;
			res += '\n';
			cnt++;
		}
		for (itv = ifsent.begin(); itv != ifsent.end(); itv++) {
			for (int i = 0; i <= cnt; i++) {
				res += "    ";
			}
			res += *itv;
			res += '\n';
			cnt++;
		}
		for (int i = 0; i <= cnt; i++) {
			res += "    ";
		}
		res += cal;
		for (int i = cnt; i>0; i--) {
			res += '\n';
			for (int j = 0; j<i; j++) {
				res += "    ";
			}
			res += '}';
		}
	}
	void printinfo() {
		cout << "-------sentence info:" << endl;
		vector<string>::iterator it;
		vector<pair<string, string> >::iterator itp;
		vector<TSRef>::iterator itr;
		cout << "-------paridx:" << endl;
		for (it = paridx.begin(); it != paridx.end(); it++) {
			cout << "  paridx:" << *it << endl;
		}
		cout << "-------idx:" << endl;
		for (itp = idx.begin(); itp != idx.end(); itp++) {
			cout << "  idx name:" << itp->first << " bound:" << itp->second << endl;
		}
		cout << "-------ref in the sentence:" << endl;
		for (itr = refvec.begin(); itr != refvec.end(); itr++) {
			itr->printinfo();
		}
		cout << "-------sym build" << endl;
		for (itr = refvec.begin(); itr != refvec.end(); itr++) {
			cout << build_idsymbol(*itr) << endl;
		}
		cout << "-------cal:" << endl;
		cout << cal << endl;
		cout << "-------ifsent:" << endl;
		for (it = ifsent.begin(); it != ifsent.end(); it++) {
			cout << *it << endl;
		}
		cout << "-------forsent" << endl;
		for (it = forsent.begin(); it != forsent.end(); it++) {
			cout << *it << endl;
		}
		cout << "-------look up table:" << endl;
		for (itp = lut.begin(); itp != lut.end(); itp++) {
			cout << " idx " << itp->first << " bound " << itp->second << endl;
		}
		cout << "=======res" << endl;
		cout << res << endl;
		cout << endl;
	}
};

struct Func {

	vector<pair<Node*, Node*> > kernelvec;//input

	string funcname;//input
	string datatype;//input
	string funcsig;// n
	string res;// n

	vector<string> signame;//input
	vector<string> sigsym;// n
	vector<pair<string, string> > idx_bound;// c
	vector<Sent> sentvec;// c
	vector<TSRef> refvec;// c
	vector<string> refname;//input
	vector<string> parname;// n

	Func() {}
	Func(const vector<pair<Node*, Node*> >& ker, const Json::Value& obj) {
		vector<TSRef> Refs;
		kernelvec = ker;
		datatype = obj["data_type"].asString();
		funcname = obj["name"].asString();
		int s_in = obj["ins"].size(), s_out = obj["outs"].size(), s_grad = obj["grad_to"].size(), kerlen = ker.size(), nref, i;
		for (i = 0; i < kerlen; i++)
			getRefInfo(kernelvec[i].second, Refs);
		set<string> S;
		for (nref = Refs.size(), i = 0; i < nref; i++)
			S.insert(Refs[i].name);
		for (i = 0; i < s_in; i++) 
			if (S.find(obj["ins"][i].asString()) != S.end())
				signame.push_back(obj["ins"][i].asString());
		for (i = 0; i < s_out; i++)
			signame.push_back(string("d")+obj["outs"][i].asString());
		for (i = 0; i < s_grad; i++)
			signame.push_back(string("d")+obj["grad_to"][i].asString());
	}
	~Func() {
		idx_bound.clear();
		sentvec.clear();
		refvec.clear();
	}

	void Init_Func() {
		vector<pair<Node*, Node*> >::iterator itp;
		vector<pair<string, string> >::iterator itps;
		vector<TSRef>::iterator itr;
		vector<Sent>::iterator itsent;

		for (itp = kernelvec.begin(); itp != kernelvec.end(); itp++) {
			Sent temp;
			temp.kernel = *itp;
			temp.Master = this;
			temp.Init_Sent();
			sentvec.push_back(temp);

			for (itr = temp.refvec.begin(); itr != temp.refvec.end(); itr++) {
				refvec.push_back(*itr);
			}

			for (itps = temp.idx.begin(); itps != temp.idx.end(); itps++) {
				idx_bound.push_back(*itps);
			}
		}
		for (itsent = sentvec.begin(); itsent != sentvec.end(); itsent++) {
			itsent->lut = idx_bound;
		}
		for (itsent = sentvec.begin(); itsent != sentvec.end(); itsent++) {
			itsent->build_Sent();
		}


	}
	void build_sigsymbol() {
		vector<TSRef>::iterator itts;
		vector<string>::iterator its;
		vector<pair<string, string> >::iterator itp;
		string sym;
		int parcnt;
		for (its = signame.begin(); its != signame.end(); its++) {
			parcnt = 0;
			sym = datatype;
			sym += " (&";
			sym += *its;
			sym += ')';
			for (itts = refvec.begin(); itts != refvec.end(); itts++) {
				if (itts->name.compare(*its) == 0) {
					for (itp = itts->idx.begin(); itp != itts->idx.end(); itp++) {
						parcnt++;
						sym += '[';
						sym += itp->second;
						sym += ']';
					}
					break;
				}
			}
			if (parcnt == 0) {
				sym = datatype;
				sym += " &";
				sym += *its;
			}
			sigsym.push_back(sym);
		}
	}
	void build_sig() {
		vector<string>::iterator its;
		funcsig = "void ";
		funcsig += funcname;
		funcsig += '(';
		for (its = sigsym.begin(); its != sigsym.end(); its++) {
			if (its != sigsym.begin()) {
				funcsig += ", ";
			}
			funcsig += *its;
		}
		funcsig += ')';
	}
	void build_Func() {
		build_sigsymbol();
		build_sig();
		res = funcsig;
		res += '{';
		res += '\n';
		vector<Sent>::iterator itsent;
		for (itsent = sentvec.begin(); itsent != sentvec.end(); itsent++) {
			if (itsent != sentvec.begin()) {
				res += '\n';
			}
			res += itsent->res;
		}
		res += '\n';
		res += '}';
	}
	void printinfo() {
		vector<TSRef>::iterator itr;
		vector<Sent>::iterator its;
		cout << "all ref info" << endl;
		for (itr = refvec.begin(); itr != refvec.end(); itr++) {
			itr->printinfo();
		}
		for (its = sentvec.begin(); its != sentvec.end(); its++) {
			its->printinfo();
		}
		cout << "=======func res" << endl;
		cout << res << endl;
	}
	string getAnswer() {
		Init_Func();
		build_Func();
		return res;
	}
};

//Operators of Node struct
void getNodeInfo(Node *n, Type &t, string &v, vector<Node *> &cl) {
	n->getinfo(t, v, cl);
}

void getNodeValue(Node *n, string &v) {
	n->getvalue(v);
}

void getNodeChild(Node *n, vector<Node *> &cl) {
	n->getchild(cl);
}

void getNodeType(Node *n, Type &t) {
	n->gettype(t);
}

void isNodeTerminal(Node *n) {
	n->isTerminal();
}

//Functions that can help to get information from the Node Tree

//Form complete IdExpr of AList using string
void buildExpr(Node *n, string &s) {
	string v;
	vector<Node *> cl;
	vector<Node *>::iterator it;
	getNodeChild(n, cl);
	for (it = cl.begin(); it != cl.end(); it++) {
		if ((*it)->type == Id || (*it)->type == OpLow || (*it)->type == OpHigh || (*it)->type == IntV || (*it)->type == FloatV) {
			getNodeValue(*it, v);
			s = s + v;
		}
		else {
			buildExpr(*it, s);
		}
	}
}

Node* CopyTree(Node* p)
{
	if (p->isTerminal()) {
		Terminal* r = new Terminal;
		r->type = p->type;
		r->value = ((Terminal*)p)->value;
		return r;
	}
	else {
		nTerminal* r = new nTerminal;
		int n = ((nTerminal*)p)->child.size();
		r->type = p->type;
		r->child.resize(n);
		for (int i = 0; i < n; i++)
			r->child[i] = CopyTree(((nTerminal*)p)->child[i]);
		return r;
	}
}

void DestroyTree(Node* p)
{
	if (!p->isTerminal())
	{
		nTerminal* q = (nTerminal*)p;
		int i, sz = q->child.size();
		for (i = 0; i < sz; i++)
			DestroyTree(q->child[i]);
		q->child.clear();
	}
	delete p;
}

vector<Node*> Tree2List(nTerminal* r, Type _type) // _type is Expr, CList or AList
{
	vector<Node*> res;
	if (r->child.size() == 1) {
		res.resize(1);
		res[0] = r->child[0];
		return res;
	}
	else {
		res = Tree2List((nTerminal*)r->child[0], _type);
		if (_type == Expr)
			res.push_back(((nTerminal*)r->child[2])->child[0]);
		else
			res.push_back(r->child[2]);
		return res;
	}
}

Node* List2Tree(const vector<Node*> & L, Type _type, Token op) // _type is Expr, Clist or Alist
{
	int len = L.size();
	if (len == 1) {
		nTerminal *res = new nTerminal;
		res->type = _type;
		res->child.resize(1);
		res->child[0] = L[0];
		return res;
	}
	else {
		Node *lop, *rop, *res;
		lop = new nTerminal;
		lop->type = _type;
		((nTerminal*)lop)->child.resize(1);
		((nTerminal*)lop)->child[0] = L[0];
		if (_type != Expr)
			rop = L[1];
		else {
			rop = new nTerminal;
			rop->type = Expr;
			((nTerminal*)rop)->child.resize(1);
			((nTerminal*)rop)->child[0] = L[1];
		}
		res = new nTerminal;
		res->type = _type;
		((nTerminal*)res)->child.resize(3);
		((nTerminal*)res)->child[0] = lop;
		((nTerminal*)res)->child[1] = new Terminal(op);
		((nTerminal*)res)->child[2] = rop;
		for (int kk = 2; kk < len; kk++)
		{
			lop = res;
			if (_type != Expr)
				rop = L[kk];
			else {
				rop = new nTerminal;
				rop->type = Expr;
				((nTerminal*)rop)->child.resize(1);
				((nTerminal*)rop)->child[0] = L[kk];
			}
			res = new nTerminal;
			res->type = _type;
			((nTerminal*)res)->child.resize(3);
			((nTerminal*)res)->child[0] = lop;
			((nTerminal*)res)->child[1] = new Terminal(op);
			((nTerminal*)res)->child[2] = rop;
		}
		return res;
	}
}

nTerminal* Str2Node(const string& _id)
{
	Terminal* l = new Terminal(Token(Id, _id));
	nTerminal* r = new nTerminal;
	r->type = IdExpr;
	r->child.resize(1);
	r->child[0] = l;
	return r;
}

struct Tensor {
	string name;
	vector<Node*> dim; // range of each dimension
	vector<Node*> iexp; // index expression
	Tensor() {}
	~Tensor() {}
	Tensor(Node* q)
	{
		if (q->type != TRef)
			return;
		nTerminal *p = (nTerminal*)q;
		name = ((Terminal*)p->child[0])->value;
		dim = Tree2List((nTerminal*)p->child[2], CList);
		iexp = Tree2List((nTerminal*)p->child[5], AList);
	}
};

Node* Tensor2Tree(const Tensor& A) // re-construct syntax tree from tensor
{
	nTerminal* r = new nTerminal;
	r->type = TRef;
	r->child.resize(7);
	r->child[0] = new Terminal(Token(Id, A.name));
	r->child[1] = new Terminal(Token(Langle, "<"));
	r->child[2] = List2Tree(A.dim, CList, Token(Comma, ","));
	r->child[3] = new Terminal(Token(Rangle, ">"));
	r->child[4] = new Terminal(Token(Lbracket, "["));
	r->child[5] = List2Tree(A.iexp, AList, Token(Comma, ","));
	r->child[6] = new Terminal(Token(Rbracket, "]"));
	return r;
}

string IdExpr2Str(Node* p)
{
	if (p->isTerminal())
		return ((Terminal*)p)->value;
	else {
		nTerminal* q = (nTerminal*)p;
		string res("");
		int k, sz = q->child.size();
		for (k = 0; k < sz; k++)
			res += IdExpr2Str(q->child[k]);
		return res;
	}
}

bool isConst(Node* p)
{
	if (p->type != Expr)
		return false;
	nTerminal* q = (nTerminal*)p;
	return q->child.size() == 1 && (q->child[0]->type == FloatV || q->child[0]->type == IntV);
}

bool isMultChain(Node* p)
{
	if (p->type != Expr)
		return false;
	nTerminal* q = (nTerminal*)p;
	return q->child.size() == 1 || q->child[0]->type == Expr;
}

bool isSingleId(Node* p)
{
	if (p->type != IdExpr)
		return false;
	nTerminal* q = (nTerminal*)p;
	return q->child.size() == 1;
}

nTerminal* ZeroExpr()
{
	Terminal* l = new Terminal(Token(FloatV, "0.0"));
	nTerminal* r = new nTerminal;
	r->type = Expr;
	r->child.resize(1);
	r->child[0] = l;
	return r;
}

// assuming p is chain of multiplication, gradient to X
Node* Differentiate(Node* p, const string &X, Tensor A, vector<bool> &extralv) 
{
	Node *prev = NULL, *curr, *pA;
	vector<Node*> VN = Tree2List((nTerminal*)p, Expr);
	vector<Node*> VND;
	A.name = string("d") + A.name;
	int len = VN.size(), k;
	vector<Tensor> VT;
	for (k = 0; k < len; k++)
		VT.push_back(Tensor(VN[k]));
	for (k = 0; k < len; k++)
		if (VT[k].name == X) {
			VND = VN;
			Node* tempnode = VND[k];
			vector<Node*> IdExprSet = Tree2List((nTerminal*)((nTerminal*)tempnode)->child[5], AList);
			pA = new Expr_Constraint(*(nTerminal*)Tensor2Tree(A));
			int sz = extralv.size();
			for (int j = 0; j < sz; j++)
				if (extralv[j]) {
					((Expr_Constraint*)pA)->conds.push_back(make_pair(string("_") + (char)('0' + j), IdExpr2Str(IdExprSet[j])));
				}
			VND[k] = pA;
			curr = List2Tree(VND, Expr, Token(OpHigh, "*"));
			if (prev == NULL) {
				prev = curr;
			}
			else {
				nTerminal* temp = new nTerminal;
				temp->type = RHS;
				temp->child.resize(3);
				temp->child[0] = prev;
				temp->child[1] = new Terminal(Token(OpLow, "+"));
				temp->child[2] = curr;
				prev = new nTerminal;
				prev->type = Expr;
				((nTerminal*)prev)->child.resize(3);
				((nTerminal*)prev)->child[0] = new Terminal(Token(Lpar, "("));
				((nTerminal*)prev)->child[1] = temp;
				((nTerminal*)prev)->child[2] = new Terminal(Token(Rpar, ")"));
			}
		}
	return prev;
}
void Transform(Node* &p, const string &X, Tensor &A, vector<bool> &extralv)
{
	if (p->type == RHS) {
		int k, sz = ((nTerminal*)p)->child.size();
		for (k = 0; k < sz; k++)
			if (!((nTerminal*)p)->child[k]->isTerminal())
				Transform(((nTerminal*)p)->child[k], X, A, extralv);
	}
	else if (p->type == Expr) {
		if (isConst(p)) {
			p = ZeroExpr();
			return;
		}
		else if (isMultChain(p)) {
			p = Differentiate(p, X, A, extralv);
		}
		else {
			int k, sz = ((nTerminal*)p)->child.size();
			for (k = 0; k < sz; k++)
				if (!((nTerminal*)p)->child[k]->isTerminal())
					Transform(((nTerminal*)p)->child[k], X, A, extralv);
		}
	}
}

//Form two tables about element of AList, one for idxname another for their bound
//pay attention: r+s is treated as one element in this function!!!
void getIdxInfo(Node *n, vector<string> &idxname, vector<string> &bound, int cmd) {
	Type t;
	string v, accv;
	vector<Node *> cl;
	vector<Node *>::iterator it;
	getNodeInfo(n, t, v, cl);
	if (t == CList) {
		cmd = 1;
	}
	else if (t == AList) {
		cmd = 2;
	}
	for (it = cl.begin(); it != cl.end(); it++) {
		if (cmd == 1) {
			if ((*it)->type == IntV) {
				getNodeValue(*it, accv);
				if (accv.compare("1") != 0) {
					bound.push_back(accv);
				}
				continue;
			}
			else {
				getIdxInfo(*it, idxname, bound, cmd);
			}
		}
		else if (cmd == 2) {
			if ((*it)->type == IdExpr) {
				string e;
				buildExpr(*it, e);
				idxname.push_back(e);
				continue;
			}
			else {
				getIdxInfo(*it, idxname, bound, cmd);
			}
		}
		else {
			getIdxInfo(*it, idxname, bound, cmd);
		}
	}
}

//Form one table about name of AList's element 
//Pay attention: r+s is treated as two saperate elements in this function!!!
void getParIdx(Node *n, vector<string> &idxname, int cmd) {
	Type t;
	string v, accv;
	vector<Node *> cl;
	vector<Node *>::iterator it;
	getNodeInfo(n, t, v, cl);
	if (t == AList) {
		cmd = 1;
	}
	for (it = cl.begin(); it != cl.end(); it++) {
		if (cmd == 1) {
			if ((*it)->type == Id) {
				string e;
				getNodeValue(*it, e);
				idxname.push_back(e);
				continue;
			}
			else {
				getParIdx(*it, idxname, cmd);
			}
		}
		else {
			getParIdx(*it, idxname, cmd);
		}
	}
}

//get the name of TRef or SRef
void getRefName(Node *n, string &name) {
	Type t;
	string v, accv;
	vector<Node *> cl;
	vector<Node *>::iterator it;
	getNodeInfo(n, t, v, cl);
	for (it = cl.begin(); it != cl.end(); it++) {
		if ((*it)->type == Id) {
			getNodeValue(*it, name);
			break;
		}
	}
}

//get the infomation about the Ref in the sentence
void getRefInfo(Node *n, vector<TSRef> &Ref_vec) {
	Type t;
	string v, accv;
	vector<Node *> cl;
	vector<Node *>::iterator it;
	vector<string> parname_vec;
	vector<string> name_vec;
	vector<string> bound_vec;
	TSRef temp;
	getNodeInfo(n, t, v, cl);
	if (t == TRef || t == SRef) {
		getRefName(n, accv);
		getParIdx(n, parname_vec, 0);
		getIdxInfo(n, name_vec, bound_vec, 0);
		temp.name = accv;
		temp.paridx = parname_vec;
		int sz = name_vec.size();
		for (int x = 0; x<sz; x++) {
			temp.idx.push_back(make_pair(name_vec[x], bound_vec[x]));
		}
		temp.conds.clear();
		if (dynamic_cast<Expr_Constraint*>(n)) {
			temp.conds = ((Expr_Constraint*)n)->conds;
		}
		Ref_vec.push_back(temp);
	}
	for (it = cl.begin(); it != cl.end(); it++) {
		getRefInfo(*it, Ref_vec);
	}
}

//only for debug
void printTreeInfo(Node *n, int d) {
	Type t;
	string v;
	vector<Node *> cl;
	vector<Node *>::iterator it;
	getNodeInfo(n, t, v, cl);
	// cout << "type is " << Typename(t) << " v= " << v << endl;
	if (v.length() > 0)
		// cout << '(' << v << ',' << d << ')' << ' ';
		cout << v;
	for (it = cl.begin(); it != cl.end(); it++) {
		// for (int i = 0; i < d; cout << ' ', i++);
		printTreeInfo(*it, d + 1);
	}
}

void printIdxInfo(Node *n) {
	vector<string> idxname;
	vector<string> bound;
	getIdxInfo(n, idxname, bound, 0);
	int sz = idxname.size();
	for (int i = 0; i<sz; i++) {
		cout << "No." << i << " idxname = " << idxname[i] << endl;
	}
	for (int i = 0; i<sz; i++) {
		cout << "No." << i << " idxbound = " << bound[i] << endl;
	}
}

void printParInfo(Node *n) {
	vector<string> idxname;
	getParIdx(n, idxname, 0);
	int sz = idxname.size();
	for (int i = 0; i<sz; i++) {
		cout << "No." << i << " idxname = " << idxname[i] << endl;
	}
}

void printRefInfo(Node *n) {
	vector<TSRef> refvec;
	getRefInfo(n, refvec);
	int sz = refvec.size();
	for (int i = 0; i<sz; i++) {
		cout << "No." << i << " refname = " << refvec[i].name << endl;
		int psize = refvec[i].paridx.size(), isize = refvec[i].idx.size();
		for (int j = 0; j<psize; j++) {
			cout << "  No." << j << " paridxname = " << refvec[i].paridx[j] << endl;
		}
		for (int j = 0; j<isize; j++) {
			cout << " No." << j << " idxname = " << refvec[i].idx[j].first << " idxbound = " << refvec[i].idx[j].second << endl;
		}
	}
}

Node* Parenthese = NULL;

void Analyze(Node* p)
{
	bool reduced = false;
	States.push(current);
	switch (current)
	{
	case 0: {
		switch (p->type)
		{
		case RHS: {
			current = 1;
			break;
		}
		case Expr: {
			current = 2;
			break;
		}
		case Lpar: {
			current = 3;
			break;
		}
		case TRef: {
			current = 4;
			break;
		}
		case SRef: {
			current = 5;
			break;
		}
		case FloatV:
		case IntV: {
			current = 6;
			break;
		}
		case Id: {
			current = 7;
			break;
		}
		default:;
		}
		break;
	}
	case 1: {
		if (p->type == OpLow)
			current = 8;
		else {
			Right_root = Symbols.top();
			while (!Symbols.empty()) Symbols.pop();
			while (!States.empty()) States.pop();
			reduced = true;
		}
		break;
	}
	case 2: {
		if (p->type == OpHigh)
			current = 9;
		else {
			nTerminal* q = new nTerminal(RHS);
			q->Reduce(1);
			Analyze(q);
			Analyze(p);
			reduced = true;
		}
		break;
	}
	case 3: {
		switch (p->type)
		{
		case RHS: {
			current = 10;
			break;
		}
		case Expr: {
			current = 2;
			break;
		}
		case Lpar: {
			current = 3;
			break;
		}
		case TRef: {
			current = 4;
			break;
		}
		case SRef: {
			current = 5;
			break;
		}
		case FloatV:
		case IntV: {
			current = 6;
			break;
		}
		case Id: {
			current = 7;
			break;
		}
		default:;
		}
		break;
	}
	case 4:
	case 5:
	case 6: {
		nTerminal* q = new nTerminal(Expr);
		q->Reduce(1);
		Analyze(q);
		Analyze(p);
		reduced = true;
		break;
	}
	case 7: {
		if (p->type == Langle)
			current = 11;
		break;
	}
	case 8: {
		switch (p->type)
		{
		case RHS: {
			current = 12;
			break;
		}
		case Expr: {
			current = 2;
			break;
		}
		case Lpar: {
			current = 3;
			break;
		}
		case TRef: {
			current = 4;
			break;
		}
		case SRef: {
			current = 5;
			break;
		}
		case FloatV:
		case IntV: {
			current = 6;
			break;
		}
		case Id: {
			current = 7;
			break;
		}
		default:;
		}
		break;
	}
	case 9: {
		switch (p->type)
		{
		case Expr: {
			current = 13;
			break;
		}
		case Lpar: {
			current = 3;
			break;
		}
		case TRef: {
			current = 4;
			break;
		}
		case SRef: {
			current = 5;
			break;
		}
		case FloatV:
		case IntV: {
			current = 6;
			break;
		}
		case Id: {
			current = 7;
			break;
		}
		default:;
		}
		break;
	}
	case 10: {
		if (p->type == Rpar)
			current = 14;
		else if (p->type == OpLow)
			current = 8;
		break;
	}
	case 11: {
		if (p->type == CList)
			current = 15;
		else if (p->type == IntV)
			current = 16;
		break;
	}
	case 12: {
		nTerminal* q = new nTerminal(RHS);
		q->Reduce(3);
		Analyze(q);
		Analyze(p);
		reduced = true;
		break;
	}
	case 13: {
		nTerminal* q = new nTerminal(Expr);
		q->Reduce(3);
		Analyze(q);
		Analyze(p);
		reduced = true;
		break;
	}
	case 14: {
		nTerminal* q = new nTerminal(Expr);
		q->Reduce(3);
		Analyze(q);
		Parenthese = q;
		Analyze(p);
		reduced = true;
		break;
	}
	case 15: {
		if (p->type == Rangle)
			current = 17;
		else if (p->type == Comma)
			current = 18;
		break;
	}
	case 16: {
		nTerminal* q = new nTerminal(CList);
		q->Reduce(1);
		Analyze(q);
		Analyze(p);
		reduced = true;
		break;
	}
	case 17: {
		if (p->type == Lbracket)
			current = 19;
		else {
			nTerminal* q = new nTerminal(SRef);
			q->Reduce(4);
			Analyze(q);
			Analyze(p);
			reduced = true;
			break;
		}
	}
	case 18: {
		if (p->type == IntV)
			current = 20;
		break;
	}
	case 19: {
		switch (p->type)
		{
		case AList: {
			current = 21;
			break;
		}
		case IdExpr: {
			current = 22;
			break;
		}
		case Id: {
			current = 23;
			break;
		}
		case Lpar: {
			current = 24;
			break;
		}
		default:;
		}
		break;
	}
	case 20: {
		nTerminal* q = new nTerminal(CList);
		q->Reduce(3);
		Analyze(q);
		Analyze(p);
		reduced = true;
		break;
	}
	case 21: {
		if (p->type == Rbracket)
			current = 25;
		else if (p->type == Comma)
			current = 26;
		break;
	}
	case 22: {
		if (p->type == OpLow)
			current = 27;
		else if (p->type == OpHigh)
			current = 28;
		else {
			nTerminal* q = new nTerminal(AList);
			q->Reduce(1);
			Analyze(q);
			Analyze(p);
			reduced = true;
			break;
		}
		break;
	}
	case 23: {
		nTerminal* q = new nTerminal(IdExpr);
		q->Reduce(1);
		Analyze(q);
		Analyze(p);
		reduced = true;
		break;
	}
	case 24: {
		if (p->type == IdExpr)
			current = 29;
		else if (p->type == Id)
			current = 23;
		else if (p->type == Lpar)
			current = 24;
		break;
	}
	case 25: {
		nTerminal* q = new nTerminal(TRef);
		q->Reduce(7);
		Analyze(q);
		Analyze(p);
		reduced = true;
		break;
	}
	case 26: {
		if (p->type == IdExpr)
			current = 30;
		else if (p->type == Id)
			current = 23;
		else if (p->type == Lpar)
			current = 24;
		break;
	}
	case 27: {
		if (p->type == IdExpr)
			current = 32;
		else if (p->type == Id)
			current = 23;
		else if (p->type == Lpar)
			current = 24;
		else if (p->type == IntV)
			current = 31;
		break;
	}
	case 28: {
		if (p->type == IdExpr)
			current = 33;
		else if (p->type == Id)
			current = 23;
		else if (p->type == Lpar)
			current = 24;
		else if (p->type == IntV)
			current = 31;
		break;
	}
	case 29: {
		if (p->type == Rpar)
			current = 34;
		else if (p->type == OpLow)
			current = 27;
		else if (p->type == OpHigh)
			current = 28;
		break;
	}
	case 30: {
		if (p->type == OpLow)
			current = 27;
		else if (p->type == OpHigh)
			current = 28;
		else {
			nTerminal* q = new nTerminal(AList);
			q->Reduce(3);
			Analyze(q);
			Analyze(p);
			reduced = true;
			break;
		}
		break;
	}
	case 31:
	case 32:
	case 33:
	case 34: {
		nTerminal* q = new nTerminal(IdExpr);
		q->Reduce(3);
		Analyze(q);
		Analyze(p);
		reduced = true;
		break;
	}
	case 35: {
		if (p->type == TRef)
			current = 36;
		else if (p->type == Id)
			current = 37;
		break;
	}
	case 36: {
		Left_root = Symbols.top();
		while (!Symbols.empty()) Symbols.pop();
		while (!States.empty()) States.pop();
		reduced = true;
		break;
	}
	case 37: {
		if (p->type == Langle)
			current = 38;
		break;
	}
	case 38: {
		if (p->type == CList)
			current = 39;
		else if (p->type == IntV)
			current = 16;
		break;
	}
	case 39: {
		if (p->type == Rangle)
			current = 40;
		else if (p->type == Comma)
			current = 18;
		break;
	}
	case 40: {
		if (p->type == Lbracket)
			current = 19;
		break;
	}
	default:;
	}
	if (!reduced)
		Symbols.push(p);
}

int main() {
	string cwd = "cases/";
	for (int _ = 1; _ <= 10; _++)
	{
		string fname = cwd + "case" + myitoa(_) + ".json";
		ifstream case_file(fname, ifstream::binary);
		Json::Value obj;
		Json::Reader reader;
		reader.parse(case_file, obj, false);
		datatype = obj["data_type"].asString();
		function_name = obj["name"].asString();
		int s_in = obj["ins"].size(), s_out = obj["outs"].size(), s_grad = obj["grad_to"].size();
		for (int i = 0; i < s_in; i++) in_id.push_back(obj["ins"][i].asString());
		for (int i = 0; i < s_out; i++) out_id.push_back(obj["outs"][i].asString());
		for (int i = 0; i < s_grad; i++) grad_id.push_back(obj["grad_to"][i].asString());
		Pp = obj["kernel"].asString();
		Pp += '?';
		int k, L = Pp.length();
		string P;
		for (int i = 0; i < L; i++) {
			if (Pp[i] == ' ') continue;
			P += Pp[i];
		}
		PP.str(string());
		PP << P;
		read_P();
		L = tokens.size();
		vector<pair<Node*, Node*> > Kernel;
		for (int kk = 0; kk < s_grad; kk++)
		{
			string &grad_to = grad_id[kk];
			for (k = 0, current = 35; tokens[k].type != Equal; k++)
				Analyze(new Terminal(tokens[k]));
			Analyze(new Terminal(tokens[k++]));
			for (Parenthese = NULL, current = 0; tokens[k].type != Semicolon; k++)
				Analyze(new Terminal(tokens[k]));
			Analyze(new Terminal(tokens[k++]));
			
			vector<TSRef> Refs, Refs_ans;
			getRefInfo(Right_root, Refs);
			vector<bool> extralv; // for the tensor onto which the gradient is calculated, is an extra loop variable needed?
			int dim = 0, sz = Refs.size(); 
			Tensor Tleft;
			for (int k = 0; k < sz; k++)
			{
				if (Refs[k].name != grad_to)
					continue;
				if (extralv.empty())
					extralv.resize(dim = Refs[k].idx.size());
				for (int j = 0; j < dim; j++)
					if (isExpr(Refs[k].idx[j].first))
						extralv[j] = true;
			}
			for (int k = 0; k < sz; k++) // construct left-side Tensor of resulting expression
				if (Refs[k].name == grad_to && Tleft.dim.empty()) {
					Tleft.name = string("d") + Refs[k].name;
					Tleft.dim.resize(dim);
					Tleft.iexp.resize(dim);
					for (int j = 0; j < dim; j++) 
					{
						Tleft.dim[j] = new Terminal;
						Tleft.dim[j]->type = IntV;
						((Terminal*)Tleft.dim[j])->value = Refs[k].idx[j].second;
						if (extralv[j])
							Tleft.iexp[j] = Str2Node(string("_") + (char)('0' + j));
						else
							Tleft.iexp[j] = Str2Node(Refs[k].idx[j].first);
					}
					break;
				}
			Tensor Torg(CopyTree(Left_root));
			Left_root = Tensor2Tree(Tleft);
			if (Parenthese)
				Transform(((nTerminal*)Parenthese)->child[1], grad_to, Torg, extralv);
			else
				Transform(Right_root, grad_to, Torg, extralv);
			Kernel.push_back(make_pair(Left_root, Right_root));
		}
		Func fun(Kernel, obj);
		string answer = fun.getAnswer();
		fstream fout;
		fout.open("kernels/grad_case" + myitoa(_) + ".cc", ios::out);
		fout << "#include \"../run2.h\"" << endl;
		fout << answer << endl;
		fout.close();
		in_id.clear(), out_id.clear(), grad_id.clear();
		Kernel.clear();
		tokens.clear();
	}
	return 0;
}
