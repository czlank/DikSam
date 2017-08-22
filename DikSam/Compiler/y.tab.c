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

/* Line 214 of yacc.c  */
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



/* Line 214 of yacc.c  */
#line 248 "y.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 260 "y.tab.c"

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
#define YYFINAL  63
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   351

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  58
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  42
/* YYNRULES -- Number of rules.  */
#define YYNRULES  113
/* YYNRULES -- Number of states.  */
#define YYNSTATES  193

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   312

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
      55,    56,    57
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    14,    17,    20,
      22,    24,    26,    28,    30,    32,    34,    40,    48,    55,
      64,    66,    69,    76,    83,    94,    95,    97,   101,   105,
     109,   113,   119,   120,   125,   128,   135,   141,   148,   154,
     157,   162,   164,   168,   170,   174,   176,   180,   182,   186,
     188,   192,   194,   198,   202,   204,   208,   212,   216,   220,
     222,   226,   230,   232,   236,   240,   244,   246,   249,   252,
     254,   256,   261,   265,   270,   274,   277,   280,   284,   286,
     288,   290,   292,   294,   296,   298,   300,   304,   309,   313,
     318,   320,   323,   327,   330,   334,   335,   337,   341,   342,
     345,   347,   349,   351,   353,   355,   357,   359,   361,   363,
     365,   367,   371,   372
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      59,     0,    -1,    60,    -1,    59,    60,    -1,    75,    -1,
      62,    -1,    62,    -1,    61,    62,    -1,    78,    21,    -1,
      63,    -1,    66,    -1,    67,    -1,    69,    -1,    70,    -1,
      71,    -1,    72,    -1,     7,    15,    78,    16,    73,    -1,
       7,    15,    78,    16,    73,     8,    73,    -1,     7,    15,
      78,    16,    73,    64,    -1,     7,    15,    78,    16,    73,
      64,     8,    73,    -1,    65,    -1,    64,    65,    -1,     8,
       7,    15,    78,    16,    73,    -1,    95,     9,    15,    78,
      16,    73,    -1,    95,    10,    15,    68,    21,    68,    21,
      68,    16,    73,    -1,    -1,    78,    -1,    11,    68,    21,
      -1,    12,    99,    21,    -1,    13,    99,    21,    -1,    98,
       6,    21,    -1,    98,     6,    24,    78,    21,    -1,    -1,
      17,    74,    61,    18,    -1,    17,    18,    -1,    98,     6,
      15,    76,    16,    73,    -1,    98,     6,    15,    16,    73,
      -1,    98,     6,    15,    76,    16,    21,    -1,    98,     6,
      15,    16,    21,    -1,    98,     6,    -1,    76,    23,    98,
       6,    -1,    79,    -1,    77,    23,    79,    -1,    79,    -1,
      78,    23,    79,    -1,    80,    -1,    87,    96,    79,    -1,
      81,    -1,    80,    26,    81,    -1,    82,    -1,    81,    25,
      82,    -1,    83,    -1,    82,    27,    83,    -1,    82,    28,
      83,    -1,    84,    -1,    83,    29,    84,    -1,    83,    30,
      84,    -1,    83,    31,    84,    -1,    83,    32,    84,    -1,
      85,    -1,    84,    33,    85,    -1,    84,    34,    85,    -1,
      86,    -1,    85,    35,    86,    -1,    85,    36,    86,    -1,
      85,    37,    86,    -1,    87,    -1,    34,    86,    -1,    40,
      86,    -1,    88,    -1,    90,    -1,    88,    19,    78,    20,
      -1,    87,    41,     6,    -1,    87,    15,    77,    16,    -1,
      87,    15,    16,    -1,    87,    47,    -1,    87,    48,    -1,
      15,    78,    16,    -1,     6,    -1,     3,    -1,     4,    -1,
       5,    -1,    38,    -1,    39,    -1,    14,    -1,    89,    -1,
      17,    94,    18,    -1,    17,    94,    23,    18,    -1,    57,
      97,    91,    -1,    57,    97,    91,    93,    -1,    92,    -1,
      91,    92,    -1,    19,    78,    20,    -1,    19,    20,    -1,
      93,    19,    20,    -1,    -1,    79,    -1,    94,    23,    79,
      -1,    -1,     6,    22,    -1,    24,    -1,    42,    -1,    43,
      -1,    44,    -1,    45,    -1,    46,    -1,    53,    -1,    54,
      -1,    55,    -1,    56,    -1,    97,    -1,    98,    19,    20,
      -1,    -1,     6,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    61,    61,    62,    66,    67,    77,    81,    88,    92,
      93,    94,    95,    96,    97,    98,   102,   106,   110,   114,
     121,   122,   129,   136,   143,   152,   155,   159,   166,   173,
     180,   184,   192,   191,   199,   207,   211,   215,   219,   226,
     230,   237,   241,   248,   249,   256,   257,   264,   265,   271,
     272,   279,   280,   284,   291,   292,   296,   300,   304,   311,
     312,   316,   323,   324,   328,   332,   339,   340,   344,   351,
     352,   356,   360,   364,   368,   372,   376,   380,   384,   388,
     389,   390,   391,   395,   399,   403,   407,   411,   418,   422,
     429,   430,   437,   444,   448,   456,   459,   463,   471,   474,
     481,   485,   489,   493,   497,   501,   508,   512,   516,   520,
     527,   531,   539,   542
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INT_LITERAL", "DOUBLE_LITERAL",
  "STRING_LITERAL", "IDENTIFIER", "IF", "ELSE", "WHILE", "FOR", "RETURN_T",
  "BREAK", "CONTINUE", "NULL_T", "LEFTP", "RIGHTP", "LC", "RC", "LB", "RB",
  "SEMICOLON", "COLON", "COMMA", "ASSIGN_T", "LOGICAL_AND", "LOGICAL_OR",
  "EQ", "NE", "GT", "GE", "LT", "LE", "ADD", "SUB", "MUL", "DIV", "MOD",
  "TRUE_T", "FALSE_T", "EXCLAMATION", "DOT", "ADD_ASSIGN_T",
  "SUB_ASSIGN_T", "MUL_ASSIGN_T", "DIV_ASSIGN_T", "MOD_ASSIGN_T",
  "INCREMENT", "DECREMENT", "TRY", "CATCH", "FINALLY", "THROW",
  "BOOLEAN_T", "INT_T", "DOUBLE_T", "STRING_T", "NEW", "$accept",
  "translation_unit", "definition_or_statement", "statement_list",
  "statement", "if_statement", "elsif_list", "elsif", "while_statement",
  "for_statement", "expression_opt", "return_statement", "break_statement",
  "continue_statement", "declaration_statement", "block", "@1",
  "function_definition", "parameter_list", "argument_list", "expression",
  "assignment_expression", "logical_or_expression",
  "logical_and_expression", "equality_expression", "relational_expression",
  "additive_expression", "multiplicative_expression", "unary_expression",
  "primary_expression", "primary_no_new_array", "array_literal",
  "array_creation", "dimension_expression_list", "dimension_expression",
  "dimension_list", "expression_list", "label_opt", "assignment_operator",
  "basic_type_specifier", "type_specifier", "identifier_opt", 0
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
     305,   306,   307,   308,   309,   310,   311,   312
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    58,    59,    59,    60,    60,    61,    61,    62,    62,
      62,    62,    62,    62,    62,    62,    63,    63,    63,    63,
      64,    64,    65,    66,    67,    68,    68,    69,    70,    71,
      72,    72,    74,    73,    73,    75,    75,    75,    75,    76,
      76,    77,    77,    78,    78,    79,    79,    80,    80,    81,
      81,    82,    82,    82,    83,    83,    83,    83,    83,    84,
      84,    84,    85,    85,    85,    85,    86,    86,    86,    87,
      87,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    89,    89,    90,    90,
      91,    91,    92,    93,    93,    94,    94,    94,    95,    95,
      96,    96,    96,    96,    96,    96,    97,    97,    97,    97,
      98,    98,    99,    99
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     2,     2,     1,
       1,     1,     1,     1,     1,     1,     5,     7,     6,     8,
       1,     2,     6,     6,    10,     0,     1,     3,     3,     3,
       3,     5,     0,     4,     2,     6,     5,     6,     5,     2,
       4,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     3,     1,     3,     3,     3,     3,     1,
       3,     3,     1,     3,     3,     3,     1,     2,     2,     1,
       1,     4,     3,     4,     3,     2,     2,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     4,     3,     4,
       1,     2,     3,     2,     3,     0,     1,     3,     0,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     0,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      98,    79,    80,    81,    78,     0,    25,   112,   112,    84,
       0,    95,     0,    82,    83,     0,   106,   107,   108,   109,
       0,    98,     2,     5,     9,    10,    11,    12,    13,    14,
      15,     4,     0,    43,    45,    47,    49,    51,    54,    59,
      62,    66,    69,    85,    70,     0,   110,     0,    99,     0,
      78,     0,    26,   113,     0,     0,     0,    96,     0,    67,
      66,    68,     0,     1,     3,     8,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   100,     0,   101,   102,   103,   104,   105,    75,    76,
       0,     0,     0,     0,     0,     0,     0,    27,    28,    29,
      77,    86,     0,     0,    88,    90,    44,    48,    50,    52,
      53,    55,    56,    57,    58,    60,    61,    63,    64,    65,
      74,     0,    41,    72,    46,     0,     0,    25,     0,    30,
       0,   111,     0,    87,    97,     0,     0,    91,    89,    73,
       0,    71,     0,     0,     0,     0,     0,     0,    32,    16,
      92,    93,     0,    42,     0,    25,    38,    36,     0,     0,
      39,    31,    34,    98,     0,    18,    20,    94,    23,     0,
      37,    35,     0,    98,     6,     0,     0,    17,     0,    21,
      25,    40,    33,     7,     0,     0,    19,     0,     0,     0,
       0,    24,    22
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    21,    22,   173,    23,    24,   165,   166,    25,    26,
      51,    27,    28,    29,    30,   149,   163,    31,   145,   121,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,   104,   105,   138,    58,    45,    90,    46,
      47,    54
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -128
static const yytype_int16 yypact[] =
{
     184,  -128,  -128,  -128,   -13,    15,   294,    29,    29,  -128,
     294,   294,   294,  -128,  -128,   294,  -128,  -128,  -128,  -128,
     100,    11,  -128,  -128,  -128,  -128,  -128,  -128,  -128,  -128,
    -128,  -128,    75,  -128,    27,    30,    52,    96,   117,   140,
    -128,    71,   104,  -128,  -128,   162,  -128,     0,  -128,   294,
    -128,   145,   155,  -128,   171,   179,   -11,  -128,   -10,  -128,
      22,  -128,   183,  -128,  -128,  -128,   294,   294,   294,   294,
     294,   294,   294,   294,   294,   294,   294,   294,   294,   294,
     211,  -128,   197,  -128,  -128,  -128,  -128,  -128,  -128,  -128,
     294,   294,   189,   190,    68,   186,    16,  -128,  -128,  -128,
    -128,  -128,   249,   294,   188,  -128,  -128,    30,    52,    96,
      96,   117,   117,   117,   117,   140,   140,  -128,  -128,  -128,
    -128,    59,  -128,  -128,  -128,   141,   294,   294,     5,  -128,
     294,  -128,   191,  -128,  -128,   142,   256,  -128,   192,  -128,
     294,  -128,    61,   198,    73,   106,    14,   116,   194,   201,
    -128,  -128,   193,  -128,   191,   294,  -128,  -128,   131,   100,
    -128,  -128,  -128,   184,    24,   202,  -128,  -128,  -128,   199,
    -128,  -128,    21,   129,  -128,    72,   206,  -128,    24,  -128,
     294,  -128,  -128,  -128,   149,   294,  -128,   213,   122,   191,
     191,  -128,  -128
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -128,  -128,   209,  -128,  -127,  -128,  -128,    66,  -128,  -128,
    -126,  -128,  -128,  -128,  -128,  -102,  -128,  -128,  -128,  -128,
      -6,    -9,  -128,   165,   166,   111,    86,   118,    -5,    32,
    -128,  -128,  -128,  -128,   132,  -128,  -128,  -128,  -128,   215,
    -125,   225
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      52,   143,    57,   146,    56,   100,    94,    59,   101,    48,
      61,    63,    66,   102,     1,     2,     3,     4,     5,    95,
     160,   144,     6,     7,     8,     9,    10,   181,    11,   169,
      49,   176,   132,    95,   172,    53,   174,    80,   175,    66,
      95,   148,   157,    96,    60,    12,   183,    60,   175,    13,
      14,    15,   168,    67,   187,    68,   171,   106,    16,    17,
      18,    19,   177,    82,    16,    17,    18,    19,    20,    88,
      89,   122,   117,   118,   119,   139,   186,   154,   184,    69,
      70,   124,   140,   128,    66,   125,    80,   191,   192,   129,
     148,    95,   130,   134,   156,    81,    65,   135,    66,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    82,    83,    84,    85,    86,    87,    88,    89,
     142,    52,   158,    91,   147,    71,    72,    73,    74,   159,
     135,   153,     1,     2,     3,     4,     5,   161,   190,    66,
       6,     7,     8,     9,    10,    66,    11,   182,   148,    52,
      75,    76,   170,    16,    17,    18,    19,   111,   112,   113,
     114,   141,   150,    12,    66,    66,    97,    13,    14,    15,
     129,    92,    93,   130,    52,    77,    78,    79,    66,   188,
     109,   110,    16,    17,    18,    19,    20,     1,     2,     3,
       4,     5,    98,   115,   116,     6,     7,     8,     9,    10,
      99,    11,   103,   123,   126,   127,   131,   136,   148,   164,
     178,   152,   162,   167,     1,     2,     3,    50,    12,   155,
     180,   185,    13,    14,    15,     9,    10,   120,    11,   189,
      64,   179,   107,    55,   108,    62,   137,    16,    17,    18,
      19,    20,     0,     0,     0,    12,     0,     0,     0,    13,
      14,    15,     1,     2,     3,    50,     0,     0,     0,     1,
       2,     3,    50,     9,    10,     0,    11,   133,    20,     0,
       9,    10,     0,    11,     0,     0,   151,     0,     0,     0,
       0,     0,     0,    12,     0,     0,     0,    13,    14,    15,
      12,     0,     0,     0,    13,    14,    15,     1,     2,     3,
      50,     0,     0,     0,     0,     0,    20,     0,     9,    10,
       0,    11,     0,    20,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    12,     0,
       0,     0,    13,    14,    15,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    20
};

static const yytype_int16 yycheck[] =
{
       6,   127,    11,   128,    10,    16,     6,    12,    18,    22,
      15,     0,    23,    23,     3,     4,     5,     6,     7,    19,
       6,    16,    11,    12,    13,    14,    15,     6,    17,   155,
      15,     7,    16,    19,   159,     6,   163,    15,   163,    23,
      19,    17,   144,    49,    12,    34,   173,    15,   173,    38,
      39,    40,   154,    26,   180,    25,   158,    66,    53,    54,
      55,    56,   164,    41,    53,    54,    55,    56,    57,    47,
      48,    80,    77,    78,    79,    16,   178,    16,     6,    27,
      28,    90,    23,    15,    23,    91,    15,   189,   190,    21,
      17,    19,    24,   102,    21,    24,    21,   103,    23,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    41,    42,    43,    44,    45,    46,    47,    48,
     126,   127,    16,    19,   130,    29,    30,    31,    32,    23,
     136,   140,     3,     4,     5,     6,     7,    21,    16,    23,
      11,    12,    13,    14,    15,    23,    17,    18,    17,   155,
      33,    34,    21,    53,    54,    55,    56,    71,    72,    73,
      74,    20,    20,    34,    23,    23,    21,    38,    39,    40,
      21,     9,    10,    24,   180,    35,    36,    37,    23,   185,
      69,    70,    53,    54,    55,    56,    57,     3,     4,     5,
       6,     7,    21,    75,    76,    11,    12,    13,    14,    15,
      21,    17,    19,     6,    15,    15,    20,    19,    17,     8,
       8,    19,    18,    20,     3,     4,     5,     6,    34,    21,
      21,    15,    38,    39,    40,    14,    15,    16,    17,    16,
      21,   165,    67,     8,    68,    20,   104,    53,    54,    55,
      56,    57,    -1,    -1,    -1,    34,    -1,    -1,    -1,    38,
      39,    40,     3,     4,     5,     6,    -1,    -1,    -1,     3,
       4,     5,     6,    14,    15,    -1,    17,    18,    57,    -1,
      14,    15,    -1,    17,    -1,    -1,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    -1,    -1,    38,    39,    40,
      34,    -1,    -1,    -1,    38,    39,    40,     3,     4,     5,
       6,    -1,    -1,    -1,    -1,    -1,    57,    -1,    14,    15,
      -1,    17,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      -1,    -1,    38,    39,    40,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    57
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,    11,    12,    13,    14,
      15,    17,    34,    38,    39,    40,    53,    54,    55,    56,
      57,    59,    60,    62,    63,    66,    67,    69,    70,    71,
      72,    75,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    95,    97,    98,    22,    15,
       6,    68,    78,     6,    99,    99,    78,    79,    94,    86,
      87,    86,    97,     0,    60,    21,    23,    26,    25,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      15,    24,    41,    42,    43,    44,    45,    46,    47,    48,
      96,    19,     9,    10,     6,    19,    78,    21,    21,    21,
      16,    18,    23,    19,    91,    92,    79,    81,    82,    83,
      83,    84,    84,    84,    84,    85,    85,    86,    86,    86,
      16,    77,    79,     6,    79,    78,    15,    15,    15,    21,
      24,    20,    16,    18,    79,    78,    19,    92,    93,    16,
      23,    20,    78,    68,    16,    76,    98,    78,    17,    73,
      20,    20,    19,    79,    16,    21,    21,    73,    16,    23,
       6,    21,    18,    74,     8,    64,    65,    20,    73,    68,
      21,    73,    98,    61,    62,    98,     7,    73,     8,    65,
      21,     6,    18,    62,     6,    15,    73,    68,    78,    16,
      16,    73,    73
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
  ;
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
        case 5:

/* Line 1464 of yacc.c  */
#line 68 "DikSam.y"
    {
            DKC_Compiler *compiler = dkc_get_current_compiler();

            compiler->statement_list
                = dkc_chain_statement_list(compiler->statement_list, (yyvsp[(1) - (1)].statement));
        }
    break;

  case 6:

/* Line 1464 of yacc.c  */
#line 78 "DikSam.y"
    {
            (yyval.statement_list) = dkc_create_statement_list((yyvsp[(1) - (1)].statement));
        }
    break;

  case 7:

/* Line 1464 of yacc.c  */
#line 82 "DikSam.y"
    {
            (yyval.statement_list) = dkc_chain_statement_list((yyvsp[(1) - (2)].statement_list), (yyvsp[(2) - (2)].statement));
        }
    break;

  case 8:

/* Line 1464 of yacc.c  */
#line 89 "DikSam.y"
    {
          (yyval.statement) = dkc_create_expression_statement((yyvsp[(1) - (2)].expression));
        }
    break;

  case 16:

/* Line 1464 of yacc.c  */
#line 103 "DikSam.y"
    {
            (yyval.statement) = dkc_create_if_statement((yyvsp[(3) - (5)].expression), (yyvsp[(5) - (5)].block), NULL, NULL);
        }
    break;

  case 17:

/* Line 1464 of yacc.c  */
#line 107 "DikSam.y"
    {
            (yyval.statement) = dkc_create_if_statement((yyvsp[(3) - (7)].expression), (yyvsp[(5) - (7)].block), NULL, (yyvsp[(7) - (7)].block));
        }
    break;

  case 18:

/* Line 1464 of yacc.c  */
#line 111 "DikSam.y"
    {
            (yyval.statement) = dkc_create_if_statement((yyvsp[(3) - (6)].expression), (yyvsp[(5) - (6)].block), (yyvsp[(6) - (6)].elsif), NULL);
        }
    break;

  case 19:

/* Line 1464 of yacc.c  */
#line 115 "DikSam.y"
    {
            (yyval.statement) = dkc_create_if_statement((yyvsp[(3) - (8)].expression), (yyvsp[(5) - (8)].block), (yyvsp[(6) - (8)].elsif), (yyvsp[(8) - (8)].block));
        }
    break;

  case 21:

/* Line 1464 of yacc.c  */
#line 123 "DikSam.y"
    {
            (yyval.elsif) = dkc_chain_elsif_list((yyvsp[(1) - (2)].elsif), (yyvsp[(2) - (2)].elsif));
        }
    break;

  case 22:

/* Line 1464 of yacc.c  */
#line 130 "DikSam.y"
    {
            (yyval.elsif) = dkc_create_elsif((yyvsp[(4) - (6)].expression), (yyvsp[(6) - (6)].block));
        }
    break;

  case 23:

/* Line 1464 of yacc.c  */
#line 137 "DikSam.y"
    {
            (yyval.statement) = dkc_create_while_statement((yyvsp[(1) - (6)].identifier), (yyvsp[(4) - (6)].expression), (yyvsp[(6) - (6)].block));
        }
    break;

  case 24:

/* Line 1464 of yacc.c  */
#line 145 "DikSam.y"
    {
            (yyval.statement) = dkc_create_for_statement((yyvsp[(1) - (10)].identifier), (yyvsp[(4) - (10)].expression), (yyvsp[(6) - (10)].expression), (yyvsp[(8) - (10)].expression), (yyvsp[(10) - (10)].block));
        }
    break;

  case 25:

/* Line 1464 of yacc.c  */
#line 152 "DikSam.y"
    {
            (yyval.expression) = NULL;
        }
    break;

  case 27:

/* Line 1464 of yacc.c  */
#line 160 "DikSam.y"
    {
            (yyval.statement) = dkc_create_return_statement((yyvsp[(2) - (3)].expression));
        }
    break;

  case 28:

/* Line 1464 of yacc.c  */
#line 167 "DikSam.y"
    {
            (yyval.statement) = dkc_create_break_statement((yyvsp[(2) - (3)].identifier));
        }
    break;

  case 29:

/* Line 1464 of yacc.c  */
#line 174 "DikSam.y"
    {
            (yyval.statement) = dkc_create_continue_statement((yyvsp[(2) - (3)].identifier));
        }
    break;

  case 30:

/* Line 1464 of yacc.c  */
#line 181 "DikSam.y"
    {
            (yyval.statement) = dkc_create_declaration_statement((yyvsp[(1) - (3)].type_specifier), (yyvsp[(2) - (3)].identifier), NULL);
        }
    break;

  case 31:

/* Line 1464 of yacc.c  */
#line 185 "DikSam.y"
    {
            (yyval.statement) = dkc_create_declaration_statement((yyvsp[(1) - (5)].type_specifier), (yyvsp[(2) - (5)].identifier), (yyvsp[(4) - (5)].expression));
        }
    break;

  case 32:

/* Line 1464 of yacc.c  */
#line 192 "DikSam.y"
    {
            (yyval.block) = dkc_open_block();
        }
    break;

  case 33:

/* Line 1464 of yacc.c  */
#line 196 "DikSam.y"
    {
            (yyval.block) = dkc_close_block((yyvsp[(2) - (4)].block), (yyvsp[(3) - (4)].statement_list));
        }
    break;

  case 34:

/* Line 1464 of yacc.c  */
#line 200 "DikSam.y"
    {
            Block *empty_block = dkc_open_block();
            (yyval.block) = dkc_close_block(empty_block, NULL);
        }
    break;

  case 35:

/* Line 1464 of yacc.c  */
#line 208 "DikSam.y"
    {
            dkc_function_define((yyvsp[(1) - (6)].type_specifier), (yyvsp[(2) - (6)].identifier), (yyvsp[(4) - (6)].parameter_list), (yyvsp[(6) - (6)].block));
        }
    break;

  case 36:

/* Line 1464 of yacc.c  */
#line 212 "DikSam.y"
    {
            dkc_function_define((yyvsp[(1) - (5)].type_specifier), (yyvsp[(2) - (5)].identifier), NULL, (yyvsp[(5) - (5)].block));
        }
    break;

  case 37:

/* Line 1464 of yacc.c  */
#line 216 "DikSam.y"
    {
            dkc_function_define((yyvsp[(1) - (6)].type_specifier), (yyvsp[(2) - (6)].identifier), (yyvsp[(4) - (6)].parameter_list), NULL);
        }
    break;

  case 38:

/* Line 1464 of yacc.c  */
#line 220 "DikSam.y"
    {
            dkc_function_define((yyvsp[(1) - (5)].type_specifier), (yyvsp[(2) - (5)].identifier), NULL, NULL);
        }
    break;

  case 39:

/* Line 1464 of yacc.c  */
#line 227 "DikSam.y"
    {
            (yyval.parameter_list) = dkc_create_parameter((yyvsp[(1) - (2)].type_specifier), (yyvsp[(2) - (2)].identifier));
        }
    break;

  case 40:

/* Line 1464 of yacc.c  */
#line 231 "DikSam.y"
    {
            (yyval.parameter_list) = dkc_chain_parameter((yyvsp[(1) - (4)].parameter_list), (yyvsp[(3) - (4)].type_specifier), (yyvsp[(4) - (4)].identifier));
        }
    break;

  case 41:

/* Line 1464 of yacc.c  */
#line 238 "DikSam.y"
    {
            (yyval.argument_list) = dkc_create_argument_list((yyvsp[(1) - (1)].expression));
        }
    break;

  case 42:

/* Line 1464 of yacc.c  */
#line 242 "DikSam.y"
    {
            (yyval.argument_list) = dkc_chain_argument_list((yyvsp[(1) - (3)].argument_list), (yyvsp[(3) - (3)].expression));
        }
    break;

  case 44:

/* Line 1464 of yacc.c  */
#line 250 "DikSam.y"
    {
            (yyval.expression) = dkc_create_comma_expression((yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));
        }
    break;

  case 46:

/* Line 1464 of yacc.c  */
#line 258 "DikSam.y"
    {
            (yyval.expression) = dkc_create_assign_expression((yyvsp[(1) - (3)].expression), (yyvsp[(2) - (3)].assignment_operator), (yyvsp[(3) - (3)].expression));
        }
    break;

  case 48:

/* Line 1464 of yacc.c  */
#line 266 "DikSam.y"
    {
            (yyval.expression) = dkc_create_binary_expression(LOGICAL_OR_EXPRESSION, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));
        }
    break;

  case 50:

/* Line 1464 of yacc.c  */
#line 273 "DikSam.y"
    {
            (yyval.expression) = dkc_create_binary_expression(LOGICAL_AND_EXPRESSION, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));
        }
    break;

  case 52:

/* Line 1464 of yacc.c  */
#line 281 "DikSam.y"
    {
            (yyval.expression) = dkc_create_binary_expression(EQ_EXPRESSION, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));
        }
    break;

  case 53:

/* Line 1464 of yacc.c  */
#line 285 "DikSam.y"
    {
            (yyval.expression) = dkc_create_binary_expression(NE_EXPRESSION, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));
        }
    break;

  case 55:

/* Line 1464 of yacc.c  */
#line 293 "DikSam.y"
    {
            (yyval.expression) = dkc_create_binary_expression(GT_EXPRESSION, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));
        }
    break;

  case 56:

/* Line 1464 of yacc.c  */
#line 297 "DikSam.y"
    {
            (yyval.expression) = dkc_create_binary_expression(GE_EXPRESSION, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));
        }
    break;

  case 57:

/* Line 1464 of yacc.c  */
#line 301 "DikSam.y"
    {
            (yyval.expression) = dkc_create_binary_expression(LT_EXPRESSION, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));
        }
    break;

  case 58:

/* Line 1464 of yacc.c  */
#line 305 "DikSam.y"
    {
            (yyval.expression) = dkc_create_binary_expression(LE_EXPRESSION, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));
        }
    break;

  case 60:

/* Line 1464 of yacc.c  */
#line 313 "DikSam.y"
    {
            (yyval.expression) = dkc_create_binary_expression(ADD_EXPRESSION, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));
        }
    break;

  case 61:

/* Line 1464 of yacc.c  */
#line 317 "DikSam.y"
    {
            (yyval.expression) = dkc_create_binary_expression(SUB_EXPRESSION, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));
        }
    break;

  case 63:

/* Line 1464 of yacc.c  */
#line 325 "DikSam.y"
    {
            (yyval.expression) = dkc_create_binary_expression(MUL_EXPRESSION, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));
        }
    break;

  case 64:

/* Line 1464 of yacc.c  */
#line 329 "DikSam.y"
    {
            (yyval.expression) = dkc_create_binary_expression(DIV_EXPRESSION, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));
        }
    break;

  case 65:

/* Line 1464 of yacc.c  */
#line 333 "DikSam.y"
    {
            (yyval.expression) = dkc_create_binary_expression(MOD_EXPRESSION, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression));
        }
    break;

  case 67:

/* Line 1464 of yacc.c  */
#line 341 "DikSam.y"
    {
            (yyval.expression) = dkc_create_minus_expression((yyvsp[(2) - (2)].expression));
        }
    break;

  case 68:

/* Line 1464 of yacc.c  */
#line 345 "DikSam.y"
    {
            (yyval.expression) = dkc_create_logical_not_expression((yyvsp[(2) - (2)].expression));
        }
    break;

  case 71:

/* Line 1464 of yacc.c  */
#line 357 "DikSam.y"
    {
            (yyval.expression) = dkc_create_index_expression((yyvsp[(1) - (4)].expression), (yyvsp[(3) - (4)].expression));
        }
    break;

  case 72:

/* Line 1464 of yacc.c  */
#line 361 "DikSam.y"
    {
            (yyval.expression) = dkc_create_member_expression((yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].identifier));
        }
    break;

  case 73:

/* Line 1464 of yacc.c  */
#line 365 "DikSam.y"
    {
            (yyval.expression) = dkc_create_function_call_expression((yyvsp[(1) - (4)].expression), (yyvsp[(3) - (4)].argument_list));
        }
    break;

  case 74:

/* Line 1464 of yacc.c  */
#line 369 "DikSam.y"
    {
            (yyval.expression) = dkc_create_function_call_expression((yyvsp[(1) - (3)].expression), NULL);
        }
    break;

  case 75:

/* Line 1464 of yacc.c  */
#line 373 "DikSam.y"
    {
            (yyval.expression) = dkc_create_incdec_expression((yyvsp[(1) - (2)].expression), INCREMENT_EXPRESSION);
        }
    break;

  case 76:

/* Line 1464 of yacc.c  */
#line 377 "DikSam.y"
    {
            (yyval.expression) = dkc_create_incdec_expression((yyvsp[(1) - (2)].expression), DECREMENT_EXPRESSION);
        }
    break;

  case 77:

/* Line 1464 of yacc.c  */
#line 381 "DikSam.y"
    {
            (yyval.expression) = (yyvsp[(2) - (3)].expression);
        }
    break;

  case 78:

/* Line 1464 of yacc.c  */
#line 385 "DikSam.y"
    {
            (yyval.expression) = dkc_create_identifier_expression((yyvsp[(1) - (1)].identifier));
        }
    break;

  case 82:

/* Line 1464 of yacc.c  */
#line 392 "DikSam.y"
    {
            (yyval.expression) = dkc_create_boolean_expression(DVM_TRUE);
        }
    break;

  case 83:

/* Line 1464 of yacc.c  */
#line 396 "DikSam.y"
    {
            (yyval.expression) = dkc_create_boolean_expression(DVM_FALSE);
        }
    break;

  case 84:

/* Line 1464 of yacc.c  */
#line 400 "DikSam.y"
    {
            (yyval.expression) = dkc_create_null_expression();
        }
    break;

  case 86:

/* Line 1464 of yacc.c  */
#line 408 "DikSam.y"
    {
            (yyval.expression) = dkc_create_array_literal_expression((yyvsp[(2) - (3)].expression_list));
        }
    break;

  case 87:

/* Line 1464 of yacc.c  */
#line 412 "DikSam.y"
    {
            (yyval.expression) = dkc_create_array_literal_expression((yyvsp[(2) - (4)].expression_list));
        }
    break;

  case 88:

/* Line 1464 of yacc.c  */
#line 419 "DikSam.y"
    {
            (yyval.expression) = dkc_create_array_creation((yyvsp[(2) - (3)].basic_type_specifier), (yyvsp[(3) - (3)].array_dimension), NULL);
        }
    break;

  case 89:

/* Line 1464 of yacc.c  */
#line 423 "DikSam.y"
    {
            (yyval.expression) = dkc_create_array_creation((yyvsp[(2) - (4)].basic_type_specifier), (yyvsp[(3) - (4)].array_dimension), (yyvsp[(4) - (4)].array_dimension));
        }
    break;

  case 91:

/* Line 1464 of yacc.c  */
#line 431 "DikSam.y"
    {
            (yyval.array_dimension) = dkc_chain_array_dimension((yyvsp[(1) - (2)].array_dimension), (yyvsp[(2) - (2)].array_dimension));
        }
    break;

  case 92:

/* Line 1464 of yacc.c  */
#line 438 "DikSam.y"
    {
            (yyval.array_dimension) = dkc_create_array_dimension((yyvsp[(2) - (3)].expression));
        }
    break;

  case 93:

/* Line 1464 of yacc.c  */
#line 445 "DikSam.y"
    {
            (yyval.array_dimension) = dkc_create_array_dimension(NULL);
        }
    break;

  case 94:

/* Line 1464 of yacc.c  */
#line 449 "DikSam.y"
    {
            (yyval.array_dimension) = dkc_chain_array_dimension((yyvsp[(1) - (3)].array_dimension), dkc_create_array_dimension(NULL));
        }
    break;

  case 95:

/* Line 1464 of yacc.c  */
#line 456 "DikSam.y"
    {
            (yyval.expression_list) = NULL;
        }
    break;

  case 96:

/* Line 1464 of yacc.c  */
#line 460 "DikSam.y"
    {
            (yyval.expression_list) = dkc_create_expression_list((yyvsp[(1) - (1)].expression));
        }
    break;

  case 97:

/* Line 1464 of yacc.c  */
#line 464 "DikSam.y"
    {
            (yyval.expression_list) = dkc_chain_expression_list((yyvsp[(1) - (3)].expression_list), (yyvsp[(3) - (3)].expression));
        }
    break;

  case 98:

/* Line 1464 of yacc.c  */
#line 471 "DikSam.y"
    {
            (yyval.identifier) = NULL;
        }
    break;

  case 99:

/* Line 1464 of yacc.c  */
#line 475 "DikSam.y"
    {
            (yyval.identifier) = (yyvsp[(1) - (2)].identifier);
        }
    break;

  case 100:

/* Line 1464 of yacc.c  */
#line 482 "DikSam.y"
    {
            (yyval.assignment_operator) = NORMAL_ASSIGN;
        }
    break;

  case 101:

/* Line 1464 of yacc.c  */
#line 486 "DikSam.y"
    {
            (yyval.assignment_operator) = ADD_ASSIGN;
        }
    break;

  case 102:

/* Line 1464 of yacc.c  */
#line 490 "DikSam.y"
    {
            (yyval.assignment_operator) = SUB_ASSIGN;
        }
    break;

  case 103:

/* Line 1464 of yacc.c  */
#line 494 "DikSam.y"
    {
            (yyval.assignment_operator) = MUL_ASSIGN;
        }
    break;

  case 104:

/* Line 1464 of yacc.c  */
#line 498 "DikSam.y"
    {
            (yyval.assignment_operator) = DIV_ASSIGN;
        }
    break;

  case 105:

/* Line 1464 of yacc.c  */
#line 502 "DikSam.y"
    {
            (yyval.assignment_operator) = MOD_ASSIGN;
        }
    break;

  case 106:

/* Line 1464 of yacc.c  */
#line 509 "DikSam.y"
    {
            (yyval.basic_type_specifier) = DVM_BOOLEAN_TYPE;
        }
    break;

  case 107:

/* Line 1464 of yacc.c  */
#line 513 "DikSam.y"
    {
            (yyval.basic_type_specifier) = DVM_INT_TYPE;
        }
    break;

  case 108:

/* Line 1464 of yacc.c  */
#line 517 "DikSam.y"
    {
            (yyval.basic_type_specifier) = DVM_DOUBLE_TYPE;
        }
    break;

  case 109:

/* Line 1464 of yacc.c  */
#line 521 "DikSam.y"
    {
            (yyval.basic_type_specifier) = DVM_STRING_TYPE;
        }
    break;

  case 110:

/* Line 1464 of yacc.c  */
#line 528 "DikSam.y"
    {
            (yyval.type_specifier) = dkc_create_type_specifier((yyvsp[(1) - (1)].basic_type_specifier));
        }
    break;

  case 111:

/* Line 1464 of yacc.c  */
#line 532 "DikSam.y"
    {
            (yyval.type_specifier) = dkc_create_array_type_specifier((yyvsp[(1) - (3)].type_specifier));
        }
    break;

  case 112:

/* Line 1464 of yacc.c  */
#line 539 "DikSam.y"
    {
            (yyval.identifier) = NULL;
        }
    break;



/* Line 1464 of yacc.c  */
#line 2443 "y.tab.c"
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
#line 544 "DikSam.y"

