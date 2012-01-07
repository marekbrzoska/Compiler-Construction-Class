

#include "program.h"

program::program( )
   : last(1000)
{ }


void program::insert( const identifier& name, treetype tt, 
                      const flowgraph& fcode )
{
   indices. insert(
      std::pair< identifier, unsigned int > ( name, last ));
   code. insert( 
      std::pair< unsigned int, function > ( last, function( tt, fcode )));

   ++ last;
}


void program::erase( )
{
   indices. clear( );
   code. clear( );
   last = 1000; 
}


void program::erase( const identifier& name )
{
   std::map< identifier, unsigned int > :: iterator p = indices. find( name );
   if( p != indices. end( ))
   {
      code. erase( p -> second );
      indices. erase( p );
   }
}


std::ostream& operator << ( std::ostream& stream, const program& pr )
{
   stream << "Functions in Program:\n\n";

   for( std::map< identifier, unsigned int > :: const_iterator
           p = pr. indices. begin( );
           p != pr. indices. end( );
           ++ p )
   {
      stream << "Name of Function: " << ( p -> first ) << "\n";
      stream << "Address of Function: " << ( p -> second ) << "\n";
      std::map< unsigned int, program::function > :: const_iterator 
         q = pr. code. find( p -> second ); 
      stream << "Type of Function: " << ( q -> second. tt ) << "\n"; 
      stream << ( q -> second. code ) << "\n";
   }

   return stream;
}


