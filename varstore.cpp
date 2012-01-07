
#include "varstore.h"

namespace
{
   std::string capitalize( const std::string& s )
   {
      std::string res;

      for( unsigned int i = 0; i < s. size( ); ++ i )
      {
         int c = s[i];
         if( c >= 'a' && c <= 'z' )
            c = c - 'a' + 'A';
         res. push_back(c);
      }
      return res;
   }
}


const identifier& 
varstore::assign( const identifier& name, const treetype& tt )
{
   unsigned int i = variables. size( ); 

   variables. push_back( variable( name, tt, 
                            identifier::unique( 
                                   name. size( ) ? capitalize( name [0] ) : 
                                                   std::string("R") )));
   indices [ name ]. push_front(i);
      // This is for quick lookup of the variable.

   return variables. back( ). reg;
}


const varstore::variable* varstore::lookup( const identifier& name ) const 
{
   std::map< identifier, std::list< unsigned int > > :: const_iterator
      p = indices. find( name );
   if( p == indices. end( ))
       return 0;
   else
      return & variables [ p -> second. front( ) ];
}


const varstore::variable& varstore::back( ) const
{
   ASSERT( variables. size( ));
   return variables. back( );
}


void varstore::pop_back( )
{
   ASSERT( variables. size( ));

   // We must also clean up the reference in indices:

   std::map< identifier, std::list< unsigned int > > :: iterator
      p = indices. find( variables. back( ). name );
   p -> second. pop_front( );

   // If we made the list of references for the name empty, we delete it:

   if( p -> second. size( ) == 0 )
      indices. erase(p);

   variables. pop_back( ); 
}


std::ostream& operator << ( std::ostream& stream,
                            const varstore::variable& v )
{
   stream << v. name << "  -->  " << v. tt << ", " << v. reg;
   return stream;
}


std::ostream& operator << ( std::ostream& stream,
                            const varstore& vs )
{
   stream << "Varstore:\n\n";
   for( unsigned int i = 0; i < vs. variables. size( ); ++ i )
      stream << vs. variables [i] << "\n";
   stream << "\n"; 
   return stream;
}


