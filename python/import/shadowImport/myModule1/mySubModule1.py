#!/usr/bin/python

def func1():
    print( "myModule1::mySubModule1::func1 called" )

def func2():
    print( "myModule1::mySubModule1::func2 called" )

class myClass( object ):
    def run( self ):
        print( "myModule1::mySubModule1::myClass::Run method called" )
    def printName( self ):
        print( "myModule1::mySubModule1::myClass::printName method called" )
