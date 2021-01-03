#include <iostream>
#include <string>

using namespace ::std;

#include "symbol.h"
#include "tree.h"

symbol_table symtbl;

int symbol_table::lookup(string name,int number)
{
	for (int i = 0; i < size; i++)
		if (table[i].name == name && table[i].symbol_number==number)
			return i;
	exit(1);
}

int symbol_table::insert(string name, int token)
{
	if (size >= MAX_ID)
		exit(1);
	table[size].name = name;
	table[size].token = token;
	table[size].type = Notype;
	size++;
	return size - 1;
}

int symbol_table::gettoken(int pos)
{
	if (pos < 0 || pos >= size)
	{
		cerr << "Bad identifier" << endl;
		exit(1);
	}
	return table[pos].token;
}

string& symbol_table::getname(int pos)
{
	if (pos < 0 || pos >= size)
	{
		cerr << "Bad identifier" << endl;
		exit(1);
	}
	return table[pos].name;
}

int symbol_table::set_type(int pos, int type)
{
	if (pos < 0 || pos >= size)
	{
		cerr << "Bad identifier" << endl;
		exit(1);
	}

	table[pos].type = type;
	return 0;
}

int symbol_table::get_type(int pos)
{
	if (pos < 0 || pos >= size)
	{
		cerr << "Bad identifier" << endl;
		exit(1);
	}

	return table[pos].type;
}