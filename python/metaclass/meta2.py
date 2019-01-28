#!/usr/bin/python

###############################
# Class creation
#   1) Metaclass __prepare__
#   2) Metaclass __new__
#   3) Metaclass __init__
#
# Instance creation
#   1) metaclass __call__
#   2) metaMetaClass __call__
#   2) class __new__
#   3) class __init__
class aMetaMeta(type):
    def __call__(cls, *args, **kwargs):
        print( ' aMetaMeta.__call__(cls=%s, args=%s, kwargs=%s)' % (
            cls, args, kwargs ) )
        return super().__call__(*args, **kwargs)

class aMeta(aMetaMeta):
    @classmethod
    def __prepare__( mcs, name, bases, **kwargs ):
        print( ' aMeta.__prepare__(mcs=%s, name=%r, bases=%s, **%s)' % (
            mcs, name, bases, kwargs ) )
        # __prepare__ is called before 'class' body is executed, it must
        # return a dict-like object that is used as local namespace for
        # code in the class body. The returned object will be later passed
        # to __new__. 
        return {'dummy1': 1}
   
    def __new__(mcs, name, bases, attrs, **kwargs ):
        print( ' aMeta.__new__(mcs=%s, name=%r, bases=%s, attrs=[%s], **%s)' % (
            mcs, name, bases, ', '.join(attrs), kwargs ) )
        attrs['dummy2'] = 2
        return super().__new__(mcs, name, bases, attrs )

    def __init__(cls, name, bases, attrs, **kwargs ):
        print( ' aMeta.__init__(cls=%s, name=%r, bases=%s, attrs=[%s], **%s)' % (
            cls, name, bases, ', '.join(attrs), kwargs ) )
        attrs['dummy3'] = 3
        return super().__init__(name, bases, attrs )

    def __call__(cls, *args, **kwargs):
        print( ' aMeta.__call__(cls=%s, args=%s, kwargs=%s)' % (
            cls, args, kwargs ) )
        return super().__call__(*args, **kwargs)

class superClass():
    pass

class aFoobar( superClass, metaclass=aMeta, extra1=1, extra2=2):
    def __new__(cls, myarg):
        print( ' aFoobar.__new__(cls=%s, myarg=%s)' % (
            cls, myarg ) )
        return super().__new__(cls)

    def __init__(self, myarg):
        self.myarg = myarg
        print(' aFoobar.__init__(self=%s, myarg=%s)' % ( 
            self, myarg ) )
        return super().__init__()

    def __str__(self):
        return "<instance of Foobar; myargs=%s>" % (
            getattr(self, 'myarg', 'MISSING') )

foo = aFoobar( myarg='someArg' )
newFoobar = aMeta( "newFoobar", ( aFoobar, ), {}, **{'extra':1} )
newFoo = newFoobar(2)

###############################
# Metaclass are inherited by subclasses. Metaclass can be any
# callable, including functions. However subclasses won't inherit
# the function, but instead the type of whatever the function returns.
class bMetaMeta(type):
    pass

class bMeta(bMetaMeta):
    x = 5
    @classmethod
    def __prepare__( mcs, name, bases, **kwargs ):
        print( ' bMeta.__prepare__(mcs=%s, name=%r, bases=%s, **%s)' % (
            mcs, name, bases, kwargs ) )
        # __prepare__ is called before 'class' body is executed, it must
        # return a dict-like object that is used as local namespace for
        # code in the class body. The returned object will be later passed
        # to __new__. 
        return {'dummy1': 1}

def bCallableMeta( name, bases, attrs, **kwargs ):
    return bMeta

class bFoobar(metaclass=bCallableMeta):
    pass

bFoobar
type(bFoobar)
breakpoint()

###############################
# class may inherit from multiple metaclasses as long
# as there is a common leaf
#
# for example, this would throw a metaclass conflict TypeError:
#
# class cMeta1(type):
#    pass
#
# class cMeta2(type):
#    pass
#
# class cBase1(metaclass=Meta1):
#    pass
#
# class cBase2(metaclass=Meta2);
#    pass
#
# class cFoobar(Base1, Base2):
#    pass

class cMeta(type):
    pass

class cSubMeta(Meta):
    pass

class cBase1(metaclass=Meta):
    pass

class cBase2(metaclass=SubMeta):
    pass

class cFoobar(Base1, Base2):
    pass

type(cFoobar)
#<class '__main__.SubMeta'>
