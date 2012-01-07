
// Written by Hans de Nivelle, for course Compiler Construction.
// December 2011.

#ifndef PRIMTYPE_INCLUDED
#define PRIMTYPE_INCLUDED 1

#include "identifier.h"
#include "assert.h" 


// Primitive types are things like int, float, double, bool, etc.
// Structures and classes are also primitive types. 

struct primtype
{
   identifier name; 
   unsigned int size; 

   explicit primtype( const identifier& name, unsigned int size )
      : name( name ), size( size )
   { }

   static int compare( const primtype& p1, const primtype& p2 )
   {
      int c = identifier::compare( p1. name, p2. name );
      if(c) return c;
      
      if( p1. size < p2. size ) return -1;
      if( p1. size > p2. size ) return 1;
      return 0;
   }

};


inline std::ostream& operator << ( std::ostream& stream, const primtype& p )
{
   stream << "primitive( " << p. name << ", " << p. size << " )"; 
   return stream;
}


inline bool operator == ( const primtype& p1, const primtype& p2 )
{
   return primtype::compare( p1, p2 ) == 0; 
}


inline bool operator != ( const primtype& p1, const primtype& p2 )
{
   return primtype::compare( p1, p2 ) != 0; 
}


#endif


