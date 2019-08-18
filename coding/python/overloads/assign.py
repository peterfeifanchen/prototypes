#!/usr/bin/python

import math

# Overloading assignment operators in Python.

class Unnatural( object ):
   def __init__( self, x ):
      self.x = x

   def __iadd__( self, obj ):
      print( "Overloaded += operation" )
      self.x = self.x + obj.x
      return self

   def __isub__( self, obj ):
      print( "Overloaded -= operation" )
      self.x = self.x - obj.x
      return self

   def __imul__( self, obj ):
      print( "Overloaded * operation" )
      self.x = self.x * obj.x
      return self

   def __itruediv__( self, obj ):
      print( "Overloaded / operation" )
      self.x = self.x / obj.x
      return self

   def __ifloordiv__( self, obj ):
      print( "Overloaded // operation" )
      self.x = self.x // obj.x
      return self

   def __ipow__( self, obj ):
      print( "Overloaded ** operation" )
      self.x = self.x ** obj.x
      return self

   def __iand__( self, obj ):
      print( "Overloaded & operation" )
      self.x = int(self.x) & int(obj.x)
      return self

   def __imod__( self, obj ):
      print( "Overloaded % operation" )
      self.x = int(self.x) % int(obj.x)
      return self

   def __ior__( self, obj ):
      print( "Overloaded | operation" )
      self.x = int(self.x) | int(obj.x)
      return self

   def __ixor__( self, obj ):
      print( "Overloaded ^ operation" )
      self.x = int(self.x) ^ int(obj.x)
      return self

   def __ilshift__( self, obj ):
      print( "Overloaded << operation" )
      self.x = int(self.x) << int(obj.x)
      return self

   def __irshift__( self, obj ):
      print( "Overloaded >> operation" )
      self.x = int(self.x) >> int(obj.x)
      return self

   def __str__( self ):
      return f"Unnatural({self.x})"

if __name__ == '__main__':
   u1 = Unnatural(3)
   u2 = Unnatural(2)

   print( str(u1) )
   u1 += u2
   print( "+=" + str(u2) + "->" + str(u1) )
   u1 -= u2
   print( "-=" + str(u2) + "->" + str(u1) )
   u1 *= u2
   print( "*=" + str(u2) + "->" + str(u1) )
   u1 /= u2
   print( "/=" + str(u2) + "->" + str(u1) )
   u1 //= u2
   print( "//=" + str(u2) + "->" + str(u1) )
   u1 **= u2
   print( "**=" + str(u2) + "->" + str(u1) )
   u1 %= u2
   print( "%=" + str(u2) + "->" + str(u1) )
   u1 &= u2
   print( "&=" + str(u2) + "->" + str(u1) )
   u1 |= u2
   print( "|=" + str(u2) + "->" + str(u1) )
   u1 ^= u2
   print( "^=" + str(u2) + "->" + str(u1) )
   u1 <<= u2
   print( "<<=" + str(u2) + "->" + str(u1) )
   u1 >>= u2
   print( ">>=" + str(u2) + "->" + str(u1) )
