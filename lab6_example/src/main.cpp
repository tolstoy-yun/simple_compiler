#include "common.h"
#include <fstream>
#include<stack>

extern parse_tree;
extern FILE *yyin;
extern int yyparse();
ofstream fout("result.txt",ios::app);
int scope=0; //记录当前所处的作用域的标号
int symbolNum=0; //记录当前记录到第几个符号
stack<TreeNode*> currentScope; //栈中记录当前作用域的变量

using namespace std;
int main(int argc, char *argv[])
{
    yyin = fopen(argv[1], "r");
    ostream* asm_out = new ofstream(argv[2]);
    yyparse();
    parse_tree.get_label();
    parse_tree.gen_code(*asm_out);
    return 0;
}