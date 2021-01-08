#include <iostream>
#include <string>

using namespace ::std;

#include "tree.h"
#include "symbol.h"

extern int lineno;
extern symbol_table symtbl;
extern int scope; //记录当前所处的作用域的标号
extern int symbolNum; //记录当前记录到第几个符号
extern stack<Node*> currentScope; //栈中记录当前作用域的变量

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
	this->label.begin_label = "";
	this->label.next_label = "";
	this->label.true_label = "";
	this->label.false_label = "";
	cout<<"建成节点，行号为："<<this->lineno<<"，类型为"<<this->kind<<endl;
}

Node::Node(){
	this->label.begin_label = "";
	this->label.next_label = "";
	this->label.true_label = "";
	this->label.false_label = "";
}

void tree::type_check(Node *t)
{
	// type check, write your own code here
	cout<<"检查到第"<<t->lineno<<"行,类型为"<<t->kind<<endl;
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
					if(t->children->sibling->type!= Boolean)
					{
						cerr <<"[line " <<t->lineno<<"]：Bad boolean type." << endl;
						exit(1);
					}
					break;
				case STMT_IF_ELSE:
					if(t->children->type!=Boolean)
					{
						cerr <<"[line " <<t->lineno<<"]：Bad boolean type." << endl;
						exit(1);
					}
					break;
				case STMT_IF:
					if(t->children->type!=Boolean)
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
				default:
					cerr <<"[line " <<t->lineno<<"]：Bad type." << endl;
					exit(1);
			}
			break;
		case NODE_EXPR:
			if(t->kind_kind==EXPR_OP)
			{	
				switch(t->attr.op)
				{	
					//+=，两个操作数都为int
					case OP_PLUS_ASSIGN:
						if(t->children->type==Integer&&t->children->sibling->type==Integer){
							break;
						}
						else{
							cerr <<"[line " <<t->lineno<<"]：Bad \"+=\" type." << endl;
							exit(1);
						}
						break;
					//-=，两个操作数可以都为int，都为char，一个是int，一个是char
					case OP_MINUS_ASSIGN:
						if(t->children->type==Integer&&t->children->sibling->type==Integer){
							break;
						}
						else{
							cerr <<"[line " <<t->lineno<<"]：Bad \"-=\" type." << endl;
							exit(1);
						}
						break;
					//=，赋值，两个操作数一致
					case OP_ASSIGN:
						if((t->children->type==t->children->sibling->type)){
							break;
						}
						else{
							cerr <<"[line " <<t->lineno<<"]：Bad ASSIGN type." << endl;
							exit(1);
						}
						break;
					//+，加，两个操作数可以都为int，都为char，一个是int，一个是char
					case OP_PLUS:
						//加运算
						if(t->children->sibling){
							if(t->children->type==Integer&&t->children->sibling->type==Integer){
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
					case OP_MINUS:
						//减运算
						if(t->children->sibling!=nullptr){
							if(t->children->type==Integer&&t->children->sibling->type==Integer){
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
					case OP_MUL:
						if(t->children->type==Integer&&t->children->sibling->type==Integer){
							break;
						}
						else{
							cerr <<"[line " <<t->lineno<<"]：Bad \"*\" type." << endl;
							exit(1);
						}
						break;
					case OP_DIV:
						if(t->children->type==Integer&&t->children->sibling->type==Integer){
							break;
						}
						else{
							cerr <<"[line " <<t->lineno<<"]：Bad \"/\" type." << endl;
							exit(1);
						}
						break;
					case OP_MOD:
						if(t->children->type==Integer&&t->children->sibling->type==Integer){
							break;
						}
						else{
							cerr <<"[line " <<t->lineno<<"]：Bad \"%\" type." << endl;
							exit(1);
						}
						break;
					//++，其孩子节点需要为int或char
					case OP_INC:
						if(t->children->type!=Integer){
							cerr <<"[line " <<t->lineno<<"]：Bad \"++\" type." << endl;
							exit(1);
						}
						break;
					//--
					case OP_DEC:
						if(t->children->type!=Integer){
							cerr <<"[line " <<t->lineno<<"]：Bad \"--\" type." << endl;
							exit(1);
						}
						break;
					//&&，两个孩子都要是boolean
					case OP_AND:
						if(t->children->type!=Boolean || t->children->sibling->type!=Boolean){
							cerr <<"[line " <<t->lineno<<"]：Bad \"&&\" type." << endl;
							exit(1);
						}
						break;
					// ||
					case OP_OR:
						if(t->children->type!=Boolean || t->children->sibling->type!=Boolean){
							cerr <<"[line " <<t->lineno<<"]：Bad \"||\" type." << endl;
							exit(1);
						}
						break;
					// !
					case OP_OPPSITE:
						if(t->children->type!=Boolean){
							cerr <<"[line " <<t->lineno<<"]：Bad \"!\" type." << endl;
							exit(1);
						}
						break;
					//<=，两个操作数可以都为int
					case OP_LE:
						if(t->children->type==Integer&&t->children->sibling->type==Integer){
							break;
						}
						else{
							cerr <<"[line " <<t->lineno<<"]：Bad \"<=\" type." << endl;
							exit(1);
						}
						break;
					//>=，两个操作数可以都为int
					case OP_GE:
						if(t->children->type==Integer&&t->children->sibling->type==Integer){
							break;
						}
						else{
							cerr <<"[line " <<t->lineno<<"]：Bad \">=\" type." << endl;
							exit(1);
						}
						break;
					//!=
					case OP_NZ:
						if(t->children->type==t->children->sibling->type==Integer){
							break;
						}
						else{
							cerr <<"[line " <<t->lineno<<"]：Bad \"!=\" type." << endl;
							exit(1);
						}
						break;
					//>
					case OP_GT:
						if(t->children->type==Integer&&t->children->sibling->type==Integer){
							break;
						}
						else{
							cerr <<"[line " <<t->lineno<<"]：Bad \">\" type." << endl;
							exit(1);
						}
						break;
					//<
					case OP_LT:
						if(t->children->type==Integer&&t->children->sibling->type==Integer){
							break;
						}
						else{
							cerr <<"[line " <<t->lineno<<"]：Bad \"<\" type." << endl;
							exit(1);
						}
						break;
					//==
					case OP_EQ:
						if(t->children->type==t->children->sibling->type){
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
			//遍历函数的孩子节点，找到statement对应的结点
			while(temp){
				//在statement节点的孩子节点中找到return语句
				if(temp->kind==NODE_STMT){
					Node* cur=temp;
					while(cur){
						if(cur->kind_kind==STMT_RETURN){
							//如果函数的返回值不等于函数的返回类型，报错
							if(cur->children->type!=t->type){
								cerr <<"[line " <<t->lineno<<"]：Bad return type." << endl;
								exit(1);
							}
							else{
								return;
							}
						}
						cur=cur->sibling;
					}
				}
				temp=temp->sibling;
			}
			break;
	}
	return;
}

void tree::get_temp_var(Node *t)
{
	if (t->kind == NODE_EXPR && t->kind_kind==EXPR_OP){
		if (t->attr.op < OP_PLUS || t->attr.op > OP_MINUS_ASSIGN)
			return;
	} 
	else if(t->kind!=NODE_VAR){
		return;
	}
		
	if (t->kind == NODE_EXPR && t->kind_kind==EXPR_OP){
		Node *arg1 = t->children;
		Node *arg2 = t->children->sibling;

		if (arg1->kind==NODE_EXPR && arg1->kind_kind == EXPR_OP){
			tree::temp_var_seq--;
		}	
		if (arg2 && arg2->kind==NODE_EXPR && arg2->kind_kind == EXPR_OP){
			tree::temp_var_seq--;
		}
		t->children->temp_var = tree::temp_var_seq;
		tree::temp_var_seq++;
	}
	else if(t->kind==NODE_VAR){
		t->temp_var=tree::temp_var_seq;
		tree::temp_var_seq++;
	}
}

Node* tree::NewRoot(int lineno,NodeType kind, int kind_kind, NodeAttr attr, int type,
				   Node *child)
{
	Node *t = new Node();
	
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
	sprintf(tmp, ".L%d", tree::label_seq);
	tree::label_seq++;
	return tmp;
}

void tree::stmt_get_label(Node *t)
{
	switch (t->kind_kind)
	{
	case STMT_WHILE:
		{
			Node *e = t->children;//循环条件
			Node *s = t->children->sibling;//循环体

			if (t->label.begin_label == "")
				t->label.begin_label = new_label();
			//循环体的下一条语句————循环的开始（继续循环）
			s->label.next_label = t->label.begin_label;
			//循环体的开始标号即为循环条件的真值的标号
			s->label.begin_label = e->label.true_label = new_label();
			//循环结束的标号
			if (t->label.next_label == "")
				t->label.next_label = new_label();
			//循环条件的假值标号即为循环的下一条语句标号
			e->label.false_label = t->label.next_label;
			//兄弟节点的开始标号即为循环的下一条语句标号
			if (t->sibling)
				t->sibling->label.begin_label = t->label.next_label;

			//递归生成
			recursive_get_label(e);
			recursive_get_label(s);
		}
		break;
	case STMT_FOR:
		{
			Node* e1=t->children;//初次执行
			Node* e2=e1->sibling;//循环条件
			Node* e3=e2->sibling;//除了第一次，每次都执行
			Node* s=e3->sibling;//循环体
			if (t->label.begin_label == "")
				t->label.begin_label = new_label();
			//循环体的下一条语句————从循环条件开始
			s->label.next_label = e2->label.begin_label;
			//循环体的开始标号即为循环条件的真值的标号
			s->label.begin_label = e2->label.true_label = new_label();
			//循环结束的标号
			if (t->label.next_label == "")
				t->label.next_label = new_label();
			//循环条件的假值标号即为循环的下一条语句标号
			e2->label.false_label = t->label.next_label;
			//兄弟节点的开始标号即为循环的下一条语句标号
			if (t->sibling)
				t->sibling->label.begin_label = t->label.next_label;
			//递归生成
			recursive_get_label(e1);
			recursive_get_label(e2);
			recursive_get_label(e3);
			recursive_get_label(s);
		}
		break;
	case STMT_IF:
		{
			Node* e=t->children;
			Node* s=t->children->sibling;
			if (t->label.begin_label == "")
				t->label.begin_label = new_label();
			s->label.begin_label=e->label.true_label = new_label();
			if (t->label.next_label == "")
				t->label.next_label = new_label();
			s->label.next_label=t->label.next_label;
			e->label.false_label = t->label.next_label;
			if (t->sibling)
				t->sibling->label.begin_label = t->label.next_label;
			recursive_get_label(e);
			recursive_get_label(s);
		}
		break;
	case STMT_IF_ELSE:
		{
			Node* e=t->children;
			Node* s1=t->children->sibling;
			Node* s2=s1->sibling;
			if (t->label.begin_label == "")
				t->label.begin_label = new_label();
			s1->label.begin_label=e->label.true_label = new_label();
			s2->label.begin_label=e->label.false_label = new_label();
			if (t->label.next_label == "")
				t->label.next_label = new_label();
			s1->label.next_label=s2->label.next_label=t->label.next_label;
			e->label.false_label=t->label.next_label;
			if (t->sibling)
				t->sibling->label.begin_label = t->label.next_label;
			recursive_get_label(e);
			recursive_get_label(s1);
			recursive_get_label(s2);
		}
		break;
	default:
		break;
	}
}

void tree::expr_get_label(Node *t)
{
	if (t->type != Boolean)
		return;

	Node *e1 = t->children;
	Node *e2 = t->children->sibling;
	switch (t->attr.op)
	{
	case OP_AND:
		e1->label.true_label = new_label();
		e2->label.true_label = t->label.true_label;
		e1->label.false_label = e2->label.false_label = t->label.false_label;
		break;
	case OP_OR:
		e1->label.true_label = e2->label.true_label = t->label.true_label;
        e1->label.false_label = new_label();
        e2->label.false_label = t->label.false_label;
		break;
	case OP_OPPSITE:
		e1->label.true_label = t->label.false_label;
        e1->label.false_label = t->label.true_label;
		break;
	default:
		break;
	}
	if (e1)
		recursive_get_label(e1);
	if (e2)
		recursive_get_label(e2);
}

void tree::func_get_label(Node *t)
{
	t->label.begin_label=t->children->sibling->attr.var_name;
	t->label.next_label=".LRET_"+t->children->sibling->attr.var_name;
}

void tree::recursive_get_label(Node *t)
{
	if (t->kind == NODE_STMT)
		stmt_get_label(t);
	else if (t->kind == NODE_EXPR)
		expr_get_label(t);
	else if(t->kind == NODE_FUNC)
		func_get_label(t);
}

void tree::get_label(void)
{
	Node *p = root;
	p->label.begin_label = "_start";
	recursive_get_label(p);
}

void tree::gen_header(ostream &out)
{
	out << "# Generated by ZhuYun's Simple Compiler." << endl;
}

void tree::gen_decl(ostream &out, Node *t)
{ 
	//声明全局变量
	out << endl << "# define your veriables and temp veriables here" << endl;
	out << "\t.bss" << endl;
	
	Node* p=t->children;
	while(p){
		if (p->kind==NODE_STMT && p->kind_kind == STMT_DECL){
			out << "\t.align\t4" << endl;
			for (Node*q=p->children->sibling; q; q = q->sibling){
				if(q->kind==NODE_VAR){
					out << "_" << symtbl.getname(q->pos) << ":" << endl;
				}
				else{
					if(q->children->kind==NODE_VAR)
						out << "_" << symtbl.getname(q->children->pos) << ":" << endl;
				}
			}
			out << "\t.zero\t4" << endl;
		}
		p=p->sibling;
	}

	//声明临时变量       
	for (int i = 0; i < temp_var_seq; i++)
	{
		out << "\t.align\t4" << endl;
		out << "t" <<  i << ":" << endl;
        out << "\t.zero\t4" << endl;
	}
}

void tree::stmt_gen_code(ostream &out, Node *t)
{
	switch(t->kind_kind){
		case STMT_WHILE:
			if (t->label.begin_label != "")
			out << t->label.begin_label << ":" << endl;
			recursive_gen_code(out, t->children);
			recursive_gen_code(out, t->children->sibling);
			out << "\tjmp " << t->label.begin_label << endl;
			break;
		case STMT_FOR:
			break;
		case STMT_IF:
			break;
		case STMT_IF_ELSE:
			break;
		case STMT_PRINTF:
			break;
		case STMT_SCANF:
			break;
		case STMT_RETURN:
			break;
		default:
			break;	
	}
}

void tree::expr_gen_code(ostream &out, Node *t)
{
	Node *e1 = t->children;
	Node *e2 = t->children->sibling;
	switch (t->attr.op)
	{
	case OP_ASSIGN:
		break;

	case OP_PLUS:
		out << "\tmovl $";
		// if (e1->kind_kind == VAR_COMMON)
		// 	out << "_" << symtbl.getname(e1->attr.symtbl_seq);
		// else if (e1->kind_kind == CONST_EXPR)
		// 	out << e1->attr.vali;
		// else out << "t" << e1->temp_var;
		// out << ", %eax" <<endl;
		// out << "\taddl $";
		// if (e2->kind_kind == ID_EXPR)
		// 	out << "_" << symtbl.getname(e2->attr.symtbl_seq);
		// else if (e2->kind_kind == CONST_EXPR)
		// 	out << e2->attr.vali;
		// else out << "t" << e2->temp_var;
		// out << ", %eax" << endl;
		// out << "\tmovl %eax, $t" << t->temp_var << endl;
		break;
    case OP_AND:
        out << "\t# your own code of AND operation here" << endl;
        out << "\tjl @1" << endl;
        out << "\t# your asm code of AND operation end" << endl;
	case OP_LT:
		break;
	default:
		break;
    /* ... */
	}
}

void tree::recursive_gen_code(ostream &out, Node *t)
{
	if (t->kind == NODE_STMT)
	{
		stmt_gen_code(out, t);
	}
	else if (t->kind == NODE_EXPR && t->kind_kind == EXPR_OP)
	{
		expr_gen_code(out, t);
	}
	else if(t->kind == NODE_FUNC)
	{
		;
	}
}

//生成str
void tree::gen_str(ostream &out,Node* p)
{	
	static bool print_rodata = false;
	static int str_seq = 0;
	p=p->children;
	while(p){
		if(p->kind==NODE_CONST && p->type==String){
			if (!print_rodata) {
                print_rodata = true;
				out<<endl;
                out << "\t.section\t.rodata" << endl;
            }
			//还需要将str加入一个表
			out<<"STR"<<str_seq<<":"<<endl;
			out<<"\t.string\t\""<<p->attr.vals<<"\""<<endl;
			str_seq++;
		}
		else if(p->children){
			gen_str(out,p);
		}
		p=p->sibling;
	}
}

void tree::gen_code(ostream &out,Node* p)
{
	if(p==nullptr){
		return;
	}
	Node* q=p->children;
	string var="";
	switch(p->kind){
		case NODE_PROG:
			gen_header(out);
			//声明全局和临时变量
			gen_decl(out,p);
			//声明string类型
			gen_str(out,p);
			out<<endl;
			out << "\t.text" << endl;
			while(q){
				if(q->kind==NODE_STMT || q->kind==NODE_FUNC){
					gen_code(out,q);
				}
				q=q->sibling;
			}
			out<<".section"<<endl;
			break;
		case NODE_CONST:
			switch(p->type){
				case Notype:
					break;
				case Integer:
					break;
				case Boolean:
					break;
				case Char:
					break;
				case String:
					break;
				default:
					break;
			}
			break;
		case NODE_VAR:
			break;
		case NODE_EXPR:
			switch(p->attr.op){
				//+=
				case OP_PLUS_ASSIGN:
					out << "\tmovl\t" << get_var(p->children) << ", %ebx" << endl;
                 	out << "\taddl\t%ebx, %eax" << endl;
                 	out << "\tmovl\t%eax, " << get_var(p->children) << endl;
					gen_code(out,p->sibling);
					break;
				//-=
				case OP_MINUS_ASSIGN:
					out << "\tmovl\t" << get_var(p->children) << ", %ebx" << endl;
                 	out << "\tsubl\t%eax, %ebx" << endl;
                 	out << "\tmovl\t%ebx, %eax" << endl;
                 	out << "\tmovl\t%eax, " << get_var(p->children) << endl;
					gen_code(out,p->sibling);
					break;
				//=
				case OP_ASSIGN:
					gen_code(out,p->children->sibling);
					out << "\tmovl\t" << get_var(p->children->sibling) << ", %eax" << endl;
					out << "\tmovl\t%eax, " << get_var(p->children) << endl;
					gen_code(out,p->sibling);
					break;
				//+
				case OP_PLUS:
					gen_code(out,p->children);
					gen_code(out,p->children->sibling);
					out << "\tmovl\t" << get_var(p->children) << ", %eax" << endl;
					out<<"\tmovl\t%eax,%ebx"<<endl;
					out << "\tmovl\t" << get_var(p->children->sibling) << ", %eax" << endl;
					out<<"\taddl\t%ebx,%eax"<<endl;
					out<<"\tmovl\t%eax,"<<get_var(p)<<endl;
					break;
				//-
				case OP_MINUS:
					gen_code(out,p->children);
					gen_code(out,p->children->sibling);
					out << "\tmovl\t" << get_var(p->children) << ", %eax" << endl;
					out<<"\tmovl\t%eax,%ebx"<<endl;
					out << "\tmovl\t" << get_var(p->children->sibling) << ", %eax" << endl;
					out<<"\tsubl\t%eax,%ebx"<<endl;
					out << "\tmovl\t%ebx, %eax" << endl;
					out<<"\tmovl\t%eax,"<<get_var(p)<<endl;
				//*
				case OP_MUL:
					gen_code(out,p->children);
					gen_code(out,p->children->sibling);
					out << "\tmovl\t" << get_var(p->children) << ", %eax" << endl;
					out<<"\tmovl\t%eax,%ebx"<<endl;
					out << "\tmovl\t" << get_var(p->children->sibling) << ", %eax" << endl;
					out<<"\timull\t%ebx,%eax"<<endl;
					out<<"\tmovl\t%eax,"<<get_var(p)<<endl;
					break;
				// /
				case OP_DIV:
					gen_code(out,p->children);
					gen_code(out,p->children->sibling);
					out << "\tmovl\t" << get_var(p->children) << ", %eax" << endl;
					out << "\tmovl\t" << get_var(p->children->sibling) << ", %ebx" << endl;
					out<<"\tidivl\t%ebx"<<endl;
					out<<"\tmovl\t%eax,"<<get_var(p)<<endl;
					break;
				// %
				case OP_MOD:
					gen_code(out,p->children);
					gen_code(out,p->children->sibling);
					out << "\tmovl\t" << get_var(p->children) << ", %eax" << endl;
					out << "\tmovl\t" << get_var(p->children->sibling) << ", %ebx" << endl;
					out<<"\tidivl\t%ebx"<<endl;
					out<<"\tmovl\t%edx,"<<get_var(p)<<endl;
					break;
				//++
				case OP_INC:
					gen_code(out,p->children);
					out << "\tmovl\t" << get_var(p->children) << ", %eax" << endl;
					out<<"\tmovl\t%eax,%ebx"<<endl;
					out << "\tmovl\t$1" << ", %eax" << endl;
					out<<"\taddl\t%ebx,%eax"<<endl;
					out << "\tmovl\t%eax, " << get_var(p->children) << endl;
					gen_code(out,p->sibling);
					break;
				//--
				case OP_DEC:
					gen_code(out,p->children);
					out << "\tmovl\t" << get_var(p->children) << ", %eax" << endl;
					out<<"\tmovl\t%eax,%ebx"<<endl;
					out << "\tmovl\t$1" << ", %eax" << endl;
					out<<"\tsubl\t%eax,%ebx"<<endl;
					out << "\tmovl\t%ebx, %eax" << endl;
					out << "\tmovl\t%eax, " << get_var(p->children) << endl;
					gen_code(out,p->sibling);
					break;
				//&&
				case OP_AND:
					gen_code(out,p->children);
					gen_code(out,p->children->sibling);
					out << "\tmovl\t" << get_var(p->children) << ", %eax" << endl;
					out<<"\tmovl\t%eax,%ebx"<<endl;
					out << "\tmovl\t" << get_var(p->children->sibling) << ", %eax" << endl;
					out<<"\tandl\t%ebx,%eax"<<endl;
					out<<"\tmovl\t%eax,"<<get_var(p)<<endl;
					gen_code(out,p->sibling);
					break;
				//||
				case OP_OR:
					gen_code(out,p->children);
					gen_code(out,p->children->sibling);
					out << "\tmovl\t" << get_var(p->children) << ", %eax" << endl;
					out<<"\tmovl\t%eax,%ebx"<<endl;
					out << "\tmovl\t" << get_var(p->children->sibling) << ", %eax" << endl;
					out<<"\torl\t%ebx,%eax"<<endl;
					out<<"\tmovl\t%eax,"<<get_var(p)<<endl;
					gen_code(out,p->sibling);
					break;
				//!
				case OP_OPPSITE:
					gen_code(out,p->children);
					out << "\tmovl\t" << get_var(p->children) << ", %eax" << endl;
					out<<"\tmovl\t%eax,%ebx"<<endl;
					out << "\tmovl\t" << get_var(p->children->sibling) << ", %eax" << endl;
					out<<"\tandl\t%ebx,%eax"<<endl;
					out<<"\tmovl\t%eax,"<<get_var(p)<<endl;
					gen_code(out,p->sibling);
					break;
				//<=
				case OP_LE:
					gen_code(out,p->children);
					gen_code(out,p->children->sibling);
					out << "\tmovl\t" << get_var(p->children) << ", %eax" << endl;
					out<<"\tmovl\t%eax,%ebx"<<endl;
					out << "\tmovl\t" << get_var(p->children->sibling) << ", %eax" << endl;
					out<<"\tcmpl\t%ebx,%eax"<<endl;
					out<<"\tmovl\t%eax,"<<get_var(p)<<endl;
					gen_code(out,p->sibling);
					break;
				//>=
				case OP_GE:
					break;
				//!=
				case OP_NZ:
					break;
				//>
				case OP_GT:
					break;
				//<
				case OP_LT:
					break;
				//==
				case OP_EQ:
					break;
				default:
					break;
			}
			break;
		case NODE_TYPE:
			break;
		case NODE_STMT:
			break;
		case NODE_FUNC:
			cout<<endl;
			//声明函数
			out<<"\t.globl\t"<<q->sibling->attr.var_name<<endl;
			out<< "\t.type\t" << q->sibling->attr.var_name << ", @function" << endl;
            out<< q->sibling->attr.var_name << ":" << endl;
			//递归生成函数内部代码
			gen_code(out,q->sibling->sibling);
			out << "\tret" << endl;
			break;
	}
	return;
}

string tree::get_var(Node* p){
	string result="";
	if(p->kind==NODE_VAR || (p->kind==NODE_EXPR && p->kind_kind==EXPR_OP)){
		//全局变量，直接返回名字
		if(p->global_val==1){
			return p->attr.var_name;
		}
		//返回其临时变量名
		else{
			result="t"+to_string(p->temp_var);
			return result;
		}
	}
	//数字
	else if(p->kind==NODE_CONST && p->type==Integer){
		result="$"+to_string(p->attr.vali);
		return result;
	}
	else if(p->kind==NODE_CONST && p->type==Boolean){
		result="$"+to_string(p->attr.vali);
		return result;
	}
}