
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse         nx_expr_parser_parse
#define yylex           nx_expr_parser_lex
#define yyerror         nx_expr_parser_error
#define yylval          nx_expr_parser_lval
#define yychar          nx_expr_parser_char
#define yydebug         nx_expr_parser_debug
#define yynerrs         nx_expr_parser_nerrs


/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 8 "expr-grammar.y"

#include "error_debug.h"
#include "expr.h"
#include "expr-parser.h"

#define NX_LOGMODULE NX_LOGMODULE_CORE
extern int nx_expr_parser_lex();



/* Line 189 of yacc.c  */
#line 92 "expr-grammar.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
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
     TOKEN_ASSIGNMENT = 258,
     TOKEN_AND = 259,
     TOKEN_OR = 260,
     TOKEN_XOR = 261,
     TOKEN_BINAND = 262,
     TOKEN_BINOR = 263,
     TOKEN_BINXOR = 264,
     TOKEN_IF = 265,
     TOKEN_ELSE = 266,
     TOKEN_MUL = 267,
     TOKEN_DIV = 268,
     TOKEN_MOD = 269,
     TOKEN_PLUS = 270,
     TOKEN_MINUS = 271,
     TOKEN_REGMATCH = 272,
     TOKEN_NOTREGMATCH = 273,
     TOKEN_EQUAL = 274,
     TOKEN_NOTEQUAL = 275,
     TOKEN_GE = 276,
     TOKEN_GREATER = 277,
     TOKEN_LE = 278,
     TOKEN_LESS = 279,
     TOKEN_LEFTBRACKET = 280,
     TOKEN_RIGHTBRACKET = 281,
     TOKEN_LEFTBRACE = 282,
     TOKEN_RIGHTBRACE = 283,
     TOKEN_SEMICOLON = 284,
     TOKEN_COMMA = 285,
     TOKEN_NOT = 286,
     TOKEN_DEFINED = 287,
     TOKEN_FIELDNAME = 288,
     TOKEN_CAPTURED = 289,
     TOKEN_FUNCPROC = 290,
     TOKEN_STRING = 291,
     TOKEN_REGEXP = 292,
     TOKEN_REGEXPREPLACE = 293,
     TOKEN_BOOLEAN = 294,
     TOKEN_UNDEF = 295,
     TOKEN_INTEGER = 296,
     TOKEN_DATETIME = 297,
     TOKEN_IP4ADDR = 298,
     UNARY = 299
   };
#endif
/* Tokens.  */
#define TOKEN_ASSIGNMENT 258
#define TOKEN_AND 259
#define TOKEN_OR 260
#define TOKEN_XOR 261
#define TOKEN_BINAND 262
#define TOKEN_BINOR 263
#define TOKEN_BINXOR 264
#define TOKEN_IF 265
#define TOKEN_ELSE 266
#define TOKEN_MUL 267
#define TOKEN_DIV 268
#define TOKEN_MOD 269
#define TOKEN_PLUS 270
#define TOKEN_MINUS 271
#define TOKEN_REGMATCH 272
#define TOKEN_NOTREGMATCH 273
#define TOKEN_EQUAL 274
#define TOKEN_NOTEQUAL 275
#define TOKEN_GE 276
#define TOKEN_GREATER 277
#define TOKEN_LE 278
#define TOKEN_LESS 279
#define TOKEN_LEFTBRACKET 280
#define TOKEN_RIGHTBRACKET 281
#define TOKEN_LEFTBRACE 282
#define TOKEN_RIGHTBRACE 283
#define TOKEN_SEMICOLON 284
#define TOKEN_COMMA 285
#define TOKEN_NOT 286
#define TOKEN_DEFINED 287
#define TOKEN_FIELDNAME 288
#define TOKEN_CAPTURED 289
#define TOKEN_FUNCPROC 290
#define TOKEN_STRING 291
#define TOKEN_REGEXP 292
#define TOKEN_REGEXPREPLACE 293
#define TOKEN_BOOLEAN 294
#define TOKEN_UNDEF 295
#define TOKEN_INTEGER 296
#define TOKEN_DATETIME 297
#define TOKEN_IP4ADDR 298
#define UNARY 299




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 221 "expr-grammar.c"

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
# if YYENABLE_NLS
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
#define YYFINAL  42
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   407

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  19
/* YYNRULES -- Number of rules.  */
#define YYNRULES  69
/* YYNRULES -- Number of states.  */
#define YYNSTATES  114

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   299

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,    45,     2,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     4,     6,     8,    10,    13,    15,    17,
      19,    25,    28,    31,    34,    37,    39,    43,    49,    52,
      56,    61,    66,    70,    74,    78,    82,    86,    88,    89,
      91,    93,    97,    99,   101,   103,   106,   110,   113,   116,
     120,   124,   128,   132,   136,   140,   144,   148,   152,   156,
     160,   164,   168,   172,   176,   180,   184,   188,   192,   196,
     198,   202,   204,   206,   208,   210,   212,   214,   216,   218
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      47,     0,    -1,    -1,    48,    -1,    61,    -1,    49,    -1,
      48,    49,    -1,    51,    -1,    50,    -1,    52,    -1,    10,
      61,    50,    11,    50,    -1,    53,    29,    -1,    55,    29,
      -1,    56,    29,    -1,    57,    29,    -1,    29,    -1,    10,
      61,    49,    -1,    10,    61,    50,    11,    51,    -1,    27,
      28,    -1,    27,    48,    28,    -1,    35,    25,    59,    26,
      -1,    35,    25,    59,    26,    -1,    58,     3,    61,    -1,
      61,    17,    62,    -1,    61,    18,    62,    -1,    61,    17,
      63,    -1,    61,    18,    63,    -1,    61,    -1,    -1,    60,
      -1,    61,    -1,    60,    30,    61,    -1,    64,    -1,    33,
      -1,    34,    -1,    45,    61,    -1,    61,    16,    61,    -1,
      31,    61,    -1,    32,    61,    -1,    61,    12,    61,    -1,
      61,    13,    61,    -1,    61,    14,    61,    -1,    61,    15,
      61,    -1,    61,     7,    61,    -1,    61,     9,    61,    -1,
      61,     8,    61,    -1,    61,    17,    61,    -1,    61,    18,
      61,    -1,    61,    17,    62,    -1,    61,    18,    62,    -1,
      61,    19,    61,    -1,    61,    20,    61,    -1,    61,    24,
      61,    -1,    61,    23,    61,    -1,    61,    22,    61,    -1,
      61,    21,    61,    -1,    61,     4,    61,    -1,    61,     6,
      61,    -1,    61,     5,    61,    -1,    54,    -1,    25,    61,
      26,    -1,    38,    -1,    37,    -1,    36,    -1,    37,    -1,
      39,    -1,    40,    -1,    41,    -1,    42,    -1,    43,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   109,   109,   110,   119,   130,   134,   140,   141,   144,
     149,   154,   159,   164,   169,   174,   181,   186,   194,   198,
     204,   211,   218,   225,   229,   236,   240,   247,   254,   255,
     258,   263,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   280,   281,   282,   283,   284,   285,   286,   287,
     288,   289,   290,   291,   292,   293,   294,   295,   296,   297,
     298,   301,   304,   307,   308,   309,   310,   311,   312,   313
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "\"=\"", "TOKEN_AND", "TOKEN_OR",
  "TOKEN_XOR", "\"&\"", "\"|\"", "\"^\"", "TOKEN_IF", "TOKEN_ELSE",
  "\"*\"", "\"/\"", "\"%\"", "\"+\"", "\"-\"", "\"=~\"", "\"!~\"",
  "\"==\"", "\"!=\"", "\">=\"", "\">\"", "\"<=\"", "\"<\"", "\"(\"",
  "\")\"", "\"{\"", "\"}\"", "\";\"", "\",\"", "TOKEN_NOT",
  "TOKEN_DEFINED", "TOKEN_FIELDNAME", "TOKEN_CAPTURED", "TOKEN_FUNCPROC",
  "TOKEN_STRING", "TOKEN_REGEXP", "TOKEN_REGEXPREPLACE", "TOKEN_BOOLEAN",
  "TOKEN_UNDEF", "TOKEN_INTEGER", "TOKEN_DATETIME", "TOKEN_IP4ADDR",
  "UNARY", "'-'", "$accept", "nxblock", "stmt_list", "stmt", "stmt_no_if",
  "stmt_if", "stmt_block", "procedure", "function", "assignment",
  "stmt_regexpreplace", "stmt_regexp", "left_value", "opt_exprs", "exprs",
  "expr", "regexpreplace", "regexp", "literal", 0
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
     295,   296,   297,   298,   299,    45
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    46,    47,    47,    47,    48,    48,    49,    49,    50,
      50,    50,    50,    50,    50,    50,    51,    51,    52,    52,
      53,    54,    55,    56,    56,    57,    57,    58,    59,    59,
      60,    60,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    62,    63,    64,    64,    64,    64,    64,    64,    64
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     1,     1,     1,     2,     1,     1,     1,
       5,     2,     2,     2,     2,     1,     3,     5,     2,     3,
       4,     4,     3,     3,     3,     3,     3,     1,     0,     1,
       1,     3,     1,     1,     1,     2,     3,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     0,     0,    15,     0,     0,    33,    34,     0,
      63,    64,    65,    66,    67,    68,    69,     0,     0,     3,
       5,     8,     7,     9,     0,    59,     0,     0,     0,     0,
       4,    32,     0,     0,     0,    18,     0,    27,    37,    38,
      28,    35,     1,     6,    11,    12,    13,    14,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    28,     0,
       0,    16,     8,    60,    19,     0,    29,    30,    22,    56,
      58,    57,    43,    45,    44,    39,    40,    41,    42,    36,
      64,    61,    46,    48,    25,    47,    49,    26,    50,    51,
      55,    54,    53,    52,     0,    48,    49,     0,    21,     0,
      21,    10,    17,    31
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    75,    76,    37,    93,    94,    31
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -55
static const yytype_int16 yypact[] =
{
     135,   324,   324,    -6,   -55,   324,   324,   -55,   -55,   -12,
     -55,   -55,   -55,   -55,   -55,   -55,   -55,   324,    14,   135,
     -55,   -55,   -55,   -55,   -11,   -55,    -9,     3,    16,    43,
     201,   -55,    22,    79,   177,   -55,   107,   222,   -55,   -55,
     324,   -55,   -55,   -55,   -55,   -55,   -55,   -55,   324,   324,
     324,   324,   324,   324,   324,   324,   324,   324,   324,   324,
     286,   286,   324,   324,   324,   324,   324,   324,   324,   305,
     305,   -55,    60,   -55,   -55,    46,    75,   243,   243,   363,
     264,   285,    -4,   381,   391,   -55,   -55,   -55,    30,    30,
      47,   -55,    66,    61,   -55,    66,    78,   -55,    66,    66,
      66,    66,    66,    66,    97,   -55,   -55,   135,    96,   324,
     -55,   -55,   -55,   243
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -55,   -55,   123,     5,   -30,    20,   -55,   -55,   -55,   -55,
     -55,   -55,   -55,    62,   -55,     0,   -54,    67,   -55
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -63
static const yytype_int8 yytable[] =
{
      30,    33,    34,    72,     1,    38,    39,    96,    55,    56,
      57,    58,    59,    40,    42,   105,   106,    41,    44,     2,
      45,     3,    35,     4,    43,     5,     6,     7,     8,     9,
      10,    11,    46,    12,    13,    14,    15,    16,    71,    17,
      77,    43,    55,    56,    57,    47,    48,    68,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      92,    95,    98,    99,   100,   101,   102,   103,    77,    92,
      95,   107,   108,    52,    53,    54,   -62,   111,    55,    56,
      57,    58,    59,    49,    50,    51,    52,    53,    54,     1,
     -23,    55,    56,    57,    58,    59,    69,    70,    62,    63,
      64,    65,    66,    67,     2,   109,     3,   -24,     4,   113,
       5,     6,     7,     8,     9,    10,    11,     1,    12,    13,
      14,    15,    16,   110,    17,   -20,    36,   112,    97,     0,
     104,     0,     2,     0,     3,    74,     4,     0,     5,     6,
       7,     8,     9,    10,    11,     1,    12,    13,    14,    15,
      16,     0,    17,     0,     0,     0,     0,     0,     0,     0,
       2,     0,     3,     0,     4,     0,     5,     6,     7,     8,
       9,    10,    11,     0,    12,    13,    14,    15,    16,     0,
      17,    49,    50,    51,    52,    53,    54,     0,     0,    55,
      56,    57,    58,    59,    69,    70,    62,    63,    64,    65,
      66,    67,     0,    73,   -27,    49,    50,    51,    52,    53,
      54,     0,     0,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    49,    50,    51,    52,
      53,    54,     0,     0,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    49,    50,    51,
      52,    53,    54,     0,     0,    55,    56,    57,    58,    59,
      69,    70,    62,    63,    64,    65,    66,    67,    49,     0,
      51,    52,    53,    54,     0,     0,    55,    56,    57,    58,
      59,    69,    70,    62,    63,    64,    65,    66,    67,    49,
       0,     0,    52,    53,    54,     0,     0,    55,    56,    57,
      58,    59,    69,    70,    62,    63,    64,    65,    66,    67,
       0,     2,     0,     0,     0,     0,     0,     5,     6,     7,
       8,    32,    10,    90,    91,    12,    13,    14,    15,    16,
       2,    17,     0,     0,     0,     0,     5,     6,     7,     8,
      32,    10,    11,    91,    12,    13,    14,    15,    16,     2,
      17,     0,     0,     0,     0,     5,     6,     7,     8,    32,
      10,    11,     0,    12,    13,    14,    15,    16,     0,    17,
      52,    53,    54,     0,     0,    55,    56,    57,    58,    59,
      69,    70,    62,    63,    64,    65,    66,    67,    52,     0,
      54,     0,     0,    55,    56,    57,    58,    59,    52,     0,
       0,     0,     0,    55,    56,    57,    58,    59
};

static const yytype_int8 yycheck[] =
{
       0,     1,     2,    33,    10,     5,     6,    61,    12,    13,
      14,    15,    16,    25,     0,    69,    70,    17,    29,    25,
      29,    27,    28,    29,    19,    31,    32,    33,    34,    35,
      36,    37,    29,    39,    40,    41,    42,    43,    33,    45,
      40,    36,    12,    13,    14,    29,     3,    25,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    11,    26,     7,     8,     9,    29,   107,    12,    13,
      14,    15,    16,     4,     5,     6,     7,     8,     9,    10,
      29,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    30,    27,    29,    29,   109,
      31,    32,    33,    34,    35,    36,    37,    10,    39,    40,
      41,    42,    43,    26,    45,    29,     3,   107,    61,    -1,
      68,    -1,    25,    -1,    27,    28,    29,    -1,    31,    32,
      33,    34,    35,    36,    37,    10,    39,    40,    41,    42,
      43,    -1,    45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    27,    -1,    29,    -1,    31,    32,    33,    34,
      35,    36,    37,    -1,    39,    40,    41,    42,    43,    -1,
      45,     4,     5,     6,     7,     8,     9,    -1,    -1,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    -1,    26,     3,     4,     5,     6,     7,     8,
       9,    -1,    -1,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,     4,     5,     6,     7,
       8,     9,    -1,    -1,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,     4,     5,     6,
       7,     8,     9,    -1,    -1,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,     4,    -1,
       6,     7,     8,     9,    -1,    -1,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,     4,
      -1,    -1,     7,     8,     9,    -1,    -1,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      25,    45,    -1,    -1,    -1,    -1,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    25,
      45,    -1,    -1,    -1,    -1,    31,    32,    33,    34,    35,
      36,    37,    -1,    39,    40,    41,    42,    43,    -1,    45,
       7,     8,     9,    -1,    -1,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,     7,    -1,
       9,    -1,    -1,    12,    13,    14,    15,    16,     7,    -1,
      -1,    -1,    -1,    12,    13,    14,    15,    16
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    10,    25,    27,    29,    31,    32,    33,    34,    35,
      36,    37,    39,    40,    41,    42,    43,    45,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      61,    64,    35,    61,    61,    28,    48,    61,    61,    61,
      25,    61,     0,    49,    29,    29,    29,    29,     3,     4,
       5,     6,     7,     8,     9,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    17,
      18,    49,    50,    26,    28,    59,    60,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      37,    38,    61,    62,    63,    61,    62,    63,    61,    61,
      61,    61,    61,    61,    59,    62,    62,    11,    26,    30,
      26,    50,    51,    61
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
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

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
      yyerror (parser, scanner, YY_("syntax error: cannot back up")); \
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
# if YYLTYPE_IS_TRIVIAL
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
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, scanner)
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
		  Type, Value, parser, scanner); \
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
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, nx_expr_parser_t *parser, void *scanner)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, parser, scanner)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    nx_expr_parser_t *parser;
    void *scanner;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (parser);
  YYUSE (scanner);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, nx_expr_parser_t *parser, void *scanner)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, parser, scanner)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    nx_expr_parser_t *parser;
    void *scanner;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep, parser, scanner);
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
yy_reduce_print (YYSTYPE *yyvsp, int yyrule, nx_expr_parser_t *parser, void *scanner)
#else
static void
yy_reduce_print (yyvsp, yyrule, parser, scanner)
    YYSTYPE *yyvsp;
    int yyrule;
    nx_expr_parser_t *parser;
    void *scanner;
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
		       		       , parser, scanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule, parser, scanner); \
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, nx_expr_parser_t *parser, void *scanner)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, parser, scanner)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    nx_expr_parser_t *parser;
    void *scanner;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (parser);
  YYUSE (scanner);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
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
int yyparse (nx_expr_parser_t *parser, void *scanner);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */





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
yyparse (nx_expr_parser_t *parser, void *scanner)
#else
int
yyparse (parser, scanner)
    nx_expr_parser_t *parser;
    void *scanner;
#endif
#endif
{
/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

    /* Number of syntax errors so far.  */
    int yynerrs;

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
        case 2:

/* Line 1455 of yacc.c  */
#line 109 "expr-grammar.y"
    { log_debug("empty block"); }
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 111 "expr-grammar.y"
    {
		       if ( parser->parse_expression == TRUE )
		       {
			   nx_expr_parser_error(parser, NULL, "Expression required, found a statement");
		       }
		       parser->statements = (yyvsp[(1) - (1)].statement_list);
		       log_debug("finished parsing statements"); 
		   }
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 120 "expr-grammar.y"
    {
		       if ( parser->parse_expression != TRUE )
		       {
			   nx_expr_parser_error(parser, NULL, "Statement required, expression found");
		       }
		       parser->expression = (yyvsp[(1) - (1)].expr);
		       log_debug("parsed expression");
                   }
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 131 "expr-grammar.y"
    {
			(yyval.statement_list) = nx_expr_statement_list_new(parser, (yyvsp[(1) - (1)].statement));
		    }
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 135 "expr-grammar.y"
    {
			(yyval.statement_list) = nx_expr_statement_list_add(parser, (yyvsp[(1) - (2)].statement_list), (yyvsp[(2) - (2)].statement));
		    }
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 145 "expr-grammar.y"
    {
			(yyval.statement) = (yyvsp[(1) - (1)].statement);
			log_debug("statement: block");
		    }
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 150 "expr-grammar.y"
    {
			(yyval.statement) = nx_expr_statement_new_ifelse(parser, (yyvsp[(2) - (5)].expr), (yyvsp[(3) - (5)].statement), (yyvsp[(5) - (5)].statement));
			log_debug("if-else");
		    }
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 155 "expr-grammar.y"
    {
			(yyval.statement) = (yyvsp[(1) - (2)].statement);
			log_debug("statement: procedure");
		    }
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 160 "expr-grammar.y"
    {
			(yyval.statement) = (yyvsp[(1) - (2)].statement);
			log_debug("statement: assignment");
		    }
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 165 "expr-grammar.y"
    {
			(yyval.statement) = (yyvsp[(1) - (2)].statement);
			log_debug("statement: regexpreplace");
		    }
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 170 "expr-grammar.y"
    {
			(yyval.statement) = (yyvsp[(1) - (2)].statement);
			log_debug("statement: regexp");
		    }
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 175 "expr-grammar.y"
    {
			(yyval.statement) = NULL;
			log_debug("empty statement - single semicolon");
		    }
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 182 "expr-grammar.y"
    {
			(yyval.statement) = nx_expr_statement_new_ifelse(parser, (yyvsp[(2) - (3)].expr), (yyvsp[(3) - (3)].statement), NULL);
			log_debug("if");
		    }
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 187 "expr-grammar.y"
    {
			(yyval.statement) = nx_expr_statement_new_ifelse(parser, (yyvsp[(2) - (5)].expr), (yyvsp[(3) - (5)].statement), (yyvsp[(5) - (5)].statement));
			log_debug("if-else");
		    }
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 195 "expr-grammar.y"
    {
			(yyval.statement) = NULL;
		    }
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 199 "expr-grammar.y"
    {
			(yyval.statement) = nx_expr_statement_new_block(parser, (yyvsp[(2) - (3)].statement_list));
		    }
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 205 "expr-grammar.y"
    {
		       (yyval.statement) = nx_expr_statement_new_procedure(parser, (yyvsp[(1) - (4)].string), (yyvsp[(3) - (4)].args));
		       log_debug("procedure");
		   }
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 212 "expr-grammar.y"
    {
		       (yyval.expr) = nx_expr_new_function(parser, (yyvsp[(1) - (4)].string), (yyvsp[(3) - (4)].args));
		       log_debug("new function: %s", (yyvsp[(1) - (4)].string));
		   }
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 219 "expr-grammar.y"
    {
			(yyval.statement) = nx_expr_statement_new_assignment(parser, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr));
			log_debug("assignment: left_value = expr");
		    }
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 226 "expr-grammar.y"
    {
			    (yyval.statement) = nx_expr_statement_new_regexpreplace(parser, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
			}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 230 "expr-grammar.y"
    {
			    log_warn("useless use of negative pattern binding (!~) in regexp replacement");
			    (yyval.statement) = nx_expr_statement_new_regexpreplace(parser, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
			}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 237 "expr-grammar.y"
    {
			(yyval.statement) = nx_expr_statement_new_regexp(parser, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
		    }
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 241 "expr-grammar.y"
    {
			log_warn("useless use of negative pattern binding (!~) in regexp match");
			(yyval.statement) = nx_expr_statement_new_regexp(parser, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); 
		    }
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 248 "expr-grammar.y"
    {
			(yyval.expr) = (yyvsp[(1) - (1)].expr);
			log_debug("left_value expr");
		    }
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 254 "expr-grammar.y"
    { (yyval.args) = NULL; }
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 255 "expr-grammar.y"
    { (yyval.args) = (yyvsp[(1) - (1)].args); }
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 259 "expr-grammar.y"
    {
			(yyval.args) = nx_expr_arg_list_new(parser, (yyvsp[(1) - (1)].expr));
			//log_debug("expr");
		    }
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 264 "expr-grammar.y"
    {
			(yyval.args) = nx_expr_arg_list_add(parser, (yyvsp[(1) - (3)].args), (yyvsp[(3) - (3)].expr));
			//log_debug("exprs, expr");
		    }
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 270 "expr-grammar.y"
    { log_debug("literal"); (yyval.expr) = (yyvsp[(1) - (1)].expr); }
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 271 "expr-grammar.y"
    { (yyval.expr) = nx_expr_new_field(parser, (yyvsp[(1) - (1)].string)); }
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 272 "expr-grammar.y"
    { (yyval.expr) = nx_expr_new_captured(parser, (yyvsp[(1) - (1)].string)); }
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 273 "expr-grammar.y"
    { (yyval.expr) = nx_expr_new_unop(parser, TOKEN_MINUS, (yyvsp[(2) - (2)].expr)); }
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 274 "expr-grammar.y"
    { (yyval.expr) = nx_expr_new_binop(parser, TOKEN_MINUS, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 275 "expr-grammar.y"
    { (yyval.expr) = nx_expr_new_unop(parser, TOKEN_NOT, (yyvsp[(2) - (2)].expr)); }
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 276 "expr-grammar.y"
    { (yyval.expr) = nx_expr_new_unop(parser, TOKEN_DEFINED, (yyvsp[(2) - (2)].expr)); }
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 277 "expr-grammar.y"
    { (yyval.expr) = nx_expr_new_binop(parser, TOKEN_MUL, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 278 "expr-grammar.y"
    { log_debug("division"); (yyval.expr) = nx_expr_new_binop(parser, TOKEN_DIV, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 279 "expr-grammar.y"
    { (yyval.expr) = nx_expr_new_binop(parser, TOKEN_MOD, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 280 "expr-grammar.y"
    { (yyval.expr) = nx_expr_new_binop(parser, TOKEN_PLUS, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 281 "expr-grammar.y"
    { (yyval.expr) = nx_expr_new_binop(parser, TOKEN_BINAND, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 282 "expr-grammar.y"
    { (yyval.expr) = nx_expr_new_binop(parser, TOKEN_BINXOR, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 283 "expr-grammar.y"
    { (yyval.expr) = nx_expr_new_binop(parser, TOKEN_BINOR, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 284 "expr-grammar.y"
    { (yyval.expr) = nx_expr_new_binop(parser, TOKEN_REGMATCH, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 285 "expr-grammar.y"
    { (yyval.expr) = nx_expr_new_binop(parser, TOKEN_NOTREGMATCH, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 286 "expr-grammar.y"
    { (yyval.expr) = nx_expr_new_binop(parser, TOKEN_REGMATCH, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 287 "expr-grammar.y"
    { (yyval.expr) = nx_expr_new_binop(parser, TOKEN_NOTREGMATCH, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 288 "expr-grammar.y"
    { (yyval.expr) = nx_expr_new_binop(parser, TOKEN_EQUAL, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 289 "expr-grammar.y"
    { (yyval.expr) = nx_expr_new_binop(parser, TOKEN_NOTEQUAL, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 290 "expr-grammar.y"
    { (yyval.expr) = nx_expr_new_binop(parser, TOKEN_LESS, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 291 "expr-grammar.y"
    { (yyval.expr) = nx_expr_new_binop(parser, TOKEN_LE, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 292 "expr-grammar.y"
    { (yyval.expr) = nx_expr_new_binop(parser, TOKEN_GREATER, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 293 "expr-grammar.y"
    { (yyval.expr) = nx_expr_new_binop(parser, TOKEN_GE, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 294 "expr-grammar.y"
    { (yyval.expr) = nx_expr_new_binop(parser, TOKEN_AND, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 295 "expr-grammar.y"
    { (yyval.expr) = nx_expr_new_binop(parser, TOKEN_XOR, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 296 "expr-grammar.y"
    { (yyval.expr) = nx_expr_new_binop(parser, TOKEN_OR, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 297 "expr-grammar.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); }
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 298 "expr-grammar.y"
    { log_debug("( expr:%d )", (yyvsp[(2) - (3)].expr)->type); (yyval.expr) = (yyvsp[(2) - (3)].expr); }
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 301 "expr-grammar.y"
    { log_debug("regexpreplace"); (yyval.expr) = nx_expr_new_regexp(parser, (yyvsp[(1) - (1)].regexp)[0], (yyvsp[(1) - (1)].regexp)[1], (yyvsp[(1) - (1)].regexp)[2]); }
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 304 "expr-grammar.y"
    { log_debug("regexp"); (yyval.expr) = nx_expr_new_regexp(parser, (yyvsp[(1) - (1)].regexp)[0], NULL, (yyvsp[(1) - (1)].regexp)[2]); }
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 307 "expr-grammar.y"
    { (yyval.expr) = nx_expr_new_string(parser, (yyvsp[(1) - (1)].string)); }
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 308 "expr-grammar.y"
    { log_debug("regexp literal"); (yyval.expr) = nx_expr_new_regexp(parser, (yyvsp[(1) - (1)].regexp)[0], NULL, (yyvsp[(1) - (1)].regexp)[2]); }
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 309 "expr-grammar.y"
    { log_debug("boolean literal"); (yyval.expr) = nx_expr_new_boolean(parser, (yyvsp[(1) - (1)].bool)); }
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 310 "expr-grammar.y"
    { (yyval.expr) = nx_expr_new_undef(parser); }
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 311 "expr-grammar.y"
    { log_debug("integer literal: %s", (yyvsp[(1) - (1)].string)); (yyval.expr) = nx_expr_new_integer(parser, (yyvsp[(1) - (1)].string)); }
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 312 "expr-grammar.y"
    { log_debug("datetime literal: %s", (yyvsp[(1) - (1)].string)); (yyval.expr) = nx_expr_new_datetime(parser, (yyvsp[(1) - (1)].string)); }
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 313 "expr-grammar.y"
    { log_debug("ip4addr literal: %s", (yyvsp[(1) - (1)].string)); (yyval.expr) = nx_expr_new_ip4addr(parser, (yyvsp[(1) - (1)].string)); }
    break;



/* Line 1455 of yacc.c  */
#line 2127 "expr-grammar.c"
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
      yyerror (parser, scanner, YY_("syntax error"));
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
	    yyerror (parser, scanner, yymsg);
	  }
	else
	  {
	    yyerror (parser, scanner, YY_("syntax error"));
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
		      yytoken, &yylval, parser, scanner);
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
		  yystos[yystate], yyvsp, parser, scanner);
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
  yyerror (parser, scanner, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, parser, scanner);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, parser, scanner);
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



/* Line 1675 of yacc.c  */
#line 315 "expr-grammar.y"


int nx_expr_parser_lex_init(void **);
int nx_expr_parser_lex_destroy(void *);
void nx_expr_parser_set_extra(YY_EXTRA_TYPE, void *);


static void parser_do(nx_expr_parser_t *parser,
		      nx_module_t *module,
		      const char *str,
		      boolean parse_expression,
		      apr_pool_t *pool,
		      const char *filename,
		      int currline,
		      int currpos)
{
    nx_exception_t e;

    ASSERT(parser != NULL);

    memset(parser, 0, sizeof(nx_expr_parser_t));
    parser->buf = str;
    parser->length = (int) strlen(str);
    parser->pos = 0;
    parser->pool = pool;
    parser->linenum = currline;
    parser->linepos = currpos;
    if ( filename == NULL )
    {
	parser->file = NULL;
    }
    else
    {
	parser->file = apr_pstrdup(pool, filename);
    }
    parser->parse_expression = parse_expression;
    parser->module = module;

    try
    {
	nx_expr_parser_lex_init(&(parser->yyscanner));
	nx_expr_parser_set_extra(parser, parser->yyscanner);
	yyparse(parser, parser->yyscanner);
    }
    catch(e)
    {
	if ( parser->file == NULL )
	{
	    rethrow_msg(e, "couldn't parse %s at line %d, character %d",
			parse_expression == TRUE ? "expression" : "statement",
			parser->linenum, parser->linepos);
	}
	else
	{
	    rethrow_msg(e, "couldn't parse %s at line %d, character %d in %s",
			parse_expression == TRUE ? "expression" : "statement",
			parser->linenum, parser->linepos, parser->file);
	}
    }

    nx_expr_parser_lex_destroy(parser->yyscanner);
}



nx_expr_statement_list_t *nx_expr_parse_statements(nx_module_t *module,
						   const char *str,
						   apr_pool_t *pool,
						   const char *filename,
						   int currline,
						   int currpos)
{
    nx_expr_parser_t parser;

    parser_do(&parser, module, str, FALSE, pool, filename, currline, currpos);

    return ( parser.statements );
}



nx_expr_t *nx_expr_parse(nx_module_t *module,
			 const char *str,
			 apr_pool_t *pool,
			 const char *filename,
			 int currline,
			 int currpos)
{
    nx_expr_parser_t parser;

    parser_do(&parser, module, str, TRUE, pool, filename, currline, currpos);

    return ( parser.expression );
}


