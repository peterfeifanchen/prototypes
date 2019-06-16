#include<iostream>
#include<exception>
#include<stdexcept>

class myException : public std::exception {
   const std::string file;
   const int line;
   const std::string func;
   const std::string info;
   const std::string msg;
                   
 public:
   myException( const char* msg_, 
         const char* file_, 
         int line_, 
         const char* func_, 
         const char* info_ = "") : std::exception(), msg(msg_),
         file(file_), line(line_), func(func_), info(info_) {}

   const char* get_file() const { return file.c_str(); }       
   int get_line() const { return line; }
   const char* get_func() const { return func.c_str(); }
   const char* get_info() const { return info.c_str(); }
                                                         
   const char * what() const throw() {
      return ( std::string( std::exception::what() ) + " " + msg ).c_str(); 
   }
};

// You can throw an int, class, or exception
class Fail {
   std::string msg_;
 public:
   Fail( std::string msg ) : msg_(msg) {}

   const std::string& print() const { 
      return msg_;
   }
};

// This method can only throw Fail
void fail() throw(Fail) {
   throw Fail( "Failed" );
}

void func() {
   throw std::overflow_error("Divide by zero exception");
}

void myFunc() {
   throw myException( "MyException was thrown", __FILE__, __LINE__, __func__, 
         "Extra information" );
}

int main() {
   // try...catch that declares a formal parameter
   try{
      func();
   } catch ( const std::exception &e ) {
      std::cout << "I caught exception " << e.what() << std::endl;
   }
   
   // try...catch that catches a specific exception
   try{
      myFunc();
   } catch ( const std::overflow_error &e ) {
      std::cout << "I caught exception " << e.what() << std::endl;
   } catch ( const myException &e ) {
      std::cout << "I caught exception " << e.what() << std::endl;
      std::cout << "    " << e.get_file() << ":" << e.get_func() << ": " 
         << e.get_line() << "    " << e.get_info() << std::endl;
   }
   
   // try..catch all
   try {
      fail();
   } catch ( const Fail &f) {
      std::cout << f.print() << std::endl;
   }

   try {
      throw 10;
   } catch ( const int &i ) {
      std::cout << "Throw " << i << std::endl;
   }

   // try..catch all
   try {
      myFunc();
   } catch (...) {
      std::cout << "Some exception occured" << std::endl;
   }
}
