#include<iostream>
#include<vector>
#include<climits>
#include<iomanip>

// Floyd-Warshall is an algorithm running in O(n^3), where n is the number of 
// vertices, that gives the shortest path between all sources and all 
// destinations.
//
// Edges can be positive or negative weights, but must not have negative cycles.
// Edges are directed.
// Does not show the shortest path, but can with modifications to the algorithm.
class FloydWarshall {
 public:
   // Graph
   std::vector<std::vector<int>> g_;
   // Lowest cost distance graph
   std::vector<std::vector<int>> r_;
   // Number of nodes
   int nodes_;

   FloydWarshall( std::vector<std::vector<int>> g ) {
      for( int i = 0; i < g.size(); i++ ) {
         g_.push_back(g[i]);
         r_.push_back( g[i] );
      }
      nodes_ = g_.size();
   }

   class Path{
    public:
      std::vector<int> path;
   };

   // The algorithm, shortest path between any two nodes. Essentially what it does
   // is that it iteratively tests the optimum path from src i to dst j through the
   // optimum path from src i to dst k and then from src k to dst j. 
   void process() {
      for( int k = 0; k < nodes_; k++ ) {
         for( int i=0; i < nodes_; i++ ) {
            for( int j=0; j < nodes_; j++ ) {
               r_[i][j] = std::min( r_[i][j], r_[i][k] + r_[k][j] );  
            }
         }
      }
   }

   // The algorithm, shortest path between any two nodes. Prints the path for 
   // (src, dst).
   void processAndPrintPaths() {
     
      // Initialize path for each (src,dst);
      std::vector<std::vector<Path>> paths( nodes_, std::vector<Path>( nodes_ ) );
      for( int i = 0; i < nodes_; i++ ) {
         for( int j = 0; j < nodes_; j++ ) {
            paths[i][j].path.push_back(i);
            paths[i][j].path.push_back(j);
         }
      }

      for( int k = 0; k < nodes_; k++ ) {
         for( int i=0; i < nodes_; i++ ) {
            for( int j=0; j < nodes_; j++ ) {
               if( r_[i][k] + r_[k][j] < r_[i][j] ) {
                  r_[i][j] = r_[i][k] + r_[k][j];
                  paths[i][j].path = std::vector<int>( paths[i][k].path );
                  paths[i][j].path.insert( paths[i][j].path.end(),
                        paths[k][j].path.begin()+1, paths[k][j].path.end() );
               }
            }
         }
      }

      for( int i = 0; i < nodes_; i++ ) {
         for( int j = 0; j < nodes_; j++ ) {
            std::cout << "Src " << i << " to Dst " << j << ": ";
            for( int k = 0; k < paths[i][j].path.size()-1; k++ ) {
               std::cout << paths[i][j].path[k] << "->";
            }
            std::cout << paths[i][j].path[paths[i][j].path.size()-1] << std::endl;
         }
      }
   }

   // Negative cycles can be detected by looking at the diagonal of the graph matrix.
   // If at any point they are negative, this means that there is a negative cycle.
   // This is because for the lowest cost path for node i to node i (i,i) must be 0.
   // If at any point (i, j, k, l, ..., i) < 0, then there must be a negative cycle,
   // and this path would be discovered by Floyd-Warshall. 
   bool negativeCycle() {
      for( int k = 0; k < nodes_; k++ ) {
         for( int i=0; i < nodes_; i++ ) {
            for( int j=0; j < nodes_; j++ ) {
               r_[i][j] = std::min( r_[i][j], r_[i][k] + r_[k][j] );  
            }
         }

         for( int i = 0; i < nodes_; i++ ) {
            if( r_[i][i] < 0 ) return true;
         }
      }

      return false;
   }

   void print() {
      std::cout << "Input graph: " << std::endl;
      for( auto &v : g_ ) {
         for( auto &i : v ) {
            std::cout << std::right << std::setw(12) << i << " ";
         }
         std::cout << std::endl;
      }
      std::cout << std::endl;
      std::cout << "Floyd-Warshall distance: " << std::endl;
      for( auto &v : r_ ) {
         for( auto &i : v ) {
            std::cout << std::right << std::setw(12) << i << " ";
         }
         std::cout << std::endl;
      }
   }
};

int main() {
   // example from wikipedia
   std::vector<std::vector<int>> graph1{
      { 0, 10000, -2, 10000 },
      { 4, 0, 3, 10000 },
      { 10000, 10000, 0, 2 },
      { 10000, -1, 10000, 0 } };

   FloydWarshall fw1( graph1 );
   fw1.processAndPrintPaths();
   fw1.print();
   std::cout << std::endl << "Graph has a negative cycle? " 
            <<  ( fw1.negativeCycle() ? "true" : "false" ) << std::endl;
   
   std::vector<std::vector<int>> graph2{
      { 0, 10000, -2, 10000 },
      { 4, 0, 3, -1 },
      { 10000, 10000, 0, 2 },
      { 10000, -1, 10000, 0 } };

   FloydWarshall fw2( graph2 );
   fw2.processAndPrintPaths();
   fw2.print();
   std::cout << std::endl << "Graph has a negative cycle? " 
            <<  ( fw2.negativeCycle() ? "true" : "false" ) << std::endl;

   
}
