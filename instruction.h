
// Written by Hans de Nivelle for class Compiler Construction, December 2011.

#ifndef INSTRUCTION_INCLUDED
#define INSTRUCTION_INCLUDED  1


#include <iostream>
#include <list>
#include <vector> 
#include "identifier.h"
#include "treetype.h"


struct instruction
{

   // Types of arguments of instructions: 
   
   enum argtype
   {
      arg_imm,     // Immediate data. Immediate data is always represented
                   // by a vector of characters.
      arg_sizeof,  // sizeof(T), where T is some type. It is also a 
                   // constant. 
      arg_reg,     // A register, which is represented by an identifier.
                   // We do not distinguish pointer registers from
                   // other registers anymore, contrary to the slides. 
      arg_ind,     // An indirect register, represented by an identifier.
      arg_sp,      // The stack pointer.
      arg_static   // Name of a static variable or a function.
                   // If the type is not a function type, then the 
                   // static variable is created on the static stack,
                   // when it occurs for the first time. 
   };


   class argument
   {
      argtype t;
      std::vector< char > imm;     // Used by immediate.
      std::list< identifier > id;  // For reg(A), ind(A), and static(A,T).
      std::list< treetype > stype; // For sizeof(T), and static(A,T).

   public: 
      // Construct immediate argument:

      argument( const std::vector< char > & d );

      // Construct an argument that reads the local stack pointer 'SP'.

      argument( );

      // Construct register argument:

      argument( const identifier& id ); 

      // Construct sizeof(tt):

      argument( const treetype& tt ); 

      // Construct indirect register reference [ id ]. 
      // The int argument is ignored. 

      argument( const identifier& id, int );

      // Construct argument that refers to static variable: 

      argument( const identifier& id, const treetype& tt ); 

      argtype gettype( ) const; 

      const std::vector< char > & getimmediate( ) const;
      const identifier& getidentifier( ) const;
      const treetype& gettreetype( ) const; 
   };


   // Only a few instructions are currently implemented.
   // I will add more if the exercises will get so far.

   enum opcode
   {
      move_bool,
      move_char, 
      move_int,
      move_double,
      move_pointer,
         // We do not worry anymore about the type of the pointer 
         // at this point. 

      add_bool,
      add_char, 
      add_int,
      add_double,

      sub_bool,
      sub_char,
      sub_int,
      sub_double,

      add_pointer_int,
      sub_pointer_int,

      mult_bool,
      mult_char,
      mult_int,
      mult_double,

      div_bool,
      div_char,
      div_int,
      div_double,

      cmp_bool,
      cmp_char,
      cmp_int,
      cmp_double,
         // Will be -1 on <,  0 on == and +1 on >.
         // After that, one can branch on the outcome.

      conv_bool_char,
      conv_char_bool,
      conv_char_int,
      conv_int_char,
      conv_int_double,
      conv_double_int,
         // Some are lossy, some are not. Other conversions
         // can be obtained by chaining.

      push_int,
      pop_int, 
         // Add or subtract from stack pointer.
         // Int should be positive.

      print_bool,
      print_char,
      print_int,
      print_double,
      print_pointer,

      phi_bool,
      phi_char,
      phi_int,
      phi_double,
      phi_pointer,
         // The famous phi-functions of the single static assignment form. 

      branch_bool,
      branch_char,
      branch_int,
      branch_double,

      call, 
      nop,
         // Does nothing.
      show,
         // Show state of system to user. 
      clear,
         // Clears all registers.
      exit
         // Exists.
   };

private:
   opcode c;
   std::list< argument > args;

public: 
   instruction( opcode c )
      : c(c)
   { }

   instruction( opcode c, const argument& a1 )
      : c(c)
   {
      args. push_back( a1 );
   }

   instruction( opcode c, const argument& a1, const argument& a2 )
      : c(c)
   {
      args. push_back( a1 );
      args. push_back( a2 );
   }

   instruction( opcode c, const argument& a1, const argument& a2,
                          const argument& a3 )
      : c(c)
   {
      args. push_back( a1 );
      args. push_back( a2 );
      args. push_back( a3 );
   }

   const argument& operator [ ] ( unsigned int i ) const;
   opcode getopcode( ) const { return c; }  
};


std::ostream& operator << ( std::ostream& stream, 
                            const instruction::argument& a ); 

std::ostream& operator << ( std::ostream& stream, const instruction& ins );


#endif


