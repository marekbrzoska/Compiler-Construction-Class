

// Code written by Hans de Nivelle, June 2006.

#include "assert.h"
#include "tree.h"
#include "data.h"


tree::tree( const tree& other )
   : repr( other. repr ),
     attr( other. attr ),
     tt( other. tt )  
{
   ++ ( repr -> refcount );
}


void tree::operator = ( const tree& other )
{
   ++ ( other. repr -> refcount );

   -- ( repr -> refcount );
   if( ( repr -> refcount ) == 0 )
      delete repr;

   repr = other. repr;
   attr = other. attr;
   tt = other. tt; 
}


tree:: ~tree( ) 
{
   ASSERT( repr -> refcount );

   -- ( repr -> refcount );
   if( ( repr -> refcount ) == 0 )
      delete repr;
}



tree::tree( const identifier& id, const std::list< tree > & subtrees,
            const treetype& tt ) 
   : repr( new treenode( id )),
     tt( tt ) 
{
   ( repr -> subtrees ) = subtrees; 
}


tree::tree( const identifier& id, const std::vector< char > & a, 
            const treetype& tt )
   : repr( new treenode( id )),
     tt( tt ), 
     attr(a)
{
}


tree::tree( const identifier& id, const tree& sub1, const treetype& tt )
   : repr( new treenode( id )),
     tt( tt ) 
{
   repr -> subtrees. push_back( sub1 ); 
}


tree::tree( const identifier& id, 
            const tree& sub1, const tree& sub2,
            const treetype& tt )
   : repr( new treenode( id )),
     tt( tt ) 
{
   repr -> subtrees. push_back( sub1 );
   repr -> subtrees. push_back( sub2 );
}


tree::tree( const identifier& id,
            const tree& sub1, const tree& sub2, const tree& sub3, 
            const treetype& tt )
   : repr( new treenode( id )),
     tt( tt )
{
   repr -> subtrees. push_back( sub1 );
   repr -> subtrees. push_back( sub2 );
   repr -> subtrees. push_back( sub3 );
}

   
int tree::compare( const tree& t1, const tree& t2 )
{

   int c = identifier::compare( t1. getidentifier( ), 
                                t2. getidentifier( )); 
      
   if(c) return c;

   // After the identifiers, we compare the attributes:

   unsigned int s1 = t1. attr. size( );
   unsigned int s2 = t2. attr. size( );

   for( unsigned int i = 0; i < s1 && i < s2; ++ i )
   {
      if( t1. attr [i] < t2. attr [i] ) return -1;
      if( t1. attr [i] > t2. attr [i] ) return 1;
   }

   if( s1 < s2 ) return -1;
   if( s1 > s2 ) return 1;

   // Attributes are equal, we compare subtrees: 

   unsigned int n1 = t1. getnrsubtrees( );
   unsigned int n2 = t2. getnrsubtrees( );
     
   for( unsigned int i = 0; i < n1 && i < n2; ++ i )
   {
      c = compare( t1[i], t2[i] );
      if(c) return c;
   }

   if( n1 < n2 ) return -1;
   if( n1 > n2 ) return 1;

   return 0;
}
      

const tree& tree::operator [ ] ( unsigned int i ) const 
{
   ASSERT( i < repr -> subtrees. size( ));

   std::list< tree > :: const_iterator p = repr -> subtrees. begin( );
   while( i )
   {
      -- i;
      ++ p;
   }
   return *p;
}


void tree::prettyprint( std::ostream& stream, unsigned int indent ) const 
{
   for( unsigned int i = 0; i < indent; ++ i )
      stream << ".    ";

   stream << getidentifier( );
   if( getnrsubtrees( ) == 0 )
   {
      stream << "[ " << attr << " ]\n";
   }
   else
   {
      stream << '\n';
      for( unsigned int i = 0; i < getnrsubtrees( ); ++ i )
      {
         operator[](i). prettyprint( stream, indent + 1 );
            // operator[](i) is the same as
            // (*this)[i]. 
      }
   }
  
   for( unsigned int i = 0; i < indent; ++ i )
      stream << ".    ";
   stream << ": " << tt << "\n";
}


std::ostream& operator << ( std::ostream& stream, const tree& t )
{
   stream << t. getidentifier( );
   if( t. getnrsubtrees( ) > 0 )
   {
      stream << "( ";
      for( unsigned int i = 0; i < t. getnrsubtrees( ); ++ i )
      {
         if( i != 0 )
            stream << ", "; 
         stream << t[i];
      }
      stream << " )"; 
   }
   else
      stream << "[ " << t. attr << " ]";
 
   stream << " : " << t. tt;
   return stream; 
}


