
#include "data.h"

namespace
{
   // hexprint i, using len positions. 
   // len = 2, hexprints the last 8 bit.
   // len = 4, hexprints the last 16 bits.
   // len = 8, prints a complete unsigned int, when it is 32 bits long. 

   void hexprint( std::ostream& stream, unsigned int len, unsigned int i )
   {
      while( len )   
      {
         char c = ( i >> ( 4 * ( len - 1 ))) & 0X0F;
         if( c >= 10 )
            stream << (char) ( c + 'A' - 10 );
         else
            stream << (char) ( c + '0' );
 
         -- len;
      }
   }
}


void hexprint( std::ostream& stream, 
               const std::vector< char > & vect, 
               unsigned int i1, unsigned int i2 )
{

   // If the index that we start on is not divisible by 16,
   // we print the last index before it, that was divisible by 16,
   // and we print spaces.

   if( i1 & 0X0F )
   {
      hexprint( stream, 8, i1 & 0XFFFFFFF0 );
      stream << ": ";
      for( unsigned int i = 0; i < ( i1 & 0X0F ); ++ i )
         stream << "   ";
   }

   for( unsigned int i = i1; i < i2; ++ i )
   {
      // We start a new line when the address is divisible by 16,
      // and we print the address:

      if( ( i & 0X0F ) == 0 )
      {
         stream << "\n"; 
         hexprint( stream, 8, i );
         stream << ": ";
      }
      
      hexprint( stream, 2, vect [i] ); stream << ' ';
   }
   stream << '\n';
}


std::ostream& operator << ( std::ostream& stream, 
                            const std::vector< char > & vect )
{
   for( std::vector< char > :: const_iterator 
           p = vect. begin( ); p < vect. end( ); ++ p )
   {
      if( p != vect. begin( ))
         stream << ' ';
      hexprint( stream, 2, *p ); 
   }
   return stream; 
}


std::vector< char > :: iterator& operator << ( 
                           std::vector< char > :: iterator& p, int k )
{
   char* k_ptr = reinterpret_cast< char* > ( &k ); 

   p -= sizeof( int );
   for( unsigned int i = 0; i < sizeof( int ); ++ i )
      p[i] = k_ptr [i];    

   return p; 
}


std::vector< char > :: const_iterator& operator >> ( 
                           std::vector< char > :: const_iterator& p,
                           int& k )
{
   char* k_ptr = reinterpret_cast< char* > ( &k );

   for( unsigned int i = 0; i < sizeof( int ); ++ i )
      * ( k_ptr ++ ) = * ( p ++ );

   return p;
}


std::vector< char > :: iterator& operator << (
                           std::vector< char > :: iterator& p, double d )
{
   char* d_ptr = reinterpret_cast< char* > ( &d );

   p -= sizeof( double );
   for( unsigned int i = 0; i < sizeof( double ); ++ i )
      p[i] = d_ptr [i];

   return p; 
}


std::vector< char > :: const_iterator& operator >> ( 
                           std::vector< char > :: const_iterator& p,
                           double& d )
{
   char* d_ptr = reinterpret_cast< char * > ( &d );

   for( unsigned int i = 0; i < sizeof( double ); ++ i )
      * ( d_ptr ) ++ = * ( p ++ );
 
   return p;
}


std::vector< char > :: iterator& operator << (
                           std::vector< char > :: iterator& p, char c )
{
   char* c_ptr = reinterpret_cast< char* > ( &c );

   p -= sizeof( char );
   for( unsigned int i = 0; i < sizeof( char ); ++ i )
      p[i] = c_ptr [i];

   return p;
}


std::vector< char > :: const_iterator& operator >> (
                           std::vector< char > :: const_iterator& p,
                           char& c )
{
   char* c_ptr = reinterpret_cast< char * > ( &c );

   for( unsigned int i = 0; i < sizeof( char ); ++ i )
      * ( c_ptr ) ++ = * ( p ++ );

   return p;
}


std::vector< char > :: iterator& operator << (
                           std::vector< char > :: iterator& p, bool b )
{
   char* b_ptr = reinterpret_cast< char* > ( &b );

   p -= sizeof( bool );
   for( unsigned int i = 0; i < sizeof( bool ); ++ i )
      p[i] = b_ptr [i];

   return p;
}


std::vector< char > :: const_iterator& operator >> (
                           std::vector< char > :: const_iterator& p,
                           bool& b )
{
   char* b_ptr = reinterpret_cast< char * > ( &b );

   for( unsigned int i = 0; i < sizeof( bool ); ++ i )
      * ( b_ptr ) ++ = * ( p ++ );

   return p;
}


std::vector< char > :: iterator& operator << ( 
                           std::vector< char > :: iterator& p, void *q )
{
   char* q_pntr = reinterpret_cast< char* > ( &q );

   p -= sizeof( void* );
   for( unsigned int i = 0; i < sizeof( void* ); ++ i )
      p[i] = q_pntr [i];

   return p;
}


std::vector< char > :: const_iterator& operator >> ( 
                           std::vector< char > :: const_iterator& p,
                           void* &q )
{
   char* q_ptr = reinterpret_cast< char * > ( &q );

   for( unsigned int i = 0; i < sizeof( void* ); ++ i )
      * ( q_ptr ) ++ = * ( p ++ );
   
   return p;
}

 
std::vector< char > :: iterator& operator << (
                           std::vector< char > :: iterator& p, 
                           const std::string& s )
{
   p -= s. size( ) + 1; 
      // one extra for terminating 0. 

   for( unsigned int i = 0; i < s. size( ); ++ i )
      p[i] = s[i];
   p [ s. size( ) ] = 0; 
   return p;
}


std::vector< char > :: const_iterator& operator >> (
                           std::vector< char > :: const_iterator & p,
                           std::string& s )
{
   s. clear( ); 
   while( *p )
   { 
      s. push_back( * ( p ++ ) );
   } 
   p ++ ;   // Skip terminating 0. 

   return p;
}


/////////////////////////////////////////////////////////////
// All the tobinary functions use the previous printing
// methods.
////////////////////////////////////////////////////////////

std::vector< char > tobinary( bool b )
{
   std::vector< char > res = std::vector< char > ( sizeof( bool ));
   std::vector< char > :: iterator p = res. end( );
   p << b;
   return res;
}


std::vector< char > tobinary( char c )
{
   std::vector< char > res = std::vector< char > ( sizeof( char ));
   std::vector< char > :: iterator p = res. end( );
   p << c;
   return res;
}


std::vector< char > tobinary( int x )
{
   std::vector< char > res = std::vector< char > ( sizeof( int ));
   std::vector< char > :: iterator p = res. end( );
   p << x;
   return res;
}


std::vector< char > tobinary( double d )
{
   std::vector< char > res = std::vector< char > ( sizeof( double ));
   std::vector< char > :: iterator p = res. end( );
   p << d; 
   return res;
}


std::vector< char > tobinary( void* q )
{
   std::vector< char > res = std::vector< char > ( sizeof( void* ));
   std::vector< char > :: iterator p = res. end( );
   p << q;
   return res;
}


