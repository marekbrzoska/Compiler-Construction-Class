

#include "assert.h"
#include "treetype.h"
#include "substitution.h"


treetype::treetype( type_constr t )
   : repr( new typenode(t))
{
   ASSERT( t == type_void || t == type_unknown );
}


treetype::treetype( type_constr t, const primtype& p )
   : repr( new typenode(t))
{
   ASSERT( t == type_prim );
   
   repr -> prim. push_back(p);
}


treetype::treetype( type_constr t, const treetype& sub )
   : repr( new typenode(t))
{
   ASSERT( t == type_const || t == type_raw ||
           t == type_pointer || t == type_ref ||
           t == type_reg || t == type_array );

   repr -> subtypes. push_back( sub );
}


treetype::treetype( type_constr t, unsigned int i, const treetype& sub )
   : repr( new typenode(t))
{
   ASSERT( t == type_reg || t == type_array );
   repr -> i = i;
   repr -> subtypes. push_back( sub );
}


treetype::treetype( type_constr t, const treetype& res,
                    const std::list< treetype > & args )
   : repr( new typenode(t))
{
   ASSERT( t == type_func );
   repr -> subtypes. push_back( res );

   for( std::list< treetype > :: const_iterator
           p = args. begin( );
           p != args. end( );
           ++ p )
   {
      repr -> subtypes. push_back( *p );
   }
}


const treetype& treetype::getsubtype( ) const
{
   ASSERT( repr -> t == type_const || repr -> t == type_raw ||
           repr -> t == type_pointer || repr -> t == type_ref || 
           repr -> t == type_reg || repr -> t == type_array );

   return repr -> subtypes. front( );
}


const primtype& treetype::getprimtype( ) const
{
   ASSERT( repr -> t == type_prim );

   return repr -> prim. front( );
}


unsigned int treetype::getinteger( ) const
{
   ASSERT( repr -> t == type_reg || repr -> t == type_array );

   return repr -> i;
}


unsigned int treetype::getarity( ) const
{
   ASSERT( repr -> t == type_func );
   return repr -> subtypes. size( ) - 1;
}


const treetype& treetype::getargtype( unsigned int i ) const
{
   ASSERT( repr -> t == type_func && i <= repr -> subtypes. size( ));

   std::list< treetype > :: const_iterator p = repr -> subtypes. begin( );
   ++ p;
      // The first element is the result, so we skip it.  
   while(i)
   {
      ++ p;
      -- i;
   }

   return *p;
}


const treetype& treetype::getresulttype( ) const
{
   ASSERT( repr -> t == type_func );

   return repr -> subtypes. front( );
}


treetype::treetype( const treetype& other )
   : repr( other. repr )
{
   ++ ( repr -> refcount );
}


void treetype::operator = ( const treetype& other )
{
   ++ ( other. repr -> refcount ); 
   -- ( repr -> refcount );
   if( repr -> refcount == 0 )
      delete repr;

   repr = other. repr;
}


treetype :: ~treetype( )
{
   -- ( repr -> refcount );
   if( repr -> refcount == 0 )
      delete repr;
}


int treetype::compare( const treetype& t1, const treetype& t2 )
{

   if( t1. repr -> t < t2. repr -> t )
      return -1; 
   if( t1. repr -> t > t2. repr -> t ) 
      return 1;

   // Type constructors were equal, we compare primitive types:

   std::list< primtype > :: const_iterator p1 = t1. repr -> prim. begin( );
   std::list< primtype > :: const_iterator p2 = t2. repr -> prim. begin( );

   while( p1 != t1. repr -> prim. end( ) && p2 != t2. repr -> prim. end( ))
   {
      int c = primtype::compare( *p1, *p2 );
      if(c) return c;
    
      ++ p1; ++ p2;
   }

   if( p1 == t1. repr -> prim. end( ) && p2 != t2. repr -> prim. end( ))
      return -1;
   if( p1 != t1. repr -> prim. end( ) && p2 == t2. repr -> prim. end( ))
      return 1;

   std::list< treetype > :: const_iterator q1 = t1. repr -> subtypes. begin( );
   std::list< treetype > :: const_iterator q2 = t2. repr -> subtypes. begin( );

   while( q1 != t1. repr -> subtypes. end( ) && 
          q2 != t2. repr -> subtypes. end( ))
   {
      int c = compare( *q1, *q2 );
      if(c) return c;

      ++ q1; ++ q2;
   }

   if( q1 == t1. repr -> subtypes. end( ) && q2 != t2. repr -> subtypes. end( ))
      return -1;
   if( q1 != t1. repr -> subtypes. end( ) && q2 == t2. repr -> subtypes. end( ))
      return 1;

   return 0; 
}


std::ostream& operator << ( std::ostream& stream, const treetype& t )
{
   switch( t. getconstr( ))
   {
   case type_prim:
      stream << t. getprimtype( );
      return stream; 

   case type_const:
      stream << "const( " << t. getsubtype( ) << " )";
      return stream;

   case type_raw:
      stream << "raw( " << t. getsubtype( ) << " )";
      return stream;

   case type_pointer:
      stream << "pointer( " << t. getsubtype( ) << " )";
      return stream;

   case type_ref:
      stream << "ref( " << t. getsubtype( ) << " )";
      return stream;

   case type_reg:
      stream << "reg( " << t. getinteger( ) << ", ";
      stream << t. getsubtype( ) << " )"; 
      return stream;

   case type_void:
      stream << "void";
      return stream;

   case type_unknown:
      stream << "unknown-type";
      return stream;

   case type_array:
      stream << "array( " << t. getinteger( ) << ", ";
      stream << t. getsubtype( ) << " )";
      return stream;

   case type_func:
      stream << "func( " << t. getresulttype( );
      if( t. getarity( ))
      {
         stream << "; ";
         for( unsigned int i = 0; i < t. getarity( ); ++ i )
         {
            if( i != 0 )
               stream << ", ";
            stream << t. getargtype(i);
         }
      }
      stream << " )";
      return stream; 

   default:
      std::cerr << "the impossible has just happend\n"; 
      ASSERT( false ); 

   }
}


treetype treetype::subst( const substitution& s ) const
{
   switch( getconstr( ))
   {
   case type_prim:
      {
         // If the primitive type has size 0, and the name of the 
         // primitive type occurs in s, we replace it. 
         // Otherwise, we leave it unchanged. 

         const primtype& p = getprimtype( );
         if( p. size == 0 )
         {
            const treetype* t = s. type_lookup( p. name );
            if(t)
               return *t;
         }
         return *this;
      } 
   case type_void:
   case type_unknown:
      return *this;

   case type_const:
   case type_raw:
   case type_pointer:
   case type_ref:
      return treetype( getconstr( ), getsubtype( ). subst(s) ); 
         // Recursively substitute in the subtype.

   case type_reg:
   case type_array:
      return treetype( getconstr( ), getinteger( ), 
                                     getsubtype( ). subst(s) );

   case type_func:
      {
         treetype res = getresulttype( ). subst(s);
   
         std::list< treetype > args;
         for( unsigned int i = 0; i < getarity( ); ++ i )
            args. push_back( getargtype(i). subst(s) );

         return treetype( type_func, res, args );
      }
   }

   ASSERT( false );
}

namespace
{
   bool contains( const std::list< identifier > & varlist,
                  const identifier& id )
   { 
      for( std::list< identifier > :: const_iterator
              p = varlist. begin( );
              p != varlist. end( );
              ++ p )
      {
         if( *p == id )
            return true;
      }
      return false;
   }
}


bool treetype::match_into( const treetype& goaltype, substitution& s,
                           const std::list< identifier > & varlist ) const 
{

#if 0
   std::cout << "matching\n";
   std::cout << "      " << *this << "\n";
   std::cout << "into\n";
   std::cout << "      " << goaltype << "\n";
   std::cout << s << "\n";
#endif

   type_constr c = getconstr( );
   if( c == type_prim )
   {
      const primtype& p = getprimtype( );
      if( p. size == 0 && contains( varlist, p. name ))
      {

         // This means that *this should be treated like a variable.
         // If *this already has a value in s, then the value must be 
         // equal to goaltype. If we have no value, then we assign. 
       
         const treetype* t = s. type_lookup( p. name ); 
         if(t)
         {
            return (*t) == goaltype; 
         }
         else
         {
            s. type_assign( p. name, goaltype );
            return true;
         }
      }
   } 
   
   // *this is not a variable. 

   if( c != goaltype. getconstr( ))
      return false;

   // This can happen when *this is a primtype, but not a variable:

   if( c == type_prim && getprimtype( ) != goaltype. getprimtype( ))
      return false; 

   // It is not clear how integers should be matched. 
   // We match the integer 0 into every integer. 
   // If the integer is not zero, it must be equal. 

   if( c == type_reg || c == type_array )
   {
      unsigned int i = getinteger( );
      if( i != 0 && i != goaltype. getinteger( ))
         return false;
   }

   // Number of subtypes must be equal:

   if( repr -> subtypes. size( ) != goaltype. repr -> subtypes. size( ))
      return false;

   // We recursively match the subtypes: 

   std::list< treetype > :: const_iterator 
         p1 = repr -> subtypes. begin( );
   std::list< treetype > :: const_iterator 
         p2 = goaltype. repr -> subtypes. begin( ); 

   bool b = true; 
   while( p1 != repr -> subtypes. end( ) && b )
   {
      b = p1 -> match_into( *p2, s, varlist );
      ++ p1;
      ++ p2;
   }

   return b;
}

        
