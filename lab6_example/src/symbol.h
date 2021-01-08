#ifndef SYMBOL_H
#define SYMBOL_H
#include "./pch.h"
#define MAX_ID 1024
using namespace std;

typedef struct TAG_SYMBOL_ENTRY
{
	string name;
	int token;
	int type;
	int symbol_number;
	int scope;

} symbol_entry;

class symbol_table
{
public:
	symbol_entry table[MAX_ID];
	int size;

public:
	int lookup(string name,int number);
	int insert(string name, int token);
	int gettoken(int pos);
	string &getname(int pos);
	int set_type(int pos,int type);
	int get_type(int pos);
	int set_scope(int pos,int scope);
	int get_scope(int pos);
};
#endif