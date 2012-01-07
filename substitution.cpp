

#include "substitution.h"
#include "assert.h"


void substitution::type_assign( const identifier& v, const treetype& t )
{
   type_assignments. push_back( type_assignment( v, t ));
}

void substitution::tree_assign( const identifier& v, const tree& t )
{
   tree_assignments. push_back( tree_assignment( v, t ));
}

const treetype* substitution::type_lookup( const identifier& v ) const
{
   for( std::vector< substitution::type_assignment > :: const_iterator
           p = type_assignments. begin( );
           p != type_assignments. end( );
           ++ p )
   {
      if( p -> v == v )
         return & ( p -> t );
   }
   return 0;
}

const tree* substitution::tree_lookup( const identifier& v ) const
{
   for( std::vector< substitution::tree_assignment > :: const_iterator
           p = tree_assignments. begin( );
           p != tree_assignments. end( );
           ++ p )
   {
      if( p -> v == v )
         return & ( p -> t );
   }
   return 0;
}

void substitution::type_backtrack( unsigned int s )
{
   ASSERT( s <= type_size( ));
   while( s < type_size( ))
      type_assignments. pop_back( );
}

void substitution::tree_backtrack( unsigned int s )
{
   ASSERT( s <= tree_size( ));
   while( s < tree_size( ))
      tree_assignments. pop_back( );
}

 
std::ostream& operator << ( std::ostream& stream, const substitution& subst ) 
{
   stream << "Substitution:\n\n";
   for( std::vector< substitution::type_assignment > :: const_iterator
           p = subst. type_assignments. begin( );
           p != subst. type_assignments. end( );
           ++ p )
   {
      stream << "   " << ( p -> v ) << " --> " << ( p -> t ) << "\n";
   }
   stream << "\n";

   for( std::vector< substitution::tree_assignment > :: const_iterator
           p = subst. tree_assignments. begin( );
           p != subst. tree_assignments. end( );
           ++ p )
   {
      stream << "   " << ( p -> v ) << " --> " << ( p -> t ) << "\n";
   }
   stream << "\n";

   return stream;
}


