%{
    #include "common.h"
    #define YYSTYPE Node *  
    extern int lineno;
    extern ofstream fout;
    extern symbol_table symtbl;
    // extern int scope; //记录当前所处的作用域域的标号
    // extern int symbolNum; //记录当前记录到第几个符号
    // extern stack<Node*> currentScope; //栈中记录当前作用域的变量
    int yylex();
    int yyerror( char const * );
%}
%token T_CHAR T_INT T_STRING T_BOOL T_VOID IDPointer IDQuote

%token S_FOR S_IF S_WHILE S_ELSE S_SKIP

%token P_PRINTF P_SCANF

%token LOP_ASSIGN LOP_PLUS LOP_MINUS LOP_MUL LOP_DIV LOP_MOD LOP_INC
%token LOP_DEC LOP_PLUS_ASSIGN LOP_MINUS_ASSIGN LOP_AND LOP_OR LOP_OPPSITE
%token LOP_LE LOP_GE LOP_NZ LOP_GT LOP_LT LOP_EQ LOP_IAND

%token SEMICOLON COMMA LPAREN RPAREN LBRACE RBRACE

%token IDENTIFIER INTEGER CHAR BOOL STRING

%left COMMA
%left LOP_OR
%left LOP_AND
%left LOP_IAND 
%left LOP_EQ LOP_NZ    
%left LOP_LE LOP_GE LOP_LT LOP_GT
%left LOP_PLUS LOP_MINUS
%left LOP_MUL LOP_DIV LOP_MOD
%left LOP_PLUS_ASSIGN LOP_MINUS_ASSIGN

%right LOP_ASSIGN LOP_INC LOP_DEC  
%right LOP_OPPSITE

%nonassoc LOWER_THAN_ELSE
%nonassoc S_ELSE
%%

program
: statements {
    NodeAttr attr=NodeAttr();
    paese_tree.root =paese_tree.NewRoot(0, NODE_PROG,-1,attr,Notype,$1); 
};

statements
:  statement {$$=$1;}
|  statements statement {$$=$1; $$->addSibling($2);}
;

statement
: SEMICOLON  {NodeAttr attr=NodeAttr(); $$ = new Node(lineno, NODE_STMT,STMT_SKIP,attr,Notype); node->seq=tree::node_seq++;}
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
    node->addChild($2);
    $$ = node;
}
;

declaration
: T IDLIST {
    NodeAttr attr=NodeAttr();
    Node* node = new Node($1->lineno, NODE_STMT,STMT_DECL,attr,$1->type);
    node->seq=tree::node_seq++;
    //如果$2的类型是var，将$2在符号表里的type设为与T一致
    if($2->nodeType==NODE_VAR){
        symtbl.set_type($2->pos,$1->type);
         //将$2的兄弟结点也设置type
         Node* temp=$2->sibling;
         while(temp){
             symtbl.set_type(temp->pos,$1->type);
         }
    }
    //$2的类型是expr，对应T IDENTIFIER=n,……;的情况
    else{
        Node* t=nullptr;
        t=$2;
        while(t){
            Node* cur=t->child;
            symtbl.set_type(cur->pos,$1->type);
            t=t->sibling;
        }
    }
    node->addChild($1);
    node->addChild($2);
    $$ = node;
}
;

func: T IDENTIFIER LPAREN specialID RPAREN statement{
    NodeAttr attr=NodeAttr();
    Node* node=new Node($2->lineno,NODE_FUNC,-1,attr,Notype);
    node->seq=tree::node_seq++;
    symtbl.set_type($2->pos,$1->type);

    // //变量遇到T开头的定义，压入currentScope栈，并分配一个符号（=symbolNum）
    // $2->number=symbolNum;
    // symbolNum++;
    // currentScope.push($2);

    node->addChild($1);
    node->addChild($2);
    node->addChild($4);
    node->addChild($6);
    $$=node;
}
| T IDENTIFIER LPAREN RPAREN statement{
    NodeAttr attr=NodeAttr();
    Node* node=new Node($2->lineno,NODE_FUNC,-1,attr,Notype);
    node->seq=tree::node_seq++;
    symtbl.set_type($2->pos,$1->type);
    // //变量遇到T开头的定义，压入currentScope栈，并分配一个符号（=symbolNum）
    // $2->number=symbolNum;
    // symbolNum++;
    // currentScope.push($2);
    node->addChild($1);
    node->addChild($2);
    node->addChild($5);
    $$=node;
}
;

T: T_INT {NodeAttr attr=NodeAttr(); $$ = new Node(lineno, NODE_TYPE,TYPE_INT,attr,Integer); node->seq=tree::node_seq++;}
| T_CHAR {NodeAttr attr=NodeAttr(); $$ = new Node(lineno, NODE_TYPE,TYPE_CHAR,attr,Char); node->seq=tree::node_seq++;}
| T_BOOL {NodeAttr attr=NodeAttr(); $$ = new Node(lineno, NODE_TYPE,TYPE_BOOL,attr,Boolean); node->seq=tree::node_seq++;}
| T_VOID {NodeAttr attr=NodeAttr(); $$ = new Node(lineno, NODE_TYPE,FUNC_VOID,attr,Notype); node->seq=tree::node_seq++;}
| T_STRING {NodeAttr attr=NodeAttr(); $$ = new Node(lineno, NODE_TYPE,TYPE_STRING,attr,String); node->seq=tree::node_seq++;}
;

IDLIST: IDLIST COMMA expr {$$=$1; $$->addSibling($3);}
| expr {$$=$1;}
;

for_stmt: S_FOR LPAREN expr SEMICOLON expr SEMICOLON expr RPAREN statement {
    NodeAttr attr=NodeAttr();
    Node* node=new Node($1->lineno,NODE_STMT,STMT_FOR,attr,Notype);
    node->seq=tree::node_seq++;
    node->addChild($3);
    node->addChild($5);
    node->addChild($7);
    node->addChild($9);
    $$=node;
}
| S_FOR LPAREN declaration SEMICOLON expr SEMICOLON expr RPAREN statement {
    NodeAttr attr=NodeAttr();
    Node* node=new Node($1->lineno,NODE_STMT,STMT_FOR,attr,Notype);
    node->seq=tree::node_seq++;
    node->addChild($3);
    node->addChild($5);
    node->addChild($7);
    node->addChild($9);
    $$=node;
}
;

while_stmt: S_WHILE LPAREN expr RPAREN statement {
    NodeAttr attr=NodeAttr();
    Node* node=new Node($1->lineno,NODE_STMT,STMT_WHILE,attr,Notype);
    node->seq=tree::node_seq++;
    node->addChild($3);
    node->addChild($5);
    $$=node;
}
;

if_stmt: S_IF LPAREN expr RPAREN statement %prec LOWER_THAN_ELSE{
    NodeAttr attr=NodeAttr();
    Node* node=new Node($3->lineno,NODE_STMT,STMT_IF_ELSE,attr,Notype);
    node->seq=tree::node_seq++;
    node->addChild($3);
    node->addChild($5);
    $$=node;
}
| S_IF LPAREN expr RPAREN statement S_ELSE statement{
    NodeAttr attr=NodeAttr();
    Node* node=new Node($3->lineno,NODE_STMT,STMT_IF_ELSE,attr,Notype);
    node->seq=tree::node_seq++;
    node->addChild($3);
    node->addChild($5);
    node->addChild($7);
    $$=node;
}


printf: P_PRINTF LPAREN STRING RPAREN {
    NodeAttr attr=NodeAttr();
    Node* node=new Node($1->lineno,NODE_STMT,STMT_PRINTF,attr,Notype);
    node->seq=tree::node_seq++;
    node->addChild($3);
    $$=node;
}
| P_PRINTF LPAREN STRING COMMA specialID RPAREN {
    NodeAttr attr=NodeAttr();
    Node* node=new Node($1->lineno,NODE_STMT,STMT_PRINTF,attr,Notype);
    node->seq=tree::node_seq++;
    node->addChild($3);
    node->addChild($5);
    $$=node;
}
;

scanf: P_SCANF LPAREN STRING COMMA specialID RPAREN{
    NodeAttr attr=NodeAttr();
    Node* node=new Node($1->lineno,NODE_STMT,STMT_SCANF,attr,Notype);
    node->seq=tree::node_seq++;
    node->addChild($3);
    node->addChild($5);
    $$=node;
}
;

specialID: IDENTIFIER{$$=$1;}
| IDQuote{$$=$1;}
| IDPointer{$$=$1;}
| specialID COMMA IDENTIFIER{$$=$1;$$->addSibling($3);}
| specialID COMMA IDQuote{$$=$1;$$->addSibling($3);}
| specialID COMMA IDPointer{$$=$1;$$->addSibling($3);}
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
    node->seq=tree::node_seq++;
    node->addChild($1);
    node->addChild($3);
    $$=node;
}
| expr LOP_MINUS_ASSIGN expr{
    NodeAttr attr=NodeAttr(OP_MINUS_ASSIGN);
    Node* node=new Node($1->lineno,NODE_EXPR,EXPR_OP,attr,Notype);
    node->seq=tree::node_seq++;
    node->addChild($1);
    node->addChild($3);
    $$=node;
}
| expr LOP_ASSIGN expr{
    NodeAttr attr=NodeAttr(OP_ASSIGN);
    Node* node=new Node($1->lineno,NODE_EXPR,EXPR_OP,attr,Notype);
    node->seq=tree::node_seq++;
    node->addChild($1);
    node->addChild($3);
    $$=node;
}
| expr LOP_PLUS expr{
    NodeAttr attr=NodeAttr(OP_PLUS);
    Node* node=new Node($1->lineno,NODE_EXPR,EXPR_OP,attr,Notype);
    node->seq=tree::node_seq++;
    node->addChild($1);
    node->addChild($3);
    $$=node;
}
| expr LOP_MINUS expr{
    NodeAttr attr=NodeAttr(OP_MINUS);
    Node* node=new Node($1->lineno,NODE_EXPR,EXPR_OP,attr,Notype);
    node->addChild($1);
    node->addChild($3);
    $$=node;
}
| expr LOP_MUL expr{
    NodeAttr attr=NodeAttr(OP_MUL);
    Node* node=new Node($1->lineno,NODE_EXPR,EXPR_OP,attr,Notype);
    node->seq=tree::node_seq++;
    node->addChild($1);
    node->addChild($3);
    $$=node;
}
| expr LOP_DIV expr{
    NodeAttr attr=NodeAttr(OP_DIV);
    Node* node=new Node($1->lineno,NODE_EXPR,EXPR_OP,attr,Notype);
    node->seq=tree::node_seq++;
    node->addChild($1);
    node->addChild($3);
    $$=node;
}
| expr LOP_MOD expr{
    NodeAttr attr=NodeAttr(OP_MOD);
    Node* node=new Node($1->lineno,NODE_EXPR,EXPR_OP,attr,Notype);
    node->seq=tree::node_seq++;
    node->addChild($1);
    node->addChild($3);
    $$=node;
}
| expr LOP_INC{
    NodeAttr attr=NodeAttr(OP_INC);
    Node* node=new Node($1->lineno,NODE_EXPR,EXPR_OP,attr,Notype);
    node->seq=tree::node_seq++;
    node->addChild($1);
    $$=node;
}
| expr LOP_DEC{
    NodeAttr attr=NodeAttr(OP_DEC);
    Node* node=new Node($1->lineno,NODE_EXPR,EXPR_OP,attr,Notype);
    node->seq=tree::node_seq++;
    node->addChild($1);
    $$=node;
}
| expr LOP_AND expr{
    NodeAttr attr=NodeAttr(OP_AND);
    Node* node=new Node($1->lineno,NODE_EXPR,EXPR_OP,attr,Notype);
    node->seq=tree::node_seq++;
    node->addChild($1);
    node->addChild($3);
    $$=node;
}
| expr LOP_OR expr{
    NodeAttr attr=NodeAttr(OP_OR);
    Node* node=new Node($1->lineno,NODE_EXPR,EXPR_OP,attr,Notype);
    node->seq=tree::node_seq++;
    node->addChild($1);
    node->addChild($3);
    $$=node;
}
| LOP_OPPSITE expr{
    NodeAttr attr=NodeAttr(OP_OPPSITE);
    Node* node=new Node($2->lineno,NODE_EXPR,EXPR_OP,attr,Notype);
    node->seq=tree::node_seq++;
    node->addChild($2);
    $$=node;
}
| expr LOP_LE expr{
    NodeAttr attr=NodeAttr(OP_LE);
    Node* node=new Node($1->lineno,NODE_EXPR,EXPR_OP,attr,Notype);
    node->seq=tree::node_seq++;
    node->addChild($1);
    node->addChild($3);
    $$=node;
}
| expr LOP_GE expr{
    NodeAttr attr=NodeAttr(OP_GE);
    Node* node=new Node($1->lineno,NODE_EXPR,EXPR_OP,attr,Notype);
    node->seq=tree::node_seq++;
    node->addChild($1);
    node->addChild($3);
    $$=node;
}
| expr LOP_NZ expr{
    NodeAttr attr=NodeAttr(OP_NZ);
    Node* node=new Node($1->lineno,NODE_EXPR,EXPR_OP,attr,Notype);
    node->seq=tree::node_seq++;
    node->addChild($1);
    node->addChild($3);
    $$=node;
}
| expr LOP_GT expr{
    NodeAttr attr=NodeAttr(OP_GT);
    Node* node=new Node($1->lineno,NODE_EXPR,EXPR_OP,attr,Notype);
    node->seq=tree::node_seq++;
    node->addChild($1);
    node->addChild($3);
    $$=node;
}
| expr LOP_LT expr{
    NodeAttr attr=NodeAttr(OP_LT);
    Node* node=new Node($1->lineno,NODE_EXPR,EXPR_OP,attr,Notype);
    node->seq=tree::node_seq++;
    node->addChild($1);
    node->addChild($3);
    $$=node;
}
| expr LOP_EQ expr{
    NodeAttr attr=NodeAttr(OP_EQ);
    Node* node=new Node($1->lineno,NODE_EXPR,EXPR_OP,attr,Notype);
    node->seq=tree::node_seq++;
    node->addChild($1);
    node->addChild($3);
    $$=node;
}
| LOP_IAND expr{
    NodeAttr attr=NodeAttr(OP_IAND);
    Node* node=new Node($2->lineno,NODE_EXPR,EXPR_OP,attr,Notype);
    node->seq=tree::node_seq++;
    node->addChild($2);
    $$=node;
}
| LPAREN expr RPAREN{$$=$1;}
| LOP_MINUS expr{
    NodeAttr attr=NodeAttr(OP_MINUS);
    Node* node=new Node($2->lineno,NODE_EXPR,EXPR_OP,attr,Notype);
    node->seq=tree::node_seq++;
    node->addChild($2);
    $$=node;
}
| LOP_PLUS expr{
    NodeAttr attr=NodeAttr(OP_PLUS);
    Node* node=new Node($2->lineno,NODE_EXPR,EXPR_OP,attr,Notype);
    node->seq=tree::node_seq++;
    node->addChild($2);
    $$=node;
}
;

%%

int yyerror(char const* message)
{
  cout << message << " at line " << lineno << endl;
  return -1;
}