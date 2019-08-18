#!/usr/bin/python

class String( object ):
   def __init__( self, initStr ):
      self.initStr = initStr

   def __str__( self ):
      #  {var} to format string
      # {{ }} to escape and print literal '{' and '}'
      return f"Overloaded {{{self.initStr}}}"

if __name__ == '__main__':
  print(  str( String( 'hello' ) ) )
