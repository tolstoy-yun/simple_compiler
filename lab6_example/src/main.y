%{
    #include "common.h"
    #define YYSTYPE Node *  
    extern int lineno;
    extern ofstream fout;
    extern symbol_table symtbl;
    extern tree parse_tree;
    extern int scope; //记录当前所处的作用域域的标号
    extern int symbolNum; //记录当前记录到第几个符号
    extern stack<Node*> currentScope; //栈中记录当前作用域的变量
    int yylex();
    int yyerror( char const * );
%}
%token T_CHAR T_INT T_STRING T_BOOL T_VOID IDPointer IDQuote

%token S_FOR S_IF S_WHILE S_ELSE S_SKIP S_RETURN

%token P_PRINTF P_SCANF

%token LOP_ASSIGN LOP_PLUS LOP_MINUS LOP_MUL LOP_DIV LOP_MOD LOP_INC
%token LOP_DEC LOP_PLUS_ASSIGN LOP_MINUS_ASSIGN LOP_AND LOP_OR LOP_OPPSITE
%token LOP_LE LOP_GE LOP_NZ LOP_GT LOP_LT LOP_EQ

%token SEMICOLON COMMA LPAREN RPAREN LBRACE RBRACE QUOTE

%token IDENTIFIER INTEGER CHAR BOOL STRING

%left COMMA
%right LOP_ASSIGN LOP_PLUS_ASSIGN LOP_MINUS_ASSIGN
%left LOP_OR
%left LOP_AND
%left LOP_EQ LOP_NZ    
%left LOP_LE LOP_GE LOP_LT LOP_GT
%left LOP_PLUS LOP_MINUS
%left LOP_MUL LOP_DIV LOP_MOD
%right LOP_INC LOP_DEC LOP_OPPSITE

%nonassoc LOWER_THAN_ELSE
%nonassoc S_ELSE
%%

program
: statements {
    NodeAttr attr=NodeAttr();
    parse_tree.root =parse_tree.NewRoot(0, NODE_PROG,-1,attr,Notype,$1); 
    parse_tree.type_check(parse_tree.root);
};

statements
:  statement {$$=$1;}
|  statements statement {$$=$1; $$->addSibling($2);}
;

statement
: SEMICOLON  {
    NodeAttr attr=NodeAttr(); 
    Node* node = new Node(lineno, NODE_STMT,STMT_SKIP,attr,Notype); 
    node->seq=parse_tree.node_seq++;
    parse_tree.type_check(node);
    $$=node;
}
| declaration SEMICOLON {$$ = $1;}
| LBRACE statements RBRACE {$$=$2;}
| if_stmt {$$=$1;}
| while_stmt {$$=$1;}
| for_stmt {$$=$1;}
| printf SEMICOLON {$$=$1;}
| scanf SEMICOLON {$$=$1;}
| expr SEMICOLON {$$=$1;}
| func {$$=$1;}
| return_stmt {$$ = $1;}
;

return_stmt
: S_RETURN expr SEMICOLON{
    NodeAttr attr=NodeAttr();
    Node* node = new Node($2->lineno, NODE_STMT,STMT_RETURN,attr,$2->type);
    node->seq=parse_tree.node_seq++;
    node->addChild($2);
    parse_tree.type_check(node);
    $$ = node;
}
;

declaration
: T IDLIST {
    NodeAttr attr=NodeAttr();
    Node* node = new Node($1->lineno, NODE_STMT,STMT_DECL,attr,$1->type);
    node->seq=parse_tree.node_seq++;
    //如果$2的类型是var
    //将$2压入currentScope栈，并分配一个符号（=symbolNum）
    if($2->kind==NODE_VAR){
        //疑似重定义位为1，说明存在重定义现象，报错，退出
        if($2->suspected_redefine==1){
            cerr << "[line " <<$2->lineno<<"]："<<$2->attr.var_name <<" has been declared before."<< endl;
            exit(1);
        }
        // 疑似重定义位为0，说明不存在重定义
        // 将其加入符号表，并将$2在符号表里的type设为与T一致
        $2->pos=symtbl.insert($2->attr.var_name,VAR_COMMON);
        $2->type=$1->type;
        symtbl.set_type($2->pos,$1->type);
        $2->number=symbolNum;
        symbolNum++;
        currentScope.push($2);
         //将$2的兄弟结点也设置type
         Node* temp=$2->sibling;
         while(temp){
            if(temp->suspected_redefine==1){
                cerr << "[line " <<temp->lineno<<"]："<<temp->attr.var_name <<" has been declared before."<< endl;
                exit(1);
            }
            temp->pos=symtbl.insert(temp->attr.var_name,VAR_COMMON);
            temp->type=$1->type;
            symtbl.set_type(temp->pos,$1->type);
            temp->number=symbolNum;
            currentScope.push(temp);
            symbolNum++;
         }
    }
    //$2的类型是expr，对应T IDENTIFIER=n,……;的情况
    //遍历$2及其兄弟结点，将它们的第一个孩子结点压栈并分配符号，设置其类型
    else{
        Node* t=nullptr;
        t=$2;
        while(t){
            Node* cur=t->children;
            if(cur->suspected_redefine==1){
                cerr << "[line " <<cur->lineno<<"]："<<cur->attr.var_name <<" has been declared before."<< endl;
                exit(1);
            }
            cur->pos=symtbl.insert(cur->attr.var_name,VAR_COMMON);
            cur->type=$1->type;
            symtbl.set_type(cur->pos,$1->type);
            cur->number=symbolNum;
            symbolNum++;
            currentScope.push(cur);
            t=t->sibling;
        }
    }
    node->addChild($1);
    node->addChild($2);
    parse_tree.type_check(node);
    $$ = node;
}
;

func: T IDENTIFIER LPAREN specialID RPAREN statement{
    NodeAttr attr=NodeAttr();
    Node* node=new Node($2->lineno,NODE_FUNC,-1,attr,$1->type);
    node->seq=parse_tree.node_seq++;
    //如果没有重定义，则放入符号表
    if($2->suspected_redefine==1){
        cerr << "[line " <<$2->lineno<<"]："<<$2->attr.var_name <<" has been declared before."<< endl;
        exit(1);
    }
    $2->pos=symtbl.insert($2->attr.var_name,VAR_COMMON);
    $2->type=$1->type;
    symtbl.set_type($2->pos,$1->type);

    //变量遇到T开头的定义，压入currentScope栈，并分配一个符号（=symbolNum）
    $2->number=symbolNum;
    symbolNum++;
    currentScope.push($2);

    node->addChild($1);
    node->addChild($2);
    node->addChild($4);
    node->addChild($6);
    parse_tree.type_check(node);
    $$=node;
}
| T IDENTIFIER LPAREN RPAREN statement{
    NodeAttr attr=NodeAttr();
    Node* node=new Node($2->lineno,NODE_FUNC,-1,attr,$1->type);
    node->seq=parse_tree.node_seq++;
    if($2->suspected_redefine==1){
        cerr << "[line " <<$2->lineno<<"]："<<$2->attr.var_name <<" has been declared before."<< endl;
        exit(1);
    }
    $2->pos=symtbl.insert($2->attr.var_name,VAR_COMMON);
    $2->type=$1->type;
    symtbl.set_type($2->pos,$1->type);

    //变量遇到T开头的定义，压入currentScope栈，并分配一个符号（=symbolNum）
    $2->number=symbolNum;
    symbolNum++;
    currentScope.push($2);

    node->addChild($1);
    node->addChild($2);
    node->addChild($5);
    parse_tree.type_check(node);
    $$=node;
}
;

T: T_INT {
    NodeAttr attr=NodeAttr(); 
    Node* node = new Node(lineno, NODE_TYPE,TYPE_INT,attr,Integer); 
    node->seq=parse_tree.node_seq++;
    parse_tree.type_check(node);
    $$=node;
}
| T_CHAR {
    NodeAttr attr=NodeAttr(); 
    Node* node = new Node(lineno, NODE_TYPE,TYPE_CHAR,attr,Char); 
    node->seq=parse_tree.node_seq++;
    parse_tree.type_check(node);
    $$=node;
}
| T_BOOL {
    NodeAttr attr=NodeAttr(); 
    Node* node = new Node(lineno, NODE_TYPE,TYPE_BOOL,attr,Boolean); 
    node->seq=parse_tree.node_seq++;
    parse_tree.type_check(node);
    $$=node;
}
| T_VOID {
    NodeAttr attr=NodeAttr(); 
    Node* node = new Node(lineno, NODE_TYPE,FUNC_VOID,attr,Notype); 
    node->seq=parse_tree.node_seq++;
    parse_tree.type_check(node);
    $$=node;
}
| T_STRING {
    NodeAttr attr=NodeAttr(); 
    Node* node = new Node(lineno, NODE_TYPE,TYPE_STRING,attr,String); 
    node->seq=parse_tree.node_seq++;
    parse_tree.type_check(node);
    $$=node;
}
;

IDLIST: IDLIST COMMA expr {$$=$1; $$->addSibling($3);}
| expr {$$=$1;}
;

for_stmt: S_FOR LPAREN expr SEMICOLON expr SEMICOLON expr RPAREN statement {
    NodeAttr attr=NodeAttr();
    Node* node=new Node($1->lineno,NODE_STMT,STMT_FOR,attr,Notype);
    node->seq=parse_tree.node_seq++;
    node->addChild($3);
    node->addChild($5);
    node->addChild($7);
    node->addChild($9);
    parse_tree.type_check(node);
    $$=node;
}
| S_FOR LPAREN declaration SEMICOLON expr SEMICOLON expr RPAREN statement {
    NodeAttr attr=NodeAttr();
    Node* node=new Node($1->lineno,NODE_STMT,STMT_FOR,attr,Notype);
    node->seq=parse_tree.node_seq++;
    node->addChild($3);
    node->addChild($5);
    node->addChild($7);
    node->addChild($9);
    parse_tree.type_check(node);
    $$=node;
}
;

while_stmt: S_WHILE LPAREN expr RPAREN statement {
    NodeAttr attr=NodeAttr();
    Node* node=new Node($1->lineno,NODE_STMT,STMT_WHILE,attr,Notype);
    node->seq=parse_tree.node_seq++;
    node->addChild($3);
    node->addChild($5);
    parse_tree.type_check(node);
    $$=node;
}
;

if_stmt: S_IF LPAREN expr RPAREN statement %prec LOWER_THAN_ELSE{
    NodeAttr attr=NodeAttr();
    Node* node=new Node($3->lineno,NODE_STMT,STMT_IF_ELSE,attr,Notype);
    node->seq=parse_tree.node_seq++;
    node->addChild($3);
    node->addChild($5);
    parse_tree.type_check(node);
    $$=node;
}
| S_IF LPAREN expr RPAREN statement S_ELSE statement{
    NodeAttr attr=NodeAttr();
    Node* node=new Node($3->lineno,NODE_STMT,STMT_IF_ELSE,attr,Notype);
    node->seq=parse_tree.node_seq++;
    node->addChild($3);
    node->addChild($5);
    node->addChild($7);
    parse_tree.type_check(node);
    $$=node;
}


printf: P_PRINTF LPAREN STRING RPAREN {
    NodeAttr attr=NodeAttr();
    Node* node=new Node($1->lineno,NODE_STMT,STMT_PRINTF,attr,Notype);
    node->seq=parse_tree.node_seq++;
    node->addChild($3);
    parse_tree.type_check(node);
    $$=node;
}
| P_PRINTF LPAREN STRING COMMA specialID RPAREN {
    NodeAttr attr=NodeAttr();
    Node* node=new Node($1->lineno,NODE_STMT,STMT_PRINTF,attr,Notype);
    node->seq=parse_tree.node_seq++;
    node->addChild($3);
    node->addChild($5);
    parse_tree.type_check(node);
    $$=node;
}
;

scanf: P_SCANF LPAREN STRING COMMA specialID RPAREN{
    NodeAttr attr=NodeAttr();
    Node* node=new Node($1->lineno,NODE_STMT,STMT_SCANF,attr,Notype);
    node->seq=parse_tree.node_seq++;
    node->addChild($3);
    node->addChild($5);
    parse_tree.type_check(node);
    $$=node;
}
;

specialID: IDENTIFIER{$$=$1;}
| QUOTE IDENTIFIER{$$=$2;}
| specialID COMMA IDENTIFIER{$$=$1;$$->addSibling($3);}
| specialID COMMA QUOTE IDENTIFIER{$$=$1;$$->addSibling($4);}
;

expr
: IDENTIFIER {
    $$ = $1;
}
| INTEGER {
    $$ = $1;
}
| CHAR {
    $$ = $1;
}
| STRING {
    $$ = $1;
}
| expr LOP_PLUS_ASSIGN expr{
    NodeAttr attr=NodeAttr(OP_PLUS_ASSIGN);
    Node* node=new Node($1->lineno,NODE_EXPR,EXPR_OP,attr,Notype);
    node->seq=parse_tree.node_seq++;
    node->addChild($1);
    node->addChild($3);
    parse_tree.type_check(node);
    $$=node;
}
| expr LOP_MINUS_ASSIGN expr{
    NodeAttr attr=NodeAttr(OP_MINUS_ASSIGN);
    Node* node=new Node($1->lineno,NODE_EXPR,EXPR_OP,attr,Notype);
    node->seq=parse_tree.node_seq++;
    node->addChild($1);
    node->addChild($3);
    parse_tree.type_check(node);
    $$=node;
}
| expr LOP_ASSIGN expr{
    NodeAttr attr=NodeAttr(OP_ASSIGN);
    Node* node=new Node($1->lineno,NODE_EXPR,EXPR_OP,attr,Notype);
    node->seq=parse_tree.node_seq++;
    node->addChild($1);
    node->addChild($3);
    parse_tree.type_check(node);
    $$=node;
}
| expr LOP_PLUS expr{
    NodeAttr attr=NodeAttr(OP_PLUS);
    Node* node=new Node($1->lineno,NODE_EXPR,EXPR_OP,attr,$1->type);
    node->seq=parse_tree.node_seq++;
    node->addChild($1);
    node->addChild($3);
    parse_tree.type_check(node);
    $$=node;
}
| expr LOP_MINUS expr{
    NodeAttr attr=NodeAttr(OP_MINUS);
    Node* node=new Node($1->lineno,NODE_EXPR,EXPR_OP,attr,$1->type);
    node->seq=parse_tree.node_seq++;
    node->addChild($1);
    node->addChild($3);
    parse_tree.type_check(node);
    $$=node;
}
| expr LOP_MUL expr{
    NodeAttr attr=NodeAttr(OP_MUL);
    Node* node=new Node($1->lineno,NODE_EXPR,EXPR_OP,attr,$1->type);
    node->seq=parse_tree.node_seq++;
    node->addChild($1);
    node->addChild($3);
    parse_tree.type_check(node);
    $$=node;
}
| expr LOP_DIV expr{
    NodeAttr attr=NodeAttr(OP_DIV);
    Node* node=new Node($1->lineno,NODE_EXPR,EXPR_OP,attr,$1->type);
    node->seq=parse_tree.node_seq++;
    node->addChild($1);
    node->addChild($3);
    parse_tree.type_check(node);
    $$=node;
}
| expr LOP_MOD expr{
    NodeAttr attr=NodeAttr(OP_MOD);
    Node* node=new Node($1->lineno,NODE_EXPR,EXPR_OP,attr,$1->type);
    node->seq=parse_tree.node_seq++;
    node->addChild($1);
    node->addChild($3);
    parse_tree.type_check(node);
    $$=node;
}
| expr LOP_INC{
    NodeAttr attr=NodeAttr(OP_INC);
    Node* node=new Node($1->lineno,NODE_EXPR,EXPR_OP,attr,$1->type);
    node->seq=parse_tree.node_seq++;
    node->addChild($1);
    parse_tree.type_check(node);
    $$=node;
}
| expr LOP_DEC{
    NodeAttr attr=NodeAttr(OP_DEC);
    Node* node=new Node($1->lineno,NODE_EXPR,EXPR_OP,attr,$1->type);
    node->seq=parse_tree.node_seq++;
    node->addChild($1);
    parse_tree.type_check(node);
    $$=node;
}
| expr LOP_AND expr{
    NodeAttr attr=NodeAttr(OP_AND);
    Node* node=new Node($1->lineno,NODE_EXPR,EXPR_OP,attr,Boolean);
    node->seq=parse_tree.node_seq++;
    node->addChild($1);
    node->addChild($3);
    parse_tree.type_check(node);
    $$=node;
}
| expr LOP_OR expr{
    NodeAttr attr=NodeAttr(OP_OR);
    Node* node=new Node($1->lineno,NODE_EXPR,EXPR_OP,attr,Boolean);
    node->seq=parse_tree.node_seq++;
    node->addChild($1);
    node->addChild($3);
    parse_tree.type_check(node);
    $$=node;
}
| LOP_OPPSITE expr{
    NodeAttr attr=NodeAttr(OP_OPPSITE);
    Node* node=new Node($2->lineno,NODE_EXPR,EXPR_OP,attr,Boolean);
    node->seq=parse_tree.node_seq++;
    node->addChild($2);
    parse_tree.type_check(node);
    $$=node;
}
| expr LOP_LE expr{
    NodeAttr attr=NodeAttr(OP_LE);
    Node* node=new Node($1->lineno,NODE_EXPR,EXPR_OP,attr,Boolean);
    node->seq=parse_tree.node_seq++;
    node->addChild($1);
    node->addChild($3);
    parse_tree.type_check(node);
    $$=node;
}
| expr LOP_GE expr{
    NodeAttr attr=NodeAttr(OP_GE);
    Node* node=new Node($1->lineno,NODE_EXPR,EXPR_OP,attr,Boolean);
    node->seq=parse_tree.node_seq++;
    node->addChild($1);
    node->addChild($3);
    parse_tree.type_check(node);
    $$=node;
}
| expr LOP_NZ expr{
    NodeAttr attr=NodeAttr(OP_NZ);
    Node* node=new Node($1->lineno,NODE_EXPR,EXPR_OP,attr,Boolean);
    node->seq=parse_tree.node_seq++;
    node->addChild($1);
    node->addChild($3);
    parse_tree.type_check(node);
    $$=node;
}
| expr LOP_GT expr{
    NodeAttr attr=NodeAttr(OP_GT);
    Node* node=new Node($1->lineno,NODE_EXPR,EXPR_OP,attr,Boolean);
    node->seq=parse_tree.node_seq++;
    node->addChild($1);
    node->addChild($3);
    parse_tree.type_check(node);
    $$=node;
}
| expr LOP_LT expr{
    NodeAttr attr=NodeAttr(OP_LT);
    Node* node=new Node($1->lineno,NODE_EXPR,EXPR_OP,attr,Boolean);
    node->seq=parse_tree.node_seq++;
    node->addChild($1);
    node->addChild($3);
    parse_tree.type_check(node);
    $$=node;
}
| expr LOP_EQ expr{
    NodeAttr attr=NodeAttr(OP_EQ);
    Node* node=new Node($1->lineno,NODE_EXPR,EXPR_OP,attr,Boolean);
    node->seq=parse_tree.node_seq++;
    node->addChild($1);
    node->addChild($3);
    parse_tree.type_check(node);
    $$=node;
}
| LPAREN expr RPAREN{$$=$1;}
| LOP_MINUS expr{
    NodeAttr attr=NodeAttr(OP_MINUS);
    Node* node=new Node($2->lineno,NODE_EXPR,EXPR_OP,attr,$2->type);
    node->seq=parse_tree.node_seq++;
    node->addChild($2);
    parse_tree.type_check(node);
    $$=node;
}
| LOP_PLUS expr{
    NodeAttr attr=NodeAttr(OP_PLUS);
    Node* node=new Node($2->lineno,NODE_EXPR,EXPR_OP,attr,$2->type);
    node->seq=parse_tree.node_seq++;
    node->addChild($2);
    parse_tree.type_check(node);
    $$=node;
}
;

%%

int yyerror(char const* message)
{
  cout << message << " at line " << lineno << endl;
  return -1;
}