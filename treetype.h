

#ifndef TREETYPE_INCLUDED
#define TREETYPE_INCLUDED   1


// Written by Hans de Nivelle, November 2011, for the course Compiler
// Construction. Changed the interface in December 2011. 


#include <list>
#include "primtype.h"

// The type constructors:

enum type_constr
{
   type_prim, 
   
   type_const, 
   type_raw,

   type_pointer,
   type_ref,

   type_reg, 
  
   type_void,
   type_unknown,
    
   type_array,
   type_func 
};


class treetype;
class substitution;

struct typenode
{
   unsigned int refcount;

   type_constr t;   
   unsigned int i; 
      // Only type_array and type_reg have an integer.

   std::list< treetype > subtypes;

   std::list< primtype > prim;
      // Only type_prim has a primitive type. 

   typenode( type_constr t )
      : refcount(1),
        t(t), 
        i(0)
   { }

};


// I know that 'type' is not a reserved word in C++, but I am still
// afraid to call the class 'type'. This is why it is called 'treetype',
// After all, it is the type of a tree. 

class treetype
{

   typenode* repr;


public:
   treetype( type_constr t );
      // Constructs void or unknown.

   treetype( type_constr t, const primtype& p );
      // Constructs a primitive type.

   treetype( type_constr t, const treetype& sub );
      // Constructs const(sub), raw(sub), pointer(sub), 
      // ref(sub), reg(0,sub), array(0,sub).

   treetype( type_constr t, unsigned int i, const treetype& sub );
      // Constructs reg( i, sub ) or array( i, sub ). 

   treetype( type_constr t, const treetype& res, 
                            const std::list< treetype > & args );
      // Constructs func( res; args[0], ..., args[n-1] ), where res is 
      // result type, and args is the list of argument types. 


   type_constr getconstr( ) const { return repr -> t; }

   const treetype& getsubtype( ) const; 
      // Get our subtype, when we are const,raw,pointer,ref,reg,
      // or array. 

   const primtype& getprimtype( ) const;
      // Gets the primitive type, which we only have if we are indeed 
      // a primitive type.

   unsigned int getinteger( ) const;
      // Gets the integer, only reg( i, sub ) and array( i, sub ) have one.  

   unsigned int getarity( ) const; 
      // Gets our arity, when we are a function. 

   const treetype& getargtype( unsigned int i ) const;
      // Gets the type of our i-th argument, when we are function, and 
      // 0 <= i < getarity( ). 

   const treetype& getresulttype( ) const;
      // Gets the type of our result, when are a function type.

   treetype( const treetype& t ); 
   void operator = ( const treetype& t );
   ~ treetype( ); 

   static int compare( const treetype& t1, const treetype& t2 ); 

   bool operator < ( const treetype& other ) const
      { return compare( *this, other ) == -1; }

   bool operator > ( const treetype& other ) const
      { return compare( *this, other ) == 1; }

   bool operator == ( const treetype& other ) const
      { return compare( *this, other ) == 0; }

   bool operator != ( const treetype& other ) const
      { return compare( *this, other ) != 0; }

   bool operator >= ( const treetype& other ) const
      { return compare( *this, other ) != -1; }

   bool operator <= ( const treetype& other ) const
      { return compare( *this, other ) != 1; }

   
   treetype subst( const substitution& ) const;
      // Instantiate by substitution. We replace primitive types
      // with size 0, whose identifiers occur in the substitution. 

   bool match_into( const treetype& goaltype, substitution& s, 
                    const std::list< identifier > & varlist ) const; 
      // Try to find a substitution s, s.t. this -> subst(s) == goaltype.
      // The substitution should be cleared (or freshly constructed) before
      // calling match_into.

};


std::ostream& operator << ( std::ostream& , const treetype& ); 

#endif

