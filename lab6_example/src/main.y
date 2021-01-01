%{
    #include "common.h"
    #define YYSTYPE Node *  
    TreeNode* root;
    extern int lineno;
    extern ofstream fout;
    extern int scope; //记录当前所处的作用域域的标号
    extern int symbolNum; //记录当前记录到第几个符号
    extern stack<Node*> currentScope; //栈中记录当前作用域的变量
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
: statements {root = new TreeNode(0, NODE_PROG); root->addChild($1);};

statements
:  statement {$$=$1;}
|  statements statement {$$=$1; $$->addSibling($2);}
;

statement
: SEMICOLON  {$$ = new TreeNode(lineno, NODE_STMT); $$->stype = STMT_SKIP;}
| declaration SEMICOLON {$$ = $1;}
| LBRACE statements RBRACE {$$=$2;}
| if_stmt {$$=$1;}
| while_stmt {$$=$1;}
| for_stmt {$$=$1;}
| printf SEMICOLON {$$=$1;}
| scanf SEMICOLON {$$=$1;}
| expr SEMICOLON {$$=$1;}
| func {$$=$1;}
;

declaration
: T IDLIST {
    TreeNode* node = new TreeNode($1->lineno, NODE_STMT);
    node->stype = STMT_DECL;
    //$2的类型是var，将$2压入currentScope栈，并分配一个符号（=symbolNum）
    if($2->nodeType==NODE_VAR){
        $2->number=symbolNum;
        symbolNum++;
        currentScope.push($2);
        //将$2的兄弟结点也压入栈并分配符号
        TreeNode* temp=$2->sibling;
        while(temp){
            temp->number=symbolNum;
            currentScope.push(temp);
            symbolNum++;
            temp=temp->sibling;
        }
    }
    /*$2的类型是expr，对应T IDENTIFIER=n,……;的情况
    * 遍历$2及其兄弟结点，将它们的第一个孩子结点压栈并分配符号
    */
    else{
        TreeNode* t=nullptr;
        t=$2;
        while(t){
            TreeNode* cur=t->child;
            cur->number=symbolNum;
            symbolNum++;
            currentScope.push(cur);
            t=t->sibling;
        }
    }
    node->addChild($1);
    node->addChild($2);
    $$ = node;
}
;

func: T IDENTIFIER LPAREN specialID RPAREN statement{
    TreeNode* node=new TreeNode($2->lineno,NODE_FUNC);
    //变量遇到T开头的定义，压入currentScope栈，并分配一个符号（=symbolNum）
    $2->number=symbolNum;
    symbolNum++;
    currentScope.push($2);
    node->addChild($1);
    node->addChild($2);
    node->addChild($4);
    node->addChild($6);
    $$=node;
}
| T IDENTIFIER LPAREN RPAREN statement{
    TreeNode* node=new TreeNode($2->lineno,NODE_FUNC);
    //变量遇到T开头的定义，压入currentScope栈，并分配一个符号（=symbolNum）
    $2->number=symbolNum;
    symbolNum++;
    currentScope.push($2);
    node->addChild($1);
    node->addChild($2);
    node->addChild($5);
    $$=node;
}
;

T: T_INT {$$ = new TreeNode(lineno, NODE_TYPE); $$->type = TYPE_INT;} 
| T_CHAR {$$ = new TreeNode(lineno, NODE_TYPE); $$->type = TYPE_CHAR;}
| T_BOOL {$$ = new TreeNode(lineno, NODE_TYPE); $$->type = TYPE_BOOL;}
| T_VOID {$$ = new TreeNode(lineno, NODE_TYPE); $$->type = FUNC_VOID;}
| T_STRING {$$ = new TreeNode(lineno, NODE_TYPE); $$->type = TYPE_STRING;}
;

IDLIST: IDLIST COMMA expr {$$=$1; $$->addSibling($3);}
| expr {$$=$1;}
;

for_stmt: S_FOR LPAREN expr SEMICOLON expr SEMICOLON expr RPAREN statement {
    TreeNode* node=new TreeNode($1->lineno,NODE_STMT);
    node->stype=STMT_FOR;
    node->addChild($3);
    node->addChild($5);
    node->addChild($7);
    node->addChild($9);
    $$=node;
}
| S_FOR LPAREN declaration SEMICOLON expr SEMICOLON expr RPAREN statement {
    TreeNode* node=new TreeNode($1->lineno,NODE_STMT);
    node->stype=STMT_FOR;
    node->addChild($3);
    node->addChild($5);
    node->addChild($7);
    node->addChild($9);
    $$=node;
}
;

while_stmt: S_WHILE LPAREN expr RPAREN statement {
    TreeNode* node=new TreeNode($1->lineno,NODE_STMT);
    node->stype=STMT_WHILE;
    node->addChild($3);
    node->addChild($5);
    $$=node;
}
;

if_stmt: S_IF LPAREN expr RPAREN statement %prec LOWER_THAN_ELSE{
    TreeNode* node=new TreeNode($3->lineno,NODE_STMT);
    node->stype=STMT_IF_ELSE;
    node->addChild($3);
    node->addChild($5);
    $$=node;
}
| S_IF LPAREN expr RPAREN statement S_ELSE statement{
    TreeNode* node=new TreeNode($3->lineno,NODE_STMT);
    node->stype=STMT_IF_ELSE;
    node->addChild($3);
    node->addChild($5);
    node->addChild($7);
    $$=node;
}


printf: P_PRINTF LPAREN STRING RPAREN {
    TreeNode* node=new TreeNode($1->lineno,NODE_STMT);
    node->stype=STMT_PRINTF;
    node->addChild($3);
    $$=node;
}
| P_PRINTF LPAREN STRING COMMA specialID RPAREN {
    TreeNode* node=new TreeNode($1->lineno,NODE_STMT);
    node->stype=STMT_PRINTF;
    node->addChild($3);
    node->addChild($5);
    $$=node;
}
;

scanf: P_SCANF LPAREN STRING COMMA specialID RPAREN{
    TreeNode* node=new TreeNode($1->lineno,NODE_STMT);
    node->stype=STMT_SCANF;
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
    TreeNode* node=new TreeNode($1->lineno,NODE_EXPR);
    node->optype=OP_PLUS_ASSIGN;
    node->addChild($1);
    node->addChild($3);
    $$=node;
}
| expr LOP_MINUS_ASSIGN expr{
    TreeNode* node=new TreeNode($1->lineno,NODE_EXPR);
    node->optype=OP_MINUS_ASSIGN;
    node->addChild($1);
    node->addChild($3);
    $$=node;
}
| expr LOP_ASSIGN expr{
    TreeNode* node=new TreeNode($1->lineno,NODE_EXPR);
    node->optype=OP_ASSIGN;
    node->addChild($1);
    node->addChild($3);
    $$=node;
}
| expr LOP_PLUS expr{
    TreeNode* node=new TreeNode($1->lineno,NODE_EXPR);
    node->optype=OP_PLUS;
    node->addChild($1);
    node->addChild($3);
    $$=node;
}
| expr LOP_MINUS expr{
    TreeNode* node=new TreeNode($1->lineno,NODE_EXPR);
    node->optype=OP_MINUS;
    node->addChild($1);
    node->addChild($3);
    $$=node;
}
| expr LOP_MUL expr{
    TreeNode* node=new TreeNode($1->lineno,NODE_EXPR);
    node->optype=OP_MUL;
    node->addChild($1);
    node->addChild($3);
    $$=node;
}
| expr LOP_DIV expr{
    TreeNode* node=new TreeNode($1->lineno,NODE_EXPR);
    node->optype=OP_DIV;
    node->addChild($1);
    node->addChild($3);
    $$=node;
}
| expr LOP_MOD expr{
    TreeNode* node=new TreeNode($1->lineno,NODE_EXPR);
    node->optype=OP_MOD;
    node->addChild($1);
    node->addChild($3);
    $$=node;
}
| expr LOP_INC{
    TreeNode* node=new TreeNode($1->lineno,NODE_EXPR);
    node->optype=OP_INC;
    node->addChild($1);
    $$=node;
}
| expr LOP_DEC{
    TreeNode* node=new TreeNode($1->lineno,NODE_EXPR);
    node->optype=OP_DEC;
    node->addChild($1);
    $$=node;
}
| expr LOP_AND expr{
    TreeNode* node=new TreeNode($1->lineno,NODE_EXPR);
    node->optype=OP_AND;
    node->addChild($1);
    node->addChild($3);
    $$=node;
}
| expr LOP_OR expr{
    TreeNode* node=new TreeNode($1->lineno,NODE_EXPR);
    node->optype=OP_OR;
    node->addChild($1);
    node->addChild($3);
    $$=node;
}
| LOP_OPPSITE expr{
    TreeNode* node=new TreeNode($2->lineno,NODE_EXPR);
    node->optype=OP_OPPSITE;
    node->addChild($2);
    $$=node;
}
| expr LOP_LE expr{
    TreeNode* node=new TreeNode($1->lineno,NODE_EXPR);
    node->optype=OP_LE;
    node->addChild($1);
    node->addChild($3);
    $$=node;
}
| expr LOP_GE expr{
    TreeNode* node=new TreeNode($1->lineno,NODE_EXPR);
    node->optype=OP_GE;
    node->addChild($1);
    node->addChild($3);
    $$=node;
}
| expr LOP_NZ expr{
    TreeNode* node=new TreeNode($1->lineno,NODE_EXPR);
    node->optype=OP_NZ;
    node->addChild($1);
    node->addChild($3);
    $$=node;
}
| expr LOP_GT expr{
    TreeNode* node=new TreeNode($1->lineno,NODE_EXPR);
    node->optype=OP_GT;
    node->addChild($1);
    node->addChild($3);
    $$=node;
}
| expr LOP_LT expr{
    TreeNode* node=new TreeNode($1->lineno,NODE_EXPR);
    node->optype=OP_LT;
    node->addChild($1);
    node->addChild($3);
    $$=node;
}
| expr LOP_EQ expr{
    TreeNode* node=new TreeNode($1->lineno,NODE_EXPR);
    node->optype=OP_EQ;
    node->addChild($1);
    node->addChild($3);
    $$=node;
}
| LOP_IAND expr{
    TreeNode* node=new TreeNode($2->lineno,NODE_EXPR);
    node->optype=OP_IAND;
    node->addChild($2);
    $$=node;
}
| LPAREN expr RPAREN{$$=$1;}
| LOP_MINUS expr{
    TreeNode* node=new TreeNode($2->lineno,NODE_EXPR);
    node->optype=OP_MINUS;
    node->addChild($2);
    $$=node;
}
| LOP_PLUS expr{
    TreeNode* node=new TreeNode($2->lineno,NODE_EXPR);
    node->optype=OP_PLUS;
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