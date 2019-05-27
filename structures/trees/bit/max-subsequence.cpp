#include<iostream>
#include<vector>
#include<map>
#include<algorithm>

// Given an array, find the largest sum of an increasing subsequence.
//
// 1) Build a map of unique elements in the array. This will take O(N).
// 2) Build a BIT tree for the unique elements. However, rather than a BIT tree
//    that represents the sum of elements from 0...x, it is the maximum value
//    from 0...x. This represents the maximum value of an increasing subsequence
//    ending with the number represented by the index. Sum operation would
//    traverse all unique elements smaller than or equal to the current number 
//    we encountered and then add the new number as we iterate through the vector.
//    This would take O(N*logN).
// 3) The answer is then the sum() from the end of the BIT tree. 
//
//    Alternatives may include iterating each unique number smaller than the new
//    number. This would take O(N^2). 

// default for c++ maps are less<Key>. We allow on this mechanism for iterating each
// unique value with its rank. The default comparator class would work, but here we
// show the actual mechanism and class definitions if you would want to override.
struct cmp {
   bool operator()( int a, int b ) {
      return a < b;
   }
};

int sum( int index, std::vector<int>& b ) {
   int maxVal = b[index];
   for( int i = index-1; i > 0; i -= i & (-i) ) {
      maxVal = std::max( b[i], maxVal ); 
   }
   return maxVal;
}

int main() {
   //std::vector<int> seq{1,4,2,8,3,4,7};
   std::vector<int> seq{1,2,4,5,6}; 

   std::map<int, int, cmp> uniqueVal;
   for( auto &v : seq ) {
      uniqueVal[v] = 0;   
   }

   int size = 0;
   for( auto it = uniqueVal.begin(); it != uniqueVal.end(); it++ ) {
      size++;
      uniqueVal[it->first] = size;
      //std::cout << it->first << " " << size <<  std::endl;
   }

   //std::cout << "size: " << size << std::endl;
   std::vector<int> bit( size+1 );
   for( auto &v : seq ) {
      int bitIndex = uniqueVal[v];
      bit[bitIndex] = sum( bitIndex, bit ) + v;
      //std::cout << "v: " << v << std::endl;
      //for( auto &b : bit ) {
      //   std::cout << b << " ";
      //}
      //std::cout << std::endl;
   }

   std::cout << "max increasing subsequence: " << sum( size, bit ) << std::endl;
   return 0;
}
