#include "./pch.h"
#define MAX_ID 1024
using namespace std;

typedef struct TAG_SYMBOL_ENTRY
{
	string name;
	int token;
	int type;
} symbol_entry;

enum ValueType
{
    VALUE_BOOL,
    VALUE_INT,
    VALUE_CHAR,
    VALUE_STRING,
    COMPOSE_STRUCT,
    COMPOSE_UNION,
    COMPOSE_FUNCTION,
    COMPOSE_VOID
};

class symbol_table
{
private:
	symbol_entry table[MAX_ID];
	int size;

public:
	ValueType type;
	ValueType* sibling;

public:
	int lookup(string name);
	int insert(string name, int token);
	int gettoken(string name);
	string &getname(int pos);
	int set_type(int pos, int type);
	int get_type(int pos);
};