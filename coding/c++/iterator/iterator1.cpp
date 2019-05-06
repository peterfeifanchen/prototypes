#include<vector>
#include<string>
#include<iostream>
#include<assert.h>

template< typename Iterator > 
class StdIterator {
 public:      
   typedef Iterator value_type; 
   typedef value_type * pointer;             
   typedef value_type & reference;               
   typedef size_t difference_type;                   
   typedef std::forward_iterator_tag iterator_category;
                                             
   StdIterator( Iterator iter ) : iter_( std::move(iter) ) {}
                                                  
   pointer operator->() {                    
      assert( iter_ );                   
      return &iter_;                  
   }                        
                 
   reference operator*() {  
      //assert( iter_ );                   
      return iter_;
   }
                                
   bool operator==( StdIterator const & rhs ) const { return iter_ == rhs.iter_; }
   bool operator!=( StdIterator const & rhs ) const { return iter_ != rhs.iter_; }
                
   // Prefix ++
   StdIterator & operator++() {
      assert( iter_ );
      ++iter_;
      return *this;
   }
 
   // Postfix ++
   StdIterator operator++( int ) {
      assert( iter_ );
      StdIterator self{ *this };
      ++( *this );
      return self;
   }

   void tempfunc();

 private:
   Iterator iter_;
};

template< typename Iterator>
void StdIterator<Iterator>::tempfunc() {
   std::cout << "iterator" << std::endl;
}

int main( int argc, char** argv ) {
   std::vector<int> dummy{1,2,3,4,5};

   int i = 0;
   for( auto iter = dummy.begin(); iter != dummy.end(); iter++ ) {
      std::cout << "v: " << *iter 
                << " iter addr: " << &iter 
                << " vector addr: " << &dummy[i] << std::endl;
      i++;
   }

   auto iter = dummy.begin(); 
   std::cout << *iter << std::endl;
   StdIterator<std::vector<int>::iterator> stdIter( iter );
   std::cout << &(*(*stdIter)) << std::endl;
   std::cout << *iter << std::endl;
   stdIter.tempfunc();
}
