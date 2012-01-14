
# Automatically generated Makefile
# Makefile-generator programmed by Hans de Nivelle, 2002


Flags = -Wreturn-type -pedantic -pedantic-errors -Wundef
CPP = g++-4.4


intermediate : Makefile   intermediate.o identifier.o treetype.o tree.o substitution.o data.o instruction.o flowgraph.o program.o runtimestate.o varstore.o typesystem.o token.o tokenizer.o parser.o 
	$(CPP) $(Flags) -o intermediate   intermediate.o identifier.o treetype.o tree.o substitution.o data.o instruction.o flowgraph.o program.o runtimestate.o varstore.o typesystem.o token.o tokenizer.o parser.o 

intermediate.o : Makefile   intermediate.cpp   identifier.stat identifier.h assert.h parser.h token.h tree.h treetype.h primtype.h tokenizer.h reader.h typesystem.h varstore.h substitution.h runtimestate.h program.h flowgraph.h instruction.h error.h data.h 
	$(CPP) -c $(Flags) intermediate.cpp -o  intermediate.o


identifier.o : Makefile   identifier.cpp   assert.h identifier.h 
	$(CPP) -c $(Flags) identifier.cpp -o  identifier.o


treetype.o : Makefile   treetype.cpp   substitution.h identifier.h assert.h treetype.h primtype.h tree.h 
	$(CPP) -c $(Flags) treetype.cpp -o  treetype.o


tree.o : Makefile   tree.cpp   data.h tree.h identifier.h assert.h treetype.h primtype.h 
	$(CPP) -c $(Flags) tree.cpp -o  tree.o


substitution.o : Makefile   substitution.cpp   assert.h substitution.h identifier.h treetype.h primtype.h tree.h 
	$(CPP) -c $(Flags) substitution.cpp -o  substitution.o


data.o : Makefile   data.cpp   data.h 
	$(CPP) -c $(Flags) data.cpp -o  data.o


instruction.o : Makefile   instruction.cpp   data.h instruction.h identifier.h assert.h treetype.h primtype.h 
	$(CPP) -c $(Flags) instruction.cpp -o  instruction.o


flowgraph.o : Makefile   flowgraph.cpp   data.h assert.h flowgraph.h instruction.h identifier.h treetype.h primtype.h 
	$(CPP) -c $(Flags) flowgraph.cpp -o  flowgraph.o


program.o : Makefile   program.cpp   program.h flowgraph.h instruction.h identifier.h assert.h treetype.h primtype.h 
	$(CPP) -c $(Flags) program.cpp -o  program.o


runtimestate.o : Makefile   runtimestate.cpp   data.h assert.h runtimestate.h program.h flowgraph.h instruction.h identifier.h treetype.h primtype.h error.h 
	$(CPP) -c $(Flags) runtimestate.cpp -o  runtimestate.o


varstore.o : Makefile   varstore.cpp   varstore.h treetype.h primtype.h identifier.h assert.h 
	$(CPP) -c $(Flags) varstore.cpp -o  varstore.o


typesystem.o : Makefile   typesystem.cpp   substitution.h identifier.h assert.h treetype.h primtype.h tree.h typesystem.h 
	$(CPP) -c $(Flags) typesystem.cpp -o  typesystem.o


token.o : Makefile   token.cpp   token.h identifier.h assert.h tree.h treetype.h primtype.h 
	$(CPP) -c $(Flags) token.cpp -o  token.o


tokenizer.o : Makefile   tokenizer.cpp   data.h tokenizer.h reader.h token.h identifier.h assert.h tree.h treetype.h primtype.h 
	$(CPP) -c $(Flags) tokenizer.cpp -o  tokenizer.o


parser.o : Makefile   parser.cpp   parser.h token.h identifier.h assert.h tree.h treetype.h primtype.h tokenizer.h reader.h varstore.h 
	$(CPP) -c $(Flags) parser.cpp -o  parser.o


