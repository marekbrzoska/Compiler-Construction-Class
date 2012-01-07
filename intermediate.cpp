
#include "error.h"
#include "identifier.h"
#include "primtype.h"
#include "treetype.h" 
#include "tree.h"
#include "data.h"
#include "flowgraph.h"
#include "program.h" 
#include "runtimestate.h"
#include "substitution.h"
#include "varstore.h"
#include "typesystem.h"

#include "identifier.stat"


int main( int argc, char* argv [ ] )
{
   std::cout << "hallo\n";

   for( unsigned int i = 0; i < 1; ++ i )
   {
      primtype p1 = primtype( identifier( "int" ), 4 );
      primtype p2 = primtype( identifier( "double" ), 8 );

      treetype tt1 = treetype( type_prim, p1 );
      treetype tt2 = treetype( type_prim, p2 );

      std::list< treetype > args1;
      std::list< treetype > args2;

      args1. push_back( treetype( type_prim, p1 ));
      args1. push_back( treetype( type_void ));
      args1. push_back( tt1 );

      args2. push_back( treetype( type_prim, p1 ));
      args2. push_back( treetype( type_void ));

      std::cout << ( treetype( type_func, tt1, args1 ) == 
                     treetype( type_func, tt1, args1 ) ) << "\n";
 
      treetype g = treetype( type_func, tt2, args1 );
      std::cout << g. getargtype(2) << "\n";


      // Here starts code that you need in exercise 9. 

      // We build treetypes char/int/double:

      treetype tt_bool = treetype( type_prim, primtype( "bool", sizeof(bool)));
      treetype tt_char = treetype( type_prim, primtype( "char", sizeof(char))); 
      treetype tt_int = treetype( type_prim, primtype( "int", sizeof(int)));
      treetype tt_double = treetype( type_prim, 
                                     primtype( "double", sizeof( double )));

      tree pi = tree( "pi", 
                      tobinary( 3.141592653589793238462643383279 ),
                      tt_double );

      pi. prettyprint( std::cout );  
      
      tree e = tree( "e", 
                     tobinary( 2.718281828459045235360287471352 ),
                     tt_double );

      tree zero = tree( "zero", tobinary(0), tt_int );
      tree one = tree( "one", tobinary(1), tt_int );
      tree two = tree( "two", tobinary(2), tt_int );
      tree t = tree( "+", tree( "*", pi, zero ), two ); 
      t. prettyprint( std::cout );

      // We put a few function declarations in functionlist.
      // Can be used as example in task 2 and task 3: 

      typesystem ts; 
     
      std::list< treetype > argtypes;
      argtypes. push_back( tt_int );
      argtypes. push_back( tt_int );
 
      ts. instances [ identifier( "+" ) ]. push_back( 
              typesystem::concretefunc( identifier( "+int" ), 
              treetype( type_func, tt_int, argtypes )));

      argtypes. clear( );
      argtypes. push_back( tt_double );
      argtypes. push_back( tt_double );

      ts. instances [ identifier( "+" ) ]. push_back(
              typesystem::concretefunc( identifier( "+double" ),
              treetype( type_func, tt_double, argtypes )));

      argtypes. clear( );
      argtypes. push_back( treetype( type_pointer, 
                              treetype( type_prim, primtype( "X", 0 ))));
      argtypes. push_back( tt_int ); 

      ts. instances [ identifier( "+" ) ]. push_back(
              typesystem::concretefunc( identifier( "+pntr" ), 
                                        identifier( "X" ),
              treetype( type_func, treetype( type_pointer,
                              treetype( type_prim, primtype( "X", 0 ))),
                              argtypes )));
              
      std::cout << "Typesystem:\n\n";

      ts. convfunctions. push_back( typesystem::convfunc( "conv_int_double", 4,
                                              tt_int, tt_double )); 
      ts. convfunctions. push_back( typesystem::convfunc( "conv_char_int", 5,
                                              tt_char, tt_int )); 
      ts. convfunctions. push_back( typesystem::convfunc( "conv_char_double", 
                                              50,
                                              tt_char, tt_double ));
      ts. convfunctions. push_back( typesystem::convfunc( "array2pointer", 1,
                                              identifier( "X" ),
                                              treetype( type_ref, 
                                                 treetype( type_array, 0, 
                                                    treetype( type_prim, 
                                                       primtype( "X", 0 )))),
                                              treetype( type_pointer, 
                                                 treetype( type_prim,
                                                    primtype( "X", 0 )))));
      std::cout << ts << "\n";

      std::cout << ts. allconversions( 
                  tree( "x", tobinary(44), 
                    treetype( type_ref, 
                       treetype( type_array, tt_char ))));

      return 0;

      // We make our first program: 

      flowgraph fl;
      {
         flowgraph::iterator it = fl. begin( );
         instruction ins( instruction::move_pointer, 
                          instruction::argument( "x", tt2 ), 
                          instruction::argument( "x" ));

         fl. insert( ins, it, 0 );
         it = it. succ(0);

         ins = instruction( instruction::move_pointer,
                            instruction::argument( "y", tt2 ),
                            instruction::argument( "y" ));
         fl. insert( ins, it, 0 );
         it = it. succ(0);

         ins = instruction( instruction::move_double,
                            instruction::argument( tobinary( 33.0 )),
                            instruction::argument( "x", 0 ));

         fl. insert( ins, it, 0 );   
         it = it. succ(0);

         ins = instruction( instruction::move_double,
                            instruction::argument( tobinary( 12.0 )),
                            instruction::argument( "y", 0 )); 
   
         fl. insert( ins, it, 0 );
         it = it. succ(0);

         ins = instruction( instruction::mult_double,
                            instruction::argument( "x", 0 ),
                            instruction::argument( "y", 0 ),
                            instruction::argument( "z" )); 

         fl. insert( ins, it, 0 );
         it = it. succ(0); 

         ins = instruction( instruction::print_double,
                            instruction::argument( "z" ));
         fl. insert( ins, it, 0 );
         it = it. succ(0);

      }
   
      program pr;
      pr. insert( "main", treetype( type_void ), fl ); 
      pr. insert( "hallo", treetype( type_void ), fl ); 
      std::cout << pr << "\n";

      runtimestate rs( 256 );
      rs. tracing = true;
 
      try 
      {
         call( pr, "main", rs );
      }
      catch( const error& err )
      {
         std::cout << err << "\n";
      }
     
   }
   std::cout << "\n";
   identifier::printnormtable( std::cout );

}



