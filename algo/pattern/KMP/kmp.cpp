#include<iostream>
#include<string>
#include<vector>

class KMP {
 public:
   static void kmpAlgo( std::string &txt, std::string &patt ) {
      // Construct lps array
      std::vector<int> lps( patt.size(), 0 );
      int L = 0;
      int i = 1;
      while( i < patt.size() ) {   
         if( patt[i] == patt[L] ) {
            L++;
            lps[i] = L;
            i++;
         } else {
            if( L == 0 ) {
               // No prefix that is also a suffix
               lps[i] = 0;
               i++;
            } else {
               L = lps[L-1];
            }
         }
      }

      std::cout << "lps array: ";
      for( auto &i : lps ) {
         std::cout << i << " ";
      }
      std::cout << std::endl;

      // Apply KMP to text
      int t = 0;
      int p = 0;
      while( t < txt.size() ) {
         while( txt[t] == patt[p] && p < patt.size() ) {
            t++;
            p++;
         } 
         
         if( p == patt.size() ) {
            //Match
            std::cout << "Pattern found in text @ [" << t-patt.size() << "," << t 
                     << ")" << std::endl;
            // We don't need to do t++ here as the while loop before would have done
            // it in its last iteration
            p = lps[ p-1 ];
         } else { 
            // No match
            if( p == 0 ) {
               // p is already reset to the beginning of the pattern, no match is
               // possible. We advance t.
               t++;
            } else {
               //we go to lps array to find where we should reset p to
               p = lps[ p-1 ];
            } 
         }
      }
   }
};

int main() {
   std::string txt = "ABABDABACDABABCABAB";
   std::string patt = "ABABCABAB";
   KMP::kmpAlgo( txt, patt );
}
