

#ifndef TYPESYSTEM_INCLUDED
#define TYPESYSTEM_INCLUDED  1

#include <list>
#include <vector>
#include "tree.h"


class typesystem
{

public: 
   struct error
   {
      tree t;

      error( const tree& t ) : t(t) { }
   };


   // A conversion is the result of a possible conversion of a tree, 
   // together with the cost that was required to obtain it. 

   struct conversion
   {
      tree tt;  
         // Since the tree already contains a type, there is no need to
         // store a type separately. 

      unsigned int cost;
         // Total cost of the conversion from the original tree
         // into the current tree. 

      const treetype& gettype( ) const { return tt. tt; } 
  
      bool replacewhencheaper( const conversion& other )
      {
         if( other. cost < cost ) 
         {
            *this = other; 
            return true;
         }
         else
            return false;
      }

      conversion( const tree& tt, unsigned int cost )
         : tt(tt), cost( cost ) 
      { }

      conversion( const tree& tt ) 
         : tt(tt), cost(0) 
      { } 

   };


   struct conversionset
   {
      std::list< conversion > list;
         // You can do this also with a map 
         // (std::map< treetype, result > ) if you want.

      const conversion* findtype( const treetype& tt ) const;
      conversion* findtype( const treetype& tt ); 
         // Returns a pointer to a conversion for type tt, if we have one.
         // Otherwise the null pointer.  
   }; 
   

   // It turns out necessary to support polymorphic (template) types.
   // For example, addition of a pointer and an integer is the
   // same operation for all types.
   // This means that we should give it type 
   // addpointer: func( pointer(X); pointer(X), int ). 
  
   struct concretefunc 
   {
      identifier name;
      std::list< identifier > typevars; 
      treetype tt;

      // Non-polymorphic function:

      concretefunc( const identifier& name, const treetype& tt )
         : name( name ), tt( tt )
      { } 

      // Polymorphic function, in most cases there is only one variable: 

      concretefunc( const identifier& name, const identifier& var, 
                    const treetype& tt )
         : name( name ), tt( tt )
      {
         typevars. push_back( var );
      }

      concretefunc( const identifier& name, 
                    const identifier& var1, const identifier& var2,
                    const treetype& tt )
         : name( name ), tt( tt )
      {
         typevars. push_back( var1 );
         typevars. push_back( var2 );
      }

   };


   std::map< identifier, std::list< concretefunc > > instances;
      // Maps overloaded functions into to their concrete instances. 

   struct convfunc
   {
      identifier name;
      unsigned int cost;
      std::list< identifier > typevars;
      treetype tt;

      convfunc( const identifier& name, unsigned int cost, 
                const treetype& from, const treetype& to )
         : name( name ), cost( cost ), 
           tt( treetype( type_unknown ))
      {
         std::list< treetype > args;
         args. push_back( from );
         tt = treetype( type_func, to, args );
      }

      convfunc( const identifier& name, unsigned int cost,
                const identifier& var, 
                const treetype& from, const treetype& to )
         : name( name ), cost( cost ),
           tt( treetype( type_unknown ))
      {
         std::list< treetype > args;
         args. push_back( from );
         tt = treetype( type_func, to, args );
         typevars. push_back( var );
      }
   };

   std::list< convfunc > convfunctions; 
      // Unfortunately, we cannot index converters by type,
      // because of the polymorphism.

   conversionset allconversions( const tree& t ) const; 


   // An application consists of a function, and a set of conversions 
   // whose types are such that the function can be applied on them.
   //
   // (For example, func = +int, and args =
   //  100:int (cost 1),  200:int (cost 2).

   struct application
   {
      tree func;
      std::list< conversion > args;

      application( const tree& func, const std::list< conversion > & args )
         : func( func ),
           args( args )
      {
         ASSERT( func. tt. getconstr( ) == type_func );
         ASSERT( func. tt. getarity( ) == args. size( ));
         // We could check the types, but we don't do that.
      }

      bool strictlymoregeneralthan( const application& a ) const;  
         // True if we are strictly more general than a. 

   };

   std::list< application > 
   allapplications( const identifier& func,
                    const std::list< std::list< conversion > > & args ) const;

   static const application& mostgeneral( const std::list< application > & a )
   throw( error );   
   // Select the most general application from a if it is uniquely defined.
   // Otherwise, throw an error. 
    
};

std::ostream& operator << ( std::ostream& , const typesystem::error& ); 
std::ostream& operator << ( std::ostream& , const typesystem::conversion& );
std::ostream& operator << ( std::ostream& , const typesystem::conversionset& );
std::ostream& operator << ( std::ostream& , const typesystem::concretefunc& );
std::ostream& operator << ( std::ostream& , const typesystem::convfunc& ); 
std::ostream& operator << ( std::ostream& , const typesystem::application& a );
std::ostream& operator << ( std::ostream& , const typesystem& );

#endif

