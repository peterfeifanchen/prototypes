#include<iostream>
#include<vector>
#include<iomanip>

// 2D BIT returns the sum of all indices in the rectangle (0,0), (x,y).
//
// (0,0)------------>
//   |(x1,y1)_____
//   |      |     |
//   |      -------
//   |          (x2,y2)
//   |
//   
//
// Therefore the sum of any rectangle in the 2D array is equal to:
//      (x2, y2) - (x1,y2) - (x2,y1) + (x1,y1)
//
// 2D BIT is basically a an array of column-wise BIT trees. To update the array of
// BIT trees, you advance across the columns and then iterate over the rows in the
// same way you do for a single BIT tree. Summing the array of BIT trees is also
// similar to how you would do it for a single BIT tree. Conceptually, you can think
// of it as a BIT tree where each node is also a sub-BIT tree. 
class BIT2D {
   std::vector<std::vector<int>> v_;
   std::vector<std::vector<int>> b_;
 public:
   BIT2D( std::vector<std::vector<int>> &v ) : v_(v) {
      b_.resize( v_.size()+1, std::vector<int>( v_[0].size()+1 ) );
      printB();
      printV();
      construct2DBIT();
   }

   int sum( int x, int y ) {
      int sum = 0;
      for( int i = x+1; i > 0; i = i - ( i & (-i) ) ) {
         for( int j = y+1; j > 0; j = j - ( j & (-j) ) ) {
            sum += b_[i][j];
         }
      }
      return sum;
   }

   void updateBIT( int x, int y, int val ) {
      for( int i = x+1; i < b_.size(); i = i + ( i & (-i) ) ) {
         for( int j = y+1; j < b_[i].size(); j = j + ( j & (-j) ) ) {
            b_[i][j] += val;
         }
      }
   }
   
   void printV() {
      std::cout << std::endl;
      for( int i = 0; i < v_.size(); i++ ) {
         for( int j = 0; j < v_[i].size(); j++ ) {
            std::cout << v_[i][j] << " ";
         }
         std::cout << std::endl;
      }
      std::cout << std::endl;
   }

   void printB() {
      std::cout << std::endl;
      for( int i = 0; i < b_.size(); i++ ) {
         for( int j = 0; j < b_[i].size(); j++ ) {
            std::cout << std::right << std::setw(3) <<  b_[i][j] << " ";
         }
         std::cout << std::endl;
      }
      std::cout << std::endl;
   }

   void construct2DBIT() {
      for( int i = 0; i < v_.size(); i++ ) {
         for( int j = 0; j < v_[0].size(); j++ ) {
            int sumA = sum( i, j );
            int sumB = sum( i-1, j );
            int sumC = sum( i, j-1 );
            int sumD = sum( i-1, j-1 );

            updateBIT( i, j, v_[i][j] );
            //std::cout << "[" << i << "][" << j << "]" << std::endl;
            printB();
         }
      }
   }

   int query( int x1, int y1, int x2, int y2 ) {
      //std::cout << sum( x2, y2 ) << " " << sum( x1-1, y2 ) << " " << sum( x2, y1-1 ) 
      //          << " " << sum( x1-1, y1-1 ) << std::endl;
      return sum( x2, y2 ) - sum( x1-1, y2 ) - sum( x2, y1-1 ) + sum( x1-1, y1-1 );
   }
};

int main( int argc, char** argv ) {
   std::vector<std::vector<int>>  testCase{
      {1, 2, 3, 4}, //10 
      {5, 3, 8, 1}, //17                                 
      {4, 6, 7, 5}, //22          
      {2, 4, 8, 9}  //23
   };

   BIT2D b( testCase );
   std::cout << b.query( 1, 1, 3, 2 ) << std::endl; //36
   std::cout << b.query( 2, 3, 3, 3 ) << std::endl; //14
   std::cout << b.query( 1, 1, 1, 1 ) << std::endl; //3
}


