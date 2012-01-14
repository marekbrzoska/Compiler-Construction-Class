
%token EOF SCANERROR
%token COLON SEMICOLON 
%token IDENTIFIER NUMBER 
%token PLUS TIMES MINUS DIVIDES
%token EQUAL NOTEQUAL LESSTHAN GREATERTHAN LESSEQUALTHAN GREATEREQUALTHAN
%token AND OR NOT 
%token LPAR RPAR
%token LSQPAR RSQPAR
%token ADDRESSOF DOT ARROW
%token PLUSPLUS MINUSMINUS
%token ASSIGN

// Non-terminal symbols:

%token B1 B2 B3 B4 
%token A1
%token N1 N2 N3 
%token E1 E2 

%startsymbol B1 SEMICOLON

%attribute tt            tree
%attribute id            identifier 
%attribute reason        std::string

%constraint B1 tt 1 2
%constraint B2 tt 1 2 
%constraint B3 tt 1 2 
%constraint B4 tt 1 2
%constraint A1 tt 1 2
%constraint N1 tt 1 2
%constraint N2 tt 1 2
%constraint N3 tt 1 2
%constraint E1 tt 1 2
%constraint E2 tt 1 2
%constraint IDENTIFIER id 1 2
%constraint NUMBER tt 1 2
%constraint SCANERROR reason 0 2 

%intokenheader #include "identifier.h"
%intokenheader #include "tree.h" 
%intokenheader #include "assert.h"

%global symboltable varstore

#include "varstore.h"

% B1 : B2 

        B21 -> type = tkn_B1;
        return B21;
 
%    | B1 OR B2	

        token t = tkn_B1;
        t. tt. push_back( tree( "||", B11 -> tt. front( ), 
                                      B23 -> tt. front( )));
        return t;

%         ;

%  B2  : B3 
   
        B31 -> type = tkn_B2;
        return B31;

%      | B2 AND B3 

          token t = tkn_B2;
          t. tt. push_back( tree( "&&", B21 -> tt. front( ),
                                        B33 -> tt. front( )));
          return t; 

%      ; 

% B3   : B4 

          B41 -> type = tkn_B3;
          return B41;

%      | B4 LESSTHAN B4

          token t = tkn_B3;
          t. tt. push_back( tree( "<", B41 -> tt. front( ),
                                       B43 -> tt. front( )));
          return t; 

%      | B4 LESSEQUALTHAN B4

          token t = tkn_B3;
          t. tt. push_back( tree( "<=", B41 -> tt. front( ),
                                        B43 -> tt. front( )));
          return t; 

%      | B4 GREATERTHAN B4 

          token t = tkn_B3;
          t. tt. push_back( tree( ">", B41 -> tt. front( ),
                                       B43 -> tt. front( )));
          return t; 

%      | B4 GREATEREQUALTHAN B4 

          token t = tkn_B3;
          t. tt. push_back( tree( ">=", B41 -> tt. front( ),
                                        B43 -> tt. front( )));
          return t; 

%      | B4 EQUAL B4

          token t = tkn_B3;
          t. tt. push_back( tree( "==", B41 -> tt. front( ),
                                        B43 -> tt. front( )));
          return t; 

%      | B4 NOTEQUAL B4

          token t = tkn_B4;
          t. tt. push_back( tree( "!=", B41 -> tt. front( ),
                                        B43 -> tt. front( )));
          return t; 

%      ;  

% B4   : A1 

       A11 -> type = tkn_B4;
       return A11;

%      | NOT B4

       token t = tkn_B4;
       t. tt. push_back( tree( "!", B42 -> tt. front( )));  
       return t; 

%      ;

% A1   : N1 

       N11 -> type = tkn_A1;
       return N11;

%      | N1 ASSIGN N1

       token t = tkn_A1;
       t. tt. push_back( tree( "=", N11 -> tt. front( ),
                                    N13 -> tt. front( )));
       return t;

%      ;

% N1   : N2 

       N21 -> type = tkn_N1;
       return N21;

%      | N1 PLUS N2 

       token t = tkn_N1;
       t. tt. push_back( tree( "+", N11 -> tt. front( ),
                                    N23 -> tt. front( )));
       return t;

%      | N1 MINUS N2

       token t = tkn_N1;
       t. tt. push_back( tree( "-", N11 -> tt. front( ),
                                    N23 -> tt. front( )));
       return t;

%      ; 

% N2   : N3

       N31 -> type = tkn_N2;
       return N31;

%      | N2 TIMES N3

       token t = tkn_N2;
       t. tt. push_back( tree( "*", N21 -> tt. front( ),
                                    N33 -> tt. front( )));
       return t;

%      | N2 DIVIDES N3

       token t = tkn_N2;
       t. tt. push_back( tree( "/", N21 -> tt. front( ),
                                    N33 -> tt. front( )));
       return t;

%      ;

% N3   : E1 

       E11 -> type = tkn_N3;
       return E11;

%      | MINUS N3

       token t = tkn_N3;
       t. tt. push_back( tree( "unary-", N32 -> tt. front( )));
       return t;

%      ;

%  E1  : E2 
    
        E21 -> type = tkn_E1;
        return E21;

%      | ADDRESSOF E1

       token t = tkn_E1;
       t. tt. push_back( tree( "unary&", E12 -> tt. front( )));
       return t;

%      | TIMES E1 
       
       token t = tkn_E1;
       t. tt. push_back( tree( "unary*", E12 -> tt. front( )));
       return t;

%      | PLUSPLUS E1

       token t = tkn_E1;
       t. tt. push_back( tree( "prefix++", E12 -> tt. front( )));
       return t;

%      | MINUSMINUS E1

       token t = tkn_E1; 
       t. tt. push_back( tree( "prefix--", E12 -> tt. front( )));
       return t;

%      ;

% E2   : E2 DOT IDENTIFIER
 
       token t = tkn_E2;
       t. tt. push_back( 
                 tree( identifier( "field" ) + IDENTIFIER3 -> id. front( ),
                       E21 -> tt. front( )));
       return t;

%      | E2 ARROW IDENTIFIER

       // It is an abbreviation for (*e). id 

       token t = tkn_E2;
       t. tt. push_back( 
                 tree( identifier( "field" ) + IDENTIFIER3 -> id. front( ),
                       tree( "unary*", E21 -> tt. front( ))));
       return t;

%      | E2 LSQPAR B1 RSQPAR 

       // It is an abbreviation for *( E2 + B1 )

       token t = tkn_E2;
       t. tt. push_back( tree( "unary*", 
                            tree( "+", E21 -> tt. front( ),
                                       B13 -> tt. front( ))));
       return t; 

%      | E2 PLUSPLUS
      
       token t = tkn_E2;  
       t. tt. push_back( tree( "postfix++", E21 -> tt. front( )));
       return t;

%      | E2 MINUSMINUS

       token t = tkn_E2;
       t. tt. push_back( tree( "postfix--", E21 -> tt. front( )));
       return t;

%      | NUMBER 

       NUMBER1 -> type = tkn_E2; 
       return NUMBER1;

%      | IDENTIFIER

       token t = tkn_E2;
       t. tt. push_back( tree( IDENTIFIER1 -> id. front( ),
                         std::vector< char > ( ))); 
      
       const varstore::variable* 
       var = symboltable. lookup( IDENTIFIER1 -> id. front( ));

       if( var ) 
          t. tt. back( ). tt = treetype( type_ref, var -> tt );
             // Type of a variable is always ref.
 
       // Otherwise, type remains unknown.
       return t; 

%      | LPAR B1 RPAR

       B12 -> type = tkn_E2;
       return B12;

%      ;

