
#ifndef ERROR_INCLUDED
#define ERROR_INCLUDED  1

#include <sstream>

struct error
{
   std::stringstream s;

   error( ) { }

   // It seems that the throw( ) mechanism requires 
   // a copy constructor, when we do not construct the error
   // in the throw statement: 

   error( const error& err )
   {
      ( this -> s ) << err. s. str( );
   }

   error( const std::string& s )
   {
      ( this -> s ) << s; 
   }
};


inline std::ostream& operator << ( std::ostream& stream, const error& err )
{
   stream << err. s. str( );
   return stream;
}

#endif

