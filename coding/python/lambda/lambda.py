#!/usr/bin/python
if __name__ == '__main__':
   f = lambda x, y: x*y
   print( 'f(2,3)=', f(2,3) )

   o = list( map( lambda x: x+1, [1,2,3,4] ) )
   print( 'increment by 1 every item:', o )
