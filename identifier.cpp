

#include "identifier.h"
#include "assert.h"



namespace
{

   // Copied from Geo2011:


   inline bool isdigit( char c )
   {
      return c >= '0' && c <= '9';
   }


   void appendcounter( std::string& s, unsigned int cntr,
                       unsigned int pos )
   {
      // First we remove the trailing digits of s, if there are any.

      unsigned int k = s. size( );
      while( k && isdigit( s [ k - 1 ] ))
         -- k;
      if( k != s. size( ))
         s. erase( k, s. size( ));

      // If s became empty, we replace it by 'x':

      if( s. size( ) == 0 )
         s = 'x';

      unsigned int m = 10;
      unsigned int d = 1;
         // m is the smallest number that does not fit in d digits.

      while( pos > d || cntr >= m )
      {
         m *= 10;
         ++ d;
      }

      // We now use m and d to construct the decimal representation
      // cntr.

      m /= 10;
      for( unsigned int i = 0; i < d; ++ i )
      {
         s += '0' + cntr / m;
         cntr = cntr % m;
         m = m / 10;
      }
   }

}


identifier::uniquestring* identifier::normalize( const std::string& s )
{
 
   uniquestring u = uniquestring(s);
      // There is no guarantee that u is unique, but we need it 
      // for lookup. 

   std::map< uniquestring* , bool, uniquestring_cmp > :: iterator
      p = normtable. find( &u );

   if( p != normtable. end( ))
   {
      // We found it in the table.

      return ( p -> first );
   }
   else
   {
      // We didn't find it, and it needs to be inserted.

      uniquestring* n = new uniquestring(s);
         // And this one is really unique.

      normtable. insert( std::pair< uniquestring*, bool > ( n, true ));
      return n;
   }
}


void identifier::increasecounters( ) const
{
   for( std::list< uniquestring* > :: const_iterator
           p = repr. begin( );
           p != repr. end( );
           ++ p )
   {
      ++ ( (*p) -> refcount );
   }
}


void identifier::decreasecounters( ) const
{

   // We decrease the reference counters, and delete the unique strings
   // for which the reference counter becomes zero.
   // What can be more sad than being deleted like this?

   for( std::list< uniquestring* > :: const_iterator
           p = repr. begin( );
           p != repr. end( );
           ++ p )
   {
      -- ( (*p) -> refcount );
      if( ( (*p) -> refcount ) == 0 )
      {
         normtable. erase( *p ); 
         delete *p;
      }
   }
}



identifier::identifier( ) 
{
}


identifier::identifier( const std::string& s1 )
{
   repr. push_back( normalize( s1 ));
   increasecounters( ); 
}


identifier::identifier( const std::string& s1, 
                        const std::string& s2 )
{
   repr. push_back( normalize( s1 ));
   repr. push_back( normalize( s2 ));
   increasecounters( );
}

   
identifier::identifier( const std::string& s1, 
                        const std::string& s2,
                        const std::string& s3 )
{
   repr. push_back( normalize( s1 ));
   repr. push_back( normalize( s2 ));
   repr. push_back( normalize( s3 ));
   increasecounters( ); 
}


identifier::identifier( const std::string& s1,
                        const std::string& s2,
                        const std::string& s3,
                        const std::string& s4 )
{
   repr. push_back( normalize( s1 ));
   repr. push_back( normalize( s2 ));
   repr. push_back( normalize( s3 ));
   repr. push_back( normalize( s4 ));
   increasecounters( );
}



identifier::identifier( const char* c1 )
{
   repr. push_back( normalize( std::string( c1 )));
   increasecounters( ); 
}


identifier::identifier( const char* c1, const char* c2 )
{
   repr. push_back( normalize( std::string( c1 )));
   repr. push_back( normalize( std::string( c2 )));
   increasecounters( ); 
}


identifier::identifier( const char* c1, const char* c2, const char* c3 )
{
   repr. push_back( normalize( std::string( c1 )));
   repr. push_back( normalize( std::string( c2 )));
   repr. push_back( normalize( std::string( c3 )));
   increasecounters( ); 
}


identifier::identifier( const char* c1, const char* c2,
                        const char* c3, const char* c4 )
{
   repr. push_back( normalize( std::string( c1 )));
   repr. push_back( normalize( std::string( c2 )));
   repr. push_back( normalize( std::string( c3 )));
   repr. push_back( normalize( std::string( c4 )));
   increasecounters( ); 
}


identifier::identifier( const identifier& id )
   : repr( id. repr )
{
   increasecounters( ); 
}


void identifier::operator = ( const identifier& id )
{
   id. increasecounters( );
   decreasecounters( );
      // The order matters!

   repr = id. repr;
}

 
identifier :: ~identifier( )
{
   decreasecounters( ); 
}


const std::string& identifier::operator [ ] ( unsigned int s ) const
{
   ASSERT( s < repr. size( ));
   std::list< uniquestring * > :: const_iterator p = repr. begin( );

   while( s )
   {
      ++ p; 
      -- s; 
   }
  
   return ( (*p) -> s );
}
 
 
void identifier::operator += ( const identifier& id )
{
   id. increasecounters( );

   unsigned int s = id. size( );
   std::list< uniquestring * > :: const_iterator p = id. repr. begin( );

   while( s )
   {
      repr. push_back( *p );
      -- s; 
   }
}


int identifier::compare( const identifier& id1, const identifier& id2 )
{
   std::list< uniquestring* > :: const_iterator p1 = id1. repr. begin( );
   std::list< uniquestring* > :: const_iterator p2 = id2. repr. begin( );

   while( p1 != id1. repr. end( ) && p2 != id2. repr. end( ))
   {
      // We compare only addresses, not strings. Actually, these two lines
      // here are the main reason that this class exists: 

      if( *p1 < *p2 ) return -1;
      if( *p1 > *p2 ) return 1;

      ++ p1;
      ++ p2;
   }

   if( p1 == id1. repr. end( ) && p2 != id2. repr. end( ))
      return -1;
   if( p1 != id1. repr. end( ) && p2 == id2. repr. end( ))
      return 1;

   return 0;
}


std::ostream& operator << ( std::ostream& stream, const identifier& id )
{
   for( unsigned int i = 0; i < id. size( ); ++ i )
   {
      if(i)
         stream << " :: ";
      stream << id [i];
   }
   return stream; 
}


void identifier::printnormtable( std::ostream& stream ) 
{
   std::cout << "Normalization Table: \n";
   
   for( std::map< uniquestring* , bool, uniquestring_cmp > :: const_iterator
           p = normtable. begin( );
           p != normtable. end( );
           ++ p )
   {
      std::cout << ( p -> first ) -> s;
      std::cout << " [ " << ( p -> first ) -> refcount << " ]\n";
   }
   std::cout << "\n";
}


identifier identifier::unique( const std::string& s )
{
   static std::map< std::string, unsigned int > indexmap;

#if 1
   std::cout << "uniqueness table: \n";
   for( std::map< std::string, unsigned int > :: const_iterator
           p = indexmap. begin( );
           p != indexmap. end( );
           ++ p )
   {
      std::cout << ( p -> first ) << ", " << ( p -> second ) << "\n";
   }
   std::cout << "\n\n";
#endif

   unsigned int index = indexmap [s]; 
   while( true )
   {
      std::string s1 = s;
      appendcounter( s1, index, 4 );

      identifier res = identifier( s1 );
      if( res. repr. back( ) -> refcount == 1 )
      {
         // This means that we have the only copy of this identifier.

         indexmap [s] = index + 1; // Next time we start searching here. 
         return res;
      }
     
      index = index + 1; 
   }
}


