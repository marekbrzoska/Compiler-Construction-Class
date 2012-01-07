
#include <stdio.h>
#include "runtimestate.h"
#include "assert.h"
#include "data.h"


runtimestate::runtimestate( unsigned int size )
   : data( std::vector< char > ( size )),
     ssp( 0 ),
     sp( data. size( )),
     tracing( false )  
{
   ASSERT( size >= 16 );
      // Arbitrary border, but it should be more than four.
 
}


void runtimestate::push( unsigned int s )
{
   if( ssp + s > sp )
      throw error( "out of stack space" );

   while(s)
   {
      data [ -- sp ] = 0; 
      -- s;
   }
}


void runtimestate::pop( unsigned int s )
{
   if( sp + s > data. size( ))
      throw error( "pop below end of stack" );

   sp += s;
}


unsigned int runtimestate::findstatic( const identifier& name, unsigned int s )
{
   std::map< identifier, unsigned int > :: const_iterator 
      p = staticvars. find( name );
   
   if( p != staticvars. end( ))
      return p -> second; 

   // It has to be created:

   unsigned int res = ssp; 
   if( ssp + s > sp )
      throw error( "out of stack space during creation of static variable" );

   staticvars. insert( 
      std::pair< identifier, unsigned int > ( name, ssp ));
   
   for( unsigned int i = 0; i < s; ++ i )
      data [ ssp ++ ] = 0;

   return res; 
} 


flowgraph::const_iterator runtimestate::pc( ) const
{
   ASSERT( localstates. size( ));
   return localstates. back( ). position;
}


flowgraph::const_iterator& runtimestate::pc( ) 
{
   ASSERT( localstates. size( ));
   return localstates. back( ). position;
}


flowgraph::const_iterator runtimestate::currentend( ) const
{
   ASSERT( localstates. size( ));
   return localstates. back( ). end;
}


std::vector< char > & runtimestate::findregister( const identifier& id )
{
   ASSERT( localstates. size( ));
   return localstates. back( ). registers [ id ];
}


std::ostream& operator << ( std::ostream& stream, const runtimestate& rs )
{
   stream << "State:\n";
   stream << "   size of reserved memory = " << rs. data. size( ) << "\n\n";
 
   // We print the last 16 bytes of the static stack:

   unsigned int border = 0; 
   if( rs. ssp > 16 )
      border = rs. ssp - 16;  

   stream << "Static Stack:\n";
   hexprint( stream, rs. data, border, rs. ssp );
   stream << "\n";

   // We print the first 64 bytes of the run time stack:
 
   border = rs. sp + 64;
   if( border > rs. data. size( ))
      border = rs. data. size( );
   
   stream << "Local Stack:\n";
   hexprint( stream, rs. data, rs. sp, border );
   stream << "\n";

   stream << "Static Variables:\n";
   for( std::map< identifier, unsigned int > :: const_iterator
           p = rs. staticvars. begin( );
           p != rs. staticvars. end( );
           ++ p )
   {
      stream << "   " << ( p -> first ) << " = ";
      stream << tobinary( (int) ( p -> second )) << "\n";
   }
   stream << "\n";

   if( rs. localstates. size( ))
   {
      stream << "Function: " << rs. localstates. back( ). funcname << "\n";
      stream << "Position: " << rs. localstates. back( ). position << "\n";
      stream << "End:      " << rs. localstates. back( ). end << "\n";
      stream << "\n"; 

      stream << "Registers:\n";
      for( std::map< identifier, std::vector< char > > :: const_iterator
              p = rs. localstates. back( ). registers. begin( );
              p != rs. localstates. back( ). registers. end( );
              ++ p )
      {
         stream << "   " << ( p -> first ) << " = " << ( p -> second ) << "\n";
      }
      stream << "\n"; 
   }
 
   return stream;    
}


unsigned int getsizeof( const treetype& tt )
{
   switch( tt. getconstr( ))
   {
   case type_prim:
      return tt. getprimtype( ). size;

   case type_const:
   case type_raw:
      return getsizeof( tt. getsubtype( ));

   case type_pointer:
   case type_ref:
      return sizeof( unsigned int );
      
   case type_void:
      return 0;

   case type_array:
      return tt. getinteger( ) * getsizeof( tt. getsubtype( ));
   }

   // This is for reg, unknown and func, because they cannot
   // be created in memory: 

   error err;
   err. s << "cannot compute size of " << tt;
   throw err; 
}


std::vector< char > read( const program& prog, runtimestate& rs,
                          const instruction::argument& arg,
                          unsigned int reqsize )
{
   switch( arg. gettype( ))
   {
   case instruction::arg_imm:
      {
         if( arg. getimmediate( ). size( ) != reqsize )
         {
            error err;
            err. s << "size of immediate argument " << arg. getimmediate( );
            err. s << " does not fit to required size " << reqsize; 
            throw err; 
         }
         return arg. getimmediate( );
      }

   case instruction::arg_sizeof:
      {
         int s = getsizeof( arg. gettreetype( ));  
         if( reqsize != sizeof( int ))
         {
            error err;
            err. s << "size " << reqsize;
            err.s << " of data does not corresond to sizeof( )";
            throw err;
         } 
         return tobinary(s); 
      }

   case instruction::arg_reg:
      {
         const std::vector< char > & val =  
                       rs. findregister( arg. getidentifier( ));

         if( val. size( ) == 0 )
         {
            error err;
            err. s << "register " << arg. getidentifier( ) << " has no value";
            throw err;
         }

         if( val. size( ) != reqsize )
         {
            error err;
            err. s << "data " << val. size( ) << " not of right size";
            throw err;
         }

         return val; 
      } 

   case instruction::arg_ind:
      {
         const std::vector< char > & val = 
                              rs. findregister( arg. getidentifier( ));

         if( val. size( ) == 0 )
         {
            error err;
            err. s << "register " << arg. getidentifier( ) << " has no value";
            throw err;
         }

         if( val. size( ) != sizeof( int ))
         {
            error err;
            err. s << "value " << val << " of register ";
            err. s << arg. getidentifier( ) << " is not pointer";
            throw err;
         } 

         int addr;
         std::vector< char > :: const_iterator q = val. begin( );
         q >> addr;
         if( addr + reqsize > rs. data. size( ))
         {
            error err;
            err. s << "address " << addr << "is out of range";
            throw err;
         }
         std::vector< char > res = std::vector< char > ( reqsize );  
         for( unsigned int i = 0; i < reqsize; ++ i )
            res [i] = rs. data [ addr + i ];
         return res; 
      } 

   case instruction::arg_sp:
         return tobinary( (int) rs. sp ); 

   case instruction::arg_static:
      {
         if( reqsize != sizeof( int ))
         {
            error err;
            std::cout << 
               "type of instruction does not correspond to pointer";
            throw err;
         }

         int addr = 0;
         if( arg. gettreetype( ). getconstr( ) == type_func )
         {
            if( ! prog. isfunction( arg. getidentifier( )))
            {
               error err;
               err. s << "could not find function " << arg. getidentifier( );
               throw err;
            }

            addr = prog. getaddress( arg. getidentifier( ));    
         }
         else
         {
            addr = rs. findstatic( arg. getidentifier( ), 
                                   getsizeof( arg.  gettreetype( )));
         }

         return tobinary( addr );
      }         
   }
   error err;
   err. s << "cannot read from argument " << arg; 
   throw err; 
}


bool readbool( const program& prog, runtimestate& rs, 
               const instruction::argument& arg )
{
   std::vector< char > res = read( prog, rs, arg, sizeof( bool ));
   std::vector< char > :: const_iterator p = res. begin( );
   bool b;
   p >> b;
   return b;
}


char readchar( const program& prog, runtimestate& rs, 
               const instruction::argument& arg )
{
   std::vector< char > res = read( prog, rs, arg, sizeof( char ));
   std::vector< char > :: const_iterator p = res. begin( );
   char c;
   p >> c;
   return c;
}


int readint( const program& prog, runtimestate& rs, 
             const instruction::argument& arg )
{
   std::vector< char > res = read( prog, rs, arg, sizeof( int ));
   std::vector< char > :: const_iterator p = res. begin( );
   int i;
   p >> i;
   return i;
}


double readdouble( const program& prog, runtimestate& rs, 
                   const instruction::argument& arg )
{
   std::vector< char > res = read( prog, rs, arg, sizeof( double ));
   std::vector< char > :: const_iterator p = res. begin( );
   double d;
   p >> d;
   return d;
}



void write( const program& prog, runtimestate& rs, 
            const std::vector< char > & value,
            const instruction::argument& arg )
{
   switch( arg. gettype( ))
   {
   case instruction::arg_sp:
      {
         if( value. size( ) != sizeof( int ))
         {
            error err;
            err. s << "cannot move " << value 
                   << " into SP (size does not fit) ";
            throw err;
         } 
         int i;
         std::vector< char > :: const_iterator p = value. begin( ); 
         p >> i;
         if( i > rs. data. size( ))
         {
            error err;
            err. s << "value " << i << " being read into SP is out of range"; 
            throw err;
         }
         rs. sp = i;
         return;
      } 
      
   case instruction::arg_reg:
      rs. findregister( arg. getidentifier( )) = value;
      return; 

   case instruction::arg_ind:
      {
         const std::vector< char > & binaddr = 
                       rs. findregister( arg. getidentifier( ));

         if( binaddr. size( ) == 0 )
         {
            error err;
            err. s << "in indirect register write, @" << arg. getidentifier( );
            err. s << " has no value"; 
            throw err;
         }

         if( binaddr. size( ) != sizeof( unsigned int ))
         {
            error err;
            err. s << "in indirect register write, " << arg. getidentifier( );
            err. s << " is not a pointer"; 
            throw err;
         }

         int x;
         std::vector< char > :: const_iterator q = binaddr. begin( ); 
         q >> x;
         unsigned int addr = x;

         if( addr + value. size( ) > rs. data. size( ))
         {
            error err;
            err. s << "address " << addr << "is out of range";
            throw err;
         }
 
         for( unsigned int i = 0; i < value. size( ); ++ i )
            rs. data [ addr + i ] = value [i];
         return;
      } 
   }

   error err;
   err. s << "argument " << arg << " is not writable";
   throw err; 
}


void call( const program& prog, const identifier& start, runtimestate& rs )
throw( error ) 
{
   if( ! prog. isfunction( start ))
   {
      error err;
      err. s << "tried to call unknown function " << start;
      throw err;
   }

   unsigned int ad = prog. getaddress( start );

   rs. localstates. push_back( 
         runtimestate::localstate( start, prog. getfunctiongraph( ad )));

   if( rs. pc( ). nrsuccessors( ) != 1 )
   {
      error err;
      err. s << "function " << start;
      err. s << " does not have a unique starting point";
      throw err;
   } 

   // Push fake return address on local stack:

   rs. data [ -- rs. sp ] = -1;
   rs. data [ -- rs. sp ] = -1;
   rs. data [ -- rs. sp ] = -1;
   rs. data [ -- rs. sp ] = -1; 
 
   rs. pc( ) = rs. pc( ). succ(0);
      // Move pc into 0-th successor.

   while( true ) 
   {
      if( rs. tracing )
      {
         std::cout << "------------------------------------------------\n";
         std::cout << rs << "\n";
         if( rs. pc( ) != rs. currentend( ))
         {
            std::cout << "instruction:  " << * rs. pc( ) << "\n";
         }
         char c = getchar( );
         if( c == 'q' || c == 'Q' )  
            throw error( "user quit" );
      }

      
      if( rs. pc( ) == rs. currentend( ))
      {
         std::vector< char > :: const_iterator p = rs. data. begin( ) + rs. sp;
         int ret;
         p >> ret;
         if( rs. localstates. size( ) > 1 && 
             ret != rs. localstates. size( ) - 2 )
            throw error( "return address messed up" );
         if( rs. localstates. size( ) == 1 && ret != -1 )
            throw error( "return address messed up" ); 

         rs. sp = p - rs. data. begin( );
         return; 
      }

      unsigned int stepinto = 0;
         // By default, we step into the 0-th successor.

      const instruction& ins = * rs. pc( ); 
      switch( ins. getopcode( ))
      {
      case instruction::move_bool:
         write( prog, rs, read( prog, rs, ins [0], sizeof( bool )), ins [1] );
         break;

      case instruction::move_char:
         write( prog, rs, read( prog, rs, ins [0], sizeof( char )), ins [1] );
         break;

      case instruction::move_int:
         write( prog, rs, read( prog, rs, ins [0], sizeof( int )), ins [1] );
         break;

      case instruction::move_double:
         write( prog, rs, read( prog, rs, ins [0], sizeof( double )), ins [1] );
         break;

      case instruction::move_pointer:
         write( prog, rs, read( prog, rs, ins [0], sizeof( int )), ins [1] ); 
         break;

      case instruction::mult_int:
         write( prog, rs, tobinary( readint( prog, rs, ins [0] ) *
                                    readint( prog, rs, ins [1] )), ins [2] ); 
         break;
 
      case instruction::mult_double:  
         write( prog, rs, tobinary( readdouble( prog, rs, ins [0] ) * 
                                    readdouble( prog, rs, ins [1] )), ins [2] );
         break;

      case instruction::print_bool:
         std::cout << readbool( prog, rs, ins [0] );
         break;

      case instruction::print_char:
         std::cout << readchar( prog, rs, ins [0] );
         break;

      case instruction::print_int:
         std::cout << readint( prog, rs, ins [0] );
         break;

      case instruction::print_double:
         std::cout << readdouble( prog, rs, ins [0] );
         break;

      case instruction::print_pointer:
         std::cout << read( prog, rs, ins [0], sizeof( int )); 
      default:
         error err;
         err. s << "unknown instruction " << ins;
         throw err; 
      }

      rs. pc( ) = rs. pc( ). succ( stepinto ); 
   }

   ASSERT( false );
}


