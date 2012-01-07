

// Written by Hans de Nivelle, November 2011, for the lab on 
// compiler construction.

#ifndef TREE_INCLUDED 
#define TREE_INCLUDED    1

#include "identifier.h"
#include "treetype.h"


#include <list>
#include <iostream> 
#include <vector>


class tree;


struct treenode
{
   unsigned int refcount;

   identifier id; 
   std::list< tree > subtrees; 

   treenode( const identifier& id )
      : refcount(1),
        id( id ) 
   {
   }
};


class tree
{

private:
   treenode* repr;
   
public: 
   std::vector< char > attr;
      // Pretty much everything that you want can be put in an attribute,
      // using the binary printing functions in data.h 
      // 
      // If you want to put a double inside, you can write 
      //    attr = std::vector< char > :: vector( sizeof(double), 0 );
      //    std::vector< char > :: iterator p = attr. end( );
      //    p << 3.1415926535;
      //    ASSERT( p == attr. begin( ));
      // 
      // Only leaves (trees without subtrees) can have attributes. 

   treetype tt;
      
   tree( const tree& other );
   void operator = ( const tree& other );
   ~tree( );


   tree( const identifier& id, const std::list< tree > & subtrees,
         const treetype& tt = treetype( type_unknown )); 


   // Construct a leaf tree. Only leaf trees can have attributes:

   tree( const identifier& id, const std::vector< char > & a, 
         const treetype& tt = treetype( type_unknown ));


   // A few more constructors for trees with a small number
   // of subtrees:

   tree( const identifier& id, const tree& sub1, 
         const treetype& tt = treetype( type_unknown ));

   tree( const identifier& id, 
         const tree& sub1, const tree& sub2, 
         const treetype& tt = treetype( type_unknown ));

   tree( const identifier& id, 
         const tree& sub1, const tree& sub2, const tree& sub3,
         const treetype& tt = treetype( type_unknown ));


   const identifier& getidentifier( ) const
      { return repr -> id; }

   unsigned int getnrsubtrees( ) const
      { return repr -> subtrees. size( ); } 

   const tree& operator [] ( unsigned int i ) const; 
      // Get the i-th subtree. 

   static int compare( const tree& t1, const tree& t2 );
      // -1 means:      t1 < t2. 
      //  0 means:      t1 == t2. 
      //  1 means:      t1 > t2. 
      // The order is not a well-order.


   bool operator < ( const tree& other ) const
      { return compare( *this, other ) == -1; }

   bool operator > ( const tree& other ) const
      { return compare( *this, other ) == 1; }

   bool operator == ( const tree& other ) const
      { return compare( *this, other ) == 0; }

   bool operator != ( const tree& other ) const  
      { return compare( *this, other ) != 0; }

   bool operator >= ( const tree& other ) const
      { return compare( *this, other ) != -1; }

   bool operator <= ( const tree& other ) const
      { return compare( *this, other ) != 1; }

   void prettyprint( std::ostream& stream, unsigned int indent ) const;

   void prettyprint( std::ostream& stream ) const
      { prettyprint( stream, 0 ); }

};


std::ostream& operator << ( std::ostream& stream, const tree& t );
   // Simple print. Attributes of inner nodes (non-leaves) are 
   // not printed. 

#endif


