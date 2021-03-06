#include<iostream>
#include<math.h>
#include<vector>
#include<cstdlib>
#include<limits>

// Our f() is sum of the range from l,r
struct merge {
   int operator()( int a, int b ) {
      return a ^ b;
   }

   int mergeValue( int origVal, int newVal ) {
      return origVal ^ newVal;
   }
};

template< class f>
class st{
   std::vector<int> segmentTree;
   int arraySize;
   f op;
   
   int callGet( int desiredL, int desiredR, int index, int curL, int curR ) {
      //std::cout << "    desiredL: " << desiredL << " desiredR: " << desiredR 
      //         << " index: " << index << " curL: " << curL << " curR: " 
      //         << curR << std::endl;
      if( desiredL == curL && desiredR == curR ) return segmentTree[index];
      if( curL > desiredR || curR < desiredL ) return 0;

      int mid = ( curL + curR )/2;
      //std::cout << "    mid: " << mid << std::endl;
      return op( callGet( desiredL, mid > desiredR ? desiredR : mid, 2*index+1, curL, mid ),
             callGet( mid+1 < desiredL ? desiredL : mid+1 , desiredR, 2*index+2, mid+1, curR ) );
   }

   void callUpdate( int curIndex, int incrVal, int l, int r, int index ) {
      //std::cout << "curIndex: " << curIndex << " incrVal: " << incrVal << " index: " << index 
      //         << " l: " << l << " r: " << r << std::endl;
      if( index < l || index > r ) return;
         
      segmentTree[curIndex] = op( segmentTree[curIndex], incrVal);
      if( l==r ) return;

      int mid = (l+r)/2;
      callUpdate( curIndex*2+1, incrVal, l, mid, index );
      callUpdate( curIndex*2+2, incrVal, mid+1, r, index );
   }
   
   int buildSegmentTree( int l, int r, int index, std::vector<int> &src ) {
      int mid = (l+r)/2;
      //std::cout << "l: " << l << " mid: " << mid << " r: " << r << " index: " 
      //   << index << std::endl;
      if( l == r ) {
         segmentTree[index] = src[l];
         return src[l];
      }
      
      int lSubTree = buildSegmentTree( l, mid, index*2+1, src );
      int rSubTree = buildSegmentTree( mid+1, r, index*2+2, src );


      segmentTree[index] = op( lSubTree, rSubTree );
      return segmentTree[index];
   }

public:
   st( std::vector<int> &src ) {
      segmentTree.resize( ( 1 << (int)ceil(log2(2*src.size())) ) - 1 );
      arraySize = src.size();
      //std::cout << segmentTree.size() << std::endl;
      
      buildSegmentTree( 0, src.size()-1, 0, src );
      for( auto &i : segmentTree ) {
         std::cout << i << " ";
      }
      std::cout << std::endl;
   }

   int get( int l, int r ) {
      //std::cout << "l: " << l << " r: " << r << std::endl;
      if( l >= arraySize ) return 0;
      if( r >= arraySize ) r = arraySize-1;
      return callGet( l, r, 0, 0, arraySize-1 );
   }

   int update( int index, int origVal, int newVal ) {
      int mVal = op.mergeValue( origVal, newVal );

      callUpdate( 0, mVal, 0, arraySize-1, index );
   }
};

int main() {
   std::vector<int> v{1, 3, 5, 7, 9, 11};
   st<merge> s( v ); 

   std::cout << "sum[1, 3]=" << s.get(1, 3) << std::endl; //1

   s.update( 1, 3, 10 );
   std::cout << "sum[1, 3]=" << s.get(1, 3) << std::endl; //8
}
