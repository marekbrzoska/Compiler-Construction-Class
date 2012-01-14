

#include "assert.h"
#include "reader.h"
#include "tokenizer.h"
#include "parser.h"


int main( int argc, char* argv [] )
{

   tokenizer tt;

#if 1
   tt. scan( );

   // Test the tokenizer:

   while( tt. lookahead. size( ) && 
          tt. lookahead. front( ). type != tkn_EOF )
   {
      std::cout << tt. lookahead. front( ) << "\n";
      tt. lookahead. pop_front( ); 
      tt. scan( ); 
   }

   // Actually if the size of lookahead gets 0, something went wrong.

   ASSERT( tt. lookahead. size( ));
   std::cout << tt. lookahead. front( ) << "\n";
      // This is an EOF token. 
   return 0;
 
#endif

#if 0 
   parser( tt, vs, tkn_Session, 10 );

   ASSERT( tt. lookahead. size( ));
   
   if( tt. lookahead. front( ). type != tkn_Session )
      std::cout << "ended with an error\n";

#endif

}


