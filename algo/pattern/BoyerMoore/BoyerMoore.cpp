#include<iostream>
#include<map>
#include<string>

class BoyerMoore {
 
 public:
   // NOTE: len(pattern) <= len(text), otherwise it would not match
   static void bmAlgo( std::string &text, std::string &pattern ) {
      std::map<char, int> badChar;

      // Pass through the pattern to build badChar
      for( int i = 0; i < pattern.size(); i++ ) {
         badChar[ pattern[i] ] = i; 
      }

      // Pass over the text
      int shift = 0;
      int n = text.length();
      int m = pattern.length();
      while( shift <= n - m ) {
         std::cout << "shift: " << shift << std::endl;
         // We match from the end index
         int j = m - 1;
         while( j >= 0 && pattern[j] == text[shift+j] ) j--;

         // If we match, j == -1, otherwise j > 0
         if( j < 0 ) {
            std::cout << "matched: [" << shift << "," << shift+m-1 << "]" << std::endl; 
            // On match we shift by m - badChar[ text[shift+m] ]
            // text[ shift+m ] is the first character after the match
            // badChar[ text[ shift+m ] ] is the index of the last occurence of
            //      the character text[ shift+m ]
            // if badChar[ text[ shift+m ] ] does not exist, we should shift over
            //      text[ shift+m ] which means shift by m + 1
            if( shift + m >= n ) shift++;
            else if( badChar.find( text[ shift+m ] ) == badChar.end() ) {
               // badChar.find is O(logN), but if N, number of distinct letters in
               // the pattern is small, this is basicaly O(1). Alternatively, you
               // can implement your own hashtable that can find empty slots in O(1)
               // time.
               shift += m + 1;
            } else {
               shift += m - badChar[ text[ shift+m ] ];
            }
         } else {
            // On mismatch we shift by j - badChar[ text[ shift+j ] ] or if the
            // letter does not exit by j + 1. We do have to watch out for a 
            // character to the right of current index j in the pattern. This will
            // result in a match and a negative shift. So in this case we have to
            // shift by max( 1, j - badChar[ text[ shift+j ] ] )
            if( badChar.find( text[ shift+m ] ) == badChar.end() ) shift += j + 1;
            else shift += std::max( 1, j - badChar[ text[ shift+j ] ] );
         }
      }
   }
};

int main() {
   std::string text = "AAAAAXXGGGTG";
   std::string pattern = "GGGTG";

   BoyerMoore::bmAlgo( text, pattern );
}
