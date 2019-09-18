/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

#ifndef YY_YY_INTERPRETER_PARSER_H_INCLUDED
# define YY_YY_INTERPRETER_PARSER_H_INCLUDED
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
    IMPORT = 258,
    FROM = 259,
    RETURN = 260,
    OBJECT = 261,
    BELONG = 262,
    MACHINE_OPERATIONS = 263,
    IF = 264,
    ELSE = 265,
    ARRAY_MARK = 266,
    MAP_MARK = 267,
    COMMA = 268,
    COLON = 269,
    SEMICOLON = 270,
    OPEN_CURVE_BRACKET = 271,
    CLOSE_CURVE_BRACKET = 272,
    INT = 273,
    STR = 274,
    IDENTIFIER = 275,
    NUM = 276,
    STR_LITERAL = 277,
    PERCENTAGE = 278,
    EQUAL = 279,
    LESS_THAN = 280,
    GREATER_THAN = 281,
    LESS_OR_EQUAL = 282,
    GREATER_OR_EQUAL = 283,
    NOT_EQUAL = 284,
    PLUS = 285,
    MINUS = 286,
    MUL = 287,
    DIV = 288,
    ASSIGNMENT = 289,
    OPEN_PAREN = 290,
    CLOSE_PAREN = 291,
    DOT = 292,
    OPEN_SQUARE_BRACKET = 293,
    CLOSE_SQUARE_BRACKET = 294
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 1 "./interpreter/tScript.y" /* yacc.c:1909  */

    char *str;
    int integer;
    list *list_;
    Program *program;
    Statement *statement;
    Expression *expression;

    pair *Pair;
    typeInfo *type_info;

    ObjectDeclBody *objDeclBody;
    ObjectDeclItem *objDeclItem;

#line 109 "./interpreter/parser.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_INTERPRETER_PARSER_H_INCLUDED  */
