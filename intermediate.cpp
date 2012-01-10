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
#include <set>
#include <list>
#include <vector>
#include <iostream>
#include <typeinfo>

#include "identifier.stat"


using namespace std;


// Prints list of trees. This may be useful:
ostream& operator<< ( ostream& stream, const list<tree> & treelist )
{
   for( list<tree>::const_iterator p = treelist. begin(); p != treelist. end( ); ++p )
   {
      stream << "   " << *p << "\n";
   }
   stream << "\n";
   return stream;
}


list< tree > functionlist()
{
    // Marek: exrc 2
    list< tree > flist;
    list< treetype > argtypes;


    vector< char > attr;// = vector< char > :: vector( sizeof( int ), 0 );


    treetype tt_int = treetype( type_prim, primtype( "int", sizeof(int)));
    argtypes. push_back( tt_int );
    argtypes. push_back( tt_int );
    attr. clear( );
    flist. push_back( tree::tree( "+", attr, treetype::treetype( type_func, tt_int, argtypes )));


    treetype tt_double = treetype( type_prim, primtype( "double", sizeof( double )));
    argtypes. clear( );
    argtypes. push_back( tt_double );
    argtypes. push_back( tt_double );
    flist. push_back( tree::tree( "+", attr, treetype::treetype( type_func, tt_double, argtypes )));


    ////treetype tt_char = treetype( type_prim, primtype( "char", sizeof(char)));
      //ts. convfunctions. push_back( typesystem::convfunc( "conv_char_int", 5,
      //                                        tt_char, tt_int )); 
      //ts. convfunctions. push_back( typesystem::convfunc( "conv_char_double", 
      //                                        50,
      //                                        tt_char, tt_double ));

    return flist;
}

list< tree > fewconversions()
{
    // Marek: exrc 3
    list< tree > clist;
    list< treetype > argtypes;


    vector< char > attr;


    treetype tt_int = treetype( type_prim, primtype( "int", sizeof(int)));
    treetype tt_bool = treetype( type_prim, primtype( "bool", sizeof(bool)));
    treetype tt_double = treetype( type_prim, primtype( "double", sizeof( double )));

    argtypes. push_back( tt_int );
    attr. clear( );
    clist. push_back( tree::tree( "conv_int_double", attr, treetype::treetype( type_func, tt_double, argtypes )));


    argtypes. clear( );
    argtypes. push_back( tt_double );
    clist. push_back( tree::tree( "conv_double_int", attr, treetype::treetype( type_func, tt_int, argtypes )));

    argtypes. clear( );
    argtypes. push_back( tt_bool );
    clist. push_back( tree::tree( "conv_bool_int", attr, treetype::treetype( type_func, tt_int, argtypes )));


    ////treetype tt_char = treetype( type_prim, primtype( "char", sizeof(char)));

    return clist;
}

list<tree> possibleconversions(const list< tree > & conversionlist, tree t) {
   // Marek: exrc 4
   bool needtocontinue;

   set<tree> possibleconvs;
   set<treetype> possibletypes;
   pair<set<treetype>::iterator, bool> insertresult;

   list<tree>::const_iterator conversion;
   possibletypes.insert(t.tt);
   do {
      needtocontinue = false;
      for (conversion = conversionlist.begin(); conversion != conversionlist.end(); ++conversion) {
         if ( possibletypes.count(conversion->tt.getargtype(0)) ) {
            needtocontinue |= possibletypes.insert(conversion->tt.getresulttype()).second;
            possibleconvs.insert(*conversion);
         }
      }
   } while ( needtocontinue );

   list<tree> result;
   for (set<tree>::iterator conv = possibleconvs.begin(); conv != possibleconvs.end(); ++conv) {
      result.push_back(*conv);
   }

   return result;
}




int main( int argc, char* argv [ ] )
{
   treetype tt_int = treetype( type_prim, primtype( "int", sizeof(int)));
   treetype tt_bool = treetype( type_prim, primtype( "bool", sizeof(bool)));
   tree zero = tree( "zero", tobinary(0), tt_int );
   tree fls = tree( "false", tobinary(false), tt_bool );
   cout << 
       "fewconversions:\n" << fewconversions() << endl << 
       "zero:\n" << zero << endl << endl;
   cout << "possible conversions for zero:" << endl;
   cout << possibleconversions(fewconversions(), zero) << "\n";
   cout << "possible conversions for false:" << endl;
   cout << possibleconversions(fewconversions(), fls) << "\n";
   return 0;

   for( unsigned int i = 0; i < 1; ++ i )
   {
      primtype p1 = primtype( identifier( "int" ), 4 );
      primtype p2 = primtype( identifier( "double" ), 8 );

      treetype tt1 = treetype( type_prim, p1 );
      treetype tt2 = treetype( type_prim, p2 );

      list< treetype > args1;
      list< treetype > args2;

      args1. push_back( treetype( type_prim, p1 ));
      args1. push_back( treetype( type_void ));
      args1. push_back( tt1 );

      args2. push_back( treetype( type_prim, p1 ));
      args2. push_back( treetype( type_void ));

      cout << ( treetype( type_func, tt1, args1 ) == 
                     treetype( type_func, tt1, args1 ) ) << "\n";
 
      treetype g = treetype( type_func, tt2, args1 );
      cout << g. getargtype(2) << "\n";


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

      pi. prettyprint( cout );  
      
      tree e = tree( "e", 
                     tobinary( 2.718281828459045235360287471352 ),
                     tt_double );

      tree zero = tree( "zero", tobinary(0), tt_int );
      tree one = tree( "one", tobinary(1), tt_int );
      tree two = tree( "two", tobinary(2), tt_int );
      tree t = tree( "+", tree( "*", pi, zero ), two ); 
      t. prettyprint( cout );

      // We put a few function declarations in functionlist.
      // Can be used as example in task 2 and task 3: 

      typesystem ts; 
     
      list< treetype > argtypes;
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
              
      cout << "Typesystem:\n\n";

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
      cout << ts << "\n";

      cout << ts. allconversions( 
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
      cout << pr << "\n";

      runtimestate rs( 256 );
      rs. tracing = true;
 
      try 
      {
         call( pr, "main", rs );
      }
      catch( const error& err )
      {
         cout << err << "\n";
      }
     
   }
   cout << "\n";
   identifier::printnormtable( cout );




}



