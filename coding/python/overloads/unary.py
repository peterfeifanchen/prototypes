#!/usr/bin/python

class Unnatural( object ):
   def __init__( self, x ):
      self.x = x

   def __neg__( self ):
      print( "Overloaded - operator" )
      return Unnatural(-self.x)

   def __pos__( self ):
      print( "Overloaded + operator" )
      return Unnatural(+self.x)

   def __abs__( self ):
      print( "Overloaded abs operation" )
      return Unnatural(abs(self.x))

   def __invert__( self ):
      print( "Overloaded ~ operation" )
      return Unnatural(~self.x)

   def __complex__( self ):
      print( "Overloaded complex operation" )
      return complex( self.x )

   def __int__( self ):
      print( "Overloaded int operation" )
      return int( self.x )
   
   def __float__( self ):
      print( "Overloaded float operator" )
      return float(self.x)
   
   def __index__( self ):
      print( "Overloaded oct/hex operator - returns str in base 8/16" )
      return self.x
   
   def __str__( self ):
      return f"Unnatural({self.x})"

if __name__ == '__main__':
   u1 = Unnatural(3)
   u2 = Unnatural(-4)

   r1 = -u1
   print( "neg(" + str(u1) + ")=" + str(r1) )
   r2 = +u2
   print( "pos(" + str(u2) + ")=" + str(r2) )
   r3 = abs(u2)
   print( "abs(" + str(u2) + ")=" + str(r3) )
   r4 = ~u2
   print( "invert(" + str(u2) + ")=" + str(r4) )
   r5 = complex(u2)
   print( "complex(" + str(u2) + ")=" + str(r5) )
   r6 = int(u2)
   print( "float(" + str(u2) + ")=" + str(r6) )
   r7 = float(u2)
   print( "int(" + str(u2) + ")=" + "%.2f" % r7 )
   r8 = oct(u2)
   print( "oct(" + str(u2) + ")=" + str(r8) )
   r9 = hex(u2)
   print( "hex(" + str(u2) + ")=" + str(r9) )
