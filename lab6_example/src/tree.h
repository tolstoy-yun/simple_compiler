#ifndef TREE_H
#define TREE_H

#include "pch.h"
#include "symbol.h"
enum NodeType
{
    NODE_CONST, 
    NODE_VAR,
    NODE_EXPR,
    NODE_TYPE,
    NODE_STMT,
    NODE_PROG,
    NODE_FUNC //函数
};

enum StmtType {
    STMT_SKIP,
    STMT_DECL,
    STMT_FOR,
    STMT_WHILE,
    STMT_IF_ELSE,
	STMT_IF,
    STMT_PRINTF,
    STMT_SCANF,
	STMT_RETURN
}
;

enum OperatorType
{
    OP_EQ,  // ==
    OP_LT,  // <
    OP_GT,  // >
    OP_LE,  // <=
    OP_GE,  // >=
    OP_NZ,  // !=
    OP_PLUS,  // +
    OP_MINUS, // -
    OP_MUL,   // *
    OP_DIV,   // /
    OP_MOD,   // %
	OP_AND,   // &&
    OP_OR,    // ||
    OP_OPPSITE, // !
    OP_PLUS_ASSIGN, // +=
    OP_MINUS_ASSIGN, // -=
    OP_INC,    // ++
    OP_DEC,    // --
	OP_ASSIGN,  // =
	
}
;

enum
{
	Notype = 0,
	Integer,
	Boolean,
	Char,
	String,
	Function
}
;

enum ExprType{
    EXPR_OP
}
;

enum VarType{
    VAR_COMMON, //一般的变量
}
;

enum TypeType{
	TYPE_INT,
	TYPE_CHAR,
	TYPE_BOOL,
	FUNC_VOID,
	TYPE_STRING
}
;

#define MAX_CHILDREN 4


struct NodeAttr {
	OperatorType op;//操作符的类型
	VarType vartype;//变量的类型
	int vali;//int的值
	char valc;//char的值
	
	string vals;//string的值
	string var_name;//变量的名称
	
	NodeAttr(void) {;}
	NodeAttr(OperatorType i)	{ op = i; }
	NodeAttr(char c) { valc = c; }
	NodeAttr(string s) {vals = s; }
};

struct Label {
	string true_label;
	string false_label;
	string begin_label;
	string next_label;
};

struct Node
{
public:
	struct Node *children=nullptr;
	struct Node *sibling=nullptr;
	int lineno;
	NodeType kind;//节点类型
	int kind_kind;//节点更具体的类型
	NodeAttr attr;
	int type;//int,char,boolean
	int seq;//序号
	int pos=-1;//变量在符号表中的位置
	int temp_var;//临时变量
	Label label;
	int firstScope=-1; //当node类型为变量时，记录下它被定义时所处的作用域标号
    int number=-1; //当node类型为变量时，其被分配的序号
	int suspected_redefine=0;//疑似重定义符号标志位，为1时代表这个符号疑似重定义了
	int global_val=0;//全局变量标志位，1为全局变量
	void output(void);

	Node(int lineno,NodeType kind, int kind_kind, NodeAttr attr, int type);
	Node();
	void addChild(Node* t);
    void addSibling(Node* t);
};

class tree
{
public:
	Node *root;
	int node_seq = 0;
	int temp_var_seq = 0;
	int label_seq = 0;

public:
	void type_check(Node *t);
	void get_temp_var(Node *t);
	string new_label(void);
	void recursive_get_label(Node *t);
	void stmt_get_label(Node *t);
	void expr_get_label(Node *t);
	void func_get_label(Node *t);
	void gen_header(ostream &out);
	void gen_decl(ostream &out, Node *t);
	void gen_str(ostream &out, Node *t);
	void recursive_gen_code(ostream &out, Node *t);
	void stmt_gen_code(ostream &out, Node *t);
	void expr_gen_code(ostream &out, Node *t);
	
public:
	Node* NewRoot(int lineno,NodeType kind, int kind_kind, NodeAttr attr, int type,
		Node *child = NULL);
	void get_label(void);
	void gen_code(ostream &out,Node* p);
	string get_var(Node* p);
};
#endif
