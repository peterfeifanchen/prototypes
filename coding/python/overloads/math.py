#!/usr/bin/python

import math

# Overloading binary operators in Python.
# NOTE: it is not currently possible to overload logical operators

class Unnatural( object ):
   def __init__( self, x ):
      self.x = x

   def __add__( self, obj ):
      print( "Overloaded + operation" )
      return Unnatural( self.x + obj.x )

   def __sub__( self, obj ):
      print( "Overloaded - operation" )
      return Unnatural( self.x - obj.x )

   def __mul__( self, obj ):
      print( "Overloaded * operation" )
      return Unnatural( self.x * obj.x )

   def __truediv__( self, obj ):
      print( "Overloaded / operation" )
      return Unnatural( self.x / obj.x )

   def __floordiv__( self, obj ):
      print( "Overloaded // operation" )
      return Unnatural( self.x // obj.x )

   def __pow__( self, obj ):
      print( "Overloaded ** operation" )
      return Unnatural( self.x ** obj.x )

   def __and__( self, obj ):
      print( "Overloaded & operation" )
      return Unnatural( self.x & obj.x )

   def __mod__( self, obj ):
      print( "Overloaded % operation" )
      return Unnatural( self.x % obj.x )

   def __or__( self, obj ):
      print( "Overloaded | operation" )
      return Unnatural( self.x | obj.x )

   def __xor__( self, obj ):
      print( "Overloaded ^ operation" )
      return Unnatural( self.x ^ obj.x )

   def __lshift__( self, obj ):
      print( "Overloaded << operation" )
      return Unnatural( self.x << obj.x )

   def __rshift__( self, obj ):
      print( "Overloaded >> operation" )
      return Unnatural( self.x >> obj.x )

   def __str__( self ):
      return f"Unnatural({self.x})"

if __name__ == '__main__':
   u1 = Unnatural(3)
   u2 = Unnatural(2)

   r1 = u1 + u2
   print( str(u1) + "+" + str(u2) + "=" + str(r1) )
   r2 = u1 - u2
   print( str(u1) + "-" + str(u2) + "=" + str(r2) )
   r3 = u1 * u2
   print( str(u1) + "*" + str(u2) + "=" + str(r3) )
   r4 = u1 / u2
   print( str(u1) + "/" + str(u2) + "=" + str(r4) )
   r5 = u1 // u2
   print( str(u1) + "//" + str(u2) + "=" + str(r5) )
   r6 = u1 ** u2
   print( str(u1) + "**" + str(u2) + "=" + str(r6) )
   r7 = u1 % u2
   print( str(u1) + "%" + str(u2) + "=" + str(r7) )
   r8 = u1 & u2
   print( str(u1) + "&" + str(u2) + "=" + str(r8) )
   r9 = u1 | u2
   print( str(u1) + "|" + str(u2) + "=" + str(r9) )
   r10 = u1 ^ u2
   print( str(u1) + "^" + str(u2) + "=" + str(r10) )
   r11 = u1 << u2
   print( str(u1) + "<<" + str(u2) + "=" + str(r11) )
   r12 = u1 >> u2
   print( str(u1) + ">>" + str(u2) + "=" + str(r12) )
