
// This code was produced by Maphoon 2008.
// Definition of struct token:


#include "identifier.h"
#include "tree.h"
#include "assert.h"



#ifndef TOKEN_INCLUDED
#define TOKEN_INCLUDED    1


#include <list>
#include <iostream>



enum tokentype
{
   tkn__recover,
   tkn__defaultred,
   tkn_EOF,
   tkn_SCANERROR,
   tkn_COLON,
   tkn_SEMICOLON,
   tkn_IDENTIFIER,
   tkn_NUMBER,
   tkn_PLUS,
   tkn_TIMES,
   tkn_MINUS,
   tkn_DIVIDES,
   tkn_EQUAL,
   tkn_NOTEQUAL,
   tkn_LESSTHAN,
   tkn_GREATERTHAN,
   tkn_LESSEQUALTHAN,
   tkn_GREATEREQUALTHAN,
   tkn_AND,
   tkn_OR,
   tkn_NOT,
   tkn_LPAR,
   tkn_RPAR,
   tkn_LSQPAR,
   tkn_RSQPAR,
   tkn_ADDRESSOF,
   tkn_DOT,
   tkn_ARROW,
   tkn_PLUSPLUS,
   tkn_MINUSMINUS,
   tkn_ASSIGN,
   tkn_B1,
   tkn_B2,
   tkn_B3,
   tkn_B4,
   tkn_A1,
   tkn_N1,
   tkn_N2,
   tkn_N3,
   tkn_E1,
   tkn_E2
};


struct token
{

   tokentype type;

   std::list< identifier > id;
   std::list< std::string > reason;
   std::list< tree > tt;

   token( tokentype t )
      : type(t)
   {
   }

   token( );
      // Should have no definition.

   bool iswellformed( ) const;
      // Check if the attributes satisfy the
      // constraints.
};

std::ostream& operator << ( std::ostream& , const token& );



#endif


