

#include "typesystem.h"
#include "substitution.h"



const typesystem::conversion*
typesystem::conversionset::findtype( const treetype& tt ) const 
{
   for( std::list< conversion > :: const_iterator
           p = list. begin( );
           p != list. end( );
           ++ p )
   {
      if( p -> gettype( ) == tt )
         return &(*p);
   }
   return 0;
}


typesystem::conversion*
typesystem::conversionset::findtype( const treetype& tt )
{
   for( std::list< conversion > :: iterator
           p = list. begin( );
           p != list. end( );
           ++ p )
   {
      if( p -> gettype( ) == tt )
         return &(*p);
   }
   return 0;
}


typesystem::conversionset
typesystem::allconversions( const tree& t ) const
{
   conversionset result;
   result. list. push_back( conversion( t, 0 ));
   bool change = true;

   while( change )
   {
      change = false;

      // We go through our current list of conversions:

      for( std::list< conversion > :: const_iterator
              p = result. list. begin( );
              p != result. list. end( );
              ++ p )
      {
         for( std::list< convfunc > :: const_iterator
                 q = convfunctions. begin( );
                 q != convfunctions. end( );
                 ++ q )
         {
            // We have to see if *q is applicable on *p;

            substitution s;
            if( q -> tt. getargtype(0). match_into( p -> tt. tt, s,
                q -> typevars ))
            {
               // We have found an applicable conversion *q.

               treetype restype = q -> tt. getresulttype( ). subst(s);
               conversion* witt = result. findtype( restype );
 
               conversion c = conversion( 
                                 tree( q -> name, p -> tt, restype ),
                                 p -> cost + q -> cost ); 
               if( witt )
               {
                  // We already have restype:

                  if( witt -> replacewhencheaper( c ))
                     change = true; 
               }
               else
                  result. list. push_back(c);
            }
         }
      } 
   }
   return result; 
}


std::ostream& operator << ( std::ostream& stream, 
                            const typesystem::error& e )
{
   stream << "could not find a type for term " << e. t << "\n";
   return stream;
}


std::ostream& operator << ( std::ostream& stream,
                            const typesystem::conversion& c )
{
   stream << c.tt << " (cost " << c. cost << ")";
   return stream;
}


std::ostream& operator << ( std::ostream& stream,
                            const typesystem::conversionset& set )
{
   stream << "[ ";
   for( std::list< typesystem::conversion > :: const_iterator
           p = set. list. begin( );
           p != set. list. end( );
           ++ p )
   {
      if( p != set. list. begin( ))
         stream << ", ";
      stream << *p;
   }
   stream << " ]\n";
   return stream;
}


std::ostream& operator << ( std::ostream& stream,
                            const typesystem::concretefunc& c )
{
   stream << c. name << " : ";
   stream << "Pi( [ ";
   for( std::list< identifier > :: const_iterator
           p = c. typevars. begin( );
           p != c. typevars. end( );
           ++ p )
   {
      if( p != c. typevars. begin( ))
         stream << ": ";
      stream << *p;
   }
   stream << " ],  " << c. tt << " )";
   return stream;
}


std::ostream& operator << ( std::ostream& stream,
                            const typesystem::convfunc& cf )
{
   stream << "conversion function " << cf. name << " : Pi( [ ";
   for( std::list< identifier > :: const_iterator
           p = cf. typevars. begin( );
           p != cf. typevars. end( );
           ++ p )
   {
      if( p != cf. typevars. begin( ))
         stream << ": ";
      stream << *p;
   }
   stream << " ], " << cf. tt << " )";
   return stream;
}


std::ostream& operator << ( std::ostream& stream, 
                            const typesystem::application& a )
{
   stream << "applying function   " << a. func << "\n";
   stream << "      on arguments ";
   for( std::list< typesystem::conversion > :: const_iterator
           p = a. args. begin( );
           p != a. args. end( );
           ++ p )
   {
      if( p != a. args. begin( ))
         stream << ", ";
      stream << *p;
   }
   stream << "\n";
   return stream;
}

 
std::ostream& operator << ( std::ostream& stream, const typesystem& ts )
{
   stream << "Type System\n\n"; 
   for( std::map< identifier, 
                  std::list< typesystem::concretefunc > > :: const_iterator
           p = ts. instances. begin( );
           p != ts. instances. end( );
           ++ p )
   {
      stream << "instances of " << ( p -> first ) << " :\n";
      for( std::list< typesystem::concretefunc > :: const_iterator  
              q = p -> second. begin( );
              q != p -> second. end( );
              ++ q )
      {
         stream << "   " << *q << "\n";
      }
   }
   stream << "\n";
   stream << "Conversion Functions:\n";
   for( std::list< typesystem::convfunc > :: const_iterator
           p = ts. convfunctions. begin( );
           p != ts. convfunctions. end( );
           ++ p )
   {
      stream << "   " << *p << "\n";
   }
   stream << "\n";
   return stream;
}



// exrc 9.1
bool typesystem::application::strictlymoregeneralthan( const application& a ) const{
    if ( args.size() != a.args.size() ) {
        return false;
    }
    int strictly_less_counter = 0;
    for (
            std::list<conversion>::const_iterator mine = args.begin(),
            his = a.args.begin();
           
            mine != args.end(),
            his != a.args.end(); 
           
            mine++, his++
        ) {
        if (mine->cost < his->cost) {
            strictly_less_counter++;
        } else if (mine->cost > his->cost) {
            return false;
        }
    }
    if (strictly_less_counter > 0) {
        return true;
    } else {
        return false;
    }
    std::cout << args.size();
}
