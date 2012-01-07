

#ifndef PROGRAM_INCLUDED
#define PROGRAM_INCLUDED   1


#include <map>
#include "instruction.h"

   
class program
{
   
   struct inlinefunction
   {
      identifier res; 
      std::list< identifier > args;
      std::list< identifier > local;

      flowgraph code;
   };


   std::map< 

   void insert( const id
   {
      code [ label ]. push_back( ins );
   }

   void erase( ) { code. clear( ); } 
      // Erase everything.

   void erase( const identifier& label ) 
      { code. erase( label ); } 
      // Erase code segment attached to label.    


   void list( std::ostream& stream );

};


#endif

