

#include "tokenizer.h"
#include "data.h" 
#include <math.h>
#include <limits.h>


namespace
{

   inline bool isletter( char c )
   {
      if( c >= 'a' && c <= 'z' )
         return true;
      if( c >= 'A' && c <= 'Z' )
         return true;
      return false;
   }


   inline bool isdigit( char c )
   {
      if( c >= '0' && c <= '9' )
         return true;
      return false;
   }

}

   
void tokenizer::scan( )
{

   // First we ignore whitespace:

   while( r. lookahead == ' ' || r. lookahead == '\n' ||
          r. lookahead == '\t' )
   {
      r. moveforward( );
   }

   if( r. lookahead == EOF )
   {
      lookahead. push_back( tkn_EOF );
      return;
   }

   if( r. lookahead == ':' )
   {
      r. moveforward( );
      lookahead. push_back( tkn_COLON );
      return;
   }
   
   if( r. lookahead == ';' )
   {
      r. moveforward( );
      lookahead. push_back( tkn_SEMICOLON );
      return;
   }

   if( r. lookahead == '|' )
   {
      r. moveforward( );
      if( r. lookahead == '|' )
      {
         r. moveforward( );
         lookahead. push_back( tkn_OR );
         return;
      }
      lookahead. push_back( tkn_SCANERROR );
      return;
   }

   if( r. lookahead == '&' )
   {
      r. moveforward( );
      if( r. lookahead == '&' )
      {
         r. moveforward( );
         lookahead. push_back( tkn_AND );
         return;
      }
      lookahead. push_back( tkn_ADDRESSOF );
      return;
   }

   if( r. lookahead == '+' )
   {
      r. moveforward( );
      if( r. lookahead == '+' )
      {
         r. moveforward( ); 
         lookahead. push_back( tkn_PLUSPLUS );
         return;
      } 
      lookahead. push_back( tkn_PLUS );
      return;
   }

   if( r. lookahead == '-' )
   {
      r. moveforward( );
      if( r. lookahead == '-' )
      {
         r. moveforward( );
         lookahead. push_back( tkn_MINUSMINUS );
         return;
      }
 
      if( r. lookahead == '>' )
      {
         r. moveforward( );
         lookahead. push_back( tkn_ARROW );
         return;
      }

      lookahead. push_back( tkn_MINUS );
      return;
   }

   if( r. lookahead == '*' )
   {
      lookahead. push_back( tkn_TIMES );
      r. moveforward( );
      return; 
   }

   if( r. lookahead == '/' )
   {
      lookahead. push_back( tkn_DIVIDES );
      r. moveforward( );
      return;
   }

   if( r. lookahead == '!' )
   {
      r. moveforward( ); 
      if( r. lookahead == '=' )
      {
         r. moveforward( );
         lookahead. push_back( tkn_NOTEQUAL );
         return;
      }
      lookahead. push_back( tkn_NOT );
      return;
   }

   if( r. lookahead == '(' )
   {
      lookahead. push_back( tkn_LPAR );
      r. moveforward( );
      return; 
   }

   if( r. lookahead == ')' )
   {
      lookahead. push_back( tkn_RPAR );
      r. moveforward( );
      return; 
   }

   if( r. lookahead == '[' )
   {
      r. moveforward( );
      lookahead. push_back( tkn_LSQPAR );
      return;
   }

   if( r. lookahead == ']' )
   {
      r. moveforward( );
      lookahead. push_back( tkn_RSQPAR );
      return;
   }

   if( r. lookahead == '=' )
   {
      r. moveforward( );
      if( r. lookahead == '=' )
      {
         r. moveforward( );
         lookahead. push_back( tkn_EQUAL );
         return;
      }
      lookahead. push_back( tkn_ASSIGN );
      return; 
   }

   if( r. lookahead == '<' )
   {
      r. moveforward( );
      if( r. lookahead == '=' )
      {
         r. moveforward( );
         lookahead. push_back( tkn_LESSEQUALTHAN );
         return;
      }
      lookahead. push_back( tkn_LESSTHAN );
      return;
   }

   if( r. lookahead == '>' )
   {
      r. moveforward( );
      if( r. lookahead == '=' )
      {
         r. moveforward( );
         lookahead. push_back( tkn_GREATEREQUALTHAN );
         return;
      }
      lookahead. push_back( tkn_GREATERTHAN );
      return;
   }

   if( r. lookahead == '.' )
   {
      r. moveforward( );
      lookahead. push_back( tkn_DOT );
      return;
   }

   if( r. lookahead == '\'' )
   {
      r. moveforward( );
      if( r. lookahead != '\'' && r. lookahead != '\n' && r. lookahead != EOF )
      {
         char c = r. lookahead; 
         r. moveforward( );
         {
            if( r. lookahead == '\'' )
            {
               r. moveforward( );
               lookahead. push_back( tkn_NUMBER );
               lookahead. back( ). tt. push_back( 
                  tree( identifier( "constant" ), tobinary( c ),
                        treetype( type_prim, 
                                  primtype( "char", sizeof( char )))));
               return;
            }
         }
      }
      lookahead. push_back( tkn_SCANERROR );
      return;
   }
  
   if( isletter( r. lookahead ) || r. lookahead == '_' )
   {
      std::string attr; 

      attr. push_back( r. lookahead );  
      r. moveforward( );

      while( isletter( r. lookahead ) || isdigit( r. lookahead ) ||
             r. lookahead == '_' )
      {
         attr. push_back( r. lookahead ); 
         r. moveforward( );
      }

      if( attr == "true" )
      {
         lookahead. push_back( tkn_NUMBER );
         lookahead. back( ). tt. push_back(
            tree( identifier( "constant" ), tobinary( true ),
                  treetype( type_prim, primtype( "bool", sizeof( bool )))));
         return; 
      }

      if( attr == "false" )
      {
         lookahead. push_back( tkn_NUMBER );
         lookahead. back( ). tt. push_back(
            tree( identifier( "constant" ), tobinary( false ),
                  treetype( type_prim, primtype( "bool", sizeof( bool )))));
         return;
      }

      lookahead. push_back( tkn_IDENTIFIER );
      lookahead. back( ). id. push_back( identifier( attr ));
         // Lookup is done in the parser. 
      return;
   }

   // We can only recognize possitive numbers, because of
   // scanning conflicts with - .

   if( isdigit( r. lookahead ))
   {
      std::string s;
         // We keep the string, so that we can put it in 
         // a scanerror, if necessary.

      bool canbeint = true; 
         // If we see a . or an exponent, we set it to false.

      double val = 0.0;
      while( isdigit( r. lookahead ))
      {
         val = val * 10.0 + ( r. lookahead - '0' );

         s += r. lookahead; 
         r. moveforward( ); 
      }

      if( r. lookahead == '.' )
      { 
         canbeint = false; 
         double pos = 0.1;

         s += r. lookahead;
         r. moveforward( );

         if( !isdigit( r. lookahead ))
         {
            s += r. lookahead;
            r. moveforward( );

            lookahead. push_back( tkn_SCANERROR );
            lookahead. back( ). reason. push_back(s);
            return;
         }
 
         while( isdigit( r. lookahead ))
         {
            val += pos * ( r. lookahead - '0' );
            pos /= 10.0;

            s += r. lookahead;
            r. moveforward( );
         }
      }

      if( r. lookahead == 'e' || r. lookahead == 'E' )
      {
         canbeint = false;

         unsigned int ee = 0;
         int polarity = 1;

         s += r. lookahead;
         r. moveforward( );

         if( r. lookahead == '+' || r. lookahead == '-' )
         {
            if( r. lookahead == '-' )
               polarity = -1;   
    
            s += r. lookahead;
            r. moveforward( );
         }

         if( !isdigit( r. lookahead ))
         {
            s += r. lookahead;
            r. moveforward( );

            lookahead. push_back( tkn_SCANERROR );
            lookahead. back( ). reason. push_back(s);
            return;
         }

         while( isdigit( r. lookahead ))
         {
            ee = ee * 10 + ( r. lookahead - '0' );
            
            s += r. lookahead;
            r. moveforward( );
         }

         // We need to put val = val * 10^( polarity * ee ):

         // Is this efficient? Probably not, but it is simple. 

         if( polarity == 1 )
         {
            while( ee )
            { 
               val *= 10.0;
               -- ee;
            }
         }
         else
         {
            while( ee )
            {
               val /= 10.0;
               -- ee;
            }
         }

      }

      lookahead. push_back( tkn_NUMBER );

      if( !canbeint || floor( val ) != val || val > (double) INT_MAX )
      {
         lookahead. back( ). tt. push_back( 
            tree( identifier( "constant" ), tobinary( val ),
                  treetype( type_prim, primtype( "double", sizeof(double)))));
         return; 
      }

      if( val == 0.0 || val == 1.0 )
      {  
         lookahead. back( ). tt. push_back(
            tree( identifier( "constant" ), tobinary( val == 1.0 ),
                  treetype( type_prim, primtype( "bool", sizeof( bool )))));
         return;
      }

      if( val < 256 ) 
      {  
         lookahead. back( ). tt. push_back(
            tree( identifier( "constant" ), 
                  tobinary( static_cast< char > ( val )),
                  treetype( type_prim, primtype( "char", sizeof( char )))));
          return;
      }

      // Remaining case is just int:

      lookahead. back( ). tt. push_back(
         tree( identifier( "constant" ),
               tobinary( static_cast< int > ( val )),
               treetype( type_prim, primtype( "int", sizeof( int )))));
           
      return;
   } 

   // If we could not recognize anything, then we produce
   // a scan error.

   lookahead. push_back( tkn_SCANERROR );

   std::string s;
   if( r. lookahead != EOF )
   { 
      s += r. lookahead;  
      r. moveforward( );
         // It is important that we always advance the reader,
         // because otherwise the tokenizer hangs.
   }
   else
   {
      s += "eof";   // This should actually be unreachable, because
                    // EOF is checked in the beginning. 
   }
   lookahead. back( ). reason. push_back(s);
   return;  
}


void tokenizer::syntaxerror( )
{
   std::cout << "Syntax error in line " << r. linenumber << "\n";
}


