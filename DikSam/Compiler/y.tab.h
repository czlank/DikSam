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
     LEFTP = 269,
     RIGHTP = 270,
     LC = 271,
     RC = 272,
     SEMICOLON = 273,
     COLON = 274,
     COMMA = 275,
     ASSIGN_T = 276,
     LOGICAL_AND = 277,
     LOGICAL_OR = 278,
     EQ = 279,
     NE = 280,
     GT = 281,
     GE = 282,
     LT = 283,
     LE = 284,
     ADD = 285,
     SUB = 286,
     MUL = 287,
     DIV = 288,
     MOD = 289,
     TRUE_T = 290,
     FALSE_T = 291,
     EXCLAMATION = 292,
     DOT = 293,
     ADD_ASSIGN_T = 294,
     SUB_ASSIGN_T = 295,
     MUL_ASSIGN_T = 296,
     DIV_ASSIGN_T = 297,
     MOD_ASSIGN_T = 298,
     INCREMENT = 299,
     DECREMENT = 300,
     TRY = 301,
     CATCH = 302,
     FINALLY = 303,
     THROW = 304,
     BOOLEAN_T = 305,
     INT_T = 306,
     DOUBLE_T = 307,
     STRING_T = 308
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
#define LEFTP 269
#define RIGHTP 270
#define LC 271
#define RC 272
#define SEMICOLON 273
#define COLON 274
#define COMMA 275
#define ASSIGN_T 276
#define LOGICAL_AND 277
#define LOGICAL_OR 278
#define EQ 279
#define NE 280
#define GT 281
#define GE 282
#define LT 283
#define LE 284
#define ADD 285
#define SUB 286
#define MUL 287
#define DIV 288
#define MOD 289
#define TRUE_T 290
#define FALSE_T 291
#define EXCLAMATION 292
#define DOT 293
#define ADD_ASSIGN_T 294
#define SUB_ASSIGN_T 295
#define MUL_ASSIGN_T 296
#define DIV_ASSIGN_T 297
#define MOD_ASSIGN_T 298
#define INCREMENT 299
#define DECREMENT 300
#define TRY 301
#define CATCH 302
#define FINALLY 303
#define THROW 304
#define BOOLEAN_T 305
#define INT_T 306
#define DOUBLE_T 307
#define STRING_T 308




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1685 of yacc.c  */
#line 9 "DikSam.y"

    char                *identifier;
    ParameterList       *parameter_list;
    ArgumentList        *argument_list;
    Expression          *expression;
    Statement           *statement;
    StatementList       *statement_list;
    Block               *block;
    Elsif               *elsif;
    AssignmentOperator   assignment_operator;
    DVM_BasicType        type_specifier;



/* Line 1685 of yacc.c  */
#line 172 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


