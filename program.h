

#ifndef PROGRAM_INCLUDED
#define PROGRAM_INCLUDED   1

#include <map>
#include <iostream>
#include "flowgraph.h" 


// Written by Hans de Nivelle for class 'Introduction to Compiler
// Construction'.


class program
{
   
   struct function 
   {
      treetype tt;  
         // type of function, should be of form func( T; .... ).

      flowgraph code;
         // Flowgraph of the function. 

      function( const treetype& tt, const flowgraph& code )
         : tt(tt), code( code )  
      { } 

   };

   std::map< identifier, unsigned int > indices; 
   std::map< unsigned int, function > code; 
      // In this way, we are able to assign a number to every function,
      // which we can use as address during execution. 

   unsigned int last;  
public: 
   program( );

   void insert( const identifier& name, treetype tt, const flowgraph& fcode );

   void erase( ); 

   bool isfunction( const identifier& name ) const
      { return indices. find( name ) != indices. end( ); }

   unsigned int getaddress( const identifier& name ) const
      { return indices. find( name ) -> second; }

   const flowgraph& getfunctiongraph( unsigned int addr ) const
      { return code. find( addr ) -> second. code; } 

   const treetype& getfunctiontype( unsigned int addr ) const
      { return code. find( addr ) -> second. tt; } 

   void erase( const identifier& name );

   friend std::ostream& operator << ( std::ostream& , const program& );
};

#endif


