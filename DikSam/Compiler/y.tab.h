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
     ELSIF = 264,
     WHILE = 265,
     FOR = 266,
     RETURN_T = 267,
     BREAK = 268,
     CONTINUE = 269,
     LEFTP = 270,
     RIGHTP = 271,
     LC = 272,
     RC = 273,
     SEMICOLON = 274,
     COLON = 275,
     COMMA = 276,
     ASSIGN_T = 277,
     LOGICAL_AND = 278,
     LOGICAL_OR = 279,
     EQ = 280,
     NE = 281,
     GT = 282,
     GE = 283,
     LT = 284,
     LE = 285,
     ADD = 286,
     SUB = 287,
     MUL = 288,
     DIV = 289,
     MOD = 290,
     TRUE_T = 291,
     FALSE_T = 292,
     EXCLAMATION = 293,
     DOT = 294,
     ADD_ASSIGN_T = 295,
     SUB_ASSIGN_T = 296,
     MUL_ASSIGN_T = 297,
     DIV_ASSIGN_T = 298,
     MOD_ASSIGN_T = 299,
     INCREMENT = 300,
     DECREMENT = 301,
     TRY = 302,
     CATCH = 303,
     FINALLY = 304,
     THROW = 305,
     VOID_T = 306,
     BOOLEAN_T = 307,
     INT_T = 308,
     DOUBLE_T = 309,
     STRING_T = 310
   };
#endif
/* Tokens.  */
#define INT_LITERAL 258
#define DOUBLE_LITERAL 259
#define STRING_LITERAL 260
#define IDENTIFIER 261
#define IF 262
#define ELSE 263
#define ELSIF 264
#define WHILE 265
#define FOR 266
#define RETURN_T 267
#define BREAK 268
#define CONTINUE 269
#define LEFTP 270
#define RIGHTP 271
#define LC 272
#define RC 273
#define SEMICOLON 274
#define COLON 275
#define COMMA 276
#define ASSIGN_T 277
#define LOGICAL_AND 278
#define LOGICAL_OR 279
#define EQ 280
#define NE 281
#define GT 282
#define GE 283
#define LT 284
#define LE 285
#define ADD 286
#define SUB 287
#define MUL 288
#define DIV 289
#define MOD 290
#define TRUE_T 291
#define FALSE_T 292
#define EXCLAMATION 293
#define DOT 294
#define ADD_ASSIGN_T 295
#define SUB_ASSIGN_T 296
#define MUL_ASSIGN_T 297
#define DIV_ASSIGN_T 298
#define MOD_ASSIGN_T 299
#define INCREMENT 300
#define DECREMENT 301
#define TRY 302
#define CATCH 303
#define FINALLY 304
#define THROW 305
#define VOID_T 306
#define BOOLEAN_T 307
#define INT_T 308
#define DOUBLE_T 309
#define STRING_T 310




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
#line 176 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


