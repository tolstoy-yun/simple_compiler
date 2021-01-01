#include "common.h"
#include <fstream>
#include<stack>

extern TreeNode *root;
extern FILE *yyin;
extern int yyparse();
ofstream fout("result.txt",ios::app);
int scope=0; //记录当前所处的作用域的标号
int symbolNum=0; //记录当前记录到第几个符号
stack<TreeNode*> currentScope; //栈中记录当前作用域的变量

using namespace std;
int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        FILE *fin = fopen(argv[1], "r");
        if (fin != nullptr)
        {
            yyin = fin;
        }
        else
        {
            cerr << "failed to open file: " << argv[1] << endl;
        }
    }
    yyparse();
    if(root != NULL) {
        root->genNodeId();
        root->printAST();
    }
    return 0;
}