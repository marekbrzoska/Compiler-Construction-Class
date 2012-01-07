

#ifndef SUBSTITUTION_INCLUDED
#define SUBSTITUTION_INCLUDED   1


#include "identifier.h"
#include "treetype.h"
#include "tree.h" 


// A substitution consists of two parts, a mapping
// from identifiers to treetypes, and a mapping
// from identifiers to trees.
// 
// The first part is needed for polymorphic (template) functions,
// like for example: - : func( int, pointerto(X), pointerto(X) );
//
// If the - is implemented, then X must be a type parameter. 
// The definition of - is: 
//
// - : func( int; pointerto(X), pointerto(X) ) 
// [ D; P1, P2; I ; X ]
//
// I = P1 - P2;
// D = I / sizeof(X). 


class substitution
{

   struct type_assignment
   {
      identifier v;
      treetype t;

      type_assignment( const identifier& v, const treetype& t )
         : v(v), t(t)
      { } 
   };

   struct tree_assignment
   {
      identifier v;
      tree t;

      tree_assignment( const identifier& v, const tree& t )
         : v(v), t(t)
      { } 
   };


   std::vector< type_assignment > type_assignments;
   std::vector< tree_assignment > tree_assignments;

public: 
   void type_assign( const identifier& v, const treetype& t );
   void tree_assign( const identifier& v, const tree& t );
      // We don't check for reassignment.

   const treetype* type_lookup( const identifier& v ) const; 
   const tree* tree_lookup( const identifier& v ) const;
      // returns null-pointer when the value does not exist.

   unsigned int type_size( ) { return type_assignments. size( ); } 
   unsigned int tree_size( ) { return tree_assignments. size( ); } 

   void type_backtrack( unsigned int s );
   void tree_backtrack( unsigned int s );
   void clear( ) { type_assignments. clear( ); tree_assignments. clear( ); }

   friend std::ostream& operator << ( std::ostream& , const substitution& ); 
};


#endif


