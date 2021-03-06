#include<iostream>
#include<vector>
#include<string>

// Type T must support <, <=, >, >=, == binary operators
template< typename T>
class TernaryTrie {
   struct Node {
      Node* left;   //less
      Node* eq;     //equal
      Node* right;  //larger
      Node* parent;

      T     val;
      bool  end;

      Node( T _val, Node* _p, bool _end ) : val(_val), parent(_p), end(_end),
      left(NULL), eq(NULL), right(NULL) {}
   
      void newLeftNode( T& _val, bool _end ) {}
      void newRightNode( T& _val, bool _end ) {}
      void newEqNode( T& _val, bool _end ) {}
   };

   enum DeleteOP {
      NOOP,
      DELETE,
      UNMARK,
      UNKNOWN
   };
 
   enum ParentType {
      LEFT,
      EQ,
      RIGHT,
      NOT_APPLICABLE
   };

   Node *root;

   // Delete Op on a ternary tree is similar to deleting a node in a BST
   // with the following features/bugs:
   // 1) word does not exist, do nothing
   // 2) word exists and is a prefix of another word, unmark the end bool
   // 3) word is not a prefix of another word (node->eq == NULL in this case)
   //       a) right != NULL, then we find the left-most child of its right
   //          node to replace the current node. The left-most child->eq is
   //          brought along, but left-most child->right becomes its parent's
   //          left. The left-most child->right become node->right. As this is
   //          the left-most child, its left is NULL, this is connected to
   //          node->left. We return NO-OP up the recursive stack as no more op
   //          is needed.
   //       b) right == NULL && left != NULL, we promote the node->left
   //          and find its right-most child and do the mirror image of a)
   //       c) right == NULL && left == NULL, this node is no longer required so
   //          we delete the node. We return DELETE up the recursive stack.
   DeleteOP deleteNode( Node* curr, std::vector<T> &w, int offset ) {
      // word found, operation depends on the previous node
      if( offset == w.size() ) {
         if( curr == NULL ) return DELETE; 
         else return UNMARK; 
      }
      // word w does not exist, do nothing
      if( curr == NULL ) return NOOP;

      DeleteOP op = UNKNOWN;

      if( w[offset] == curr->val ) op = deleteNode( curr->eq, w, offset+1 );
      else if( w[offset] < curr->val ) op = deleteNode( curr->left, w, offset );
      else op = deleteNode( curr->right, w, offset );

      // word w not found, do nothing and return up the tree
      if( op == NOOP ) return NOOP;
      else if( op == UNMARK ) {
         // word w is a prefix, unmark it as a word and do nothing more when
         // returning up the tree
         curr->end = false;
         return NOOP;
      }

      // word w is found and delete was called on curr
      // 1) figure out which children it is of the parent or if this is root
      Node **parent;
      if( curr->parent == NULL ) {
         parent = NULL;
      } else if( curr->parent->eq == curr ) {
         parent = &(curr->parent->eq);
      } else if( curr->parent->left == curr ) {
         parent = &(curr->parent->left);
      } else {
         parent = &(curr->parent->right);
      }

      if( curr->left ) {
         Node* next = curr->left;
         while( next->right != NULL ) {
            next = next->right;
         }
         next->right = curr->right;
         if( next->parent != curr ) {
            next->parent->right = next->left;
            next->left = curr->left;
         }
         if( parent ) *parent = next;
         else root = next;
         delete curr;
         return NOOP;
      } else if( curr->right ) {
         Node* next = curr->right;
         while( next->left != NULL ) {
            next = next->left;
         }
         next->left = curr->left;
         if( next->parent != curr ) {
            next->parent->left = next->right;
            next->right = curr->right;
         }
         if( parent ) *parent = next;
         else root = next;
         delete curr;
         return NOOP;
      } else {
         // This node has no left/right sub-trees. This node could be the left, eq
         // or right child of its parent.
         if( curr->parent == NULL ) {
            root = NULL;
         } else if( curr->parent->eq == curr ) {
            curr->parent->eq == NULL;
         } else if( curr->parent->left == curr ) {
            curr->parent->left == NULL;
         } else {
            curr->parent->right == NULL;
         }
         delete curr;
         return DELETE;
      }
   }

   void printPatterns( Node* curr, std::vector<T> str ) {
      if( curr == NULL ) return;
      std::vector<T> eq( str );
      eq.push_back( curr->val );
         
      printPatterns( curr->left, str );
      if( curr->end ) std::cout << std::string( eq.begin(), eq.end() ) << std::endl;
      printPatterns( curr->eq,  eq );
      printPatterns( curr->right, str );
   }

   void printTernaryTrie( Node *curr, std::string pfx, ParentType t ) {
      std::cout << pfx;
      if( t == EQ ) std::cout << "├──";
      else std::cout << "└──";
      if( curr == NULL ) {
         std::cout << "(NULL)" << std::endl;
         return;
      }
      std::cout << curr->val << std::endl;
      printTernaryTrie( curr->left, curr == root || t==RIGHT ? 
            pfx + "   " : pfx + "|  ", LEFT );
      printTernaryTrie( curr->eq, curr == root || t==RIGHT ? 
            pfx + "   " : pfx + "|  ", EQ );
      printTernaryTrie( curr->right, curr == root || t==RIGHT ?
            pfx + "   " : pfx + "|  ", RIGHT );
   }
   
   void insertWord( std::vector<T> &word, ParentType t ) {
      Node* curr = root;
      Node* parent = NULL;
      int i = 0;
      while( i < word.size() ) {
         //std::cout << word[i] << std::endl;
         if( curr == NULL ) {
            curr = new Node( word[i], parent, i == word.size()-1 );
            switch( t ) {
            case LEFT: 
               parent->left = curr;
               break;
            case EQ:
               parent->eq = curr;
               break;
            case RIGHT:
               parent->right = curr;
               break;
            default:
               break;
            }
         }
         if( root == NULL ) root = curr;
         
         // If curr node exists, we have to check whether to go down the left,
         // right or eq path
         parent = curr;
         if( word[i] == curr->val ) {
            t = EQ;
            curr = curr->eq;
            i++;
         } else if( word[i] < curr->val ) {
            t = LEFT;
            curr = curr->left;
         } else {
            t = RIGHT;
            curr = curr->right;
         }
      }
   }
 
   public:

   TernaryTrie() : root(NULL) {}
   
   void insertWord( std::vector<T> &word ) {
      insertWord( word, NOT_APPLICABLE );
   }

   void deleteWord( std::vector<T> &word ) {
      deleteNode( root, word, 0 );
   }

   void searchWord( std::vector<T> &word ) {
      int i = 0;
      Node* curr = root;
      while( curr != NULL && i < word.size() ) {
         if( word[i] < curr->val ) curr = curr->left; 
         else if( word[i] == curr->val ) {
            if( i < word.size()-1) curr = curr->eq;
            i++;
         } else {
            curr=curr->right;
         }
      }

      std::string w( word.begin(), word.end() );
      if( curr == NULL ) std::cout << w << " not found!" << std::endl;
      else if( !curr->end ) std::cout << w << " not found!" << std::endl;
      else std::cout << w << " found!" << std::endl;
   }

   void prefixWord( std::vector<T> &word ) {
      std::string pfx( word.begin(), word.end() );
      std::cout << "prefixes of " << pfx << ":" << std::endl;
      int i = 0;
      Node *curr = root;
      while( curr != NULL && i < word.size() ) {
         if( word[i] == curr->val ) {
            if( i < word.size()-1) curr = curr->eq;
            i++;
         } else if( word[i] < curr->val ) {
            curr = curr->left;
         } else {
            curr = curr->right;
         }
      }

      if( curr == NULL ) std::cout << "No words with prefix " << pfx << std::endl;
      else {
         // Once we found the node, we can depth first search for valid words using it
         // as the root.
         if( curr->end ) std::cout << pfx << std::endl;
         printPatterns( curr->eq, word );
      }
   }

   void printTernaryTrie() {
      std::cout << "The ternary trie looks like: " << std::endl;
      printTernaryTrie( root, "", NOT_APPLICABLE );
   }

   void printPatterns() {
      std::cout << "The patterns in the ternary trie are: " << std::endl;
      printPatterns( root, std::vector<T>() );
   }
};

int main() {
   TernaryTrie<char> t;
   std::vector<std::string> dict = {
      "fox",
      "focite",
      "foy",
      "foxes",
   };

   // Add dictionary words
   for( auto &pat : dict ) {
      std::vector<char> tmp( pat.begin(), pat.end() );
      t.insertWord( tmp ); 
   }

   // Print patterns & ternary trie
   t.printPatterns();
   t.printTernaryTrie();

   // Search for a word
   {
      std::cout << std::endl;
      std::vector<char> fox{ 'f', 'o', 'x' };
      t.searchWord( fox );
      std::vector<char> foxes{ 'f', 'o', 'x', 'e', 's' };
      t.searchWord( foxes );
      std::vector<char> foxe{ 'f', 'o', 'x', 'e' };
      t.searchWord( foxe );
      std::vector<char> random{ 'r', 'a', 'n', 'd', 'o', 'm' };
      t.searchWord( random );
      std::cout << std::endl;
   }
   
   // Find all dict words that has this prefix
   {
      std::vector<char> fo{ 'f', 'o' };
      t.prefixWord( fo );
      std::vector<char> so{ 's', 'o' };
      t.prefixWord( so );
      std::cout << std::endl;
   }
   
   // Delete dictionary words
   {
      std::vector<char> fox{'f','o','x'};
      t.deleteWord( fox );
      std::vector<char> foxes{'f','o','x','e','s'};
      t.deleteWord( foxes );
   }
   t.printPatterns();
   t.printTernaryTrie();
}
