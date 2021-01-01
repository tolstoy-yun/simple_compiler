#include "tree.h"
#include <iomanip>

extern ofstream fout;
static int nodeCount=0;
extern int scope; //记录当前所处的作用域的标号
extern int symbolNum; //记录当前记录到第几个符号
extern stack<TreeNode*> currentScope; //栈中记录当前作用域的变量

void TreeNode::addChild(TreeNode* child) {
    //当前结点已经有孩子结点了
    if(this->child){
        /*遍历这个孩子的结点的兄弟结点
        * 遍历到最后一个兄弟结点，将这个孩子结点加为这个结点的兄弟结点
        */
        TreeNode* temp=this->child;
        while(temp->sibling){
            temp=temp->sibling;
        }
        temp->sibling=child;
    }
    //当前结点没有孩子节点
    else{
        this->child=child;
    }
}

void TreeNode::addSibling(TreeNode* sibling){
    /*当前结点已经有兄弟结点了
    * 遍历到最后一个兄弟结点，将其加为这个结点的兄弟结点
    */
    if(this->sibling){
        TreeNode* temp=this->sibling;
        while(temp->sibling){
            temp=temp->sibling;
        }
        temp->sibling=sibling;
    }
    //当前结点没有兄弟结点
    else{
        this->sibling=sibling;
    }
}

TreeNode::TreeNode(int lineno, NodeType type) {
    this->lineno=lineno;
    this->nodeType=type;
}

void TreeNode::genNodeId() {
    this->nodeID=nodeCount++;
    //给该结点的孩子结点赋nodeID
    if(this->child){
        this->child->genNodeId();
    }
    //给该结点的兄弟结点赋nodeID
    if(this->sibling){
        this->sibling->genNodeId();
    }
}

void TreeNode::printNodeInfo() {
    fout<<"lno@"<<std::left<<std::setw(3)<<this->lineno<<"@"<<std::left<<std::setw(3)<<this->nodeID<<std::left<<std::setw(12)<<this->nodeType2String(this->nodeType);
}

void TreeNode::printChildrenId() {
    fout<<std::left<<std::setw(10)<<"children:";
    fout<<"[";
    TreeNode* temp=this->child;
    while(temp){
        fout<<std::left<<"@"<<temp->nodeID<<" ";
        temp=temp->sibling;
    }
    fout<<" ] ";
}

void TreeNode::printAST() {
    this->printNodeInfo();
    this->printChildrenId();
    this->printSpecialInfo();
    if(this->child){
        this->child->printAST();
    }
    if(this->sibling){
        this->sibling->printAST();
    }
}

// You can output more info...
void TreeNode::printSpecialInfo() {
    switch(this->nodeType){
        case NODE_CONST:
            fout<<"type: "<<this->type->getTypeInfo()<<endl;
            break;
        case NODE_VAR:
            fout<<"varname: "<<this->var_name<<" varNum: "<<this->number<<endl;
            break;
        case NODE_EXPR:
            if(this->optype>=0){
                fout<<"op: "<<this->opType2String(this->optype);
            }
            fout<<endl;
            break;
        case NODE_STMT:
            fout<<"stmt: "<<this->sType2String(this->stype)<<endl;
            break;
        case NODE_TYPE:
            fout<<"type: "<<this->type->getTypeInfo()<<endl;
            break;
        case NODE_PROG:
            fout<<endl;
            break;
        case NODE_FUNC:
            fout<<endl;
            break;
        default:
            cerr << "shouldn't reach here, typeinfo";
            assert(0);
            break;
    }
}

string TreeNode::sType2String(StmtType type) {
    switch(type){
        case STMT_SKIP:
            return "skip";
        case STMT_DECL:
            return "decl";
        case STMT_FOR:
            return "for";
        case STMT_ASSIGN:
            return "assign";
        case STMT_WHILE:
            return "while";
        case STMT_IF_ELSE:
            return "if_else";
        case STMT_PRINTF:
            return "printf";
        case STMT_SCANF:
            return "scanf";
        default:
            cerr << "shouldn't reach here, typeinfo";
            assert(0);
            break;
    }
}


string TreeNode::nodeType2String (NodeType type){
    switch(type){
        case NODE_CONST:
            return "const";
        case NODE_VAR:
            return "variable";
        case NODE_EXPR:
            return "expression";
        case NODE_TYPE:
            return "type";
        case NODE_STMT:
            return "statement";
        case NODE_PROG:
            return "program";
        case NODE_FUNC:
            return "function";
        default:
            cerr << "shouldn't reach here, typeinfo";
            assert(0);
            break;
    }
}

string TreeNode::opType2String (OperatorType type){
    switch(type){
        case OP_EQ:
            return "EQ";
        case OP_LT:
            return "LT";
        case OP_GT:
            return "GT";
        case OP_LE:
            return "LE";
        case OP_GE:
            return "GE";
        case OP_NZ:
            return "NZ";
        case OP_PLUS:
            return "PLUS";
        case OP_MINUS:
            return "MINUS";
        case OP_MUL:
            return "MUL";
        case OP_DIV:
            return "DIV";
        case OP_MOD:
            return "MOD";
        case OP_AND:
            return "AND";
        case OP_OR:
            return "OR";
        case OP_OPPSITE:
            return "OPPSITE";
        case OP_ASSIGN:
            return "ASSIGN";
        case OP_IAND:
            return "IAND";
        case OP_PLUS_ASSIGN:
            return "PLUS_ASSIGN";
        case OP_MINUS_ASSIGN:
            return "MINUS_ASSIGN";
        case OP_INC:
            return "INC";
        case OP_DEC:
            return "DEC";
        default:
            cerr << "shouldn't reach here, typeinfo";
            assert(0);
            break;
    }
}

string TreeNode::vType2String (VarType type){
    switch(type){
        case VAR_COMMON:
            return "COMMON";
        case VAR_QUOTE:
            return "QUOTE";
        case VAR_POINTER:
            return "POINTER";
        default:
            cerr << "shouldn't reach here, typeinfo";
            assert(0);
            break;
    }
}
