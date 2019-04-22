#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <string>

using namespace std;

string ltrim(const string &str) {
   string s(str);
   // This is a bit interesting.
   // std::find_if returns an iterator to the first element where the
   //   unary predicate std::not1() returns true between the range 
   //   [ s.begin(), s.end() ). It returns the same type of iterators as the
   //   iterators passed in as a range. If the iterators passed in as parameters are
   //   reverse iterators, this returns reverse iterators.
   // std::not1 provides takes in an unary predicate as a constructor parameter and
   //   creates an unary predicate that is its complement.
   // std::ptr_fun turns a function int isspace(int) into an unary predicate object
   //   as its first template parameter is the type of the arg and second is type of
   //   the return object.
   s.erase( s.begin(), find_if(s.begin(), s.end(), 
         not1(ptr_fun<int, int>(isspace))) );
   return s;
}


string rtrim(const string &str) {
   string s(str);
   // Here we have the same as ltrim. Except we use reverse iterators from the end
   // of the string s to clear trailing spaces. Reverse iterators physically point
   // to an element N, would actually dereference element N-1 when * operator is
   // applied. Here because std::find_if returns a reverse iterator, base() is 
   // called on the base iterator to turn it into the underlying forward iterator.
   s.erase( find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
         s.end() );
   return s;
}

// Reads in the following format
//  integer - number of integers to read
//  integer 1
//  integer 2
//  integer ...
//  integer k - some number

int main() {
   // This can take in a filename/filemode, but in this case for simplicity we
   // tried giviing it the stream buffer of stdout and use the default empty 
   // constructor. 
   //
   //   ofstream fout;
   //   fout.rdbuf(cout.rdbuf());
   //
   // However rdbuf for ofstream do not have the set signature, only get, unlike
   // the ostream signature on cout.
   ofstream fout( "file.txt", ios::out);

   string arr_count_temp;
   getline(cin, arr_count_temp);

   // Trim out spaces and convert to integer
   int arr_count = stoi(ltrim(rtrim(arr_count_temp)));

   vector<int> arr(arr_count);

   for (int i = 0; i < arr_count; i++) {
      string arr_item_temp;
      getline(cin, arr_item_temp);

      // Trim out spaces and convert to integer
      int arr_item = stoi(ltrim(rtrim(arr_item_temp)));
      arr[i] = arr_item;
   }
   
   string k_temp;
   getline(cin, k_temp);

   // Trim out spaces and convert to integer
   int k = stoi(ltrim(rtrim(k_temp)));

   // Print out what we read in
   fout << arr_count << "\n";
   for (int i = 0; i < arr_count; i++) {
      fout << arr[i] << "\n";
   }
   fout << k << "\n";
   fout.close();
   
   return 0;
}
