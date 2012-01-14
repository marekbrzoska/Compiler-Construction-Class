
// Code produced by Maphoon 2008.
// Declaration of parser:


#ifndef PARSER_INCLUDED
#define PARSER_INCLUDED     1


#include <list>
#include <iostream>


#include "token.h"
#include "tokenizer.h"



void parser( tokenizer& r,
      varstore & symboltable,
      tokentype start,
      unsigned int recoverlength );
         // In case of success, the parser returns a
         // start token, possibly followed by a 
         // lookahead token. Otherwise, it returns  
         // some other token. 




#endif


