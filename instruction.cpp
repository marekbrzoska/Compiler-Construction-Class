

#include "instruction.h"
#include "data.h"

// Many, many argument constructors:

instruction::argument::argument( )
   : t( arg_sp )
{ 
}

instruction::argument::argument( const std::vector< char > & d )
   : t( arg_imm )
{
   imm = d; 
}

instruction::argument::argument( const treetype& tt )
   : t( arg_sizeof ) 
{
   stype. push_back( tt );
}

instruction::argument::argument( const identifier& id )
   : t( arg_reg )
{
   ( this -> id ). push_back( id );
}

instruction::argument::argument( const identifier& id, int i )
   : t( arg_ind )
{
   ( this -> id ). push_back( id );
}

instruction::argument::argument( const identifier& id, const treetype& tt )
   : t( arg_static )
{
   ( this -> id ). push_back( id );
   stype. push_back( tt );
}


const std::vector< char > & instruction::argument::getimmediate( ) const
{
   ASSERT( t == arg_imm );
   return imm;
}

const identifier& instruction::argument::getidentifier( ) const
{
   ASSERT( t == arg_reg || t == arg_ind || t == arg_static );
   return id. front( );
}

const treetype& instruction::argument::gettreetype( ) const
{
   ASSERT( t == arg_sizeof || t == arg_static );
   return stype. front( );
}

instruction::argtype instruction::argument::gettype( ) const
{
   return t;
}

std::ostream& operator << ( std::ostream& stream,
                            const instruction::argument& a )
{
   switch( a. gettype( ))
   {
   case instruction::arg_imm:
      stream << "#" << a. getimmediate( ); 
      break;

   case instruction::arg_sizeof:
      stream << "sizeof( " << a. gettreetype( ) << " )";
      break; 
  
   case instruction::arg_reg:
      stream << "@" << a. getidentifier( );
      break; 

   case instruction::arg_ind:
      stream << "[ @" << a. getidentifier( ) << " ]";
      break; 

   case instruction::arg_sp:
      stream << "@SP";
      break;

   case instruction::arg_static:
      stream << "static " << a. getidentifier( ) << " : " << a. gettreetype( ); 
      break;

   default:
      std::cout << "unknown argument type!\n";
      ASSERT( false );
   }
   return stream; 
}


const instruction::argument& instruction::operator [] ( unsigned int i ) const
{
   ASSERT( i < args. size( ));
   std::list< argument > :: const_iterator p = args. begin( );
   while(i)
   {
      -- i;
      ++ p;
   }
   return *p;
}


std::ostream& operator << ( std::ostream& stream, const instruction& ins )
{
   switch( ins. getopcode( ))
   {
   case instruction::nop:
      stream << "nop";
      
      break; 

   case instruction::move_bool:
      stream << "move_bool ";
      stream << ins[0] << ", " << ins[1];
      break;

   case instruction::move_char:
      stream << "move_char ";
      stream << ins[0] << ", " << ins[1];
      break;

   case instruction::move_int:
      stream << "move_int ";
      stream << ins[0] << ", " << ins[1];
      break;   

   case instruction::move_double:
      stream << "move_double ";
      stream << ins[0] << ", " << ins[1];
      break;

   case instruction::add_bool:
      stream << "add_bool ";
      stream << ins[0] << ", " << ins[1];
      break;

   case instruction::add_char:
      stream << "add_char ";
      stream << ins[0] << ", " << ins[1];
      break;

   case instruction::add_int:
      stream << "add_int ";
      stream << ins[0] << ", " << ins[1];
      break;

   case instruction::add_double:
      stream << "add_double ";
      stream << ins[0] << ", " << ins[1];
      break;

   case instruction::move_pointer:
      stream << "move_pointer ";
      stream << ins[0] << ", " << ins[1];
      break;

   case instruction::mult_int:
      stream << "mult_int ";
      stream << ins[0] << ", ";
      stream << ins[1] << ", ";
      stream << ins[2]; 
      break;

   case instruction::mult_double:
      stream << "mult_double ";
      stream << ins[0] << ", "; 
      stream << ins[1] << ", ";
      stream << ins[2]; 
      break;

   case instruction::print_bool:
      stream << "print_bool ";
      stream << ins [0];
      break;

   case instruction::print_char:
      stream << "print_char ";
      stream << ins [0];
      break;

   case instruction::print_int:
      stream << "print_int ";
      stream << ins [0];
      break;

   case instruction::print_double:
      stream << "print_double ";
      stream << ins [0];
      break;

   case instruction::print_pointer:
      stream << "print_pointer ";
      stream << ins [0];
      break;

   default:
      stream << "unknown opcode " << ins. getopcode( ) << "\n";
      ASSERT( false ); 
   }

   
   return stream;
}


