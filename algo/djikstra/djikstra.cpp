#include<iostream>
#include<vector>
#include<queue>
#include<string>
#include<iomanip>

class Djikstra{
   
   class V {
    public:
       // The current vertex V
       int cur_;
       // Cost to reach current vertex V
       int cost_;
       // Path to V
       std::string p_;

       V( int cur, int cost, std::string p ) : cur_(cur), 
            cost_(cost), p_(p) {}

       V() : cur_(-1), cost_(10000), p_("") {}

       bool operator<( const V& rhs ) const {
              return this->cost_ < rhs.cost_;
       }
       
       bool operator>( const V& rhs ) const {
              return this->cost_ > rhs.cost_;
       }
   };

   std::vector<std::vector<int>> &g_;
   std::vector<bool> reached_;
   V result;
 
 public:
   Djikstra( std::vector<std::vector<int>> & g ) : g_(g) {
      reached_.resize( g.size(), false );
      for( auto &r : g_ ) {
         for( auto &v : r ) {
            std::cout << std::right << std::setw(12) << v << " ";
         }
         std::cout << std::endl;
      }
      std::cout << std::endl;

      for( auto r : reached_ ) {
         std::cout << r << " ";
      }
      std::cout << std::endl;
   }


   void process( int start, int end ) {
      std::priority_queue<V, std::vector<V>, std::greater<V>> minHeap;
      minHeap.push( V( start, 0, std::to_string( start ) ) );
      
      while( !minHeap.empty() ) {
         // This has to be a copy, not a reference. Because, as we push more things
         // into the minHeap, the minHeap std::vector is resized and element pos 
         // moved.
         V v = minHeap.top(); 
         reached_[v.cur_] = true;
         if( v.cur_ == end ) {
            result = v;
            break;
         }

         for( int i = 0; i < g_.size(); i++ ) {
            if( g_[v.cur_][i] < 10000 && v.cur_ != i && !reached_[i] ) {
               minHeap.push( V( i, v.cost_ + g_[v.cur_][i], 
                        v.p_ + "->" + std::to_string( i ) ) );
            }
         }
         minHeap.pop();
      }
   }

   void print( int start, int end ) {
      std::cout << "start: " << start << " end: " << end << std::endl;
      std::cout << result.p_ << " cost: " << result.cost_ << std::endl;
   }
};

int main() {
   std::vector<std::vector<int>> graph1{
      {0, 3, 10000, 100, 10},
      {10000, 0, 3, 3, 3},
      {10000, 10000, 0, 10000, 10000},
      {2, 2, 2, 0, 10000},
      {15, 20, 212, 10, 0}
   };

   Djikstra d1( graph1 );
   d1.process( 0, 2 ); 
   d1.print(0, 2);
   
   std::vector<std::vector<int>> graph2{
      {0, 3, 10000, 100, 10},
      {10000, 0, 3, 3, 3},
      {10000, 10000, 0, 10000, 10000},
      {2, 2, 2, 0, 10000},
      {15, 20, 212, 10, 0}
   };

   Djikstra d2( graph2 );
   d2.process(2, 0);
   d2.print(2, 0);
}

