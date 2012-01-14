
// This code was produced by Maphoon 2008.
// Code of struct token:


#include "token.h"


bool token::iswellformed( ) const
{
   switch( type )
   {
   case tkn__recover:
   case tkn__defaultred:
   case tkn_EOF:
   case tkn_COLON:
   case tkn_SEMICOLON:
   case tkn_PLUS:
   case tkn_TIMES:
   case tkn_MINUS:
   case tkn_DIVIDES:
   case tkn_EQUAL:
   case tkn_NOTEQUAL:
   case tkn_LESSTHAN:
   case tkn_GREATERTHAN:
   case tkn_LESSEQUALTHAN:
   case tkn_GREATEREQUALTHAN:
   case tkn_AND:
   case tkn_OR:
   case tkn_NOT:
   case tkn_LPAR:
   case tkn_RPAR:
   case tkn_LSQPAR:
   case tkn_RSQPAR:
   case tkn_ADDRESSOF:
   case tkn_DOT:
   case tkn_ARROW:
   case tkn_PLUSPLUS:
   case tkn_MINUSMINUS:
   case tkn_ASSIGN:
      if( id. size( ) >= 1 ) return false;
      if( reason. size( ) >= 1 ) return false;
      if( tt. size( ) >= 1 ) return false;
      return true;
   case tkn_SCANERROR:
      if( id. size( ) >= 1 ) return false;
      if( reason. size( ) >= 2 ) return false;
      if( tt. size( ) >= 1 ) return false;
      return true;
   case tkn_IDENTIFIER:
      if( id. size( ) < 1 ) return false;
      if( id. size( ) >= 2 ) return false;
      if( reason. size( ) >= 1 ) return false;
      if( tt. size( ) >= 1 ) return false;
      return true;
   case tkn_NUMBER:
   case tkn_B1:
   case tkn_B2:
   case tkn_B3:
   case tkn_B4:
   case tkn_A1:
   case tkn_N1:
   case tkn_N2:
   case tkn_N3:
   case tkn_E1:
   case tkn_E2:
      if( id. size( ) >= 1 ) return false;
      if( reason. size( ) >= 1 ) return false;
      if( tt. size( ) < 1 ) return false;
      if( tt. size( ) >= 2 ) return false;
      return true;
   }
   return false; // Because of unknown type.
}


// If you see an insane error message originating from
// this point, then the most probably reason is that
// one of the attribute types has no print function defined.

std::ostream& operator << ( std::ostream& stream, const token& t )
{
   switch( t. type )
   {
   case tkn__recover:
      stream << "_recover( "; break;
   case tkn__defaultred:
      stream << "_defaultred( "; break;
   case tkn_EOF:
      stream << "EOF( "; break;
   case tkn_SCANERROR:
      stream << "SCANERROR( "; break;
   case tkn_COLON:
      stream << "COLON( "; break;
   case tkn_SEMICOLON:
      stream << "SEMICOLON( "; break;
   case tkn_IDENTIFIER:
      stream << "IDENTIFIER( "; break;
   case tkn_NUMBER:
      stream << "NUMBER( "; break;
   case tkn_PLUS:
      stream << "PLUS( "; break;
   case tkn_TIMES:
      stream << "TIMES( "; break;
   case tkn_MINUS:
      stream << "MINUS( "; break;
   case tkn_DIVIDES:
      stream << "DIVIDES( "; break;
   case tkn_EQUAL:
      stream << "EQUAL( "; break;
   case tkn_NOTEQUAL:
      stream << "NOTEQUAL( "; break;
   case tkn_LESSTHAN:
      stream << "LESSTHAN( "; break;
   case tkn_GREATERTHAN:
      stream << "GREATERTHAN( "; break;
   case tkn_LESSEQUALTHAN:
      stream << "LESSEQUALTHAN( "; break;
   case tkn_GREATEREQUALTHAN:
      stream << "GREATEREQUALTHAN( "; break;
   case tkn_AND:
      stream << "AND( "; break;
   case tkn_OR:
      stream << "OR( "; break;
   case tkn_NOT:
      stream << "NOT( "; break;
   case tkn_LPAR:
      stream << "LPAR( "; break;
   case tkn_RPAR:
      stream << "RPAR( "; break;
   case tkn_LSQPAR:
      stream << "LSQPAR( "; break;
   case tkn_RSQPAR:
      stream << "RSQPAR( "; break;
   case tkn_ADDRESSOF:
      stream << "ADDRESSOF( "; break;
   case tkn_DOT:
      stream << "DOT( "; break;
   case tkn_ARROW:
      stream << "ARROW( "; break;
   case tkn_PLUSPLUS:
      stream << "PLUSPLUS( "; break;
   case tkn_MINUSMINUS:
      stream << "MINUSMINUS( "; break;
   case tkn_ASSIGN:
      stream << "ASSIGN( "; break;
   case tkn_B1:
      stream << "B1( "; break;
   case tkn_B2:
      stream << "B2( "; break;
   case tkn_B3:
      stream << "B3( "; break;
   case tkn_B4:
      stream << "B4( "; break;
   case tkn_A1:
      stream << "A1( "; break;
   case tkn_N1:
      stream << "N1( "; break;
   case tkn_N2:
      stream << "N2( "; break;
   case tkn_N3:
      stream << "N3( "; break;
   case tkn_E1:
      stream << "E1( "; break;
   case tkn_E2:
      stream << "E2( "; break;
   default:
      stream << "UNKNOWNTOKEN( ";
   }

   unsigned int arg = 0;
   unsigned int arginlist;

   arginlist = 0;
   for( std::list< identifier > :: const_iterator
           p = t. id. begin( );
           p != t. id. end( );
           ++ p )
   {
      if( arg != 0 && arginlist == 0 ) stream << "; ";
      if( arg != 0 && arginlist != 0 ) stream << ", ";
      stream << *p;
      ++ arg;
      ++ arginlist;
   }

   arginlist = 0;
   for( std::list< std::string > :: const_iterator
           p = t. reason. begin( );
           p != t. reason. end( );
           ++ p )
   {
      if( arg != 0 && arginlist == 0 ) stream << "; ";
      if( arg != 0 && arginlist != 0 ) stream << ", ";
      stream << *p;
      ++ arg;
      ++ arginlist;
   }

   arginlist = 0;
   for( std::list< tree > :: const_iterator
           p = t. tt. begin( );
           p != t. tt. end( );
           ++ p )
   {
      if( arg != 0 && arginlist == 0 ) stream << "; ";
      if( arg != 0 && arginlist != 0 ) stream << ", ";
      stream << *p;
      ++ arg;
      ++ arginlist;
   }

   stream << " )";
   return stream;
}


