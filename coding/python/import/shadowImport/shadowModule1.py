#!/usr/bin/python

import sys
import builtins
import myModule1.mySubModule1

class myModule1Meta( type ):
    def __getattr__( self, name ):
        return getattr( sys.modules[ 'myModule1' ], name )

    class mySubModule1Meta( type ):
        def __getattr__( self, name ):
            return getattr( sys.modules[ 'myModule1' ].mySubModule1, name )

class myModule1(metaclass=myModule1Meta):
    class mySubModule1(metaclass=myModule1Meta.mySubModule1Meta):
        class myClass( sys.modules[ 'myModule1' ].mySubModule1.myClass ):
            def run( self ):
                print( "shadowModule1::myModule1::mySubModule1::myClass run" )


def _shadowSetAttrMyModule1( shadowModule, originalModule, fromlist ):
    if fromlist:
        for fromItem in fromlist:
            if fromItem not in dir( shadowModule ):
                val = getattr( originalModule, fromItem )
                setattr( shadowModule, fromItem, val )

_savedImport = builtins.__import__
def _shadowImport1( moduleName, globals={}, locals={}, fromlist=[], level=-1 ):
    if moduleName == "myModule1" or moduleName == "myModule1.mySubModule1":
        mod = _savedImport( "shadowModule1", globals, locals, [], level )
        if moduleName == "myModule1":
            _shadowSetAttrMyModule1( mod.myModule1, sys.modules[ "myModule1" ], fromlist )
            return mod.myModule1
        else:
            _shadowSetAttrMyModule1( mod.myModule1.mySubModule1, 
                                     sys.modules[ "myModule1" ].mySubModule1, 
                                     fromlist )
            if fromlist:
                return mod.myModule1.mySubModule1
            return mod.myModule1
    return _savedImport( moduleName, globals, locals, fromlist, level )
builtins.__import__ = _shadowImport1
