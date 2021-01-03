#include <iostream>
#include <string>

using namespace ::std;

#include "tree.h"
#include "symbol.h"

extern int lineno;
extern symbol_table symtbl;
extern int scope; //记录当前所处的作用域的标号
extern int symbolNum; //记录当前记录到第几个符号
extern stack<TreeNode*> currentScope; //栈中记录当前作用域的变量

tree parse_tree;

void Node::output(void)
{
	// write your own code to print the parser tree
	cout << endl;
}

void Node::addChild(Node* child) {
    //当前结点已经有孩子结点了
    if(this->children){
        /*遍历这个孩子的结点的兄弟结点
        * 遍历到最后一个兄弟结点，将这个孩子结点加为这个结点的兄弟结点
        */
        Node* temp=this->children;
        while(temp->sibling){
            temp=temp->sibling;
        }
        temp->sibling=child;
    }
    //当前结点没有孩子节点
    else{
        this->children=child;
    }
}

void Node::addSibling(Node* sibling){
    /*当前结点已经有兄弟结点了
    * 遍历到最后一个兄弟结点，将其加为这个结点的兄弟结点
    */
    if(this->sibling){
        Node* temp=this->sibling;
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

Node::Node(int lineno,NodeType kind, int kind_kind, NodeAttr attr, int type) {
    this->lineno=lineno;
    this->kind=kind;
	this->kind_kind=kind_kind;
	this->attr=attr;
	this->type=type;
}

void tree::type_check(Node *t)
{
	// type check, write your own code here

	switch (t->kind)
	{	
		//声明时重定义的类型检查在main.y里实现
		case NODE_STMT:
			switch(t->kind_kind){
				case STMT_WHILE:
					if (t->children->type != Boolean)
					{
						cerr <<"[line " <<t->lineno<<"]：Bad boolean type." << endl;
						exit(1);
					}
					break;
				case STMT_SKIP:
					break;
				case STMT_DECL:
					break;
				case STMT_FOR:
					//第二个孩子要是boolean类型的
					if(t->children->sibling!= Boolean)
					{
						cerr <<"[line " <<t->lineno<<"]：Bad boolean type." << endl;
						exit(1);
					}
					break;
				case STMT_IF_ELSE:
					if(t->children!=Boolean)
					{
						cerr <<"[line " <<t->lineno<<"]：Bad boolean type." << endl;
						exit(1);
					}
					break;
				case STMT_PRINTF:
					break;
				case STMT_SCANF:
					break;
				case STMT_RETURN:
					break;
			}
			break;
		case NODE_CONST:
			break;
		case NODE_VAR:
			switch(t->kind_kind){
				case VAR_COMMON:
					break;
				case VAR_QUOTE:
					//引用的符号必须存在
					if(symtbl.lookup(t->attr.var_name)==-1){
						cerr <<"[line " <<t->lineno<<"]：The referenced symbol does not exist." << endl;
						exit(1);
					}
					break;
				default:
					cerr <<"[line " <<t->lineno<<"]：Bad type." << endl;
					exit(1);
			}
			break;
		case NODE_EXPR:
			if(t->kind_kind==EXPR_OP)
			{
				switch(op)
				{	
					//+=，两个操作数可以都为int，都为char，一个是int，一个是char
					case LOP_PLUS_ASSIGN:
						if((t->children->type==Integer&&t->children->sibling->type==Integer) || (t->children->type==Integer&&t->children->sibling->type==Char) || (t->children->type==Char&&t->children->sibling->type==Integer) || (t->children->type==Char&&t->children->sibling->type==Char)){
							break;
						}
						else{
							cerr <<"[line " <<t->lineno<<"]：Bad \"+=\" type." << endl;
							exit(1);
						}
						break;
					//-=，两个操作数可以都为int，都为char，一个是int，一个是char
					case LOP_MINUS_ASSIGN:
						if((t->children->type==Integer&&t->children->sibling->type==Integer) || (t->children->type==Integer&&t->children->sibling->type==Char) || (t->children->type==Char&&t->children->sibling->type==Integer) || (t->children->type==Char&&t->children->sibling->type==Char)){
							break;
						}
						else{
							cerr <<"[line " <<t->lineno<<"]：Bad \"-=\" type." << endl;
							exit(1);
						}
						break;
					//=，赋值，两个操作数可以都为int，都为char，一个是int，一个是char
					case LOP_ASSIGN:
						if((t->children->type==Integer&&t->children->sibling->type==Integer) || (t->children->type==Integer&&t->children->sibling->type==Char) || (t->children->type==Char&&t->children->sibling->type==Integer) || (t->children->type==Char&&t->children->sibling->type==Char)){
							break;
						}
						else{
							cerr <<"[line " <<t->lineno<<"]：Bad ASSIGN type." << endl;
							exit(1);
						}
						break;
					//+，加，两个操作数可以都为int，都为char，一个是int，一个是char
					case LOP_PLUS:
						//加运算
						if(t->children->sibling!=nullptr){
							if((t->children->type==Integer&&t->children->sibling->type==Integer) || (t->children->type==Integer&&t->children->sibling->type==Char) || (t->children->type==Char&&t->children->sibling->type==Integer) || (t->children->type==Char&&t->children->sibling->type==Char)){
								break;
							}
							else{
								cerr <<"[line " <<t->lineno<<"]：Bad \"+\" type." << endl;
								exit(1);
							}
							break;
						}
						//正数，+i
						else{
							if(t->children->type!=Integer){
								cerr <<"[line " <<t->lineno<<"]：Bad \"+\" type." << endl;
								exit(1);
							}
							break;
						}
						
					//-，减，两个操作数可以都为int，都为char，一个是int，一个是char
					case LOP_MINUS:
						//减运算
						if(t->children->sibling!=nullptr){
							if((t->children->type==Integer&&t->children->sibling->type==Integer) || (t->children->type==Integer&&t->children->sibling->type==Char) || (t->children->type==Char&&t->children->sibling->type==Integer) || (t->children->type==Char&&t->children->sibling->type==Char)){
								break;
							}
							else{
								cerr <<"[line " <<t->lineno<<"]：Bad \"-\" type." << endl;
								exit(1);
							}
							break;
						}
						//负数，-i
						else{
							if(t->children->type!=Integer){
								cerr <<"[line " <<t->lineno<<"]：Bad \"-\" type." << endl;
								exit(1);
							}
							break;
						}
						break;
					//*，乘，同上
					case LOP_MUL:
						if((t->children->type==Integer&&t->children->sibling->type==Integer) || (t->children->type==Integer&&t->children->sibling->type==Char) || (t->children->type==Char&&t->children->sibling->type==Integer) || (t->children->type==Char&&t->children->sibling->type==Char)){
							break;
						}
						else{
							cerr <<"[line " <<t->lineno<<"]：Bad \"*\" type." << endl;
							exit(1);
						}
						break;
					case LOP_DIV:
						if((t->children->type==Integer&&t->children->sibling->type==Integer) || (t->children->type==Integer&&t->children->sibling->type==Char) || (t->children->type==Char&&t->children->sibling->type==Integer) || (t->children->type==Char&&t->children->sibling->type==Char)){
							break;
						}
						else{
							cerr <<"[line " <<t->lineno<<"]：Bad \"/\" type." << endl;
							exit(1);
						}
						break;
					case LOP_MOD:
						if((t->children->type==Integer&&t->children->sibling->type==Integer) || (t->children->type==Integer&&t->children->sibling->type==Char) || (t->children->type==Char&&t->children->sibling->type==Integer) || (t->children->type==Char&&t->children->sibling->type==Char)){
							break;
						}
						else{
							cerr <<"[line " <<t->lineno<<"]：Bad \"%\" type." << endl;
							exit(1);
						}
						break;
					//++，其孩子节点需要为int或char
					case LOP_INC:
						if(t->children->type!=Integer && t->children!=Char){
							cerr <<"[line " <<t->lineno<<"]：Bad \"++\" type." << endl;
							exit(1);
						}
						break;
					//--
					case LOP_DEC:
						if(t->children->type!=Integer && t->children!=Char){
							cerr <<"[line " <<t->lineno<<"]：Bad \"--\" type." << endl;
							exit(1);
						}
						break;
					//&&，两个孩子都要是boolean
					case LOP_AND:
						if(t->children->type!=Boolean || t->children->sibling->type!=Boolean){
							cerr <<"[line " <<t->lineno<<"]：Bad \"&&\" type." << endl;
							exit(1);
						}
						break;
					// ||
					case LOP_OR:
						if(t->children->type!=Boolean || t->children->sibling->type!=Boolean){
							cerr <<"[line " <<t->lineno<<"]：Bad \"||\" type." << endl;
							exit(1);
						}
						break;
					// !
					case LOP_OPPSITE:
						if(t->children->type!=Boolean){
							cerr <<"[line " <<t->lineno<<"]：Bad \"!\" type." << endl;
							exit(1);
						}
						break;
					//<=，两个操作数可以都为int，都为char，一个是int，一个是char
					case LOP_LE:
						if((t->children->type==Integer&&t->children->sibling->type==Integer) || (t->children->type==Integer&&t->children->sibling->type==Char) || (t->children->type==Char&&t->children->sibling->type==Integer) || (t->children->type==Char&&t->children->sibling->type==Char)){
							break;
						}
						else{
							cerr <<"[line " <<t->lineno<<"]：Bad \"<=\" type." << endl;
							exit(1);
						}
						break;
					//>=，两个操作数可以都为int，都为char，一个是int，一个是char
					case LOP_GE:
						if((t->children->type==Integer&&t->children->sibling->type==Integer) || (t->children->type==Integer&&t->children->sibling->type==Char) || (t->children->type==Char&&t->children->sibling->type==Integer) || (t->children->type==Char&&t->children->sibling->type==Char)){
							break;
						}
						else{
							cerr <<"[line " <<t->lineno<<"]：Bad \">=\" type." << endl;
							exit(1);
						}
						break;
					//!=
					case LOP_NZ:
						if((t->children->type==Integer&&t->children->sibling->type==Integer) || (t->children->type==Integer&&t->children->sibling->type==Char) || (t->children->type==Char&&t->children->sibling->type==Integer) || (t->children->type==Char&&t->children->sibling->type==Char)){
							break;
						}
						else{
							cerr <<"[line " <<t->lineno<<"]：Bad \"!=\" type." << endl;
							exit(1);
						}
						break;
					//>
					case LOP_GT:
						if((t->children->type==Integer&&t->children->sibling->type==Integer) || (t->children->type==Integer&&t->children->sibling->type==Char) || (t->children->type==Char&&t->children->sibling->type==Integer) || (t->children->type==Char&&t->children->sibling->type==Char)){
							break;
						}
						else{
							cerr <<"[line " <<t->lineno<<"]：Bad \">\" type." << endl;
							exit(1);
						}
						break;
					//<
					case LOP_LT:
						if((t->children->type==Integer&&t->children->sibling->type==Integer) || (t->children->type==Integer&&t->children->sibling->type==Char) || (t->children->type==Char&&t->children->sibling->type==Integer) || (t->children->type==Char&&t->children->sibling->type==Char)){
							break;
						}
						else{
							cerr <<"[line " <<t->lineno<<"]：Bad \"<\" type." << endl;
							exit(1);
						}
						break;
					//==
					case LOP_EQ:
						if((t->children->type==Integer&&t->children->sibling->type==Integer) || (t->children->type==Integer&&t->children->sibling->type==Char) || (t->children->type==Char&&t->children->sibling->type==Integer) || (t->children->type==Char&&t->children->sibling->type==Char)){
							break;
						}
						else{
							cerr <<"[line " <<t->lineno<<"]：Bad \"==\" type." << endl;
							exit(1);
						}
						break;
				}
			}
			break;
		case NODE_TYPE:
			break;
		case NODE_PROG:
			break;
		//函数，返回值报错
		case NODE_FUNC:
			Node* temp=t->children;
			Node* re=nullptr;//用来存放return节点
			//遍历函数的孩子节点，找到statement对应的结点
			while(temp){
				//在statement节点的孩子节点中找到return语句
				if(temp->kind==NODE_STMT){
					Node* cur=temp->children;
					while(cur){
						if(cur->kind_kind==STMT_RETURN){
							//如果函数的返回值不等于函数的返回类型，报错
							if(cur->children->type!=t->type){
								cerr <<"[line " <<t->lineno<<"]：Bad return type." << endl;
								exit(1);
							}
						}
						cur=cur->sibling;
					}
				}
				temp=temp->sibling;
			}
			break;
		default:
			cerr <<"[line " <<t->lineno<<"]：Bad type." << endl;
			exit(1);
	}
	return;
}

void tree::get_temp_var(Node *t)
{
	if (t->kind != EXPR_NODE)
		return;
	if (t->attr.op < PLUS || t->attr.op > OVER)
		return;

	Node *arg1 = t->children[0];
	Node *arg2 = t->children[1];

	if (arg1->kind_kind == OP_EXPR)
		temp_var_seq--;
	if (arg2 && arg2->kind_kind == OP_EXPR)
		tree::temp_var_seq--;
	t->temp_var = tree::temp_var_seq;
	tree::temp_var_seq++;
}

Node* tree::NewRoot(int lineno,int kind, int kind_kind, NodeAttr attr, int type,
				   Node *child)
{
	Node *t = new Node;
	
	if (NULL == t)
		cerr << "Out of memory at line %d\n" << lineno;
	else
	{
		t->kind = kind;
		t->kind_kind = kind_kind;
		t->attr = attr;
		t->type = type;
		t->children = child;
		t->lineno = lineno;
		t->seq = tree::node_seq++;
		t->sibling = NULL;
		t->label.begin_label = "";
		t->label.next_label = "";
		t->label.true_label = "";
		t->label.false_label = "";
		root = t;
		type_check(t); // type check
		get_temp_var(t); // generate temp veriables
	}
	return t;
}

string tree::new_label(void)
{
	char tmp[20];

	sprintf(tmp, "@%d", tree::label_seq);
	tree::label_seq++;
	return tmp;
}

void tree::stmt_get_label(Node *t)
{
	switch (t->kind_kind)
	{
	case COMP_STMT:
		{
			Node *last;
			Node *p;
			for (p = t->children[0]; p->kind == DECL_NODE; p = p->sibling) ;

			p->label.begin_label = t->label.begin_label;
			for (; p; p = p->sibling)
			{
				last = p;
				recursive_get_label(p);
			}

			t->label.next_label = last->label.next_label;
			if (t->sibling)
				t->sibling->label.begin_label = t->label.next_label;
		}
		break;

	case WHILE_STMT:
		{
			Node *e = t->children[0];
			Node *s = t->children[1];

			if (t->label.begin_label == "")
				t->label.begin_label = new_label();
			s->label.next_label = t->label.begin_label;

			s->label.begin_label = e->label.true_label = new_label();

			if (t->label.next_label == "")
				t->label.next_label = new_label();
			e->label.false_label = t->label.next_label;
			if (t->sibling)
				t->sibling->label.begin_label = t->label.next_label;

			recursive_get_label(e);
			recursive_get_label(s);
		}
    /* ... */
	}
}

void tree::expr_get_label(Node *t)
{
	if (t->type != Boolean)
		return;

	Node *e1 = t->children[0];
	Node *e2 = t->children[1];
	switch (t->attr.op)
	{
	case AND:
		e1->label.true_label = new_label();
		e2->label.true_label = t->label.true_label;
		e1->label.false_label = e2->label.false_label = t->label.false_label;
		break;

	case OR:
		break;
    /* ... */
	}
	if (e1)
		recursive_get_label(e1);
	if (e2)
		recursive_get_label(e2);
}

void tree::recursive_get_label(Node *t)
{
	if (t->kind == STMT_NODE)
		stmt_get_label(t);
	else if (t->kind == EXPR_NODE)
		expr_get_label(t);
}

void tree::get_label(void)
{
	Node *p = root;

	p->label.begin_label = "_start";
	recursive_get_label(p);
}

void tree::gen_header(ostream &out)
{
	out << "# your asm code header here" << endl;
	/*your code here*/
}

void tree::gen_decl(ostream &out, Node *t)
{
    out << endl << "# define your veriables and temp veriables here" << endl;
	out << "\t.bss" << endl;
	for (; t->kind == DECL_NODE; t = t->sibling)
	{
		for (Node *p = t->children[1]; p; p = p->sibling)
			if (p->type == Integer)
				out << "_" << symtbl.getname(p->attr.symtbl_seq) << ":" << endl;
                out << "\t.zero\t4" << endl;
                out << "\t.align\t4" << endl;
	}
	
	for (int i = 0; i < temp_var_seq; i++)
	{
		out << "t" <<  i << ":" << endl;
        out << "\t.zero\t4" << endl;
        out << "\t.align\t4" << endl;
	}
}

void tree::stmt_gen_code(ostream &out, Node *t)
{
	if (t->kind_kind == COMP_STMT)
	{
		for (int i = 0; t->children[i]; i++)
		{
			recursive_gen_code(out, t->children[i]);
			for (Node *p = t->children[i]->sibling; p; p = p->sibling)
				recursive_gen_code(out, p);
		}
	}
	else if (t->kind_kind == WHILE_STMT)
	{
		if (t->label.begin_label != "")
			out << t->label.begin_label << ":" << endl;
		recursive_gen_code(out, t->children[0]);
		recursive_gen_code(out, t->children[1]);
		out << "\tjmp " << t->label.begin_label << endl;
	}
	else if (t->kind_kind == PRINT_STMT)
	{
		/* ... */
	}
    /* ... */
}

void tree::expr_gen_code(ostream &out, Node *t)
{
	Node *e1 = t->children[0];
	Node *e2 = t->children[1];
	switch (t->attr.op)
	{
	case ASSIGN:
		break;

	case PLUS:
		out << "\tmovl $";
		if (e1->kind_kind == ID_EXPR)
			out << "_" << symtbl.getname(e1->attr.symtbl_seq);
		else if (e1->kind_kind == CONST_EXPR)
			out << e1->attr.vali;
		else out << "t" << e1->temp_var;
		out << ", %eax" <<endl;
		out << "\taddl $";
		if (e2->kind_kind == ID_EXPR)
			out << "_" << symtbl.getname(e2->attr.symtbl_seq);
		else if (e2->kind_kind == CONST_EXPR)
			out << e2->attr.vali;
		else out << "t" << e2->temp_var;
		out << ", %eax" << endl;
		out << "\tmovl %eax, $t" << t->temp_var << endl;
		break;
    case AND:
        out << "\t# your own code of AND operation here" << endl;
        out << "\tjl @1" << endl;
        out << "\t# your asm code of AND operation end" << endl;
	case LT:
		break;
    /* ... */
	}
}

void tree::recursive_gen_code(ostream &out, Node *t)
{
	if (t->kind == STMT_NODE)
	{
		stmt_gen_code(out, t);
	}
	else if (t->kind == EXPR_NODE && t->kind_kind == OP_EXPR)
	{
		expr_gen_code(out, t);
	}
}

void tree::gen_code(ostream &out)
{
	gen_header(out);
	Node *p = root->children[0];
	if (p->kind == DECL_NODE)
		gen_decl(out, p);
    out << endl << endl << "# your asm code here" << endl;
	out << "\t.text" << endl;
    out << "\t.globl _start" << endl;
	recursive_gen_code(out, root);
	if (root->label.next_label != "")
		out << root->label.next_label << ":" << endl;
	out << "\tret" << endl;
}
