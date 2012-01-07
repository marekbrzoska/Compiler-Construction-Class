

#ifndef RUNTIMESTATE_INCLUDED
#define RUNTIMESTATE_INCLUDED   1


#include <vector>
#include <map>
#include "program.h" 
#include "error.h"


struct runtimestate
{

   std::vector< char > data; 
 
   unsigned int ssp;  
      // Static stack pointer. Grows from 0 upwards.

   unsigned int sp; 
      // Dynamic stack pointer. Grows from data. size( ) downwards.

   std::map< identifier, unsigned int > staticvars;
      // Values of static variables. They are indices of 
      // data. 

   bool tracing; // True if we are tracing.

   struct localstate 
   {
      identifier funcname; 
      flowgraph::const_iterator position;
      flowgraph::const_iterator end; // of the current flowgraph.

      std::map< identifier, std::vector< char > > registers;
         // Values of the registers. The registers are local
         // within a function call. 

      localstate( const identifier& funcname, const flowgraph& fl )
         : funcname( funcname ),
           position( fl. begin( )),
           end( fl. end( ))
      { } 
   }; 

   std::vector< localstate > localstates; 

   explicit runtimestate( unsigned int size ); 

   void push( unsigned int s );
   void pop( unsigned int s );
      // On/from dynamic stack.

   unsigned int findstatic( const identifier& name, unsigned int s );
      // Return the address of static variable <name> when it exists.
      // If it doesn't exist, then create it. 

   flowgraph::const_iterator pc( ) const;
   flowgraph::const_iterator& pc( ); 
   flowgraph::const_iterator currentend( ) const; 
      // Position in current flowgraph and the end of the current flowgraph.
 
   std::vector< char > & findregister( const identifier& id );
      // Looks up the value of a register in the top local state.
      // If there is no value, it creates a vector of length 0.

};


unsigned int getsizeof( const treetype& tt );
   // Compute size of tt, and throw an exception if tt has no 
   // size. 


// Read the value specified by arg.  
// The reason why rs is not const is the fact that a static
// variable reference may create the static variable.
// reqsize is the required size of the argument.
// We throw an exception when the actual size is different.

std::vector< char > read( const program& prog, runtimestate& rs, 
                          const instruction::argument& arg,
                          unsigned int reqsize );

bool readbool( const program& prog, runtimestate& rs, 
               const instruction::argument& arg );

char readchar( const program& prog, runtimestate& rs, 
               const instruction::argument& arg );

int readint( const program& prog, runtimestate& rs, 
             const instruction::argument& arg );

double readdouble( const program& prog, runtimestate& rs, 
                   const instruction::argument& arg );

// There is no readpointer, because pointer is represented by int.


void write( const program& prog, runtimestate& rs, 
            const std::vector< char > & value,
            const instruction::argument& arg );
   // Write value into the position specified by arg.

void call( const program& prog, const identifier& start,
           runtimestate& rs ) throw ( error );


std::ostream& operator << ( std::ostream& , const runtimestate& ); 

#endif


