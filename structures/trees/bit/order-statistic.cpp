#include<iostream>
#include<vector>

template< typename T >
class BIT {
 std::vector<T> b_;
 public:
   BIT( int size ) {
      // for convenience the first value starts at index 1 rather than 0
      b_.resize( size+1, 0 );
   }

   int sum( int index ) {
      int sum = 0;
      for( int i = index+1; i > 0; i = i - ( i & (-i) ) ) {
         sum += b_[i];
      }

      return sum;
   }

   void updateBIT( int index, int val ) {
      for( int i = index+1; i < b_.size(); i = i + ( i & (-i) ) ) {
         b_[i] += val;
      }
   }

   std::vector<T>& b() {
      return b_;
   }
};



// BITOS (order-statistic) supports the following operations on ordinals:
//  1) insertElement: add an element to a list of elements
//  2) removeElement: remove an element from a list of elements
//  3) rank: return the rank of an element in the list of elements. Assuming that
//           all elements are unique. E.g. in {1,2,9,10} is 3, rank(9)=3, rank(2)=2.
//  4) findKthSmallest: find the Kth smallest number in the list of elements.
//
// The BIT implementation of the above methods requires that the number of unique
// elements are bounded and comparable. For example, if we know for a vector of 
// integers that its values are range-bound between 0...1000, we can provide
// the following implementation.
//
// Instantiate a BIT tree for 0...1000. Each number is an index. Whenever a number
// is inserted, BIT update +1 to the index. The value at each number (index) then
// becomes its rank in the list of current elements. Correspondingly, findKSmallest
// becomes a binary search over the BIT tree to find the number (index) with value
// equal to k. If k > number of elements, we return the largest value.
//
// NOTE: Doesn't really make sense to template this as BIT only really works for int
// counting, but I wanted to just try out templated inheritance.
template< typename T, int SIZE>
class BITOS : public BIT<T> {
 // needed for templated inheritance to differentiate between member functions and
 // non-member functions. This can also be done using "this" as can be seen in
 // insertElement and deleteElement.
 using BIT<T>::b;
 
 public:
   BITOS() : BIT<T>( SIZE ) {}

   void insertElement( T val ) {
      this->updateBIT( val, 1 ); 
   }

   void deleteElement( T val ) {
      this->updateBIT( val, -1 );
   }

   int rank( T val ) {
      return this->sum( val );
   }

   T findKthSmallest( int k ) {
      int l = 0;
      int r = SIZE-1;
      
      if( rank( r ) < k ) k = rank( r );
      
      while( l < r ) {
         int mid = ( r + l ) / 2;
         std::cout << "mid: "  << mid << " rank: " << rank(mid) << std::endl;
         if( k <= rank( mid ) ) r = mid;
         else l = mid+1;
      }

      return l;
   }
};

int main() {
   //0...1000 has 1001 numbers
   BITOS<int, 1001> b; 

   b.insertElement( 20 );
   b.insertElement( 300 );
   b.insertElement( 310 );
   b.insertElement( 0 );

   std::cout << "Rank(20)=" << b.rank(20) << std::endl;
   std::cout << "Rank(310)=" << b.rank(310) << std::endl;
   std::cout << "Rank(320)=" << b.rank(320) << std::endl;
   std::cout << "The 2nd smallest value=" << b.findKthSmallest( 2 ) << std::endl;
   std::cout << "The 5th smallest value=" << b.findKthSmallest( 5 ) << std::endl;

   b.deleteElement(300);
   std::cout << "Rank(20)=" << b.rank(20) << std::endl;
   std::cout << "Rank(310)=" << b.rank(310) << std::endl;


   return 0;
}
