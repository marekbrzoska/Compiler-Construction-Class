

#ifndef DATA_INCLUDED
#define DATA_INCLUDED   1


#include<vector>
#include <iostream> 


// Data deals with binary representations of primitive types.
// We have a dilemma here:
// We want our intermediate representation to be machine indepedent. 
// At the same time, binary representations are machine dependent,
// so we shouldn't use them at all in the intermediate representation.
// 
// The alternative, which is to use a union, or a class hierarchy
// is also problematic. It would introduce false values of sizeof,
// which are as problematic as values that are valid on some, but not all,
// processors.
//
// Since the alternatives are equally bad, we stick with binary 
// representation because it is easy, and we promise not make use 
// of the details of the representation, in order to keep it
// generic.


void hexprint( std::ostream& stream, 
               const std::vector< char > & vect, 
               unsigned int i1, unsigned int i2 ); 
   // Print vector [ i1 .. i2 ] in a nice way.
   // On every multiple of 16, we start a new line, and print the offset in
   // the vector. 


std::ostream& operator << ( std::ostream& , const std::vector< char > & );
   // Difference with the previous is that we don't print addresses 
   // and don't format.


// Writing into an iterator is always backwards: 
//    it -= sizeof( int );
//    *it = (binary representation of ) i; 
// Reading from an iterator is always upwards:
//    i = (conversion of) *x;
//    x += sizeof(int); 

std::vector< char > :: iterator & operator << ( 
                            std::vector< char > :: iterator & p,
                            int k );

std::vector< char > :: const_iterator & operator >> ( 
                            std::vector< char > :: const_iterator & p,
                            int& k );

std::vector< char > :: iterator & operator << ( 
                            std::vector< char > :: iterator & p,
                            double d );

std::vector< char > :: const_iterator & operator >> (  
                            std::vector< char > :: const_iterator & p,
                            double& d );

std::vector< char > :: iterator & operator << (
                            std::vector< char > :: iterator & p,
                            char c );

std::vector< char > :: const_iterator & operator >> (
                            std::vector< char > :: const_iterator & p,
                            char& c );

std::vector< char > :: iterator & operator << (
                            std::vector< char > :: iterator & p,
                            bool b );

std::vector< char > :: const_iterator & operator >> (
                            std::vector< char > :: const_iterator & p,
                            bool& d );

std::vector< char > :: iterator & operator << ( 
                            std::vector< char > :: iterator & p,
                            void* q );

std::vector< char > :: const_iterator & operator >> ( 
                            std::vector< char > :: const_iterator & p,
                            void* &q ); 

// The strings should not contain a \0 character, because
// we print them like C-style strings. The string should fit. 

std::vector< char > :: iterator & operator << (
                            std::vector< char > :: iterator & p,
                            const std::string& s );

std::vector< char > :: iterator & operator >> (
                            std::vector< char > :: iterator & p,
                            std::string& s );

std::vector< char > tobinary( bool b ); 
std::vector< char > tobinary( char c );
std::vector< char > tobinary( int x );
std::vector< char > tobinary( double d );
std::vector< char > tobinary( void* q );

#endif
 

