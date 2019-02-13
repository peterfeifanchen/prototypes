#!/usr/bin/python

import builtins

def run( self ):
    print( "myShadowModule2::run called" )

_savedImport = builtins.__import__
def _shadowImport2( moduleName, globals={}, locals={}, fromlist=[], level=-1 ):
    mod = _savedImport( moduleName, globals, locals, fromlist, level )
    if moduleName == "myModule1.mySubModule1": 
        lib = mod
        if not fromlist:    
            lib = mod.mySubModule1
        cls = lib.myClass
        setattr( cls, 'run', run )
    return mod
builtins.__import__ = _shadowImport2
