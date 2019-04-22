#include <fstream>
#include <iostream>

// This is an exception-safe RAII way of replacing std::cout stream buf with that of
// a file buffer. For non-exception safe ways, see stackflow reference in README.

struct opiped {
   opiped(std::streambuf * buf, std::ostream & os) : os(os), 
      old_buf(os.rdbuf(buf)) { }
   
   ~opiped() { os.rdbuf(old_buf); }

   std::ostream& os;
   std::streambuf * old_buf;
};

int main() {
   std::ofstream of("file.txt");
   {
      opiped raii(of.rdbuf(), std::cout);
      std::cout << "going into file" << std::endl;
   }
                                                    
   std::cout << "going on screen" << std::endl;   
}

