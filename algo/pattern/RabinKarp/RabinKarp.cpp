#include <iostream>
#include <string>

template< int N>
class RabinKarp {
   
   static const long MOD = 101;
   static long hash( std::string str, int offset, int len ) {
      long hashVal = 0; 
      for( int i = offset; i < offset + len; i++ ) {
         int val = str[i];
         hashVal = ( ( hashVal * N ) % MOD + val ) % MOD;
      }
      return hashVal;
   }
   
   static long baseN( int length ) {
      long base = 1;
      //std::cout << "MOD: " << MOD << " N: " << N << std::endl;
      for( int i = 0; i < length-1; i++ ) {
         //std::cout << "i: " << i << std::endl;
         base = ( base * N ) % MOD;
      }
      return base;
   }

   static long hash( long prevHash, char newChar, char oldChar, long highBase ) {
      //std::cout << "newChar: " << newChar << " oldChar: " << oldChar << std::endl;
      //long hashVal = ( N * ( prevHash - oldChar*highBase ) + newChar ) % MOD;
      //if( hashVal < 0 ) hashVal = hashVal + MOD;
      //return hashVal;
      long hashVal = ( oldChar * highBase ) % MOD;
      //std::cout << hashVal << std::endl;
      hashVal = ( prevHash - hashVal ) % MOD;
      if( hashVal < 0 ) hashVal = hashVal + MOD;
      //std::cout << hashVal << std::endl;
      hashVal = ( hashVal * N ) % MOD;
      //std::cout << hashVal << std::endl;
      return ( hashVal + newChar ) % MOD;
   }

 public:
   static void rkAlgo( std::string &txt, std::string &pat ) {
      long hashPat = hash( pat, 0, pat.length() );
      long hashTxt = hash( txt, 0, pat.length() );
      long highBase = baseN( pat.length() );

      //std::cout << "highBase: " << highBase << std::endl;
      bool found = false;

      int txtLen = txt.length();
      int patLen = pat.length();
      for( int i = 0; i <= txtLen - patLen; i++ ) {
         //std::cout << "char " << i << ": " << txt[i] << std::endl;
         //std::cout << "pattern hash: " << hashPat << std::endl;
         //std::cout << "txt hash: " << hashTxt << std::endl;
         // if hash matches, we check if it is an actual match
         if( hashPat == hashTxt ) {
            int j = 0;
            for( ; j < patLen; j++ ) {
               if( txt[i+j] != pat[j] ) break;
            }
            if( j == patLen ) {
               // We have a match
               found = true;
               std::cout << "Found match @ [" << i << "," << i+patLen << ")" << std::endl;
            }
         }

         // if no match, we advance the hash of txt
         if( i+patLen < txtLen ) {
            //std::cout << "next char: " << txt[i+patLen] << std::endl;
            hashTxt = hash( hashTxt, txt[i+patLen], txt[i], highBase );
         }
      }

      if( !found ) std::cout << "No match found!" << std::endl;
   }   
};

int main() {
   std::string txt = "geeks for geeks";
   std::string pat = "geeks";

   RabinKarp<256>::rkAlgo( txt, pat );
}
