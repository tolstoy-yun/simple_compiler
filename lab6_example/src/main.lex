%option nounput
%{
#include "common.h"
#include "main.tab.h"  // yacc header
#include<string.h>
int lineno=1;
/*
* extern int scope; //记录当前所处的作用域域的标号
* extern int symbolNum; //记录当前记录到第几个符号
* extern stack<Node*> currentScope; //栈中记录当前作用域的变量
*/
%}

BLOCKCOMMENT \/\*([^\*^\/]*|[\*^\/*]*|[^\**\/]*)*\*\/
LINECOMMENT \/\/[^\n]*
EOL	(\r\n|\r|\n)
WHILTESPACE [[:blank:]]

INTEGER [0-9]+
S_SKIP break | continue

CHAR \'.?\'
STRING \".+\"

IDENTIFIER [[:alpha:]_][[:alpha:][:digit:]_]*
IDQuote \&[[:alpha:]_][[:alpha:][:digit:]_]*
IDPointer \*[[:alpha:]_][[:alpha:][:digit:]_]*

LBRACE \{
RBRACE \}
%%

{BLOCKCOMMENT}  /* do nothing */
{LINECOMMENT}  /* do nothing */

"int" return T_INT;
"char" return T_CHAR;
"void" return T_VOID;
"bool" return T_BOOL;

"for" return S_FOR;
"if" return S_IF;
"while" return S_WHILE;
"else" return S_ELSE;

"printf" return P_PRINTF;
"scanf" return P_SCANF;

"=" return LOP_ASSIGN;
"+" return LOP_PLUS;
"-" return LOP_MINUS;
"*" return LOP_MUL;
"/" return LOP_DIV;
"%" return LOP_MOD;
"++" return LOP_INC;
"--" return LOP_DEC;
"+=" return LOP_PLUS_ASSIGN;
"-=" return LOP_MINUS_ASSIGN;
"&&" return LOP_AND;
"||" return LOP_OR;
"!" return LOP_OPPSITE;
"<=" return LOP_LE;
">=" return LOP_GE;
"!=" return LOP_NZ;
">" return LOP_GT;
"<" return LOP_LT;
"==" return LOP_EQ;
"&" return LOP_IAND;

"," return COMMA;
";" return SEMICOLON;
"(" return LPAREN;
")" return RPAREN;

{INTEGER} {
    NodeAttr attr=NodeAttr();
    Node* node = new Node(lineno, NODE_CONST,-1,attr,Integer);
    node->attr->vali = atoi(yytext);
    yylval = node;
    return INTEGER;
}

{CHAR} {
    NodeAttr attr=NodeAttr(yytext[1]);
    Node* node = new Node(lineno, NODE_CONST,-1,attr,Char);
    yylval = node;
    return CHAR;
}

{STRING} {
    NodeAttr attr=NodeAttr(yytext[1]);
    Node* node = new Node(lineno,NODE_CONST,-1,attr,String);
    yylval=node;
    return STRING;
}

{IDENTIFIER} {
    NodeAttr attr=NodeAttr();
    Node* node = new Node(lineno, NODE_VAR,VAR_COMMON,attr,Notype);
    node->attr->var_name=string(yytext)

    /*node->firstScope=scope; //初始域号=当前域号
    /*遍历栈，判断是否有大于其起始域且var_name相同的符号
    * 如果有，则当前结点的序号=那个结点的序号，退出遍历
    */
    stack<TreeNode*> tempStack;//辅助遍历栈
    TreeNode* tempNode=nullptr;
    while(!currentScope.empty()){
        tempNode=currentScope.top();
        currentScope.pop();
        tempStack.push(tempNode);
        if(node->var_name.compare(tempNode->var_name)==0 && node->firstScope>=tempNode->firstScope){
            node->number=tempNode->number;
            break;
        }
    }
    //将tempStack里的结点重新倒回currentScope里
    while(!tempStack.empty()){
        currentScope.push(tempStack.top());
        tempStack.pop();
    }
    */

    yylval = node;
    return IDENTIFIER;
}

{IDQuote} {
    NodeAttr attr=NodeAttr(string(yytext));
    Node* node=new Node(lineno,NODE_VAR,VAR_QUOTE,attr,Notype);
    string str=string(yytext);
    str.erase(str.begin());
    node->attr->var_name=str;
    yylval=node;
    return IDQuote;
}

{WHILTESPACE} /* do nothing */

{EOL} lineno++;

. {
    cerr << "[line "<< lineno <<" ] unknown character:" << yytext << endl;
}
%%