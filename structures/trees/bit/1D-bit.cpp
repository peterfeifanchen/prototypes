#include<iostream>
#include<vector>

class BIT{
 std::vector<int> v_;
 std::vector<int> b_;
 public:
   BIT( std::vector<int> &v ) : v_(v) {
      // For convenience, we index from 1 rather than 0. 
      constructBITTree();
      //std::cout << "v_: ";
      //for( int i = 0; i < v_.size(); i++ ) {
      //   std::cout << v_[i] << " ";
      //}
      //std::cout << std::endl;
      //
      //std::cout << "b_: ";
      //for( int i = 0; i < b_.size(); i++ ) {
      //   std::cout << b_[i] << " ";
      //}
      //std::cout << std::endl;
   }

   int sum( int index ) {
      // i -= i & (-i) removes the last set bit of i. This essentially iterates over
      // the indices where index has its 1 bit set. The result is the sum from 0 to
      // index.
      int sum = 0;
      for( int i = index+1; i > 0; i = i - ( i & (-i) ) ) {
         sum += b_[i];
      }

      return sum;
   }

   void updateBIT( int index, int val, bool incrementV ) {
      // i += i & (-i) adds the last set bit of i to i. This essentially moves the
      // index to the next highest binary index. This operation is therefore O(logN)
      // as it increments only the indices that are closest powers of 2 from index
      // (leftshift-wise).
      // This way, to get the aggregate sum, you just have to iterate also over
      // closest powers of 2 (rightshift-wise).
      if( incrementV ) v_[index] += val;
      //std::cout << "index: " << index << std::endl;
      for( int i = index+1; i < b_.size(); i = i + ( i & (-i) ) ) {
         //std::cout << "i: " << i << std::endl;
         b_[i] += val;
      }
   }

   void constructBITTree() {
      b_.resize( v_.size()+1, 0 );
      for( int i = 0; i < v_.size() ; i++ ) {
         updateBIT( i, v_[i], false );
      }
   }
};


int main() { 
   std::vector<int> bitQ{2, 1, 1, 3, 2, 3, 4, 5, 6, 7, 8, 9}; 
               
   BIT b( bitQ );

   std::cout << "Sum of elements in arr[0..5] is " << b.sum(5) << std::endl; 
                     
                       
   //Update BIT for above change in arr[] 
   b.updateBIT(3, 6, true); 
   
   std::cout << "Sum of elements in arr[0..5] post update " << b.sum(5) << std::endl;
   return 0; 
} 

