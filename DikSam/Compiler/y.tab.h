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
     DO_T = 265,
     FOR = 266,
     RETURN_T = 267,
     BREAK = 268,
     CONTINUE = 269,
     NULL_T = 270,
     LEFTP = 271,
     RIGHTP = 272,
     LC = 273,
     RC = 274,
     LB = 275,
     RB = 276,
     SEMICOLON = 277,
     COLON = 278,
     COMMA = 279,
     ASSIGN_T = 280,
     LOGICAL_AND = 281,
     LOGICAL_OR = 282,
     EQ = 283,
     NE = 284,
     GT = 285,
     GE = 286,
     LT = 287,
     LE = 288,
     ADD = 289,
     SUB = 290,
     MUL = 291,
     DIV = 292,
     MOD = 293,
     TRUE_T = 294,
     FALSE_T = 295,
     EXCLAMATION = 296,
     DOT = 297,
     ADD_ASSIGN_T = 298,
     SUB_ASSIGN_T = 299,
     MUL_ASSIGN_T = 300,
     DIV_ASSIGN_T = 301,
     MOD_ASSIGN_T = 302,
     INCREMENT = 303,
     DECREMENT = 304,
     TRY = 305,
     CATCH = 306,
     FINALLY = 307,
     THROW = 308,
     VOID_T = 309,
     BOOLEAN_T = 310,
     INT_T = 311,
     DOUBLE_T = 312,
     STRING_T = 313,
     NEW = 314,
     REQUIRE = 315,
     RENAME = 316,
     CLASS_T = 317,
     INTERFACE_T = 318,
     PUBLIC_T = 319,
     PRIVATE_T = 320,
     VIRTUAL_T = 321,
     OVERRIDE_T = 322,
     ABSTRACT_T = 323,
     THIS_T = 324,
     SUPER_T = 325,
     CONSTRUCTOR = 326,
     INSTANCEOF = 327,
     DOWN_CAST_BEGIN = 328,
     DOWN_CAST_END = 329
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
#define DO_T 265
#define FOR 266
#define RETURN_T 267
#define BREAK 268
#define CONTINUE 269
#define NULL_T 270
#define LEFTP 271
#define RIGHTP 272
#define LC 273
#define RC 274
#define LB 275
#define RB 276
#define SEMICOLON 277
#define COLON 278
#define COMMA 279
#define ASSIGN_T 280
#define LOGICAL_AND 281
#define LOGICAL_OR 282
#define EQ 283
#define NE 284
#define GT 285
#define GE 286
#define LT 287
#define LE 288
#define ADD 289
#define SUB 290
#define MUL 291
#define DIV 292
#define MOD 293
#define TRUE_T 294
#define FALSE_T 295
#define EXCLAMATION 296
#define DOT 297
#define ADD_ASSIGN_T 298
#define SUB_ASSIGN_T 299
#define MUL_ASSIGN_T 300
#define DIV_ASSIGN_T 301
#define MOD_ASSIGN_T 302
#define INCREMENT 303
#define DECREMENT 304
#define TRY 305
#define CATCH 306
#define FINALLY 307
#define THROW 308
#define VOID_T 309
#define BOOLEAN_T 310
#define INT_T 311
#define DOUBLE_T 312
#define STRING_T 313
#define NEW 314
#define REQUIRE 315
#define RENAME 316
#define CLASS_T 317
#define INTERFACE_T 318
#define PUBLIC_T 319
#define PRIVATE_T 320
#define VIRTUAL_T 321
#define OVERRIDE_T 322
#define ABSTRACT_T 323
#define THIS_T 324
#define SUPER_T 325
#define CONSTRUCTOR 326
#define INSTANCEOF 327
#define DOWN_CAST_BEGIN 328
#define DOWN_CAST_END 329




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1685 of yacc.c  */
#line 9 "DikSam.y"

    char                        *identifier;
    PackageName                 *package_name;
    RequireList                 *require_list;
    RenameList                  *rename_list;
    ParameterList               *parameter_list;
    ArgumentList                *argument_list;
    Expression                  *expression;
    ExpressionList              *expression_list;
    Statement                   *statement;
    StatementList               *statement_list;
    Block                       *block;
    Elsif                       *elsif;
    AssignmentOperator           assignment_operator;
    TypeSpecifier               *type_specifier;
    DVM_BasicType                basic_type_specifier;
    ArrayDimension              *array_dimension;
    ClassOrMemberModifierList    class_or_member_modifier;
    DVM_ClassOrInterface         class_or_interface;
    ExtendsList                 *extends_list;
    MemberDeclaration           *member_declaration;
    FunctionDefinition          *function_definition;



/* Line 1685 of yacc.c  */
#line 225 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


