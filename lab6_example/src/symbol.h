#include "./pch.h"
#define MAX_ID 1024
using namespace std;

typedef struct TAG_SYMBOL_ENTRY
{
	string name;
	int token;
	int type;
	int symbol_number;

} symbol_entry;

class symbol_table
{
private:
	symbol_entry table[MAX_ID];
	int size;

public:
	int lookup(string name,int number);
	int insert(string name, int token);
	int gettoken(int pos);
	string &getname(int pos);
	int set_type(int pos,int type);
	int get_type(int pos);
};