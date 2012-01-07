
// Written by Hans de Nivelle in November 2011, for the course
// Compiler Construction.


#ifndef IDENTIFIER_INCLUDED
#define IDENTIFIER_INCLUDED   1


#include <string>
#include <list> 
#include <map>


#include "assert.h"


#include <iostream>


// Identifiers are C++ style identifiers, with namespaces, so 
// that an identifier has form s1::s2:: ... :: sn, where each si is a string.
// Of course it is possible to use n = 1. Even n = 0 is allowed. 


class identifier
{

   // In order to avoid repeated string comparisons, we ensure that
   // every string occurring in an identifier, occurs only once in memory. 
   // Under this invariant, it is sufficient to compare pointers instead
   // of strings. 
   // This replacement is done component wise, so that the string
   // s1::s2 :: ... :: sn is replaced by ( &s1, &s2, ..., &sn ). 
   // This means that we only have to compare n pointers, when
   // we want to ( &s1, ..., &sn ) to another identifier. 
   // We assume that n does not get big. 


   struct uniquestring
   { 
      std::string s;
      unsigned int refcount;
         // A string with a reference counter. We forget the string
         // when the reference counter becomes zero. 

      uniquestring( const std::string& s )
         : s(s), 
           refcount(0)
      { }
   };


   // For the normalization table, we need either an operator < on
   // uniquestrings, or a a comparison object. We chose the 
   // comparison object:
 
   struct uniquestring_cmp
   {
      bool operator( ) ( const uniquestring* s1, const uniquestring* s2 )
      {
         return ( s1 -> s ) < ( s2 -> s );
      }
   };

 
   static std::map< uniquestring*, bool, uniquestring_cmp > normtable;
  

 

   std::list< uniquestring* > repr;

   uniquestring* normalize( const std::string& s );
      // This method maintains a table of all strings and their
      // normalizations.

   // When a copy is made, all reference counters need to be 
   // increased:

   void increasecounters( ) const; 
   void decreasecounters( ) const;

   
public: 
   identifier( );
      // Default identifier is empty.

   identifier( const std::string& s1 );
   identifier( const std::string& s1, const std::string& s2 );
   identifier( const std::string& s1, const std::string& s2, 
               const std::string& s3 );
   identifier( const std::string& s1, const std::string& s2,
               const std::string& s3, const std::string& s4 );

   identifier( const char* s1 );
   identifier( const char* s1, const char* s2 );
   identifier( const char* s1, const char* s2, const char* s3 ); 
   identifier( const char* s1, const char* s2,
               const char* s3, const char* s4 );
  
   identifier( const identifier& id );
   void operator = ( const identifier& id );
   ~identifier( );
      // These are necessary because of the reference counters in
      // unique strings. 
  
   unsigned int size( ) const { return repr. size( ); }
      // The size of a string is the number of its components.

   const std::string& operator [ ] ( unsigned int s ) const;
      // Returns the s-th component of the identifier.

   void operator += ( const identifier& id );
   identifier operator + ( const identifier& id )
   {
      identifier res = *this; res += id; return res;
   }

   static int compare( const identifier& id1, const identifier& id2 );
      // Comparison is by address. This implies that that the order 
      // is unpredictable. 

   friend bool operator == ( const identifier& id1, const identifier& id2 );
   friend bool operator != ( const identifier& id1, const identifier& id2 );
   friend bool operator < ( const identifier& id1, const identifier& id2 );
   friend bool operator > ( const identifier& id1, const identifier& id2 );
   friend bool operator <= ( const identifier& id1, const identifier& id2 );
   friend bool operator >= ( const identifier& id1, const identifier& id2 );

   static void printnormtable( std::ostream& );
      // Print the normalization table.

   static identifier unique( const std::string& s );
      // Make a unique identifier of form sXXXX, where XXXX is the
      // decimal representation of some index. 
};


inline bool operator == ( const identifier& id1, const identifier& id2 )
{
   return identifier::compare( id1, id2 ) == 0;
}


inline bool operator != ( const identifier& id1, const identifier& id2 )
{
   return identifier::compare( id1, id2 ) != 0; 
}


inline bool operator < ( const identifier& id1, const identifier& id2 )
{
   return identifier::compare( id1, id2 ) == -1;
}


inline bool operator > ( const identifier& id1, const identifier& id2 )
{
   return identifier::compare( id1, id2 ) == 1;
}


inline bool operator <= ( const identifier& id1, const identifier& id2 )
{
   return identifier::compare( id1, id2 ) != 1; 
}


inline bool operator >= ( const identifier& id1, const identifier& id2 )
{
   return identifier::compare( id1, id2 ) != -1; 
}


std::ostream& operator << ( std::ostream& stream , const identifier& id ); 


#endif

