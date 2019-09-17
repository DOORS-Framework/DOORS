/* A Bison parser, made by GNU Bison 1.875a.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     STRING = 258,
     HEXSTRING = 259,
     NUM = 260,
     IPSTART = 261,
     IPXSTART = 262,
     ATSTART = 263,
     SERVICESTR = 264,
     TRANSKEY = 265
   };
#endif
#define STRING 258
#define HEXSTRING 259
#define NUM 260
#define IPSTART 261
#define IPXSTART 262
#define ATSTART 263
#define SERVICESTR 264
#define TRANSKEY 265




/* Copy the first part of user declarations.  */
#line 1 "serviceurlparser.y"


/*
//  Copyright 2003
//      Telecoms Lab, Tampere University of Technology.  All rights reserved.

//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//  1. Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
//
//  THIS SOFTWARE IS PROVIDED BY TAMPERE UNIVERSITY OF TECHNOLOGY AND
//  CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
//  BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
//  FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY
//  OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

$Log: serviceurlparser.C,v $
Revision 1.8  2009/04/20 13:44:30  bilhanan
bugfix to allow c-style string manipulation

Revision 1.7  2004/06/17 08:49:47  hartmanj
Added support for transient service URL:s

Revision 1.6  2004/03/22 11:05:47  hartmanj
Added support for pure servicetype strings such as service:lpr without the URL. Especially helpful in attribute requests.

Revision 1.5  2004/02/26 09:45:02  hartmanj
Changed serviceurl.C to include serviceurlparser.h instead of serviceurlparser.C

Revision 1.4  2004/02/25 14:17:58  hartmanj
Replaced string::compare() with == operator

Revision 1.3  2004/02/25 13:33:02  hartmanj
Repositioned license clause, added log


*/

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

#include "serviceurlparser.h"


void yyerror(char *s);
int yylex(void);

DOORS_ServiceURL_Parser_Class DOORS_ServiceURL_Parser;



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

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 59 "serviceurlparser.y"
typedef union YYSTYPE {
     const char* strval;
     struct DOORS_ServiceURL_Parser_Class::Span span;
     char chval;
} YYSTYPE;
/* Line 191 of yacc.c.  */
#line 160 "serviceurlparser.C"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 172 "serviceurlparser.C"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   71

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  20
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  29
/* YYNRULES -- Number of rules. */
#define YYNRULES  55
/* YYNRULES -- Number of states. */
#define YYNSTATES  89

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   265

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,    17,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,    14,    13,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    11,    18,
       2,    19,     2,     2,    12,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    15,     2,    16,     2,     2,     2,     2,     2,     2,
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
       5,     6,     7,     8,     9,    10
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned char yyprhs[] =
{
       0,     0,     3,     9,    12,    14,    18,    20,    23,    25,
      27,    29,    32,    37,    39,    42,    44,    46,    50,    54,
      56,    60,    62,    66,    68,    70,    72,    74,    78,    80,
      82,    86,    94,    96,   100,   104,   109,   111,   113,   115,
     122,   124,   126,   128,   135,   137,   139,   141,   143,   145,
     147,   148,   152,   156,   158,   160
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      21,     0,    -1,     9,    22,    11,    23,    46,    -1,     9,
      22,    -1,     3,    -1,    22,    11,     3,    -1,    24,    -1,
      24,    27,    -1,    25,    -1,    38,    -1,    42,    -1,     6,
      29,    -1,     6,    26,    12,    29,    -1,     3,    -1,    13,
      28,    -1,     3,    -1,     5,    -1,    28,    13,     3,    -1,
      28,    13,     5,    -1,     4,    -1,    28,    13,     4,    -1,
      30,    -1,    30,    11,     5,    -1,    31,    -1,    34,    -1,
      10,    -1,    32,    -1,    33,    14,    32,    -1,     3,    -1,
       3,    -1,    33,    14,     3,    -1,     5,    14,     5,    14,
       5,    14,     5,    -1,    35,    -1,    15,    36,    16,    -1,
      36,    11,    37,    -1,    36,    11,    11,    37,    -1,    37,
      -1,     4,    -1,     5,    -1,     8,    39,    11,    40,    17,
      41,    -1,     3,    -1,     3,    -1,     3,    -1,     7,    43,
      11,    44,    11,    45,    -1,     4,    -1,     5,    -1,     4,
      -1,     5,    -1,     4,    -1,     5,    -1,    -1,    46,    18,
      47,    -1,    48,    19,    48,    -1,     3,    -1,     5,    -1,
       4,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned char yyrline[] =
{
       0,   102,   102,   108,   114,   119,   128,   128,   135,   135,
     135,   138,   143,   150,   153,   156,   157,   158,   159,   160,
     161,   164,   164,   172,   173,   174,   177,   178,   181,   184,
     185,   188,   193,   196,   199,   200,   200,   203,   204,   207,
     214,   217,   220,   223,   230,   230,   233,   233,   236,   236,
     239,   239,   242,   248,   248,   248
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "STRING", "HEXSTRING", "NUM", "IPSTART", 
  "IPXSTART", "ATSTART", "SERVICESTR", "TRANSKEY", "':'", "'@'", "'/'", 
  "'.'", "'['", "']'", "' '", "';'", "'='", "$accept", "serviceurl", 
  "servicetype", "sap", "site", "ipsite", "user", "urlpart", "urlpath", 
  "hostport", "host", "hostname", "toplabel", "domainlabel", "hostnumber", 
  "ipv6address", "hexlist", "hex", "atsite", "atobject", "attype", 
  "atzone", "ipxsite", "ipxnet", "ipxnode", "ipxsocket", "attrlist", 
  "attr", "attrval", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,    58,    64,    47,    46,    91,    93,    32,    59,    61
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    20,    21,    21,    22,    22,    23,    23,    24,    24,
      24,    25,    25,    26,    27,    28,    28,    28,    28,    28,
      28,    29,    29,    30,    30,    30,    31,    31,    32,    33,
      33,    34,    34,    35,    36,    36,    36,    37,    37,    38,
      39,    40,    41,    42,    43,    43,    44,    44,    45,    45,
      46,    46,    47,    48,    48,    48
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     5,     2,     1,     3,     1,     2,     1,     1,
       1,     2,     4,     1,     2,     1,     1,     3,     3,     1,
       3,     1,     3,     1,     1,     1,     1,     3,     1,     1,
       3,     7,     1,     3,     3,     4,     1,     1,     1,     6,
       1,     1,     1,     6,     1,     1,     1,     1,     1,     1,
       0,     3,     3,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       0,     0,     0,     4,     3,     1,     0,     5,     0,     0,
       0,    50,     6,     8,     9,    10,    28,     0,    25,     0,
       0,    11,    21,    23,    26,     0,    24,    32,    44,    45,
       0,    40,     0,     2,     0,     7,     0,    37,    38,     0,
      36,     0,     0,     0,     0,     0,     0,    15,    19,    16,
      14,     0,     0,    33,    28,    12,    22,    28,    27,    46,
      47,     0,    41,     0,    53,    55,    54,    51,     0,     0,
       0,     0,    34,     0,     0,     0,    17,    20,    18,     0,
      35,    48,    49,    43,    42,    39,    52,     0,    31
};

/* YYDEFGOTO[NTERM-NUM]. */
static const yysigned_char yydefgoto[] =
{
      -1,     2,     4,    11,    12,    13,    20,    35,    50,    21,
      22,    23,    24,    25,    26,    27,    39,    40,    14,    32,
      63,    85,    15,    30,    61,    83,    33,    67,    68
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -53
static const yysigned_char yypact[] =
{
       6,     4,    10,   -53,     7,   -53,    14,   -53,    -2,    32,
      29,   -53,    31,   -53,   -53,   -53,    21,    33,   -53,    34,
      22,   -53,    35,   -53,   -53,    37,   -53,   -53,   -53,   -53,
      39,   -53,    41,    27,    20,   -53,    43,   -53,   -53,     0,
     -53,    -1,    44,    50,    36,    51,    23,   -53,   -53,   -53,
      42,    45,     1,   -53,    46,   -53,   -53,    47,   -53,   -53,
     -53,    52,   -53,    40,   -53,   -53,   -53,   -53,    48,    26,
      53,    34,   -53,    38,    59,    23,   -53,   -53,   -53,    54,
     -53,   -53,   -53,   -53,   -53,   -53,   -53,    60,   -53
};

/* YYPGOTO[NTERM-NUM].  */
static const yysigned_char yypgoto[] =
{
     -53,   -53,   -53,   -53,   -53,   -53,   -53,   -53,   -53,    15,
     -53,   -53,    28,   -53,   -53,   -53,   -53,   -52,   -53,   -53,
     -53,   -53,   -53,   -53,   -53,   -53,   -53,   -53,   -11
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -31
static const yysigned_char yytable[] =
{
      72,    16,    54,    17,    17,    37,    38,     3,    18,    18,
       5,    52,    71,    19,    19,     1,    53,     7,     6,    80,
       8,     9,    10,    47,    48,    49,    64,    65,    66,    76,
      77,    78,    31,   -13,    41,   -29,    28,    29,    37,    38,
      59,    60,    81,    82,    34,    46,    42,    36,    51,    56,
      44,    43,    45,    57,    62,    69,    55,    74,    79,    70,
     -29,   -30,    84,    73,    86,    88,     0,    75,    87,     0,
       0,    58
};

static const yysigned_char yycheck[] =
{
      52,     3,     3,     5,     5,     4,     5,     3,    10,    10,
       0,    11,    11,    15,    15,     9,    16,     3,    11,    71,
       6,     7,     8,     3,     4,     5,     3,     4,     5,     3,
       4,     5,     3,    12,    12,    14,     4,     5,     4,     5,
       4,     5,     4,     5,    13,    18,    11,    14,     5,     5,
      11,    14,    11,     3,     3,    13,    41,    17,     5,    14,
      14,    14,     3,    11,    75,     5,    -1,    19,    14,    -1,
      -1,    43
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     9,    21,     3,    22,     0,    11,     3,     6,     7,
       8,    23,    24,    25,    38,    42,     3,     5,    10,    15,
      26,    29,    30,    31,    32,    33,    34,    35,     4,     5,
      43,     3,    39,    46,    13,    27,    14,     4,     5,    36,
      37,    12,    11,    14,    11,    11,    18,     3,     4,     5,
      28,     5,    11,    16,     3,    29,     5,     3,    32,     4,
       5,    44,     3,    40,     3,     4,     5,    47,    48,    13,
      14,    11,    37,    11,    17,    19,     3,     4,     5,     5,
      37,     4,     5,    45,     3,    41,    48,    14,     5
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrlab1


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
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)         \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
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
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (cinluded).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short *bottom, short *top)
#else
static void
yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylineno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylineno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
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
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
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



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

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
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

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

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

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
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
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

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
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
#line 103 "serviceurlparser.y"
    {
			DOORS_ServiceURL_Parser.setAddrSpec(yyvsp[-1].span);
		;}
    break;

  case 3:
#line 109 "serviceurlparser.y"
    {
			DOORS_ServiceURL_Parser.setPureType();
		;}
    break;

  case 4:
#line 115 "serviceurlparser.y"
    {
			DOORS_ServiceURL_Parser.setConcreteType(yyvsp[0].span);
			yyval.span = yyvsp[0].span;
		;}
    break;

  case 5:
#line 120 "serviceurlparser.y"
    {
			DOORS_ServiceURL_Parser.setAbstractType(yyvsp[-2].span);
			DOORS_ServiceURL_Parser.setConcreteType(yyvsp[0].span);
			yyval.span.a = yyvsp[-2].span.a;
			yyval.span.b = yyvsp[0].span.b; 
		;}
    break;

  case 7:
#line 129 "serviceurlparser.y"
    {
			yyval.span.a = yyvsp[-1].span.a;
			yyval.span.b = yyvsp[0].span.b; 
		;}
    break;

  case 11:
#line 139 "serviceurlparser.y"
    {
			yyval.span.a = yyvsp[-1].span.a;
			yyval.span.b = yyvsp[0].span.b; 
		;}
    break;

  case 12:
#line 144 "serviceurlparser.y"
    {
			yyval.span.a = yyvsp[-3].span.a;
			yyval.span.b = yyvsp[0].span.b; 
		;}
    break;

  case 14:
#line 153 "serviceurlparser.y"
    { yyval.span.a = yyvsp[0].span.a - 1; yyval.span.b = yyvsp[0].span.b; ;}
    break;

  case 17:
#line 158 "serviceurlparser.y"
    { yyval.span.a = yyvsp[-2].span.a; yyval.span.b = yyvsp[0].span.b; ;}
    break;

  case 18:
#line 159 "serviceurlparser.y"
    { yyval.span.a = yyvsp[-2].span.a; yyval.span.b = yyvsp[0].span.b; ;}
    break;

  case 20:
#line 161 "serviceurlparser.y"
    { yyval.span.a = yyvsp[-2].span.a; yyval.span.b = yyvsp[0].span.b; ;}
    break;

  case 22:
#line 165 "serviceurlparser.y"
    {
			yyval.span.a = yyvsp[-2].span.a;
			yyval.span.b = yyvsp[0].span.b;
                        		DOORS_ServiceURL_Parser.setPort(yyvsp[0].span);
		;}
    break;

  case 23:
#line 172 "serviceurlparser.y"
    { DOORS_ServiceURL_Parser.setHostAddr(yyvsp[0].span); ;}
    break;

  case 24:
#line 173 "serviceurlparser.y"
    { DOORS_ServiceURL_Parser.setHostAddr(yyvsp[0].span); ;}
    break;

  case 25:
#line 174 "serviceurlparser.y"
    { DOORS_ServiceURL_Parser.setTransient(); ;}
    break;

  case 27:
#line 178 "serviceurlparser.y"
    { yyval.span.a = yyvsp[-2].span.a; yyval.span.b = yyvsp[0].span.b; ;}
    break;

  case 30:
#line 185 "serviceurlparser.y"
    { yyval.span.a = yyvsp[-2].span.a; yyval.span.b = yyvsp[0].span.b; ;}
    break;

  case 31:
#line 189 "serviceurlparser.y"
    {	
		yyval.span.a = yyvsp[-6].span.a;
		yyval.span.b = yyvsp[0].span.b;
	;}
    break;

  case 33:
#line 196 "serviceurlparser.y"
    { yyval.span.a = yyvsp[-1].span.a; yyval.span.b = yyvsp[-1].span.b; ;}
    break;

  case 34:
#line 199 "serviceurlparser.y"
    { yyval.span.a = yyvsp[-2].span.a; yyval.span.b = yyvsp[0].span.b; ;}
    break;

  case 35:
#line 200 "serviceurlparser.y"
    { yyval.span.a = yyvsp[-3].span.a; yyval.span.b = yyvsp[0].span.b; ;}
    break;

  case 39:
#line 208 "serviceurlparser.y"
    {
			yyval.span.a = yyvsp[-5].span.a;
			yyval.span.b = yyvsp[0].span.b;
		;}
    break;

  case 43:
#line 224 "serviceurlparser.y"
    {
			yyval.span.a = yyvsp[-5].span.a;
			yyval.span.b = yyvsp[0].span.b;
		;}
    break;

  case 52:
#line 243 "serviceurlparser.y"
    {
			DOORS_ServiceURL_Parser.addAttribute(yyvsp[-2].span,yyvsp[0].span);
 		;}
    break;


    }

/* Line 999 of yacc.c.  */
#line 1275 "serviceurlparser.C"

  yyvsp -= yylen;
  yyssp -= yylen;


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
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("syntax error, unexpected ") + 1;
	  yysize += yystrlen (yytname[yytype]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (yychar == YYEOF)
        {
	  /* Pop the error token.  */
          YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (yyss < yyssp)
	    {
	      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
	      yydestruct (yystos[*yyssp], yyvsp);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
      yydestruct (yytoken, &yylval);
      yychar = YYEMPTY;

    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*----------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
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

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      yyvsp--;
      yystate = *--yyssp;

      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


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

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 250 "serviceurlparser.y"


int yylex(void)
{
     return DOORS_ServiceURL_Parser.yylex();
}

DOORS_ServiceURL_Parser_Class::DOORS_ServiceURL_Parser_Class()
     : url(""),
       pos(0), port_(-1), serviceIsPureType_(false)
{
}

DOORS_ServiceURL_Parser_Class::~DOORS_ServiceURL_Parser_Class()
{
     attributes_.clear();
}

void DOORS_ServiceURL_Parser_Class::addAttribute(struct DOORS_ServiceURL_Parser_Class::Span a, struct DOORS_ServiceURL_Parser_Class::Span b)
{
     std::string type, value;

     type = url.substr(a.a, a.b-a.a);
     value = url.substr(b.a, b.b-a.a);

     attributes_.insert(std::map<std::string, std::string>::value_type
                        (type, value));
}

void DOORS_ServiceURL_Parser_Class::setAddrSpec(struct DOORS_ServiceURL_Parser_Class::Span a)
{
     address_ = url.substr(a.a, a.b-a.a);
}

void DOORS_ServiceURL_Parser_Class::setConcreteType(struct DOORS_ServiceURL_Parser_Class::Span a)
{
     access_ = url.substr(a.a, a.b-a.a);
}

void DOORS_ServiceURL_Parser_Class::setAbstractType(struct DOORS_ServiceURL_Parser_Class::Span a)
{
     service_ = url.substr(a.a, a.b-a.a);
}

void DOORS_ServiceURL_Parser_Class::setPureType( )
{
    serviceIsPureType_ = true;
}

std::string DOORS_ServiceURL_Parser_Class::getAbstractType(void)
{
     return service_;
}

void DOORS_ServiceURL_Parser_Class::setHostAddr(struct DOORS_ServiceURL_Parser_Class::Span a)
{
     host_ = url.substr(a.a, a.b-a.a);
}

std::string DOORS_ServiceURL_Parser_Class::getHostAddr(void)
{
     return host_;
}

int DOORS_ServiceURL_Parser_Class::getPort(void)
{
     return port_;
}

void DOORS_ServiceURL_Parser_Class::setPort(struct DOORS_ServiceURL_Parser_Class::Span a)
{
  //     port_ = strtol(url.substr(a.a, a.b-a.a).c_str(),0,10);
  std::istringstream portstr (url.substr(a.a, a.b-a.a));
  portstr >> port_;
}

std::string DOORS_ServiceURL_Parser_Class::getConcreteType(void)
{
     return access_;
}

std::string DOORS_ServiceURL_Parser_Class::getAddrSpec(void)
{
     return address_;
}

std::map<std::string,std::string> DOORS_ServiceURL_Parser_Class::getAllAttributes()
{
     return attributes_;
}

void DOORS_ServiceURL_Parser_Class::setURL(std::string URLToParse)
{
     url = URLToParse;
     pos = 0;
     isTransient = false;
     serviceIsPureType_ = false;
     attributes_.clear();
     service_ = access_ = address_ = host_ = "";
     port_ = 0;
}

void DOORS_ServiceURL_Parser_Class :: setTransient() {
    isTransient = true;
}

bool DOORS_ServiceURL_Parser_Class :: getTransient() {
    return isTransient;
}

int DOORS_ServiceURL_Parser_Class::yylex()
{
     if( pos >= url.size() )
          return 0;
     if( pos == 0 && url.size()>8 && url.substr(0, 8)==std::string("service:") ) {
          pos+=8;
          yylval.span.a = 0;
          yylval.span.b = pos;
          return SERVICESTR;
     }

     if( pos > 0 && url.size() >= pos + 4 && url.substr(pos,5).compare("[coa]") == 0 ) {
         yylval.span.a = pos;
         yylval.span.b = pos+5;
         pos += 5;
         return TRANSKEY;
     }

     yylval.span.a = pos;
     if( isalpha(url.at(pos)) ) {
          bool ishex = true;
          while(pos < url.size() && (isalpha(url.at(pos))||isdigit(url.at(pos))||url.at(pos)=='-'||url.at(pos)=='_') ) {
               if(ishex&&(!isxdigit(url.at(pos))))
                    ishex = false;
               pos++;
          }
          yylval.span.b = pos;
          if( ishex )
               return HEXSTRING;
          return STRING;
     } else if( isdigit(url.at(pos)) || (pos<url.size()-1 && url.at(pos) == '-' && isdigit(url.at(pos+1))) ) {
          if(url.at(pos) == '-') {
               pos++;
          }
          bool ispuredigit = true;
          while(pos < url.size() && isxdigit(url.at(pos))) {
               if(ispuredigit && !isdigit(url.at(pos)))
                    ispuredigit = false;
               pos++;
          }
          yylval.span.b = pos;
          if(ispuredigit)
               return NUM;
          return HEXSTRING;
     } else if( url.size() > pos+1 
                && url.at(pos) == '/'
                && url.at(pos+1) == '/' ) {
          pos+=2;
          yylval.span.b = pos;
          return IPSTART;
     } else if( url.size() > pos+3 
                && url.at(pos) == '/' && url.at(pos+1) == 'a' && url.at(pos+2) == 't'
                && url.at(pos+3) == '/' ) {
          pos+=4;
          yylval.span.b = pos;
          return ATSTART;
     } else if( url.size() > pos+4 
                && url.at(pos) == '/' && url.at(pos+1) == 'i' && url.at(pos+2) == 'p'
                && url.at(pos+3) == 'x' && url.at(pos+4) == '/' ) {
          pos+=5;
          yylval.span.b = pos;
          return IPXSTART;
     } else {
          yylval.chval = url.at(pos);
          pos++;
          return yylval.chval;
     }

     // eof
     return 0;
}

void
yyerror (char *s)  /* Called by yyparse on error */
{
  std::cout << "  ERROR: " << s << std::endl;
}


