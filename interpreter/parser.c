/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

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
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* "%code top" blocks.  */
#line 103 "./interpreter/tScript.y" /* yacc.c:316  */

    #include "list.h"
    #include "program.h"
    #include "statement.h"
    #include "expression.h"
    #include "object.h"
    #include "func.h"
    #include "pair.h"

    #define YYDEBUG 1

#line 75 "./interpreter/parser.c" /* yacc.c:316  */



/* Copy the first part of user declarations.  */

#line 81 "./interpreter/parser.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif


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
    COMMA = 266,
    COLON = 267,
    SEMICOLON = 268,
    OPEN_CURVE_BRACKET = 269,
    CLOSE_CURVE_BRACKET = 270,
    INT = 271,
    STR = 272,
    IDENTIFIER = 273,
    NUM = 274,
    STR_LITERAL = 275,
    PERCENTAGE = 276,
    EQUAL = 277,
    LESS_THAN = 278,
    GREATER_THAN = 279,
    LESS_OR_EQUAL = 280,
    GREATER_OR_EQUAL = 281,
    NOT_EQUAL = 282,
    PLUS = 283,
    MINUS = 284,
    MUL = 285,
    DIV = 286,
    ASSIGNMENT = 287,
    OPEN_PAREN = 288,
    CLOSE_PAREN = 289,
    DOT = 290,
    OPEN_SQUARE_BRACKET = 291,
    CLOSE_SQUARE_BRACKET = 292
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 1 "./interpreter/tScript.y" /* yacc.c:355  */

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

#line 171 "./interpreter/parser.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);



/* Copy the second part of user declarations.  */

#line 188 "./interpreter/parser.c" /* yacc.c:358  */

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
#else
typedef signed char yytype_int8;
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
# elif ! defined YYSIZE_T
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
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
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

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  68
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   369

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  38
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  53
/* YYNRULES -- Number of rules.  */
#define YYNRULES  106
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  186

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   292

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
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
      35,    36,    37
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   119,   119,   130,   133,   140,   141,   142,   143,   144,
     145,   146,   150,   153,   159,   160,   167,   170,   171,   172,
     173,   174,   175,   179,   188,   191,   198,   207,   219,   224,
     234,   237,   240,   243,   250,   256,   265,   268,   275,   278,
     285,   293,   304,   307,   314,   320,   325,   328,   331,   334,
     339,   342,   345,   349,   351,   355,   358,   365,   368,   369,
     370,   371,   372,   373,   374,   375,   376,   377,   381,   387,
     391,   395,   401,   407,   408,   409,   410,   414,   420,   426,
     432,   438,   439,   440,   441,   442,   443,   447,   453,   460,
     466,   472,   478,   484,   493,   499,   502,   508,   511,   517,
     520,   527,   533,   536,   543,   549,   553
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IMPORT", "FROM", "RETURN", "OBJECT",
  "BELONG", "MACHINE_OPERATIONS", "IF", "ELSE", "COMMA", "COLON",
  "SEMICOLON", "OPEN_CURVE_BRACKET", "CLOSE_CURVE_BRACKET", "INT", "STR",
  "IDENTIFIER", "NUM", "STR_LITERAL", "PERCENTAGE", "EQUAL", "LESS_THAN",
  "GREATER_THAN", "LESS_OR_EQUAL", "GREATER_OR_EQUAL", "NOT_EQUAL", "PLUS",
  "MINUS", "MUL", "DIV", "ASSIGNMENT", "OPEN_PAREN", "CLOSE_PAREN", "DOT",
  "OPEN_SQUARE_BRACKET", "CLOSE_SQUARE_BRACKET", "$accept", "PROGRAM",
  "STATEMENTS", "STATEMENT", "IF_STATEMENT", "IF_BLOCK",
  "IF_STATEMENT_WITH_ELSE", "OBJECT_DECL_STATEMENT", "OBJECT_INHERITENCE",
  "OBJECT_DEF", "OBJECT_DEF_ITEM", "IMPORT_STATEMENT", "IMPORT_LIST",
  "IDENTIFIER_LIST", "RETURN_STATEMENT", "VAR_DECL_STATEMENT",
  "VAR_DECL_LIST", "VAR_DECL", "FUNC_DECL_STATEMENT", "PARAMETERS",
  "PARAMETER", "EXPRESSION_STATEMENT", "TYPE", "DECL_QUALIFIER",
  "ARRAY_QUALIFIER", "MAP_QUALIFIER", "EXPRESSION_LIST", "EXPRESSION",
  "PERCENT_EXPRESSION", "ARRAY_EXPRESSION", "ARRAY_ELEMENTS",
  "INDEX_EXPRESSION", "IDENT_EXPRESSION", "ARITHMETIC_EXPRESSION",
  "PLUS_EXPRESSION", "MINUS_EXPRESSION", "MUL_EXPRESSION",
  "DIV_EXPRESSION", "ORDER_EXPRESSION", "LESS_THAN_EXPRESSION",
  "GREATER_THAN_EXPRESSION", "EQUAL_EXPRESSION",
  "LESS_OR_EQUAL_EXPRESSION", "GREATER_OR_EQUAL_EXPRESSION",
  "NOT_EQUAL_EXPRESSION", "DOT_EXPRESSION", "MEMBER_SELECTION_ENTITY",
  "FUNCTION_CALL_EXPRESSION", "ARGUMENTS", "ASSIGNMENT_EXPRESSION",
  "BLOCK", "BIG_BLOCK", "CONSTANT_EXPRESSION", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292
};
# endif

#define YYPACT_NINF -140

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-140)))

#define YYTABLE_NINF -53

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     187,   -16,   262,    22,    14,    16,    14,    14,    79,  -140,
    -140,   262,   262,    72,   187,  -140,  -140,  -140,  -140,  -140,
    -140,  -140,  -140,    66,   203,  -140,  -140,  -140,  -140,  -140,
    -140,  -140,  -140,  -140,  -140,  -140,  -140,  -140,  -140,  -140,
    -140,    69,  -140,    63,  -140,  -140,  -140,    95,    98,    68,
     223,   106,   109,    80,  -140,  -140,  -140,   262,  -140,  -140,
     258,  -140,   129,  -140,   295,   114,   337,    90,  -140,  -140,
     -20,    43,  -140,  -140,    97,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,  -140,  -140,   115,   116,
     262,  -140,   118,   132,  -140,  -140,   309,  -140,   337,     8,
     278,  -140,   262,  -140,   262,     9,   133,  -140,   262,   180,
     180,   180,   180,   180,   180,    60,    60,   131,   131,   131,
    -140,  -140,  -140,    78,   102,   262,  -140,  -140,   337,   337,
      14,   136,    35,  -140,   149,   141,  -140,    36,     0,   137,
    -140,   158,   145,   187,  -140,   170,  -140,   181,   184,   188,
     190,   191,   204,  -140,  -140,   337,   187,  -140,   171,   136,
    -140,  -140,   179,   200,  -140,   202,   262,    15,   123,   166,
    -140,  -140,   185,  -140,  -140,   323,   208,  -140,  -140,   262,
     102,   243,   209,  -140,   102,  -140
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,    52,     0,    52,    52,    72,   105,
     106,     0,     0,     0,     2,     4,     5,     6,     7,     8,
       9,    10,    11,     0,     0,    67,    66,    65,    64,    58,
      73,    74,    75,    76,    59,    81,    82,    83,    84,    85,
      86,    60,    94,    61,    62,    63,    33,     0,    31,    72,
       0,    25,     0,     0,    48,    50,    51,     0,    46,    47,
       0,    95,     0,    49,     0,    70,    56,     0,     1,     3,
      38,     0,    37,    45,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    96,     0,     0,
       0,    34,     0,     0,    54,    53,     0,    98,   100,     0,
       0,    57,     0,    69,     0,     0,     0,    35,     0,    89,
      87,    88,    90,    91,    92,    77,    78,    79,    80,   101,
      30,    32,    24,     0,     0,     0,    97,    71,    55,    39,
      52,     0,     0,    43,     0,    38,    36,     0,    52,     0,
      27,     0,     0,     0,   103,     0,    15,     6,     7,     8,
       9,    10,    11,    12,   102,    99,     0,    41,     0,     0,
      44,    68,     0,     0,    26,     0,     0,     0,     0,     0,
      42,    40,     0,    23,    29,     0,   104,    13,   104,     0,
       0,     0,     0,    28,     0,    16
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -140,  -140,  -104,     1,  -140,  -139,  -140,  -121,  -140,  -140,
      82,  -119,  -140,  -140,  -118,  -116,  -140,   134,  -113,  -140,
      64,   -95,  -101,   154,  -140,  -140,   130,    -2,  -140,  -140,
    -140,  -140,  -140,  -140,  -140,  -140,  -140,  -140,  -140,  -140,
    -140,  -140,  -140,  -140,  -140,  -140,  -140,  -140,  -140,  -140,
    -131,  -115,  -140
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    13,    14,   144,    16,   145,   146,    17,    93,   139,
     140,    18,    47,    48,    19,    20,    71,    72,    21,   132,
     133,    22,    23,    63,    55,    56,    65,    24,    25,    26,
      67,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    99,    44,
     153,   154,    45
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      50,    15,    46,   147,   134,   148,   149,   162,   150,    64,
      66,   151,   104,   105,    52,    69,   157,     4,     1,   125,
       2,     3,   141,     4,     5,     6,     7,   130,    52,   152,
     176,     6,     7,     8,     9,    10,    53,   177,   141,   167,
      51,   182,   126,   131,   171,   185,   158,   102,    11,    57,
      53,    12,   169,   177,   106,    96,   107,   134,    98,   147,
     100,   148,   149,   147,   150,   148,   149,   151,   150,   159,
     161,   151,    68,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,    70,   152,     4,    86,   100,   152,
      83,    84,    85,    52,     6,     7,   138,   -52,    87,    88,
     128,    60,   129,    61,    90,     1,    66,     2,     3,    89,
       4,   142,    60,    92,    61,    62,   143,    95,     6,     7,
       8,     9,    10,   155,    94,   102,     1,   103,     2,     3,
     108,     4,     5,   120,   121,    11,   122,   156,    12,     6,
       7,     8,     9,    10,    15,     4,   123,    49,     9,    10,
     156,   135,   163,     6,     7,   138,    11,    15,    54,    12,
      58,    59,    11,    85,   175,    12,    95,   160,    69,     1,
      69,     2,     3,   104,     4,     5,   165,   181,   166,     4,
     168,   178,     6,     7,     8,     9,    10,     6,     7,   130,
       1,   -17,     2,     3,   -18,     4,     5,   172,   -19,    11,
     -20,   -21,    12,     6,     7,     8,     9,    10,    81,    82,
      83,    84,    85,   173,   -22,   174,    73,   179,   -14,   184,
      11,   164,   170,    12,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    91,     0,   137,     0,
     136,     0,     0,     0,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,   183,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    49,     9,    10,     0,
      49,     9,    10,     0,     0,     0,     0,     0,     0,     0,
       0,    11,    97,     0,    12,    11,     0,     0,    12,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,     0,     0,     0,     0,   127,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,     0,   101,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,     0,   124,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,     0,   180,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85
};

static const yytype_int16 yycheck[] =
{
       2,     0,    18,   124,   105,   124,   124,     7,   124,    11,
      12,   124,    32,    33,    14,    14,   131,     8,     3,    11,
       5,     6,   123,     8,     9,    16,    17,    18,    14,   124,
      15,    16,    17,    18,    19,    20,    36,   168,   139,   143,
      18,   180,    34,    34,   159,   184,    11,    11,    33,    33,
      36,    36,   156,   184,    11,    57,    13,   158,    60,   180,
      62,   180,   180,   184,   180,   184,   184,   180,   184,    34,
      34,   184,     0,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    18,   180,     8,    18,    90,   184,
      30,    31,    32,    14,    16,    17,    18,    18,    35,     4,
     102,    33,   104,    35,    36,     3,   108,     5,     6,    11,
       8,     9,    33,     7,    35,    36,    14,    37,    16,    17,
      18,    19,    20,   125,    15,    11,     3,    37,     5,     6,
      33,     8,     9,    18,    18,    33,    18,    14,    36,    16,
      17,    18,    19,    20,   143,     8,    14,    18,    19,    20,
      14,    18,    15,    16,    17,    18,    33,   156,     4,    36,
       6,     7,    33,    32,   166,    36,    37,    18,   167,     3,
     169,     5,     6,    32,     8,     9,    18,   179,    33,     8,
      10,    15,    16,    17,    18,    19,    20,    16,    17,    18,
       3,    10,     5,     6,    10,     8,     9,    18,    10,    33,
      10,    10,    36,    16,    17,    18,    19,    20,    28,    29,
      30,    31,    32,    13,    10,    13,    13,    32,    10,    10,
      33,   139,   158,    36,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    13,    -1,   108,    -1,
     106,    -1,    -1,    -1,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    18,    19,    20,    -1,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    33,    -1,    -1,    36,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    -1,    -1,    -1,    -1,    37,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    -1,    34,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    -1,    34,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    34,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     5,     6,     8,     9,    16,    17,    18,    19,
      20,    33,    36,    39,    40,    41,    42,    45,    49,    52,
      53,    56,    59,    60,    65,    66,    67,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    87,    90,    18,    50,    51,    18,
      65,    18,    14,    36,    61,    62,    63,    33,    61,    61,
      33,    35,    36,    61,    65,    64,    65,    68,     0,    41,
      18,    54,    55,    13,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    18,    35,     4,    11,
      36,    13,     7,    46,    15,    37,    65,    34,    65,    86,
      65,    34,    11,    37,    32,    33,    11,    13,    33,    65,
      65,    65,    65,    65,    65,    65,    65,    65,    65,    65,
      18,    18,    18,    14,    34,    11,    34,    37,    65,    65,
      18,    34,    57,    58,    60,    18,    55,    64,    18,    47,
      48,    60,     9,    14,    41,    43,    44,    45,    49,    52,
      53,    56,    59,    88,    89,    65,    14,    89,    11,    34,
      18,    34,     7,    15,    48,    18,    33,    40,    10,    40,
      58,    89,    18,    13,    13,    65,    15,    88,    15,    32,
      34,    65,    43,    13,    10,    43
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    38,    39,    40,    40,    41,    41,    41,    41,    41,
      41,    41,    42,    42,    43,    43,    44,    44,    44,    44,
      44,    44,    44,    45,    46,    46,    47,    47,    48,    48,
      49,    50,    51,    51,    52,    53,    54,    54,    55,    55,
      56,    56,    57,    57,    58,    59,    60,    60,    60,    60,
      61,    61,    61,    62,    63,    64,    64,    65,    65,    65,
      65,    65,    65,    65,    65,    65,    65,    65,    66,    67,
      68,    69,    70,    71,    71,    71,    71,    72,    73,    74,
      75,    76,    76,    76,    76,    76,    76,    77,    78,    79,
      80,    81,    82,    83,    83,    84,    84,    85,    85,    86,
      86,    87,    88,    88,    89,    90,    90
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     5,     7,     3,     1,     7,     1,     1,     1,
       1,     1,     1,     7,     2,     0,     2,     1,     6,     3,
       4,     1,     3,     1,     3,     3,     3,     1,     1,     3,
       6,     5,     3,     1,     2,     2,     2,     2,     2,     2,
       1,     1,     0,     2,     2,     3,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     5,     3,
       1,     4,     1,     1,     1,     1,     1,     3,     3,     3,
       3,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     2,     1,     2,     2,     4,     3,     3,
       1,     3,     1,     1,     3,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  switch (yytype)
    {
          case 18: /* IDENTIFIER  */
#line 100 "./interpreter/tScript.y" /* yacc.c:684  */
      { fprintf(yyoutput, "%s", ((*yyvaluep).str)); }
#line 859 "./interpreter/parser.c" /* yacc.c:684  */
        break;

    case 65: /* EXPRESSION  */
#line 99 "./interpreter/tScript.y" /* yacc.c:684  */
      { fprintf(yyoutput, "%d", ((*yyvaluep).expression)->type); }
#line 865 "./interpreter/parser.c" /* yacc.c:684  */
        break;


      default:
        break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

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
#ifndef YYINITDEPTH
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
static YYSIZE_T
yystrlen (const char *yystr)
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
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
  int yytoken = 0;
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

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
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
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
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
      if (yytable_value_is_error (yyn))
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
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
     '$$ = $1'.

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
#line 119 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.program) = programGenerate((yyvsp[0].list_));
        (yyval.program)->compute((yyval.program));
    }
#line 1467 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 3:
#line 130 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        listAppend((yyval.list_), (void *)(yyvsp[0].statement));
    }
#line 1475 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 4:
#line 133 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.list_) = listCreate();
        listAppend((yyval.list_), (void *)(yyvsp[0].statement));
    }
#line 1484 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 12:
#line 150 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.statement) = (Statement *)ifStatementGenerate((yyvsp[-2].expression), (yyvsp[0].list_), NULL);
    }
#line 1492 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 13:
#line 153 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.statement) = (Statement *)ifStatementGenerate((yyvsp[-4].expression), (yyvsp[-2].list_), (yyvsp[0].list_));
    }
#line 1500 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 14:
#line 159 "./interpreter/tScript.y" /* yacc.c:1646  */
    { (yyval.list_) = (yyvsp[-1].list_); }
#line 1506 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 15:
#line 160 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.list_) = listCreate();
        listAppend((yyval.list_), (void *)(yyvsp[0].statement));
    }
#line 1515 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 16:
#line 167 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.statement) = (Statement *)ifStatementGenerate((yyvsp[-4].expression), (yyvsp[-2].list_), (yyvsp[0].list_));
    }
#line 1523 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 23:
#line 181 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.statement) = objDeclStmtGen((yyvsp[-5].str), (yyvsp[-2].objDeclBody)->newMembers,
                            (yyvsp[-4].str), (yyvsp[-2].objDeclBody)->overWrites);
    }
#line 1532 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 24:
#line 188 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
       (yyval.str) = IDENTIFIER;
    }
#line 1540 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 25:
#line 191 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.str) = null;
    }
#line 1548 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 26:
#line 198 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        if ((yyvsp[0].objDeclItem)->type == OBJECT_OVER_WRITE) {
            if ((yyval.objDeclBody)->overWrites == NULL) (yyval.objDeclBody)->overWrites = listCreate();
            listAppend((yyval.objDeclBody)->overWrites, (void *)(yyvsp[0].objDeclItem)->item);
        } else {
            listAppend((yyval.objDeclBody)->newMembers, (void *)(yyvsp[0].objDeclItem)->item);
        }
    }
#line 1561 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 27:
#line 207 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.objDeclBody) = objBodyGen();
        if ((yyvsp[0].objDeclItem)->type == OBJECT_OVER_WRITE) {
            (yyval.objDeclBody)->overWrites = listCreate();
            listAppend((yyval.objDeclBody)->overWrites, (void *)(yyvsp[0].objDeclItem)->item);
        } else {
            (yyval.objDeclBody)->newMembers = listCreate();
            listAppend((yyval.objDeclBody)->newMembers, (void *)(yyvsp[0].objDeclItem)->item);
        }
    }
#line 1576 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 28:
#line 219 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.objDeclItem) = objItemGen();
        (yyval.objDeclItem)->type = OBJECT_OVER_WRITE;
        (yyval.objDeclItem)->item = pairGen((void *)(yyvsp[-5].str), (void *)(yyvsp[-3].str));
    }
#line 1586 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 29:
#line 224 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.objDeclItem) = objItemGen();
        (yyval.objDeclItem)->type = OBJECT_MEMBER;

        Variable *v = varGen(strdup((yyvsp[-1].str)), varTypeStr2Int(TYPE_INFO_TYPE((yyvsp[-2].type_info))), NULL);
        (yyval.objDeclItem)->item = v;
    }
#line 1598 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 30:
#line 234 "./interpreter/tScript.y" /* yacc.c:1646  */
    {}
#line 1604 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 31:
#line 237 "./interpreter/tScript.y" /* yacc.c:1646  */
    { (yyval.list_) = (yyvsp[0].list_); }
#line 1610 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 32:
#line 240 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        listAppend((yyval.list_), (void *)(yyvsp[0].str));
    }
#line 1618 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 33:
#line 243 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.list_) = listCreate();
        listAppend((yyval.list_), (void *)(yyvsp[0].str));
    }
#line 1627 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 34:
#line 250 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.statement) = returnStmtGen((yyvsp[-1].expression));
    }
#line 1635 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 35:
#line 256 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        VarDeclStatement *stmt = varDeclStmtGenerate((yyvsp[-2].type_info));
        VAR_DECL_STMT_SET_EXPRS(stmt, (yyvsp[-1].list_));

        (yyval.statement) = (Statement *)stmt;
    }
#line 1646 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 36:
#line 265 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        listAppend((yyval.list_), (void *)(yyvsp[0].expression));
    }
#line 1654 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 37:
#line 268 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.list_) = listCreate();
        listAppend((yyval.list_), (void *)(yyvsp[0].expression));
    }
#line 1663 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 38:
#line 275 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.expression) = pairGen((yyvsp[0].str), identExprGen((yyvsp[0].str)));
    }
#line 1671 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 39:
#line 278 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        Expression *expr = assignExprGen(identExprGen((yyvsp[-2].str)), (yyvsp[0].expression));
        (yyval.expression) = pairGen((yyvsp[-2].str), expr);
    }
#line 1680 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 40:
#line 285 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.statement) = funcDeclStmtDefault();

        Func *f = funcGen((yyvsp[-4].str), (yyvsp[-5].type_info), (yyvsp[-2].list_), NULL);
        FUNC_SET_STATEMENT_LIST(f, (yyvsp[0].list_));

        ((FuncDeclStatement *)(yyval.statement))->f = f;
    }
#line 1693 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 41:
#line 293 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.statement) = funcDeclStmtDefault();

        Func *f = funcGen((yyvsp[-3].str), (yyvsp[-4].type_info), NULL, NULL);
        FUNC_SET_STATEMENT_LIST(f, (yyvsp[0].list_));

        ((FuncDeclStatement *)(yyval.statement))->f = f;
    }
#line 1706 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 42:
#line 304 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        paramsAdd((yyval.list_), (yyvsp[0].str));
    }
#line 1714 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 43:
#line 307 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.list_) = paramsGen;
        paramsAdd((yyval.list_), (yyvsp[0].str));
    }
#line 1723 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 44:
#line 314 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.str) = paramGen((yyvsp[0].str), (yyvsp[-1].type_info));
    }
#line 1731 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 45:
#line 320 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.statement) = exprStmtGen((yyvsp[-1].expression));
    }
#line 1739 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 46:
#line 325 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.type_info) = buildTypeInfo(strdup("Int"), (yyvsp[0].integer));
    }
#line 1747 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 47:
#line 328 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.type_info) = buildTypeInfo(strdup("String"), (yyvsp[0].integer));
    }
#line 1755 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 48:
#line 331 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.type_info) = buildTypeInfo(strdup("Ops"), (yyvsp[0].integer));
    }
#line 1763 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 49:
#line 334 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.type_info) = buildTypeInfo(strdup((yyvsp[-1].str)), (yyvsp[0].integer));
    }
#line 1771 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 50:
#line 339 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.integer) = ARRAY_TYPE;
    }
#line 1779 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 51:
#line 342 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.integer) = MAP_TYPE;
    }
#line 1787 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 52:
#line 345 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.integer) = 0;
    }
#line 1795 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 55:
#line 355 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        listAppend((yyval.list_), (yyvsp[0].expression));
    }
#line 1803 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 56:
#line 358 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.list_) = listCreate();
        listAppend((yyval.list_), (yyvsp[0].expression));
    }
#line 1812 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 57:
#line 365 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.expression) = (yyvsp[-1].expression);
    }
#line 1820 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 68:
#line 381 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.expression) = percentExprGen((yyvsp[-4].expression), (yyvsp[-1].list_));
    }
#line 1828 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 69:
#line 387 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.expression) = arrayExprGen((yyvsp[-1].list_));
    }
#line 1836 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 71:
#line 395 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.expression) = indexExprGen((yyvsp[-3].str), (yyvsp[-1].expression));
    }
#line 1844 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 72:
#line 401 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.expression) = identExprGen((yyvsp[0].str));
    }
#line 1852 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 77:
#line 414 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.expression) = plusExprGen((yyvsp[-2].expression), (yyvsp[0].expression));
    }
#line 1860 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 78:
#line 420 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.expression) = minusExprGen((yyvsp[-2].expression), (yyvsp[0].expression));
    }
#line 1868 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 79:
#line 426 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.expression) = mulExprGen((yyvsp[-2].expression), (yyvsp[0].expression));
    }
#line 1876 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 80:
#line 432 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.expression) = divExprGen((yyvsp[-2].expression), (yyvsp[0].expression));
    }
#line 1884 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 87:
#line 447 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.expression) = lessThanExprGen((yyvsp[-2].expression), (yyvsp[0].expression));
    }
#line 1892 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 88:
#line 453 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.expression) = greaterThanExprGen((yyvsp[-2].expression), (yyvsp[0].expression));

    }
#line 1901 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 89:
#line 460 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.expression) = equalExprGen((yyvsp[-2].expression), (yyvsp[0].expression));
    }
#line 1909 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 90:
#line 466 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.expression) = lessOrEqualExprGen((yyvsp[-2].expression), (yyvsp[0].expression));
    }
#line 1917 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 91:
#line 472 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.expression) = greaterOrEqualGen((yyvsp[-2].expression), (yyvsp[0].expression));
    }
#line 1925 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 92:
#line 478 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.expression) = notEqualExprGen((yyvsp[-2].expression), (yyvsp[0].expression));
    }
#line 1933 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 93:
#line 484 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        MemberSelectExpression *mExpr = (yyval.expression);
        if (MEMBER_SELECT_SUBS(mExpr) == NULL)
            MEMBER_SELECT_SET_SUBS(mExpr, (yyvsp[0].str));
        else {
            Expression *m = (yyval.expression);
            (yyval.expression) = memberSelectGen(m, (yyvsp[0].str));
        }
    }
#line 1947 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 94:
#line 493 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.expression) = memberSelectGen((yyvsp[0].expression), NULL);
    }
#line 1955 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 95:
#line 499 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.expression) = identExprGen((yyvsp[-1].str));
    }
#line 1963 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 96:
#line 502 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.expression) = (yyvsp[-1].expression);
    }
#line 1971 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 97:
#line 508 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.expression) = funcCallExprGen((yyvsp[-3].str), (yyvsp[-1].list_));
    }
#line 1979 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 98:
#line 511 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.expression) = funcCallExprGen((yyvsp[-2].str), NULL);
    }
#line 1987 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 99:
#line 517 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        argusAdd((yyval.list_), arguGen((yyvsp[0].expression)));
    }
#line 1995 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 100:
#line 520 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.list_) = argusGen();
        argusAdd((yyval.list_), arguGen((yyvsp[0].expression)));
    }
#line 2004 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 101:
#line 527 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.expression) = assignExprGen((yyvsp[-2].expression), (yyvsp[0].expression));
    }
#line 2012 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 102:
#line 533 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.list_) = (yyvsp[0].list_);
    }
#line 2020 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 103:
#line 536 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.list_) = listCreate();
        listAppend((yyval.list_), (yyvsp[0].statement));
    }
#line 2029 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 104:
#line 543 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.list_) = (yyvsp[-1].list_);
    }
#line 2037 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 105:
#line 549 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.expression) = constExprDefault();
        constExprSetInt((yyval.expression), str2Num((yyvsp[0].str)));
    }
#line 2046 "./interpreter/parser.c" /* yacc.c:1646  */
    break;

  case 106:
#line 553 "./interpreter/tScript.y" /* yacc.c:1646  */
    {
        (yyval.expression) = constExprDefault();
        constExprSetStr((yyval.expression), (yyvsp[0].str));
    }
#line 2055 "./interpreter/parser.c" /* yacc.c:1646  */
    break;


#line 2059 "./interpreter/parser.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
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

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
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

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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

#if !defined yyoverflow || YYERROR_VERBOSE
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
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
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
  return yyresult;
}
#line 560 "./interpreter/tScript.y" /* yacc.c:1906  */


void yyerror(char const *s) {
    fprintf(stderr, "%s\n", s);
}
