/* A Bison parser, made by GNU Bison 2.4.2.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "DikSam.y"

#include <stdio.h>
#include "DikSamc.h"

#define YYDEBUG     1


/* Line 189 of yacc.c  */
#line 80 "y.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


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

/* Line 214 of yacc.c  */
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



/* Line 214 of yacc.c  */
#line 290 "y.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 302 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  12
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   640

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  75
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  69
/* YYNRULES -- Number of rules.  */
#define YYNRULES  186
/* YYNRULES -- Number of states.  */
#define YYNSTATES  321

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   329

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     6,     9,    10,    13,    15,    17,    19,
      22,    26,    28,    32,    34,    37,    42,    44,    46,    48,
      50,    53,    56,    58,    60,    62,    64,    66,    68,    70,
      72,    78,    86,    93,   102,   104,   107,   114,   121,   132,
     141,   142,   144,   148,   152,   156,   160,   166,   167,   172,
     175,   182,   188,   195,   201,   204,   209,   211,   215,   217,
     221,   223,   227,   229,   233,   235,   239,   241,   245,   249,
     251,   255,   259,   263,   267,   269,   273,   277,   279,   283,
     287,   291,   293,   296,   299,   301,   304,   307,   311,   313,
     315,   317,   322,   327,   331,   336,   340,   344,   349,   351,
     353,   355,   357,   359,   361,   363,   365,   367,   372,   378,
     385,   393,   397,   402,   406,   411,   415,   420,   422,   425,
     429,   432,   436,   437,   439,   443,   444,   447,   449,   451,
     453,   455,   457,   459,   461,   463,   465,   467,   469,   471,
     475,   479,   483,   485,   487,   489,   490,   492,   493,   501,
     502,   511,   512,   519,   520,   528,   530,   533,   535,   537,
     539,   541,   543,   545,   546,   549,   551,   555,   557,   560,
     562,   564,   566,   569,   571,   574,   581,   587,   594,   600,
     607,   613,   620,   626,   628,   630,   634
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      76,     0,    -1,    77,    83,    -1,    76,    83,    -1,    -1,
      78,    81,    -1,    78,    -1,    81,    -1,    79,    -1,    78,
      79,    -1,    60,    80,    22,    -1,     6,    -1,    80,    42,
       6,    -1,    82,    -1,    81,    82,    -1,    61,    80,     6,
      22,    -1,    99,    -1,   127,    -1,    85,    -1,    85,    -1,
      84,    85,    -1,   102,    22,    -1,    86,    -1,    89,    -1,
      90,    -1,    91,    -1,    93,    -1,    94,    -1,    95,    -1,
      96,    -1,     7,    16,   102,    17,    97,    -1,     7,    16,
     102,    17,    97,     8,    97,    -1,     7,    16,   102,    17,
      97,    87,    -1,     7,    16,   102,    17,    97,    87,     8,
      97,    -1,    88,    -1,    87,    88,    -1,     8,     7,    16,
     102,    17,    97,    -1,   120,     9,    16,   102,    17,    97,
      -1,   120,    11,    16,    92,    22,    92,    22,    92,    17,
      97,    -1,   120,    10,    97,     9,    16,   102,    17,    22,
      -1,    -1,   102,    -1,    12,    92,    22,    -1,    13,   126,
      22,    -1,    14,   126,    22,    -1,   125,     6,    22,    -1,
     125,     6,    25,   102,    22,    -1,    -1,    18,    98,    84,
      19,    -1,    18,    19,    -1,   125,     6,    16,   100,    17,
      97,    -1,   125,     6,    16,    17,    97,    -1,   125,     6,
      16,   100,    17,    22,    -1,   125,     6,    16,    17,    22,
      -1,   125,     6,    -1,   100,    24,   125,     6,    -1,   103,
      -1,   101,    24,   103,    -1,   103,    -1,   102,    24,   103,
      -1,   104,    -1,   112,   121,   103,    -1,   105,    -1,   104,
      27,   105,    -1,   106,    -1,   105,    26,   106,    -1,   107,
      -1,   106,    28,   107,    -1,   106,    29,   107,    -1,   108,
      -1,   107,    30,   108,    -1,   107,    31,   108,    -1,   107,
      32,   108,    -1,   107,    33,   108,    -1,   109,    -1,   108,
      34,   109,    -1,   108,    35,   109,    -1,   110,    -1,   109,
      36,   110,    -1,   109,    37,   110,    -1,   109,    38,   110,
      -1,   111,    -1,    35,   110,    -1,    41,   110,    -1,   112,
      -1,   112,    48,    -1,   112,    49,    -1,   112,    72,   125,
      -1,   113,    -1,   115,    -1,     6,    -1,   113,    20,   102,
      21,    -1,     6,    20,   102,    21,    -1,   112,    42,     6,
      -1,   112,    16,   101,    17,    -1,   112,    16,    17,    -1,
      16,   102,    17,    -1,   112,    73,   125,    74,    -1,     3,
      -1,     4,    -1,     5,    -1,    39,    -1,    40,    -1,    15,
      -1,   114,    -1,    69,    -1,    70,    -1,    59,     6,    16,
      17,    -1,    59,     6,    16,   101,    17,    -1,    59,     6,
      42,     6,    16,    17,    -1,    59,     6,    42,     6,    16,
     101,    17,    -1,    18,   119,    19,    -1,    18,   119,    24,
      19,    -1,    59,   122,   116,    -1,    59,   122,   116,   118,
      -1,    59,   123,   116,    -1,    59,   123,   116,   118,    -1,
     117,    -1,   116,   117,    -1,    20,   102,    21,    -1,    20,
      21,    -1,   118,    20,    21,    -1,    -1,   103,    -1,   119,
      24,   103,    -1,    -1,     6,    23,    -1,    25,    -1,    43,
      -1,    44,    -1,    45,    -1,    46,    -1,    47,    -1,    54,
      -1,    55,    -1,    56,    -1,    57,    -1,    58,    -1,     6,
      -1,   122,    20,    21,    -1,     6,    20,    21,    -1,   124,
      20,    21,    -1,   122,    -1,   124,    -1,   123,    -1,    -1,
       6,    -1,    -1,   134,     6,   135,    18,   128,   137,    19,
      -1,    -1,   132,   134,     6,   135,    18,   129,   137,    19,
      -1,    -1,   134,     6,   135,    18,   130,    19,    -1,    -1,
     132,   134,     6,   135,    18,   131,    19,    -1,   133,    -1,
     132,   133,    -1,   142,    -1,    66,    -1,    67,    -1,    68,
      -1,    62,    -1,    63,    -1,    -1,    23,   136,    -1,     6,
      -1,   136,    24,     6,    -1,   138,    -1,   137,   138,    -1,
     139,    -1,   143,    -1,   140,    -1,   132,   140,    -1,   141,
      -1,   132,   141,    -1,   125,     6,    16,   100,    17,    97,
      -1,   125,     6,    16,    17,    97,    -1,   125,     6,    16,
     100,    17,    22,    -1,   125,     6,    16,    17,    22,    -1,
      71,     6,    16,   100,    17,    97,    -1,    71,     6,    16,
      17,    97,    -1,    71,     6,    16,   100,    17,    22,    -1,
      71,     6,    16,    17,    22,    -1,    64,    -1,    65,    -1,
     125,     6,    22,    -1,   132,   125,     6,    22,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    80,    80,    81,    86,    89,    93,    97,   104,   105,
     112,   119,   123,   130,   131,   138,   145,   146,   147,   157,
     161,   168,   172,   173,   174,   175,   176,   177,   178,   179,
     183,   187,   191,   195,   202,   203,   210,   217,   224,   232,
     240,   243,   247,   254,   261,   268,   272,   280,   279,   287,
     295,   299,   303,   307,   314,   318,   325,   329,   336,   337,
     344,   345,   352,   353,   359,   360,   367,   368,   372,   379,
     380,   384,   388,   392,   399,   400,   404,   411,   412,   416,
     420,   427,   428,   432,   439,   440,   444,   448,   455,   456,
     457,   464,   468,   473,   477,   481,   485,   489,   493,   494,
     495,   496,   500,   504,   508,   509,   513,   517,   521,   525,
     529,   536,   540,   547,   551,   555,   559,   566,   567,   574,
     581,   585,   593,   596,   600,   608,   611,   618,   622,   626,
     630,   634,   638,   645,   649,   653,   657,   661,   668,   675,
     680,   685,   692,   696,   697,   702,   705,   710,   709,   718,
     717,   726,   725,   734,   733,   744,   745,   752,   753,   757,
     761,   768,   772,   780,   783,   790,   794,   801,   802,   809,
     810,   814,   818,   822,   826,   833,   837,   841,   845,   852,
     856,   860,   864,   871,   875,   882,   886
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INT_LITERAL", "DOUBLE_LITERAL",
  "STRING_LITERAL", "IDENTIFIER", "IF", "ELSE", "WHILE", "DO_T", "FOR",
  "RETURN_T", "BREAK", "CONTINUE", "NULL_T", "LEFTP", "RIGHTP", "LC", "RC",
  "LB", "RB", "SEMICOLON", "COLON", "COMMA", "ASSIGN_T", "LOGICAL_AND",
  "LOGICAL_OR", "EQ", "NE", "GT", "GE", "LT", "LE", "ADD", "SUB", "MUL",
  "DIV", "MOD", "TRUE_T", "FALSE_T", "EXCLAMATION", "DOT", "ADD_ASSIGN_T",
  "SUB_ASSIGN_T", "MUL_ASSIGN_T", "DIV_ASSIGN_T", "MOD_ASSIGN_T",
  "INCREMENT", "DECREMENT", "TRY", "CATCH", "FINALLY", "THROW", "VOID_T",
  "BOOLEAN_T", "INT_T", "DOUBLE_T", "STRING_T", "NEW", "REQUIRE", "RENAME",
  "CLASS_T", "INTERFACE_T", "PUBLIC_T", "PRIVATE_T", "VIRTUAL_T",
  "OVERRIDE_T", "ABSTRACT_T", "THIS_T", "SUPER_T", "CONSTRUCTOR",
  "INSTANCEOF", "DOWN_CAST_BEGIN", "DOWN_CAST_END", "$accept",
  "translation_unit", "initial_declaration", "require_list",
  "require_declaration", "package_name", "rename_list",
  "rename_declaration", "definition_or_statement", "statement_list",
  "statement", "if_statement", "elsif_list", "elsif", "while_statement",
  "for_statement", "do_while_statement", "expression_opt",
  "return_statement", "break_statement", "continue_statement",
  "declaration_statement", "block", "@1", "function_definition",
  "parameter_list", "argument_list", "expression", "assignment_expression",
  "logical_or_expression", "logical_and_expression", "equality_expression",
  "relational_expression", "additive_expression",
  "multiplicative_expression", "unary_expression", "postfix_expression",
  "primary_expression", "primary_no_new_array", "array_literal",
  "array_creation", "dimension_expression_list", "dimension_expression",
  "dimension_list", "expression_list", "label_opt", "assignment_operator",
  "basic_type_specifier", "class_type_specifier", "array_type_specifier",
  "type_specifier", "identifier_opt", "class_definition", "$@2", "$@3",
  "$@4", "$@5", "class_or_member_modifier_list",
  "class_or_member_modifier", "class_or_interface", "extends",
  "extends_list", "member_declaration_list", "member_declaration",
  "method_member", "method_function_definition", "constructor_definition",
  "access_modifier", "field_member", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    75,    76,    76,    77,    77,    77,    77,    78,    78,
      79,    80,    80,    81,    81,    82,    83,    83,    83,    84,
      84,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      86,    86,    86,    86,    87,    87,    88,    89,    90,    91,
      92,    92,    93,    94,    95,    96,    96,    98,    97,    97,
      99,    99,    99,    99,   100,   100,   101,   101,   102,   102,
     103,   103,   104,   104,   105,   105,   106,   106,   106,   107,
     107,   107,   107,   107,   108,   108,   108,   109,   109,   109,
     109,   110,   110,   110,   111,   111,   111,   111,   112,   112,
     112,   113,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   114,   114,   115,   115,   115,   115,   116,   116,   117,
     118,   118,   119,   119,   119,   120,   120,   121,   121,   121,
     121,   121,   121,   122,   122,   122,   122,   122,   123,   124,
     124,   124,   125,   125,   125,   126,   126,   128,   127,   129,
     127,   130,   127,   131,   127,   132,   132,   133,   133,   133,
     133,   134,   134,   135,   135,   136,   136,   137,   137,   138,
     138,   139,   139,   139,   139,   140,   140,   140,   140,   141,
     141,   141,   141,   142,   142,   143,   143
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     0,     2,     1,     1,     1,     2,
       3,     1,     3,     1,     2,     4,     1,     1,     1,     1,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       5,     7,     6,     8,     1,     2,     6,     6,    10,     8,
       0,     1,     3,     3,     3,     3,     5,     0,     4,     2,
       6,     5,     6,     5,     2,     4,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     3,     1,
       3,     3,     3,     3,     1,     3,     3,     1,     3,     3,
       3,     1,     2,     2,     1,     2,     2,     3,     1,     1,
       1,     4,     4,     3,     4,     3,     3,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     5,     6,
       7,     3,     4,     3,     4,     3,     4,     1,     2,     3,
       2,     3,     0,     1,     3,     0,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     3,     1,     1,     1,     0,     1,     0,     7,     0,
       8,     0,     6,     0,     7,     1,     2,     1,     1,     1,
       1,     1,     1,     0,     2,     1,     3,     1,     2,     1,
       1,     1,     2,     1,     2,     6,     5,     6,     5,     6,
       5,     6,     5,     1,     1,     3,     4
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,     0,     0,   125,   125,     6,     8,     7,    13,    11,
       0,     0,     1,    98,    99,   100,    90,     0,    40,   145,
     145,   103,     0,   122,     0,   101,   102,     0,   133,   134,
     135,   136,   137,     0,   161,   162,   183,   184,   158,   159,
     160,   105,   106,     3,    18,    22,    23,    24,    25,    26,
      27,    28,    29,    16,     0,    58,    60,    62,    64,    66,
      69,    74,    77,    81,    84,    88,   104,    89,     0,   142,
     144,   143,     0,    17,     0,   155,     0,   157,     2,     9,
       5,    14,    10,     0,     0,     0,   126,     0,    90,     0,
      41,   146,     0,     0,     0,   123,     0,    82,    84,    83,
     138,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   127,
       0,   128,   129,   130,   131,   132,    85,    86,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   156,     0,
     163,    12,    15,   140,     0,     0,     0,    42,    43,    44,
      96,   111,     0,     0,     0,     0,   113,   117,   115,    59,
      63,    65,    67,    68,    70,    71,    72,    73,    75,    76,
      78,    79,    80,    95,     0,    56,    93,   138,    87,     0,
      61,     0,     0,    47,     0,    40,   139,   141,     0,    45,
       0,   163,     0,     0,    92,     0,   112,   124,   107,     0,
       0,     0,     0,   118,   114,   116,    94,     0,     0,    97,
      91,     0,    49,   125,     0,     0,     0,     0,     0,     0,
       0,   165,   164,   147,    30,   108,     0,   119,   120,     0,
      57,     0,   125,    19,     0,     0,    40,    53,    51,     0,
       0,    54,    46,   149,     0,     0,     0,     0,    32,    34,
     109,     0,   121,    37,    48,    20,     0,     0,     0,    52,
      50,     0,     0,     0,   166,     0,     0,     0,     0,   167,
     169,   171,   173,   170,   152,     0,    31,     0,    35,   110,
       0,    40,    55,     0,   154,     0,     0,     0,   172,   174,
     148,   168,     0,    33,    39,     0,   150,     0,     0,   185,
       0,     0,     0,     0,     0,     0,     0,   186,     0,    38,
     182,   180,     0,   178,   176,     0,    36,   181,   179,   177,
     175
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    10,     7,     8,    43,   232,
      44,    45,   248,   249,    46,    47,    48,    89,    49,    50,
      51,    52,   184,   213,    53,   217,   174,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,   156,   157,   204,    96,    68,   130,    69,    70,    71,
     266,    92,    73,   245,   262,   246,   263,   267,    75,    76,
     193,   222,   268,   269,   270,   271,   272,    77,   273
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -248
static const yytype_int16 yypact[] =
{
     221,    32,    32,   176,   244,   221,  -248,   -47,  -248,  -248,
      -9,     4,  -248,  -248,  -248,  -248,    37,    57,   532,   108,
     108,  -248,   532,   532,   532,  -248,  -248,   532,  -248,  -248,
    -248,  -248,  -248,   167,  -248,  -248,  -248,  -248,  -248,  -248,
    -248,  -248,  -248,  -248,  -248,  -248,  -248,  -248,  -248,  -248,
    -248,  -248,  -248,  -248,   239,  -248,   140,    97,   268,   222,
     270,   284,  -248,  -248,   348,   117,  -248,  -248,   217,   150,
    -248,   158,   187,  -248,   224,  -248,   192,  -248,  -248,  -248,
     -47,  -248,  -248,   201,   214,    13,  -248,   532,   164,   271,
     299,  -248,   307,   310,    27,  -248,    26,  -248,    -7,  -248,
       7,   194,   194,  -248,   532,   532,   532,   532,   532,   532,
     532,   532,   532,   532,   532,   532,   532,   532,   398,  -248,
     331,  -248,  -248,  -248,  -248,  -248,  -248,  -248,   212,   212,
     532,   532,   326,   325,   328,   324,   327,    42,  -248,   340,
     332,  -248,  -248,  -248,     1,    44,   532,  -248,  -248,  -248,
    -248,  -248,   440,   447,   343,   532,   330,  -248,   330,  -248,
      97,   268,   222,   222,   270,   270,   270,   270,   284,   284,
    -248,  -248,  -248,  -248,    73,  -248,  -248,   334,  -248,   282,
    -248,    70,   532,   338,   349,   532,  -248,  -248,   100,  -248,
     532,   332,   353,   342,  -248,   325,  -248,  -248,  -248,    78,
     345,    75,   486,  -248,   352,   352,  -248,   532,   341,  -248,
    -248,    83,  -248,   371,   347,   357,    52,    92,   359,   253,
     362,  -248,   364,   379,   391,  -248,   525,  -248,  -248,   384,
    -248,   325,   312,  -248,   394,   532,   532,  -248,  -248,    90,
     212,  -248,  -248,   388,   402,   569,   390,    19,   409,  -248,
    -248,    96,  -248,  -248,  -248,  -248,   113,   110,   396,  -248,
    -248,   413,   569,   403,  -248,   417,   418,   569,    21,  -248,
    -248,  -248,  -248,  -248,  -248,   415,  -248,    19,  -248,  -248,
     410,   532,  -248,   551,  -248,   419,     8,   428,  -248,  -248,
    -248,  -248,   532,  -248,  -248,   425,  -248,   105,   147,  -248,
     155,   112,   325,   177,   148,   178,   151,  -248,   325,  -248,
    -248,  -248,   190,  -248,  -248,   191,  -248,  -248,  -248,  -248,
    -248
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -248,  -248,  -248,  -248,   431,   445,   443,     0,   450,  -248,
    -193,  -248,  -248,   213,  -248,  -248,  -248,  -180,  -248,  -248,
    -248,  -248,  -184,  -248,  -248,    36,  -145,   -16,   -20,  -248,
     344,   354,   228,   162,   225,   -12,  -248,    35,  -248,  -248,
    -248,   367,   122,   308,  -248,  -248,  -248,   437,   438,  -248,
      -3,   452,  -248,  -248,  -248,  -248,  -248,   337,   -70,   399,
     283,  -248,   215,  -247,  -248,   209,   211,  -248,  -248
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -154
static const yytype_int16 yytable[] =
{
      72,    72,    90,    95,   138,   215,    94,    81,   199,   118,
      84,   224,    97,    82,     2,    99,    13,    14,    15,    88,
     233,   291,   194,   153,   298,   104,   275,   177,    21,    22,
     299,    23,   238,    83,   143,   120,   291,   183,     9,   255,
     290,   126,   127,  -138,   150,   151,    83,   253,    24,   154,
     152,   104,    25,    26,    27,   260,   258,    85,   188,    98,
      86,   195,    98,   276,   189,   128,   129,   190,   104,   144,
     183,   145,    33,    87,   237,    28,    29,    30,    31,    32,
      81,   251,    41,    42,   159,    36,    37,    38,    39,    40,
     206,   210,   265,   293,   104,   225,   227,   207,   175,   104,
     231,   295,   207,   170,   171,   172,   177,   104,   183,   239,
     180,   177,   259,   279,    91,   181,   240,   216,   309,   311,
     207,   314,   303,   106,   316,   178,   179,   280,   318,   308,
     144,   320,   197,   175,   104,   189,   104,   131,   190,   201,
      98,    98,    98,    98,    98,    98,    98,    98,    98,    98,
      98,    98,    98,   177,    28,    29,    30,    31,    32,    28,
      29,    30,    31,    32,   305,   312,   211,   105,   315,    90,
     135,   298,   240,   100,   219,   240,    12,   307,   136,    13,
      14,    15,    16,    17,   146,   218,   201,   230,    18,    19,
      20,    21,    22,   137,    23,   183,   183,   138,   140,   310,
     313,    28,    29,    30,    31,    32,   175,   141,   183,   183,
     234,    24,   317,   319,   155,    25,    26,    27,   177,   257,
      90,    28,    29,    30,    31,    32,   132,   133,   134,   234,
      28,    29,    30,    31,    32,    33,   142,   261,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    13,    14,    15,
      16,    17,   109,   110,   111,   112,    18,    19,    20,    21,
      22,   103,    23,   104,   287,    90,    28,    29,    30,    31,
      32,   164,   165,   166,   167,   242,   301,   104,   203,    24,
     203,     1,     2,    25,    26,    27,    34,    35,    36,    37,
      38,    39,    40,   147,   218,   218,   107,   108,    28,    29,
      30,    31,    32,    33,   113,   114,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    13,    14,    15,    16,    17,
     115,   116,   117,   104,    18,    19,    20,    21,    22,   148,
      23,   254,   149,   304,   306,   162,   163,   176,   168,   169,
      74,    74,   182,   183,   185,   186,   191,    24,   187,   200,
     202,    25,    26,    27,   208,   192,   209,   212,   214,   221,
     223,   226,   143,   235,   118,   241,    28,    29,    30,    31,
      32,    33,   229,   119,    13,    14,    15,    16,    17,   236,
     243,    41,    42,    18,    19,    20,    21,    22,   244,    23,
     120,   121,   122,   123,   124,   125,   126,   127,  -151,   247,
     256,    13,    14,    15,    88,   252,    24,  -153,   264,   274,
      25,    26,    27,    21,    22,   173,    23,   277,   281,   282,
     128,   129,   284,   285,   286,    28,    29,    30,    31,    32,
      33,   292,   294,    24,   300,   297,    79,    25,    26,    27,
      41,    42,   302,    13,    14,    15,    88,    11,    80,   160,
      13,    14,    15,    88,    78,    21,    22,    33,    23,   196,
     161,   278,    21,    22,   198,    23,   205,    41,    42,   158,
     101,   102,    93,   139,   220,    24,   288,   283,   289,    25,
      26,    27,    24,     0,     0,     0,    25,    26,    27,    13,
      14,    15,    88,     0,     0,     0,     0,     0,     0,    33,
       0,    21,    22,     0,    23,     0,    33,   228,     0,    41,
      42,     0,     0,     0,     0,     0,    41,    42,     0,     0,
       0,    24,     0,     0,     0,    25,    26,    27,    13,    14,
      15,    88,     0,     0,     0,    13,    14,    15,    88,     0,
      21,    22,   250,    23,     0,    33,     0,    21,    22,     0,
      23,     0,     0,     0,     0,    41,    42,   177,     0,     0,
      24,     0,     0,     0,    25,    26,    27,    24,     0,     0,
     296,    25,    26,    27,     0,   177,     0,     0,     0,     0,
       0,     0,     0,     0,    33,     0,     0,     0,     0,     0,
       0,    33,     0,     0,    41,    42,     0,     0,     0,     0,
       0,    41,    42,     0,     0,    28,    29,    30,    31,    32,
       0,     0,     0,     0,     0,    36,    37,    38,    39,    40,
       0,     0,   265,    28,    29,    30,    31,    32,     0,     0,
       0,     0,     0,    36,    37,    38,    39,    40,     0,     0,
     265
};

static const yytype_int16 yycheck[] =
{
       3,     4,    18,    23,    74,   185,    22,     7,   153,    16,
       6,   195,    24,    22,    61,    27,     3,     4,     5,     6,
     213,   268,    21,    16,    16,    24,     7,     6,    15,    16,
      22,    18,   216,    42,    21,    42,   283,    18,     6,   232,
      19,    48,    49,     6,    17,    19,    42,   231,    35,    42,
      24,    24,    39,    40,    41,   239,   236,    20,    16,    24,
      23,    17,    27,   247,    22,    72,    73,    25,    24,    85,
      18,    87,    59,    16,    22,    54,    55,    56,    57,    58,
      80,   226,    69,    70,   104,    64,    65,    66,    67,    68,
      17,    21,    71,   277,    24,    17,    21,    24,   118,    24,
      17,   281,    24,   115,   116,   117,     6,    24,    18,    17,
     130,     6,    22,    17,     6,   131,    24,    17,   302,   303,
      24,   305,    17,    26,   308,   128,   129,    17,   312,    17,
     146,   315,   152,   153,    24,    22,    24,    20,    25,   155,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,     6,    54,    55,    56,    57,    58,    54,
      55,    56,    57,    58,    17,    17,   182,    27,    17,   185,
      20,    16,    24,     6,   190,    24,     0,    22,    20,     3,
       4,     5,     6,     7,    20,   188,   202,   207,    12,    13,
      14,    15,    16,     6,    18,    18,    18,   267,     6,    22,
      22,    54,    55,    56,    57,    58,   226,     6,    18,    18,
     213,    35,    22,    22,    20,    39,    40,    41,     6,   235,
     236,    54,    55,    56,    57,    58,     9,    10,    11,   232,
      54,    55,    56,    57,    58,    59,    22,   240,    62,    63,
      64,    65,    66,    67,    68,    69,    70,     3,     4,     5,
       6,     7,    30,    31,    32,    33,    12,    13,    14,    15,
      16,    22,    18,    24,   267,   281,    54,    55,    56,    57,
      58,   109,   110,   111,   112,    22,   292,    24,   156,    35,
     158,    60,    61,    39,    40,    41,    62,    63,    64,    65,
      66,    67,    68,    22,   297,   298,    28,    29,    54,    55,
      56,    57,    58,    59,    34,    35,    62,    63,    64,    65,
      66,    67,    68,    69,    70,     3,     4,     5,     6,     7,
      36,    37,    38,    24,    12,    13,    14,    15,    16,    22,
      18,    19,    22,   297,   298,   107,   108,     6,   113,   114,
       3,     4,    16,    18,    16,    21,     6,    35,    21,     6,
      20,    39,    40,    41,    20,    23,    74,    19,     9,     6,
      18,    16,    21,    16,    16,     6,    54,    55,    56,    57,
      58,    59,    20,    25,     3,     4,     5,     6,     7,    22,
      18,    69,    70,    12,    13,    14,    15,    16,    24,    18,
      42,    43,    44,    45,    46,    47,    48,    49,    19,     8,
       6,     3,     4,     5,     6,    21,    35,    19,     6,    19,
      39,    40,    41,    15,    16,    17,    18,     8,    22,     6,
      72,    73,    19,     6,     6,    54,    55,    56,    57,    58,
      59,    16,    22,    35,     6,    16,     5,    39,    40,    41,
      69,    70,    17,     3,     4,     5,     6,     2,     5,   105,
       3,     4,     5,     6,     4,    15,    16,    59,    18,    19,
     106,   248,    15,    16,    17,    18,   158,    69,    70,   102,
      33,    33,    20,    74,   191,    35,   267,   262,   267,    39,
      40,    41,    35,    -1,    -1,    -1,    39,    40,    41,     3,
       4,     5,     6,    -1,    -1,    -1,    -1,    -1,    -1,    59,
      -1,    15,    16,    -1,    18,    -1,    59,    21,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,     3,     4,
       5,     6,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      15,    16,    17,    18,    -1,    59,    -1,    15,    16,    -1,
      18,    -1,    -1,    -1,    -1,    69,    70,     6,    -1,    -1,
      35,    -1,    -1,    -1,    39,    40,    41,    35,    -1,    -1,
      19,    39,    40,    41,    -1,     6,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,
      -1,    59,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    69,    70,    -1,    -1,    54,    55,    56,    57,    58,
      -1,    -1,    -1,    -1,    -1,    64,    65,    66,    67,    68,
      -1,    -1,    71,    54,    55,    56,    57,    58,    -1,    -1,
      -1,    -1,    -1,    64,    65,    66,    67,    68,    -1,    -1,
      71
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    60,    61,    76,    77,    78,    79,    81,    82,     6,
      80,    80,     0,     3,     4,     5,     6,     7,    12,    13,
      14,    15,    16,    18,    35,    39,    40,    41,    54,    55,
      56,    57,    58,    59,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    83,    85,    86,    89,    90,    91,    93,
      94,    95,    96,    99,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   120,   122,
     123,   124,   125,   127,   132,   133,   134,   142,    83,    79,
      81,    82,    22,    42,     6,    20,    23,    16,     6,    92,
     102,     6,   126,   126,   102,   103,   119,   110,   112,   110,
       6,   122,   123,    22,    24,    27,    26,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    16,    25,
      42,    43,    44,    45,    46,    47,    48,    49,    72,    73,
     121,    20,     9,    10,    11,    20,    20,     6,   133,   134,
       6,     6,    22,    21,   102,   102,    20,    22,    22,    22,
      17,    19,    24,    16,    42,    20,   116,   117,   116,   103,
     105,   106,   107,   107,   108,   108,   108,   108,   109,   109,
     110,   110,   110,    17,   101,   103,     6,     6,   125,   125,
     103,   102,    16,    18,    97,    16,    21,    21,    16,    22,
      25,     6,    23,   135,    21,    17,    19,   103,    17,   101,
       6,   102,    20,   117,   118,   118,    17,    24,    20,    74,
      21,   102,    19,    98,     9,    92,    17,   100,   125,   102,
     135,     6,   136,    18,    97,    17,    16,    21,    21,    20,
     103,    17,    84,    85,   125,    16,    22,    22,    97,    17,
      24,     6,    22,    18,    24,   128,   130,     8,    87,    88,
      17,   101,    21,    97,    19,    85,     6,   102,    92,    22,
      97,   125,   129,   131,     6,    71,   125,   132,   137,   138,
     139,   140,   141,   143,    19,     7,    97,     8,    88,    17,
      17,    22,     6,   137,    19,     6,     6,   125,   140,   141,
      19,   138,    16,    97,    22,    92,    19,    16,    16,    22,
       6,   102,    17,    17,   100,    17,   100,    22,    17,    97,
      22,    97,    17,    22,    97,    17,    97,    22,    97,    22,
      97
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:

/* Line 1464 of yacc.c  */
#line 86 "DikSam.y"
    {
            dkc_set_require_and_rename_list(NULL, NULL);
        }
    break;

  case 5:

/* Line 1464 of yacc.c  */
#line 90 "DikSam.y"
    {
            dkc_set_require_and_rename_list((yyvsp[(1) - (2)].require_list), (yyvsp[(2) - (2)].rename_list));
        }
    break;

  case 6:

/* Line 1464 of yacc.c  */
#line 94 "DikSam.y"
    {
            dkc_set_require_and_rename_list((yyvsp[(1) - (1)].require_list), NULL);
        }
    break;

  case 7:

/* Line 1464 of yacc.c  */
#line 98 "DikSam.y"
    {
            dkc_set_require_and_rename_list(NULL, (yyvsp[(1) - (1)].rename_list));
        }
    break;

  case 9:

/* Line 1464 of yacc.c  */
#line 106 "DikSam.y"
    {
            (yyval.require_list) = dkc_chain_require_list((yyvsp[(1) - (2)].require_list), (yyvsp[(2) - (2)].require_list));
        }
    break;

  case 10:

/* Line 1464 of yacc.c  */
#line 113 "DikSam.y"
    {
            (yyval.require_list) = dkc_create_require_list((yyvsp[(2) - (3)].package_name));
        }
    break;

  case 11:

/* Line 1464 of yacc.c  */
#line 120 "DikSam.y"
    {
            (yyval.package_name) = dkc_create_package_name((yyvsp[(1) - (1)].identifier));
        }
    break;

  case 12:

/* Line 1464 of yacc.c  */
#line 124 "DikSam.y"
    {
            (yyval.package_name) = dkc_chain_package_name((yyvsp[(1) - (3)].package_name), (yyvsp[(3) - (3)].identifier));
        }
    break;

  case 14:

/* Line 1464 of yacc.c  */
#line 132 "DikSam.y"
    {
            (yyval.rename_list) = dkc_chain_rename_list((yyvsp[(1) - (2)].rename_list), (yyvsp[(2) - (2)].rename_list));
        }
    break;

  case 15:

/* Line 1464 of yacc.c  */
#line 139 "DikSam.y"
    {
            (yyval.rename_list) = dkc_create_rename_list((yyvsp[(2) - (4)].package_name), (yyvsp[(3) - (4)].identifier));
        }
    break;

  case 18:

/* Line 1464 of yacc.c  */
#line 148 "DikSam.y"
    {
            DKC_Compiler *compiler = dkc_get_current_compiler();

            compiler->statement_list
                = dkc_chain_statement_list(compiler->statement_list, (yyvsp[(1) - (1)].statement));
        }
    break;

  case 19:

/* Line 1464 of yacc.c  */
#line 158 "DikSam.y"
    {
            (yyval.statement_list) = dkc_create_statement_list((yyvsp[(1) - (1)].statement));
        }
    break;

  case 20:

/* Line 1464 of yacc.c  */
#line 162 "DikSam.y"
    {
            (yyval.statement_list) = dkc_chain_statement_list((yyvsp[(1) - (2)].statement_list), (yyvsp[(2) - (2)].statement));
        }
    break;

  case 21:

/* Line 1464 of yacc.c  */
#line 169 "DikSam.y"
    {
          (yyval.statement) = dkc_create_expression_statement((yyvsp[(1) - (2)].expression));
        }
    break;

  case 30:

/* Line 1464 of yacc.c  */
#line 184 "DikSam.y"
    {
            (yyval.statement) = dkc_create_if_statement((yyvsp[(3) - (5)].expression), (yyvsp[(5) - (5)].block), NULL, NULL);
        }
    break;

  case 31:

/* Line 1464 of yacc.c  */
#line 188 "DikSam.y"
    {
            (yyval.statement) = dkc_create_if_statement((yyvsp[(3) - (7)].expression), (yyvsp[(5) - (7)].block), NULL, (yyvsp[(7) - (7)].block));
        }
    break;

  case 32:

/* Line 1464 of yacc.c  */
#line 192 "DikSam.y"
    {
            (yyval.statement) = dkc_create_if_statement((yyvsp[(3) - (6)].expression), (yyvsp[(5) - (6)].block), (yyvsp[(6) - (6)].elsif), NULL);
        }
    break;

  case 33:

/* Line 1464 of yacc.c  */
#line 196 "DikSam.y"
    {
            (yyval.statement) = dkc_create_if_statement((yyvsp[(3) - (8)].expression), (yyvsp[(5) - (8)].block), (yyvsp[(6) - (8)].elsif), (yyvsp[(8) - (8)].block));
        }
    break;

  case 35:

/* Line 1464 of yacc.c  */
#line 204 "DikSam.y"
    {
            (yyval.elsif) = dkc_chain_elsif_list((yyvsp[(1) - (2)].elsif), (yyvsp[(2) - (2)].elsif));
        }
    break;

  case 36:

/* Line 1464 of yacc.c  */
#line 211 "DikSam.y"
    {
            (yyval.elsif) = dkc_create_elsif((yyvsp[(4) - (6)].expression), (yyvsp[(6) - (6)].block));
        }
    break;

  case 37:

/* Line 1464 of yacc.c  */
#line 218 "DikSam.y"
    {
            (yyval.statement) = dkc_create_while_statement((yyvsp[(1) - (6)].identifier), (yyvsp[(4) - (6)].expression), (yyvsp[(6) - (6)].block));
        }
    break;

  case 38:

/* Line 1464 of yacc.c  */
#line 226 "DikSam.y"
    {
            (yyval.statement) = dkc_create_for_statement((yyvsp[(1) - (10)].identifier), (yyvsp[(4) - (10)].expression), (yyvsp[(6) - (10)].expression), (yyvsp[(8) - (10)].expression), (yyvsp[(10) - (10)].block));
        }
    break;

  case 39:

/* Line 1464 of yacc.c  */
#line 233 "DikSam.y"
    {
            (yyval.statement) = dkc_create_do_while_statement((yyvsp[(1) - (8)].identifier), (yyvsp[(3) - (8)].block), (yyvsp[(6) - (8)].expression));
        }
    break;

  case 40:

/* Line 1464 of yacc.c  */
#line 240 "DikSam.y"
    {
            (yyval.expression) = NULL;
        }
    break;

  case 42:

/* Line 1464 of yacc.c  */
#line 248 "DikSam.y"
    {
            (yyval.statement) = dkc_create_return_statement((yyvsp[(2) - (3)].expression));
        }
    break;

  case 43:

/* Line 1464 of yacc.c  */
#line 255 "DikSam.y"
    {
            (yyval.statement) = dkc_create_break_statement((yyvsp[(2) - (3)].identifier));
        }
    break;

  case 44:

/* Line 1464 of yacc.c  */
#line 262 "DikSam.y"
    {
            (yyval.statement) = dkc_create_continue_statement((yyvsp[(2) - (3)].identifier));
        }
    break;

  case 45:

/* Line 1464 of yacc.c  */
#line 269 "DikSam.y"
    {
            (yyval.statement) = dkc_create_declaration_statement((yyvsp[(1) - (3)].type_specifier), (yyvsp[(2) - (3)].identifier), NULL);
        }
    break;

  case 46:

/* Line 1464 of yacc.c  */
#line 273 "DikSam.y"
    {
            (yyval.statement) = dkc_create_declaration_statement((yyvsp[(1) - (5)].type_specifier), (yyvsp[(2) - (5)].identifier), (yyvsp[(4) - (5)].expression));
        }
    break;

  case 47:

/* Line 1464 of yacc.c  */
#line 280 "DikSam.y"
    {
            (yyval.block) = dkc_open_block();
        }
    break;

  case 48:

/* Line 1464 of yacc.c  */
#line 284 "DikSam.y"
    {
            (yyval.block) = dkc_close_block((yyvsp[(2) - (4)].block), (yyvsp[(3) - (4)].statement_list));
        }
    break;

  case 49:

/* Line 1464 of yacc.c  */
#line 288 "DikSam.y"
    {
            Block *empty_block = dkc_open_block();
            (yyval.block) = dkc_close_block(empty_block, NULL);
        }
    break;

  case 50:

/* Line 1464 of yacc.c  */
#line 296 "DikSam.y"
    {
            dkc_function_define((yyvsp[(1) - (6)].type_specifier), (yyvsp[(2) - (6)].identifier), (yyvsp[(4) - (6)].parameter_list), (yyvsp[(6) - (6)].block));
        }
    break;

  case 51:

/* Line 1464 of yacc.c  */
#line 300 "DikSam.y"
    {
            dkc_function_define((yyvsp[(1) - (5)].type_specifier), (yyvsp[(2) - (5)].identifier), NULL, (yyvsp[(5) - (5)].block));
        }
    break;

  case 52:

/* Line 1464 of yacc.c  */
#line 304 "DikSam.y"
    {
            dkc_function_define((yyvsp[(1) - (6)].type_specifier), (yyvsp[(2) - (6)].identifier), (yyvsp[(4) - (6)].parameter_list), NULL);
        }
    break;

  case 53:

/* Line 1464 of yacc.c  */
#line 308 "DikSam.y"
    {
            dkc_function_define((yyvsp[(1) - (5)].type_specifier), (yyvsp[(2) - (5)].identifier), NULL, NULL);
        }
    break;

  case 54:

/* Line 1464 of yacc.c  */
#line 315 "DikSam.y"
    {
            (yyval.parameter_list) = dkc_create_parameter((yyvsp[(1) - (2)].type_specifier), (yyvsp[(2) - (2)].identifier));
        }
    break;

  case 55:

/* Line 1464 of yacc.c  */
#line 319 "DikSam.y"
    {
            (yyval.parameter_list) = dkc_chain_parameter((yyvsp[(1) - (4)].parameter_list), (yyvsp[(3) - (4)].type_specifier), (yyvsp[(4) - (4)].identifier));
        }
    break;

  case 56:

/* Line 1464 of yacc.c  */
#line 326 "DikSam.y"
    {
            (yyval.argument_list) = dkc_create_argument_list((yyvsp[(1) - (1)].expression));
        }
    break;

  case 57:

/* Line 1464 of yacc.c  */
#line 330 "DikSam.y"
    {
            (yyval.argument_list) = dkc_chain_argument_list((yyvsp[(1) - (3)].argument_list), (yyvsp[(3) - (3)].expression));
        }
    break;

  case 59:

/* Line 1464 of yacc.c  */
#line 338 "DikSam.y"
    {
            (yyval.expression) = dkc_create_comma_expression((yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));
        }
    break;

  case 61:

/* Line 1464 of yacc.c  */
#line 346 "DikSam.y"
    {
            (yyval.expression) = dkc_create_assign_expression((yyvsp[(1) - (3)].expression), (yyvsp[(2) - (3)].assignment_operator), (yyvsp[(3) - (3)].expression));
        }
    break;

  case 63:

/* Line 1464 of yacc.c  */
#line 354 "DikSam.y"
    {
            (yyval.expression) = dkc_create_binary_expression(LOGICAL_OR_EXPRESSION, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));
        }
    break;

  case 65:

/* Line 1464 of yacc.c  */
#line 361 "DikSam.y"
    {
            (yyval.expression) = dkc_create_binary_expression(LOGICAL_AND_EXPRESSION, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));
        }
    break;

  case 67:

/* Line 1464 of yacc.c  */
#line 369 "DikSam.y"
    {
            (yyval.expression) = dkc_create_binary_expression(EQ_EXPRESSION, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));
        }
    break;

  case 68:

/* Line 1464 of yacc.c  */
#line 373 "DikSam.y"
    {
            (yyval.expression) = dkc_create_binary_expression(NE_EXPRESSION, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));
        }
    break;

  case 70:

/* Line 1464 of yacc.c  */
#line 381 "DikSam.y"
    {
            (yyval.expression) = dkc_create_binary_expression(GT_EXPRESSION, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));
        }
    break;

  case 71:

/* Line 1464 of yacc.c  */
#line 385 "DikSam.y"
    {
            (yyval.expression) = dkc_create_binary_expression(GE_EXPRESSION, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));
        }
    break;

  case 72:

/* Line 1464 of yacc.c  */
#line 389 "DikSam.y"
    {
            (yyval.expression) = dkc_create_binary_expression(LT_EXPRESSION, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));
        }
    break;

  case 73:

/* Line 1464 of yacc.c  */
#line 393 "DikSam.y"
    {
            (yyval.expression) = dkc_create_binary_expression(LE_EXPRESSION, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));
        }
    break;

  case 75:

/* Line 1464 of yacc.c  */
#line 401 "DikSam.y"
    {
            (yyval.expression) = dkc_create_binary_expression(ADD_EXPRESSION, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));
        }
    break;

  case 76:

/* Line 1464 of yacc.c  */
#line 405 "DikSam.y"
    {
            (yyval.expression) = dkc_create_binary_expression(SUB_EXPRESSION, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));
        }
    break;

  case 78:

/* Line 1464 of yacc.c  */
#line 413 "DikSam.y"
    {
            (yyval.expression) = dkc_create_binary_expression(MUL_EXPRESSION, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));
        }
    break;

  case 79:

/* Line 1464 of yacc.c  */
#line 417 "DikSam.y"
    {
            (yyval.expression) = dkc_create_binary_expression(DIV_EXPRESSION, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));
        }
    break;

  case 80:

/* Line 1464 of yacc.c  */
#line 421 "DikSam.y"
    {
            (yyval.expression) = dkc_create_binary_expression(MOD_EXPRESSION, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));
        }
    break;

  case 82:

/* Line 1464 of yacc.c  */
#line 429 "DikSam.y"
    {
            (yyval.expression) = dkc_create_minus_expression((yyvsp[(2) - (2)].expression));
        }
    break;

  case 83:

/* Line 1464 of yacc.c  */
#line 433 "DikSam.y"
    {
            (yyval.expression) = dkc_create_logical_not_expression((yyvsp[(2) - (2)].expression));
        }
    break;

  case 85:

/* Line 1464 of yacc.c  */
#line 441 "DikSam.y"
    {
            (yyval.expression) = dkc_create_incdec_expression((yyvsp[(1) - (2)].expression), INCREMENT_EXPRESSION);
        }
    break;

  case 86:

/* Line 1464 of yacc.c  */
#line 445 "DikSam.y"
    {
            (yyval.expression) = dkc_create_incdec_expression((yyvsp[(1) - (2)].expression), DECREMENT_EXPRESSION);
        }
    break;

  case 87:

/* Line 1464 of yacc.c  */
#line 449 "DikSam.y"
    {
            (yyval.expression) = dkc_create_instanceof_expression((yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].type_specifier));
        }
    break;

  case 90:

/* Line 1464 of yacc.c  */
#line 458 "DikSam.y"
    {
            (yyval.expression) = dkc_create_identifier_expression((yyvsp[(1) - (1)].identifier));
        }
    break;

  case 91:

/* Line 1464 of yacc.c  */
#line 465 "DikSam.y"
    {
            (yyval.expression) = dkc_create_index_expression((yyvsp[(1) - (4)].expression), (yyvsp[(3) - (4)].expression));
        }
    break;

  case 92:

/* Line 1464 of yacc.c  */
#line 469 "DikSam.y"
    {
            Expression *identifier = dkc_create_identifier_expression((yyvsp[(1) - (4)].identifier));
            (yyval.expression) = dkc_create_index_expression(identifier, (yyvsp[(3) - (4)].expression));
        }
    break;

  case 93:

/* Line 1464 of yacc.c  */
#line 474 "DikSam.y"
    {
            (yyval.expression) = dkc_create_member_expression((yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].identifier));
        }
    break;

  case 94:

/* Line 1464 of yacc.c  */
#line 478 "DikSam.y"
    {
            (yyval.expression) = dkc_create_function_call_expression((yyvsp[(1) - (4)].expression), (yyvsp[(3) - (4)].argument_list));
        }
    break;

  case 95:

/* Line 1464 of yacc.c  */
#line 482 "DikSam.y"
    {
            (yyval.expression) = dkc_create_function_call_expression((yyvsp[(1) - (3)].expression), NULL);
        }
    break;

  case 96:

/* Line 1464 of yacc.c  */
#line 486 "DikSam.y"
    {
            (yyval.expression) = (yyvsp[(2) - (3)].expression);
        }
    break;

  case 97:

/* Line 1464 of yacc.c  */
#line 490 "DikSam.y"
    {
            (yyval.expression) = dkc_create_down_cast_expression((yyvsp[(1) - (4)].expression), (yyvsp[(3) - (4)].type_specifier));
        }
    break;

  case 101:

/* Line 1464 of yacc.c  */
#line 497 "DikSam.y"
    {
            (yyval.expression) = dkc_create_boolean_expression(DVM_TRUE);
        }
    break;

  case 102:

/* Line 1464 of yacc.c  */
#line 501 "DikSam.y"
    {
            (yyval.expression) = dkc_create_boolean_expression(DVM_FALSE);
        }
    break;

  case 103:

/* Line 1464 of yacc.c  */
#line 505 "DikSam.y"
    {
            (yyval.expression) = dkc_create_null_expression();
        }
    break;

  case 105:

/* Line 1464 of yacc.c  */
#line 510 "DikSam.y"
    {
            (yyval.expression) = dkc_create_this_expression();
        }
    break;

  case 106:

/* Line 1464 of yacc.c  */
#line 514 "DikSam.y"
    {
            (yyval.expression) = dkc_create_super_expression();
        }
    break;

  case 107:

/* Line 1464 of yacc.c  */
#line 518 "DikSam.y"
    {
            (yyval.expression) = dkc_create_new_expression((yyvsp[(2) - (4)].identifier), NULL, NULL);
        }
    break;

  case 108:

/* Line 1464 of yacc.c  */
#line 522 "DikSam.y"
    {
            (yyval.expression) = dkc_create_new_expression((yyvsp[(2) - (5)].identifier), NULL, (yyvsp[(4) - (5)].argument_list));
        }
    break;

  case 109:

/* Line 1464 of yacc.c  */
#line 526 "DikSam.y"
    {
            (yyval.expression) = dkc_create_new_expression((yyvsp[(2) - (6)].identifier), (yyvsp[(4) - (6)].identifier), NULL);
        }
    break;

  case 110:

/* Line 1464 of yacc.c  */
#line 530 "DikSam.y"
    {
            (yyval.expression) = dkc_create_new_expression((yyvsp[(2) - (7)].identifier), (yyvsp[(4) - (7)].identifier), (yyvsp[(6) - (7)].argument_list));
        }
    break;

  case 111:

/* Line 1464 of yacc.c  */
#line 537 "DikSam.y"
    {
            (yyval.expression) = dkc_create_array_literal_expression((yyvsp[(2) - (3)].expression_list));
        }
    break;

  case 112:

/* Line 1464 of yacc.c  */
#line 541 "DikSam.y"
    {
            (yyval.expression) = dkc_create_array_literal_expression((yyvsp[(2) - (4)].expression_list));
        }
    break;

  case 113:

/* Line 1464 of yacc.c  */
#line 548 "DikSam.y"
    {
            (yyval.expression) = dkc_create_basic_array_creation((yyvsp[(2) - (3)].basic_type_specifier), (yyvsp[(3) - (3)].array_dimension), NULL);
        }
    break;

  case 114:

/* Line 1464 of yacc.c  */
#line 552 "DikSam.y"
    {
            (yyval.expression) = dkc_create_basic_array_creation((yyvsp[(2) - (4)].basic_type_specifier), (yyvsp[(3) - (4)].array_dimension), (yyvsp[(4) - (4)].array_dimension));
        }
    break;

  case 115:

/* Line 1464 of yacc.c  */
#line 556 "DikSam.y"
    {
            (yyval.expression) = dkc_create_class_array_creation((yyvsp[(2) - (3)].type_specifier), (yyvsp[(3) - (3)].array_dimension), NULL);
        }
    break;

  case 116:

/* Line 1464 of yacc.c  */
#line 560 "DikSam.y"
    {
            (yyval.expression) = dkc_create_class_array_creation((yyvsp[(2) - (4)].type_specifier), (yyvsp[(3) - (4)].array_dimension), (yyvsp[(4) - (4)].array_dimension));
        }
    break;

  case 118:

/* Line 1464 of yacc.c  */
#line 568 "DikSam.y"
    {
            (yyval.array_dimension) = dkc_chain_array_dimension((yyvsp[(1) - (2)].array_dimension), (yyvsp[(2) - (2)].array_dimension));
        }
    break;

  case 119:

/* Line 1464 of yacc.c  */
#line 575 "DikSam.y"
    {
            (yyval.array_dimension) = dkc_create_array_dimension((yyvsp[(2) - (3)].expression));
        }
    break;

  case 120:

/* Line 1464 of yacc.c  */
#line 582 "DikSam.y"
    {
            (yyval.array_dimension) = dkc_create_array_dimension(NULL);
        }
    break;

  case 121:

/* Line 1464 of yacc.c  */
#line 586 "DikSam.y"
    {
            (yyval.array_dimension) = dkc_chain_array_dimension((yyvsp[(1) - (3)].array_dimension), dkc_create_array_dimension(NULL));
        }
    break;

  case 122:

/* Line 1464 of yacc.c  */
#line 593 "DikSam.y"
    {
            (yyval.expression_list) = NULL;
        }
    break;

  case 123:

/* Line 1464 of yacc.c  */
#line 597 "DikSam.y"
    {
            (yyval.expression_list) = dkc_create_expression_list((yyvsp[(1) - (1)].expression));
        }
    break;

  case 124:

/* Line 1464 of yacc.c  */
#line 601 "DikSam.y"
    {
            (yyval.expression_list) = dkc_chain_expression_list((yyvsp[(1) - (3)].expression_list), (yyvsp[(3) - (3)].expression));
        }
    break;

  case 125:

/* Line 1464 of yacc.c  */
#line 608 "DikSam.y"
    {
            (yyval.identifier) = NULL;
        }
    break;

  case 126:

/* Line 1464 of yacc.c  */
#line 612 "DikSam.y"
    {
            (yyval.identifier) = (yyvsp[(1) - (2)].identifier);
        }
    break;

  case 127:

/* Line 1464 of yacc.c  */
#line 619 "DikSam.y"
    {
            (yyval.assignment_operator) = NORMAL_ASSIGN;
        }
    break;

  case 128:

/* Line 1464 of yacc.c  */
#line 623 "DikSam.y"
    {
            (yyval.assignment_operator) = ADD_ASSIGN;
        }
    break;

  case 129:

/* Line 1464 of yacc.c  */
#line 627 "DikSam.y"
    {
            (yyval.assignment_operator) = SUB_ASSIGN;
        }
    break;

  case 130:

/* Line 1464 of yacc.c  */
#line 631 "DikSam.y"
    {
            (yyval.assignment_operator) = MUL_ASSIGN;
        }
    break;

  case 131:

/* Line 1464 of yacc.c  */
#line 635 "DikSam.y"
    {
            (yyval.assignment_operator) = DIV_ASSIGN;
        }
    break;

  case 132:

/* Line 1464 of yacc.c  */
#line 639 "DikSam.y"
    {
            (yyval.assignment_operator) = MOD_ASSIGN;
        }
    break;

  case 133:

/* Line 1464 of yacc.c  */
#line 646 "DikSam.y"
    {
            (yyval.basic_type_specifier) = DVM_VOID_TYPE;
        }
    break;

  case 134:

/* Line 1464 of yacc.c  */
#line 650 "DikSam.y"
    {
            (yyval.basic_type_specifier) = DVM_BOOLEAN_TYPE;
        }
    break;

  case 135:

/* Line 1464 of yacc.c  */
#line 654 "DikSam.y"
    {
            (yyval.basic_type_specifier) = DVM_INT_TYPE;
        }
    break;

  case 136:

/* Line 1464 of yacc.c  */
#line 658 "DikSam.y"
    {
            (yyval.basic_type_specifier) = DVM_DOUBLE_TYPE;
        }
    break;

  case 137:

/* Line 1464 of yacc.c  */
#line 662 "DikSam.y"
    {
            (yyval.basic_type_specifier) = DVM_STRING_TYPE;
        }
    break;

  case 138:

/* Line 1464 of yacc.c  */
#line 669 "DikSam.y"
    {
            (yyval.type_specifier) = dkc_create_class_type_specifier((yyvsp[(1) - (1)].identifier));
        }
    break;

  case 139:

/* Line 1464 of yacc.c  */
#line 676 "DikSam.y"
    {
            TypeSpecifier *basic_type = dkc_create_type_specifier((yyvsp[(1) - (3)].basic_type_specifier));
            (yyval.type_specifier) = dkc_create_array_type_specifier(basic_type);
        }
    break;

  case 140:

/* Line 1464 of yacc.c  */
#line 681 "DikSam.y"
    {
            TypeSpecifier *class_type = dkc_create_class_type_specifier((yyvsp[(1) - (3)].identifier));
            (yyval.type_specifier) = dkc_create_array_type_specifier(class_type);
        }
    break;

  case 141:

/* Line 1464 of yacc.c  */
#line 686 "DikSam.y"
    {
            (yyval.type_specifier) = dkc_create_array_type_specifier((yyvsp[(1) - (3)].type_specifier));
        }
    break;

  case 142:

/* Line 1464 of yacc.c  */
#line 693 "DikSam.y"
    {
            (yyval.type_specifier) = dkc_create_type_specifier((yyvsp[(1) - (1)].basic_type_specifier));
        }
    break;

  case 145:

/* Line 1464 of yacc.c  */
#line 702 "DikSam.y"
    {
            (yyval.identifier) = NULL;
        }
    break;

  case 147:

/* Line 1464 of yacc.c  */
#line 710 "DikSam.y"
    {
            dkc_start_class_definition(NULL, (yyvsp[(1) - (4)].class_or_interface), (yyvsp[(2) - (4)].identifier), (yyvsp[(3) - (4)].extends_list));
        }
    break;

  case 148:

/* Line 1464 of yacc.c  */
#line 714 "DikSam.y"
    {
            dkc_class_define((yyvsp[(6) - (7)].member_declaration));
        }
    break;

  case 149:

/* Line 1464 of yacc.c  */
#line 718 "DikSam.y"
    {
            dkc_start_class_definition(&(yyvsp[(1) - (5)].class_or_member_modifier), (yyvsp[(2) - (5)].class_or_interface), (yyvsp[(3) - (5)].identifier), (yyvsp[(4) - (5)].extends_list));
        }
    break;

  case 150:

/* Line 1464 of yacc.c  */
#line 722 "DikSam.y"
    {
            dkc_class_define((yyvsp[(7) - (8)].member_declaration));
        }
    break;

  case 151:

/* Line 1464 of yacc.c  */
#line 726 "DikSam.y"
    {
            dkc_start_class_definition(NULL, (yyvsp[(1) - (4)].class_or_interface), (yyvsp[(2) - (4)].identifier), (yyvsp[(3) - (4)].extends_list));
        }
    break;

  case 152:

/* Line 1464 of yacc.c  */
#line 730 "DikSam.y"
    {
            dkc_class_define(NULL);
        }
    break;

  case 153:

/* Line 1464 of yacc.c  */
#line 734 "DikSam.y"
    {
            dkc_start_class_definition(&(yyvsp[(1) - (5)].class_or_member_modifier), (yyvsp[(2) - (5)].class_or_interface), (yyvsp[(3) - (5)].identifier), (yyvsp[(4) - (5)].extends_list));
        }
    break;

  case 154:

/* Line 1464 of yacc.c  */
#line 738 "DikSam.y"
    {
            dkc_class_define(NULL);
        }
    break;

  case 156:

/* Line 1464 of yacc.c  */
#line 746 "DikSam.y"
    {
            (yyval.class_or_member_modifier) = dkc_chain_class_or_member_modifier((yyvsp[(1) - (2)].class_or_member_modifier), (yyvsp[(2) - (2)].class_or_member_modifier));
        }
    break;

  case 158:

/* Line 1464 of yacc.c  */
#line 754 "DikSam.y"
    {
            (yyval.class_or_member_modifier) = dkc_create_class_or_member_modifier(VIRTUAL_MODIFIER);
        }
    break;

  case 159:

/* Line 1464 of yacc.c  */
#line 758 "DikSam.y"
    {
            (yyval.class_or_member_modifier) = dkc_create_class_or_member_modifier(OVERRIDE_MODIFIER);
        }
    break;

  case 160:

/* Line 1464 of yacc.c  */
#line 762 "DikSam.y"
    {
            (yyval.class_or_member_modifier) = dkc_create_class_or_member_modifier(ABSTRACT_MODIFIER);
        }
    break;

  case 161:

/* Line 1464 of yacc.c  */
#line 769 "DikSam.y"
    {
            (yyval.class_or_interface) = DVM_CLASS_DEFINITION;
        }
    break;

  case 162:

/* Line 1464 of yacc.c  */
#line 773 "DikSam.y"
    {
            (yyval.class_or_interface) = DVM_INTERFACE_DEFINITION;
        }
    break;

  case 163:

/* Line 1464 of yacc.c  */
#line 780 "DikSam.y"
    {
            (yyval.extends_list) = NULL;
        }
    break;

  case 164:

/* Line 1464 of yacc.c  */
#line 784 "DikSam.y"
    {
            (yyval.extends_list) = (yyvsp[(2) - (2)].extends_list);
        }
    break;

  case 165:

/* Line 1464 of yacc.c  */
#line 791 "DikSam.y"
    {
            (yyval.extends_list) = dkc_create_extends_list((yyvsp[(1) - (1)].identifier));
        }
    break;

  case 166:

/* Line 1464 of yacc.c  */
#line 795 "DikSam.y"
    {
            (yyval.extends_list) = dkc_chain_extends_list((yyvsp[(1) - (3)].extends_list), (yyvsp[(3) - (3)].identifier));
        }
    break;

  case 168:

/* Line 1464 of yacc.c  */
#line 803 "DikSam.y"
    {
            (yyval.member_declaration) = dkc_chain_member_declaration((yyvsp[(1) - (2)].member_declaration), (yyvsp[(2) - (2)].member_declaration));
        }
    break;

  case 171:

/* Line 1464 of yacc.c  */
#line 815 "DikSam.y"
    {
            (yyval.member_declaration) = dkc_create_method_member(NULL, (yyvsp[(1) - (1)].function_definition), DVM_FALSE);
        }
    break;

  case 172:

/* Line 1464 of yacc.c  */
#line 819 "DikSam.y"
    {
            (yyval.member_declaration) = dkc_create_method_member(&(yyvsp[(1) - (2)].class_or_member_modifier), (yyvsp[(2) - (2)].function_definition), DVM_FALSE);
        }
    break;

  case 173:

/* Line 1464 of yacc.c  */
#line 823 "DikSam.y"
    {
            (yyval.member_declaration) = dkc_create_method_member(NULL, (yyvsp[(1) - (1)].function_definition), DVM_TRUE);
        }
    break;

  case 174:

/* Line 1464 of yacc.c  */
#line 827 "DikSam.y"
    {
            (yyval.member_declaration) = dkc_create_method_member(&(yyvsp[(1) - (2)].class_or_member_modifier), (yyvsp[(2) - (2)].function_definition), DVM_TRUE);
        }
    break;

  case 175:

/* Line 1464 of yacc.c  */
#line 834 "DikSam.y"
    {
            (yyval.function_definition) = dkc_method_function_define((yyvsp[(1) - (6)].type_specifier), (yyvsp[(2) - (6)].identifier), (yyvsp[(4) - (6)].parameter_list), (yyvsp[(6) - (6)].block));
        }
    break;

  case 176:

/* Line 1464 of yacc.c  */
#line 838 "DikSam.y"
    {
            (yyval.function_definition) = dkc_method_function_define((yyvsp[(1) - (5)].type_specifier), #2, NULL, (yyvsp[(5) - (5)].block));
        }
    break;

  case 177:

/* Line 1464 of yacc.c  */
#line 842 "DikSam.y"
    {
            (yyval.function_definition) = dkc_method_function_define((yyvsp[(1) - (6)].type_specifier), (yyvsp[(2) - (6)].identifier), (yyvsp[(4) - (6)].parameter_list), NULL);
        }
    break;

  case 178:

/* Line 1464 of yacc.c  */
#line 846 "DikSam.y"
    {
            (yyval.function_definition) = dkc_method_function_define((yyvsp[(1) - (5)].type_specifier), (yyvsp[(2) - (5)].identifier), NULL, NULL);
        }
    break;

  case 179:

/* Line 1464 of yacc.c  */
#line 853 "DikSam.y"
    {
            (yyval.function_definition) = dkc_constructor_function_define((yyvsp[(2) - (6)].identifier), (yyvsp[(4) - (6)].parameter_list), (yyvsp[(6) - (6)].block));
        }
    break;

  case 180:

/* Line 1464 of yacc.c  */
#line 857 "DikSam.y"
    {
            (yyval.function_definition) = dkc_constructor_function_define((yyvsp[(2) - (5)].identifier), NULL, (yyvsp[(5) - (5)].block));
        }
    break;

  case 181:

/* Line 1464 of yacc.c  */
#line 861 "DikSam.y"
    {
            (yyval.function_definition) = dkc_constructor_function_define((yyvsp[(2) - (6)].identifier), (yyvsp[(4) - (6)].parameter_list), NULL);
        }
    break;

  case 182:

/* Line 1464 of yacc.c  */
#line 865 "DikSam.y"
    {
            (yyval.function_definition) = dkc_constructor_function_define((yyvsp[(2) - (5)].identifier), NULL, NULL);
        }
    break;

  case 183:

/* Line 1464 of yacc.c  */
#line 872 "DikSam.y"
    {
            (yyval.class_or_member_modifier) = dkc_create_class_or_member_modifier(PUBLIC_MODIFIER);
        }
    break;

  case 184:

/* Line 1464 of yacc.c  */
#line 876 "DikSam.y"
    {
            (yyval.class_or_member_modifier) = dkc_create_class_or_member_modifier(PRIVATE_MODIFIER);
        }
    break;

  case 185:

/* Line 1464 of yacc.c  */
#line 883 "DikSam.y"
    {
            (yyval.member_declaration) = dkc_create_field_member(NULL, (yyvsp[(1) - (3)].type_specifier), (yyvsp[(2) - (3)].identifier));
        }
    break;

  case 186:

/* Line 1464 of yacc.c  */
#line 887 "DikSam.y"
    {
            (yyval.member_declaration) = dkc_create_field_member(&(yyvsp[(1) - (4)].class_or_member_modifier), (yyvsp[(2) - (4)].type_specifier), (yyvsp[(3) - (4)].identifier));
        }
    break;



/* Line 1464 of yacc.c  */
#line 3207 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1684 of yacc.c  */
#line 891 "DikSam.y"

