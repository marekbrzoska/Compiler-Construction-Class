

#include "flowgraph.h"
#include "assert.h" 
#include "data.h"


flowgraph::iterator flowgraph::iterator::succ( unsigned int i ) const
{
   return nth( n -> succ, i ); 
}


flowgraph::const_iterator 
flowgraph::const_iterator::succ( unsigned int i ) const
{
   return nth( n -> succ, i );
}


flowgraph::iterator flowgraph::iterator::pred( unsigned int i ) const
{
   return nth( n -> pred, i ); 
}


flowgraph::const_iterator 
flowgraph::const_iterator::pred( unsigned int i ) const
{
   return nth( n -> pred, i );
}


unsigned int flowgraph::iterator::nrsuccessors( ) const
{
   return n -> succ. size( );
}


unsigned int flowgraph::const_iterator::nrsuccessors( ) const
{
   return n -> succ. size( );
}


flowgraph::flowgraph( const flowgraph& f )
   : n( f. n ),
     c( f. c )
{ 
   copy_from(f); 
}


void flowgraph::operator = ( const flowgraph& f )
{
   if( this != &f )
   {
      destructor d( &b );
      visitnodes( d, &b, &e );

      b. pred. clear( ); b. succ. clear( ); 
      e. pred. clear( ); e. succ. clear( ); 

      n = f. n;
      c = f. c;

      copy_from(f);
   }
}


flowgraph::~flowgraph( )
{
   destructor d( &b );
   visitnodes( d, &b, &e );
}


void flowgraph::disconnect( iterator it1, unsigned int i, iterator it2 )
{
   ASSERT( i < it1. n -> succ. size( ));

   std::list< node* > :: iterator p = it1. n -> succ. begin( );
   while( i )
   {
      ++ p;
      -- i;
   }
   ASSERT( *p == it2. n );

   ASSERT( it2. n -> pred. size( ));

   std::list< node* > :: iterator q = it2. n -> pred. begin( );
   while( *q != it1. n )
   {   
      ++ q; 
      ASSERT( q != it2. n -> pred. end( ));
   }

   it1. n -> succ. erase(p);
   it2. n -> pred. erase(q);  
   -- c; 
}


void flowgraph::connect( iterator it1, unsigned int i, iterator it2 )
{
   ASSERT( i <= it1. n -> succ. size( ));

   std::list< node * > :: iterator p = it1. n -> succ. begin( );
   while(i)
   {
      -- i;
      ++ p;
   }

   it1. n -> succ. insert( p, it2. n );
   it2. n -> pred. push_back( it1. n ); 
   ++ c; 
}


flowgraph::iterator
flowgraph::insert( const instruction& ins, iterator it, unsigned int i )
{
   node* added = new node( ins );

   // We cut the link from (ins,i) to continuation. 

   iterator succ = it. succ(i); 

   disconnect( it, i, succ ); 
   connect( it, i, added );
   connect( added, 0, succ );

   ++ n;   // One node was added. 

   return added; 
}


std::ostream& operator << ( std::ostream& stream, 
                            const flowgraph::node* n )
{
   stream << tobinary( (void*) n ) << " :    ";

   if( n -> ins )
      stream << * ( n -> ins );
   else
      stream << "    ";

   stream << "       [ ";
   for( std::list< flowgraph::node* > :: const_iterator
           p = n -> succ. begin( );
           p != n -> succ. end( );
           ++ p )
   {
      if( p != ( n -> succ. begin( ))) 
         stream << ", ";
      stream << tobinary( (void*) *p ); 
   }
   stream << " ]"; 

#if 0
   stream << ", back[ ";
   for( std::list< flowgraph::node* > :: const_iterator
           p = n -> pred. begin( );
           p != n -> pred. end( );
           ++ p )
   {
      if( p != ( n -> pred. begin( )))
         stream << ", ";
      hexprint( stream, (void*) *p ); 
   }
   stream << " ]";
#endif

   return stream; 
}


std::ostream& operator << ( std::ostream& stream, flowgraph::iterator it )
{
   stream << tobinary( (void*) it. n ); 
   return stream;
}


std::ostream& operator << ( std::ostream& stream, flowgraph::const_iterator it )
{
   stream << tobinary( (void*) it. n );
   return stream;
}


std::ostream& operator << ( std::ostream& stream, const flowgraph& g ) 
{
   stream << "Flowgraph: \n";
   stream << "-----------------------------------\n";

   flowgraph::printer pp( stream );


   // A const_cast is necessary, because I didn't want to make
   // two visitors:

   flowgraph::visitnodes( pp, const_cast< flowgraph::node* > ( & (g.b)), 
                              & (g.e) ); 

   stream << "\n";
   stream << "(Endnode = " << tobinary( (void*) ( &g. e )) << ")\n";
   return stream;
}


void flowgraph::visitnodes( visitor& v, 
                            node* startnode, const node* endnode )
{

   static std::list< flowgraph::node* > todo; 
   static std::map< flowgraph::node* , bool > visited; 
      // Ocurring in todo means: Seen but not yet visisted. 
      // occurring in visited means: Seen and visited. 

   todo. clear( );
   visited.clear( );

   todo. push_front( startnode );
   while( todo. size( ))
   {
      // We take the first node from todo:

      flowgraph::node* n = todo. front( );
      todo. pop_front( );

      // check that we didn't visit n. 

      if( visited. find( n ) == visited. end( ))
      {
         // Not visited yet. We add its successors to todo:

         for( std::list< flowgraph::node* > :: const_reverse_iterator
                 p = n -> succ. rbegin( );
                 p != n -> succ. rend( );
                 ++ p )
         {
            if( *p != endnode ) 
               todo. push_front( *p );
                  // We don't copy the endnode into seen.
         }

         // visit: 

         v. visit( n ); 

         // and remember that we visited:

         visited [ n ] = true; 
      }
   }
}


void flowgraph::printer::visit( node* n )
{
   stream << n << "\n";
}


// Not a pleasant visit:

void flowgraph::destructor::visit( node* n )
{
   // If not start node, then delete:

   if( n != b ) 
   {
      // std::cout << "destroying ";
      // hexprint( std::cout, (void*) n ); 
      // std::cout << "\n"; 
      delete n;
   }
}


void flowgraph::copier::visit( node* n )
{
   // If not the start node or the end node of g1, then we copy:

   if( n != &( g1 -> b ) && n != &( g1 -> e ))
   {
      moved [n] = ( n -> ins ) ? new node( *( n -> ins )) : new node( ); 
         // If n has an instruction, we copy the instruction. 
   }
}


flowgraph::node* flowgraph::copier::lookup( node* n ) const
{
   if( n == &( g1 -> b ))
      return &( g2 -> b );

   if( n == &( g1 -> e ))
      return &( g2 -> e );

   std::map< const node* , node* > :: const_iterator p = moved. find(n);
   ASSERT( p != moved. end( ));
   return p -> second;
}


void flowgraph::copier::copy( const std::list< node* > & from,
                              std::list< node* > & to ) const
{
   for( std::list< node* > :: const_iterator 
           p = from. begin( );
           p != from. end( );
           ++ p )
   {
      to. push_back( lookup( *p ));
   }
}


void flowgraph::copy_from( const flowgraph& f )
{
   copier cop( &f, this );
   visitnodes( cop, const_cast< flowgraph::node* > ( & ( f.b )), &( f.e ));

   ASSERT( cop. moved. size( ) == n );

   // It remains to update the connections:

   for( std::map< const node* , node* > :: const_iterator
           p = cop. moved. begin( );
           p != cop. moved. end( );   
           ++ p )
   {
      cop. copy( p -> first -> pred, p -> second -> pred );
      cop. copy( p -> first -> succ, p -> second -> succ );
   }

   cop. copy( f. b. pred, b. pred );
   cop. copy( f. b. succ, b. succ ); 
}

 
