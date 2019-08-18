#!/usr/bin/python

import math

class Unnatural( object ):
   def __init__( self, x ):
      self.x = x

   def __gt__( self, obj ):
      print( "Overloaded > operation" )
      return self.x > obj.x

   def __ge__( self, obj ):
      print( "Overloaded >= operation" )
      return self.x >= obj.x

   def __lt__( self, obj ):
      print( "Overloaded < operation" )
      return self.x < obj.x

   def __le__( self, obj ):
      print( "Overloaded <= operation" )
      return self.x <= obj.x

   def __eq__( self, obj ):
      print( "Overloaded == operation" )
      return self.x == obj.x

   def __neq__( self, obj ):
      print( "Overloaded != operation" )
      return self.x != obj.x
   
   def __str__( self ):
      return f"Unnatural({self.x})"

if __name__ == '__main__':
   u1 = Unnatural(3)
   u2 = Unnatural(2)
   u3 = Unnatural(3)

   if u1 > u2:
      print( str(u1) + ">" + str(u2) )

   if u1 >= u3:
      print( str(u1) + ">=" + str(u3) )

   if u2 < u1:
      print( str(u2) + "<" + str(u1) )
   
   if u3 <= u1:
      print( str(u3) + "<=" + str(u1) )
   
   if u3 == u1:
      print( str(u3) + "==" + str(u1) )
   
   if u2 != u1:
      print( str(u2) + "!=" + str(u1) )
