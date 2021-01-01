#ifndef TREE_H
#define TREE_H

#include "pch.h"
#include "symbol.h"
extern ofstream fout;

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
enum VarType{
    VAR_COMMON, //一般的变量
    VAR_QUOTE, //引用
    VAR_POINTER //指针
}
;

enum ExprType{
    EXPR_TYPE,
    EXPR_OP,
    EXPR_CONST,
    EXPR_ID
}

struct Label {
	string true_label;
	string false_label;
	string begin_label;
	string next_label;
};

struct TreeNode {
public:
    int nodeID;  // 用于作业的序号输出
    int lineno;
    NodeType nodeType;

    TreeNode* child = nullptr;
    TreeNode* sibling = nullptr;

    void addChild(TreeNode*);
    void addSibling(TreeNode*);
    
    void printNodeInfo();
    void printChildrenId();

    void printAST(); // 先输出自己 + 孩子们的id；再依次让每个孩子输出AST。
    void printSpecialInfo();

    void genNodeId();

//节点的属性
public:
    OperatorType optype ;  // 符号类型
    Type* type;  // 变量、类型、表达式结点，有类型。
    StmtType stype;
    VarType vtype;
    int int_val;
    char ch_val;
    bool b_val;
    string str_val;
    string var_name;
    int firstScope=-1; //当node类型为变量时，记录下它被定义时所处的作用域标号
    int number=-1; //当node类型为变量时，其被分配的序号
public:
    static string nodeType2String (NodeType type);
    static string opType2String (OperatorType type);
    static string sType2String (StmtType type);
    static string vType2String(VarType type);
public:
    int temp_var;
	Label label;

	void output(void);

public:
    TreeNode(int lineno, NodeType type);
};

#endif