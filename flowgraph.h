

#ifndef FLOWGRAPH_INCLUDED
#define FLOWGRAPH_INCLUDED  1


// Written by Hans de Nivelle, 18 dec 2011 for the class
// compiler construction.

// Currently, there are no copy constructor, and no assignment operator,
// I probably should write them. 


#include "instruction.h"
#include <list>
#include <map>


class flowgraph
{

   // The instruction in struct node is optional. The node in the local
   // variable of type flowgraph has no instruction. The nodes on the 
   // heap always have an instruction. The reason why this has to be the 
   // case is the fact that, in a doubly linked list, the first and last
   // node need to have a pointer to the local variable list, 
   // but there is no object that we can put in it.  
   // This is why the instruction in struct node must be optional.
   // We are not exactly a linked list, but we are similar, and we have
   // the same problem. 

   struct node
   {
      instruction* ins; 
      std::list< node* > pred;
      std::list< node* > succ; 

      node( ) 
         : ins(0)
      { }

      node( const instruction& ins )
         : ins( new instruction( ins ))
      { }  


      void operator = ( const node& n )
      {
         if( this != &n )
         {
            delete ins;
            ins = n. ins ? new instruction( * ( n. ins )) : 0;
         }
      }

      ~node( )
      {
         delete ins;
      }
   };


   node b, e;           // Both are without ins. 

   unsigned int n;      // Number of nodes. 
   unsigned int c;      // Number of connections. 

public: 
   class iterator
   {
      friend class flowgraph;

      node *n; 

   public: 
      iterator( ) : n(0) { } 
      iterator( node* n ) : n(n) { }
 
      iterator succ( unsigned int i ) const;
      iterator pred( unsigned int i ) const;
      unsigned int nrsuccessors( ) const;
 
      instruction& operator * ( ) const { return * ( n -> ins ); } 
      bool operator == ( iterator i ) const { return n == i. n; } 
      bool operator != ( iterator i ) const { return n != i. n; } 

      friend std::ostream& operator << ( std::ostream& , iterator );
   };


   class const_iterator
   {
      friend class flowgraph;

      const node *n;

   public:
      const_iterator( ) : n(0) { }
      const_iterator( const node* n ) : n(n) { }

      const_iterator succ( unsigned int i ) const;
      const_iterator pred( unsigned int i ) const;
      unsigned int nrsuccessors( ) const;

      const instruction& operator * ( ) const { return * ( n -> ins ); }
      bool operator == ( const_iterator i ) const { return n == i. n; }
      bool operator != ( const_iterator i ) const { return n != i. n; }

      friend std::ostream& operator << ( std::ostream& , const_iterator ); 
   };



   flowgraph( ) : n(0), c(1) 
   { b. succ. push_back( &e ); 
     e. pred. push_back( &b ); 
   }

   flowgraph( const flowgraph& f );
   void operator = ( const flowgraph& f ); 
   ~flowgraph( ); 

   iterator begin( ) { return &b; } 
   const_iterator begin( ) const { return &b; } 
   iterator end( ) { return &e; } 
   const_iterator end( ) const { return &e; }
      // See explanation above about the iterators.

   void disconnect( iterator it1, unsigned int i, iterator it2 );
      // Disconnect it1 from it2. The i-th successor of it1 must be 
      // it2. 

   void connect( iterator it1, unsigned int i, iterator it2 );
      // Connect it1 directly to it2. The new connection will be
      // the i-th connection of it1.
   
   void erase( iterator it1, iterator it2 );
      // Erase, starting at it1, and stopping just before it2.
      // *it1 is erased, *it2 will stay.

   void transfer( iterator it1, iterator it2, flowgraph& f );
      // Put flowgraph f between it1 and it2.
      // After the operation, flowgraph f is empty.

   iterator insert( const instruction& ins, iterator it, unsigned int i );
      // it must have an i-th connection. In this connection, 
      // ins is inserted. So, if we have  it[i] ---> q,  we will now have
      // it[i] ---> ins ---> q. 

   unsigned int nrnodes( ) { return n; }
   unsigned int nrconnections( ) { return c; }

   friend std::ostream& operator << ( std::ostream& , 
                                      const flowgraph::node* n ); 
   friend std::ostream& operator << ( std::ostream& , const flowgraph& g ); 
 
private:  
   // Below follows a lot of code that is not part of the user interface!

   // There are several functions that need to visit each node in
   // the graph exactly once, like destroying or printing.
   // It is nice to have one time a meaningful application of a 
   // design pattern.

   struct visitor
   {
      virtual void visit( node* n ) = 0;
   };


   static void visitnodes( visitor& v, 
                           node* startnode, const node* endnode );
      // visit all nodes between startnode and endnode.
      // It is of course possible that there are paths from startnode
      // that do not pass through endnode. In that case, the whole
      // path is visited. 
      // A visit( ) method should not call another visitor, because
      // visitnodes( ) has static variables. 

   struct printer : public visitor
   {
      std::ostream& stream;
   
      printer( std::ostream& stream ) : stream( stream ) { }
      void visit( node* n );
         // There is a problem with constness. We were too lazy to make
         // a separate visitor and const_visitor. Therefore, we have to 
         // cast constness away when calling visit( printer& );
   };

   struct destructor : public visitor
   {
      const node* b; 
         // We must remember the begin node, because it is in 
         // local memory, and it should not be destroyed.

      destructor( const node* b ) : b(b) { } 
      void visit( node* n );
   };

   struct copier : public visitor
   {
      // We copy g1 -> g2.

      const flowgraph* g1; 
      flowgraph* g2; 

      std::map< const node*, node* > moved; 
         // We remember for every node of g2 where we moved it,
         // so that we can update the pred and succ nodes later. 

      copier( const flowgraph* g1, flowgraph* g2 ) : g1(g1), g2(g2) { } 
      
      void visit( node* n );
         // During visit, we make a copy of n, and store in moved 
         // where we moved it. In a second pass through moved,
         // the connections must be updated. 

      node* lookup( node* n ) const; 

      void copy( const std::list< node* > & from,
                 std::list< node* > & to ) const;
   };

   void copy_from( const flowgraph& f ); 


   inline static node* nth( const std::list< node* > & list, unsigned int i )
   {
      ASSERT( i < list. size( ));

      std::list< node* > :: const_iterator p = list. begin( );
      while(i)
      {
         ++ p;
         -- i;
      }

      return *p;
   }

}; 

#endif

