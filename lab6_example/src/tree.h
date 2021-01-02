#ifndef TREE_H
#define TREE_H

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
    STMT_ASSIGN,
    STMT_WHILE,
    STMT_IF_ELSE,
    STMT_PRINTF,
    STMT_SCANF
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
    OP_ASSIGN,  // =
    OP_IAND,    // &
    OP_PLUS_ASSIGN, // +=
    OP_MINUS_ASSIGN, // -=
    OP_INC,    // ++
    OP_DEC    // --
};

enum
{
	VAR_DECL = 0,
};

enum
{
	Notype = 0,
	Integer,
	Boolean,
	Char,
	String
};

enum ExprType{
    EXPR_TYPE,
    EXPR_OP,
    EXPR_CONST,
    EXPR_ID
}

enum VarType{
    VAR_COMMON, //一般的变量
    VAR_QUOTE, //引用
    VAR_POINTER //指针
}
;

enum TypeType{
	TYPE_INT,
	TYPE_CHAR,
	TYPE_BOOL,
	FUNC_VOID,
	TYPE_STRING
}

#define MAX_CHILDREN 4


struct NodeAttr {
	OperatorType op;//操作符的类型
	VarType vartype;//变量的类型
	int vali;//int的值
	char valc;//char的值
	string vals;//string的值
	string var_name;//变量的名称
	int symtbl_seq;
	
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
	struct Node *children;
	struct Node *sibling;
	int lineno;
	int nodeID;
	NodeType kind;//节点类型
	int kind_kind;//节点更具体的类型
	NodeAttr attr;
	int type;//int,char,boolean
	int seq;
	int temp_var;//临时变量
	Label label;

	void output(void);

	Node(int lineno,NodeType kind, int kind_kind, NodeAttr attr, int type);
	void addChild(Node* t);
    void addSibling(Node* t);
	void genNodeId();
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
	void gen_header(ostream &out);
	void gen_decl(ostream &out, Node *t);
	void recursive_gen_code(ostream &out, Node *t);
	void stmt_gen_code(ostream &out, Node *t);
	void expr_gen_code(ostream &out, Node *t);

public:
	Node *NewRoot(NodeType kind, int kind_kind, NodeAttr attr, int type,
		Node *child = NULL);
	void get_label(void);
	void gen_code(ostream &out);
};
#endif
