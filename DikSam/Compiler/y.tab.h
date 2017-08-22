/* A Bison parser, made by GNU Bison 2.4.2.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2006, 2009-2010 Free Software
   Foundation, Inc.
   
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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INT_LITERAL = 258,
     DOUBLE_LITERAL = 259,
     STRING_LITERAL = 260,
     IDENTIFIER = 261,
     IF = 262,
     ELSE = 263,
     WHILE = 264,
     FOR = 265,
     RETURN_T = 266,
     BREAK = 267,
     CONTINUE = 268,
     NULL_T = 269,
     LEFTP = 270,
     RIGHTP = 271,
     LC = 272,
     RC = 273,
     LB = 274,
     RB = 275,
     SEMICOLON = 276,
     COLON = 277,
     COMMA = 278,
     ASSIGN_T = 279,
     LOGICAL_AND = 280,
     LOGICAL_OR = 281,
     EQ = 282,
     NE = 283,
     GT = 284,
     GE = 285,
     LT = 286,
     LE = 287,
     ADD = 288,
     SUB = 289,
     MUL = 290,
     DIV = 291,
     MOD = 292,
     TRUE_T = 293,
     FALSE_T = 294,
     EXCLAMATION = 295,
     DOT = 296,
     ADD_ASSIGN_T = 297,
     SUB_ASSIGN_T = 298,
     MUL_ASSIGN_T = 299,
     DIV_ASSIGN_T = 300,
     MOD_ASSIGN_T = 301,
     INCREMENT = 302,
     DECREMENT = 303,
     TRY = 304,
     CATCH = 305,
     FINALLY = 306,
     THROW = 307,
     BOOLEAN_T = 308,
     INT_T = 309,
     DOUBLE_T = 310,
     STRING_T = 311,
     NEW = 312
   };
#endif
/* Tokens.  */
#define INT_LITERAL 258
#define DOUBLE_LITERAL 259
#define STRING_LITERAL 260
#define IDENTIFIER 261
#define IF 262
#define ELSE 263
#define WHILE 264
#define FOR 265
#define RETURN_T 266
#define BREAK 267
#define CONTINUE 268
#define NULL_T 269
#define LEFTP 270
#define RIGHTP 271
#define LC 272
#define RC 273
#define LB 274
#define RB 275
#define SEMICOLON 276
#define COLON 277
#define COMMA 278
#define ASSIGN_T 279
#define LOGICAL_AND 280
#define LOGICAL_OR 281
#define EQ 282
#define NE 283
#define GT 284
#define GE 285
#define LT 286
#define LE 287
#define ADD 288
#define SUB 289
#define MUL 290
#define DIV 291
#define MOD 292
#define TRUE_T 293
#define FALSE_T 294
#define EXCLAMATION 295
#define DOT 296
#define ADD_ASSIGN_T 297
#define SUB_ASSIGN_T 298
#define MUL_ASSIGN_T 299
#define DIV_ASSIGN_T 300
#define MOD_ASSIGN_T 301
#define INCREMENT 302
#define DECREMENT 303
#define TRY 304
#define CATCH 305
#define FINALLY 306
#define THROW 307
#define BOOLEAN_T 308
#define INT_T 309
#define DOUBLE_T 310
#define STRING_T 311
#define NEW 312




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1685 of yacc.c  */
#line 9 "DikSam.y"

    char                *identifier;
    ParameterList       *parameter_list;
    ArgumentList        *argument_list;
    Expression          *expression;
    ExpressionList      *expression_list;
    Statement           *statement;
    StatementList       *statement_list;
    Block               *block;
    Elsif               *elsif;
    AssignmentOperator   assignment_operator;
    TypeSpecifier       *type_specifier;
    DVM_BasicType        basic_type_specifier;
    ArrayDimension      *array_dimension;



/* Line 1685 of yacc.c  */
#line 183 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


