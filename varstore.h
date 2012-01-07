
// A varstore stores only local variables.
// Global functions are stored in program, and global variables
// that are not functions can be stored in a map. 
//
// Local variables are mapped to a type and an identifier.
// (Will be name of register pointing to the variable.) 


#ifndef VARSTORE_INCLUDED
#define VARSTORE_INCLUDED   1

#include "treetype.h"
#include <vector>


class varstore
{

public: 
   struct variable 
   {
      identifier name;
      treetype tt;
      identifier reg;

      variable( const identifier& name, 
                const treetype& tt, const identifier& reg ) 
         : name( name ), tt( tt ), reg( reg )
      { } 
   };

private: 
   std::vector< variable > variables;

   std::map< identifier, std::list< unsigned int > > indices;
      // Each variable has a list of indices. The first index
      // refers to the meaning of the variable in the current scope.

public: 
   unsigned int size( ) const { return variables. size( ); } 

   const identifier& assign( const identifier& name, const treetype& tt );
      // Returns the name of register that we reserved for name. 
      // It is guaranteed to be unique.

   const variable* lookup( const identifier& name ) const;
      // Returns a pointer to the last assignment of name, or the 
      // null pointer when no assignment exists.
 
   const variable& back( ) const;
   void pop_back( );
      // The methods can be used together when quitting a scope.
      // 
      // Assume that s was the size( ) when the scope was entered.
      //
      // while( size( ) > s )
      // {
      //    v = vs. back( );
      //    organize the cleanup of v.
      //    vs. pop_back( );
      // } 

   friend std::ostream& operator << ( std::ostream& stream, 
                                      const varstore& vs ); 
};

std::ostream& operator << ( std::ostream& stream,
                            const varstore::variable& v ); 

#endif


