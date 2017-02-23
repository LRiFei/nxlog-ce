
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
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




