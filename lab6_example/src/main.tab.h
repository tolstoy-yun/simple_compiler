/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_SRC_MAIN_TAB_H_INCLUDED
# define YY_YY_SRC_MAIN_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    T_CHAR = 258,
    T_INT = 259,
    T_STRING = 260,
    T_BOOL = 261,
    T_VOID = 262,
    IDPointer = 263,
    IDQuote = 264,
    S_FOR = 265,
    S_IF = 266,
    S_WHILE = 267,
    S_ELSE = 268,
    S_SKIP = 269,
    S_RETURN = 270,
    P_PRINTF = 271,
    P_SCANF = 272,
    LOP_ASSIGN = 273,
    LOP_PLUS = 274,
    LOP_MINUS = 275,
    LOP_MUL = 276,
    LOP_DIV = 277,
    LOP_MOD = 278,
    LOP_INC = 279,
    LOP_DEC = 280,
    LOP_PLUS_ASSIGN = 281,
    LOP_MINUS_ASSIGN = 282,
    LOP_AND = 283,
    LOP_OR = 284,
    LOP_OPPSITE = 285,
    LOP_LE = 286,
    LOP_GE = 287,
    LOP_NZ = 288,
    LOP_GT = 289,
    LOP_LT = 290,
    LOP_EQ = 291,
    SEMICOLON = 292,
    COMMA = 293,
    LPAREN = 294,
    RPAREN = 295,
    LBRACE = 296,
    RBRACE = 297,
    QUOTE = 298,
    IDENTIFIER = 299,
    INTEGER = 300,
    CHAR = 301,
    BOOL = 302,
    STRING = 303,
    LOP_IAND = 304,
    LOWER_THAN_ELSE = 305
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SRC_MAIN_TAB_H_INCLUDED  */
