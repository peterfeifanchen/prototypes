#include<iostream>
#include<vector>

// This implementation uses new/delete to create new Trie nodes. This by no means
// guarantee that no memory is leaked. Not much effort is put in to make sure of
// that.
template< typename T, int N, typename slot>
class Trie {

   class Node {
      T c_;
      bool end_;
      // highest index in the alphabet with a valid child
      int highIndex_;
      // size of alphabet, why tries are very memory intensive
      // as most of these would be empty. Further as the alphabet
      // can sometimes be large, it really doesn't play well with
      // efficient memory access (e.g., hash, binary trees can be
      // easily implemented on a block of slots/arrays which can
      // be much more cache efficient. On the other hand, you can
      // always write your own really complicated memory alloc
      // that always creates trieNodes in that memory pool
      Node* child_[N]; 
      int refcnt_; 
    public:

      Node( T c, bool end ) : c_(c), end_(end), refcnt_(1), highIndex_(-1) {
         for( int i = 0; i < N; i++ ) child_[i] = NULL;
      };

      ~Node() {
         for( int i = 0; i < N; i++ ) {
            if( child_[i] == NULL ) continue;
            delete child_[i];
         }
      }
   
      T& c() { return c_; }
      bool end() { return end_; }
      void endIs( bool end ) { end_ = end; }

      void refcntDec() { refcnt_--; }
      void refcntInc() { refcnt_++; }
      int refcnt() { return refcnt_; }

      int highIndex() { return highIndex_; }

      void addNode( T c, bool end ) {
         int index = slot()( c );
         if( child_[ index ] == NULL ) {
            child_[ index ] = new Node( c, end );
            if( index > highIndex_ ) highIndex_ = index;
         } else {
            child_[ index ]->refcntInc(); 
            if( end ) child_[ index ]->endIs( end );
         }
      }

      Node* child( T c ) {
         return child_[ slot()( c ) ];
      }

      Node* child( int i ) {
         return child_[ i ];
      }

      void deleteNode( T c ) {
         Node* ch = child_[ slot()( c ) ];
         if( ch  == NULL ) return;

         ch->refcntDec();
         if( ch->refcnt() == 0 ) {
            delete child_[ slot()( c ) ];
            child_[ slot()( c ) ] = NULL;
         }
      }
   };

   Node* root;
  
   void printPatterns( Node* curr, std::vector<T> str ) {
      for( int i = 0; i < N; i++ ) {
         std::vector<T> tmp( str );
         if( curr->child( i ) == NULL ) continue;
         tmp.push_back( curr->child( i )->c() );
         if( curr->child( i )->end() ) {
            std::cout << std::string( tmp.begin(), tmp.end() ) << std::endl;
         } 
         printPatterns( curr->child( i ), tmp ); 
      }
   }

   void printTrie( Node* curr, std::string pfx, int lvl ) {
      for( int i = 0; i < N; i ++ ) {
         if( curr->child( i ) == NULL ) continue;   
         std::cout << pfx;
         if( i >= curr->highIndex() ) std::cout << "└──";
         else std::cout << "├──";
         if( curr->child( i )->end() ) std::cout << "\e[1m\033[31m";
         std::cout << curr->child( i )->c() << std::endl;
         if( curr->child( i )->end() ) std::cout << "\e[0m\033[0m";
         printTrie( curr->child( i ), 
               i >= curr->highIndex() ? pfx + "   " : pfx + "|  ", 
               lvl+1 );
      }
   }

 public:
   Trie() {
      root = new Node( T(), false );
   }

   ~Trie() {
      delete root;
   }

   void addPattern( std::vector<T> &pat ) {
      if( matchPattern( pat ) ) return;
      std::cout << "Adding " << std::string( pat.begin(), pat.end() ) << std::endl;
      Node* curr = root;
      for( int i = 0; i < pat.size(); i++ ) {
         curr->addNode( pat[i], i == pat.size()-1 ); 
         curr = curr->child( pat[i] );
      }
   }

   void deletePattern( std::vector<T> &pat ) {
      std::cout << "Delete " << std::string( pat.begin(), pat.end() ) << std::endl;
      if( !matchPattern( pat ) ) return;
      Node* curr = root;
      for( int i = 0; i < pat.size(); i++ ) {
         curr->deleteNode( pat[i] );
         if( curr->child( pat[i] ) == NULL ) break;
         else curr = curr->child( pat[i] );
      }
   }

   bool matchPattern( std::vector<T> &pat ) {
      Node* curr = root;
      for( int i = 0; i < pat.size(); i++ ) {
         if( curr->child( pat[i] ) == NULL ) return false;
         curr = curr->child( pat[i] );
      }
      if( !curr->end() ) return false;
      return true;
   }

   // We need to output all patterns that match in the text. This involves
   // running all the suffix strings of text through the trie. 
   void matchText( std::vector<T> &text ) {
      std::cout << std::string( text.begin(), text.end() ) << " matches: " << std::endl;
      for( int i = 0; i < text.size(); i++ ) {
         Node* curr = root;
         for( int j = i; j < text.size(); j++ ) {
            if( curr->child( text[j] ) == NULL ) break;
            if( curr->child( text[j] )->end() ) {
               std::cout << "start @ text[" << i << "] " 
                  << std::string( text.begin() + i, text.begin() + j + 1 ) << std::endl;
            }
            curr = curr->child( text[j] );
         }
      }
   };

   // Prints all the patterns that we have so far
   void printPatterns() {
      std::cout << "The patterns in the trie are: " << std::endl;
      printPatterns( root, std::vector<T>() );
   }
   void printTrie() {
      std::cout << "The trie looks like: " << std::endl;
      printTrie( root, "", 0 ); 
   }
};

class lowerCharSlot {
 public:
   int operator()( char c ) {
      return c - 'a';
   }
};

int main() {
   Trie< char, 26, lowerCharSlot> t;
 
   std::vector<std::string> patterns{ "aaa", "aaaaa", "s", "a", "ss", "nonsense" };
   std::vector<std::string> texts{ "aaaaaas", "nonsense" };

   // Add patterns
   for( auto &pat : patterns ) {
      std::vector<char> tmp( pat.begin(), pat.end() );
      t.addPattern( tmp );
   }

   std::cout << std::endl;
   t.printPatterns();
   std::cout << std::endl;
   t.printTrie();
   std::cout << std::endl;

   for( auto txt : texts ) {
      std::vector<char> tmp( txt.begin(), txt.end() );
      t.matchText( tmp );
   }

   // Delete patterns
   std::cout << std::endl;
   std::vector<char> tmp;
   tmp.assign( patterns[1].begin(), patterns[1].end() );
   t.deletePattern( tmp );
   tmp = {'a','a'};
   t.deletePattern( tmp );
   tmp.assign( patterns[4].begin(), patterns[4].end() );
   t.deletePattern( tmp );
   
   std::cout << std::endl;
   t.printPatterns();
   std::cout << std::endl;
   t.printTrie();
   std::cout << std::endl;
}
