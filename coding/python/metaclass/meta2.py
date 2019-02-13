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
print( '\033[4mMetaclass __prepare__, __new__, __init__, __call__\033[0m' )
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

# aFoobar inherits from superClass and is a child of metaclass aMeta. The args
# 'extra' are fed into the metaclass prepare method which creates the namespace
# for aFoobar's attributes. In a way, you can even drop in customizations.
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
# aMeta.__prepare__(
#   mcs=<class '__main__.aMeta'>, 
#   name='aFoobar', 
#   bases=(<class '__main__.superClass'>,), 
#   **{'extra1': 1, 'extra2': 2})
# aMeta.__new__(
#   mcs=<class '__main__.aMeta'>, 
#   name='aFoobar', 
#   bases=(<class '__main__.superClass'>,), 
#   attrs=[dummy1, __module__, __qualname__, __new__, __init__, __str__, __classcell__], 
#   **{'extra1': 1, 'extra2': 2})
# aMeta.__init__(
#   cls=<class '__main__.aFoobar'>, 
#   name='aFoobar', 
#   bases=(<class '__main__.superClass'>,), 
#   attrs=[dummy1, __module__, __qualname__, __new__, __init__, __str__, __classcell__, dummy2], 
#   **{'extra1': 1, 'extra2': 2})
# aMeta.__call__(
#   cls=<class '__main__.aFoobar'>, 
#   args=(), 
#   kwargs={'myarg': 'someArg'})                 
# aMetaMeta.__call__(
#   cls=<class '__main__.aFoobar'>, 
#   args=(), 
#   kwargs={'myarg': 'someArg'})             
# aFoobar.__new__(
#   cls=<class '__main__.aFoobar'>, 
#   myarg=someArg)                                       
# aFoobar.__init__(
#   self=<instance of Foobar; myargs=someArg>, 
#   myarg=someArg)  

# newFoobar is a new class created from metaclass aMeta. 
newFoobar = aMeta( "newFoobar", ( aFoobar, ), {}, **{'extra':1} )
# aMeta.__new__(
#   mcs=<class '__main__.aMeta'>, 
#   name='newFoobar', 
#   bases=(<class '__main__.aFoobar'>,), 
#   attrs=[],
#   **{'extra': 1})
# aMeta.__init__(
#   cls=<class '__main__.newFoobar'>, 
#   name='newFoobar', 
#   bases=(<class '__main__.aFoobar'>,), 
#   attrs=[dummy2], 
#   **{'extra': 1})
newFoo = newFoobar(2)
# aMeta.__call__(cls=<class '__main__.newFoobar'>, args=(2,), kwargs={})
# aMetaMeta.__call__(cls=<class '__main__.newFoobar'>, args=(2,), kwargs={})  
# aFoobar.__new__(cls=<class '__main__.newFoobar'>, myarg=2)   
# aFoobar.__init__(self=<instance of Foobar; myargs=2>, myarg=2)       
print('\n')

###############################
# Metaclass are inherited by subclasses. Metaclass can be any
# callable, including functions. However subclasses won't inherit
# the function, but instead the type of whatever the function returns.
print( '\033[4mMetaclass as a callable function\033[0m' )
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
    def __new__(mcs, name, bases, attrs, **kwargs ):
        print( ' bMeta.__new__(mcs=%s, name=%r, bases=%s, attrs=[%s], **%s)' % (
            mcs, name, bases, ', '.join(attrs), kwargs ) )
        attrs['dummy2'] = 2
        return super().__new__(mcs, name, bases, attrs )

def bCallableMeta( name, bases, attrs, **kwargs ):
    # kwargs = {'extraArg':1}
    attrs['dummy3'] = 3
    return bMeta( name, bases, attrs, **kwargs )

def bCallableMeta2( name, bases, attrs, **kwargs ):
    return bMeta

class bFoobar(metaclass=bCallableMeta, extraArg=1):
    pass

class bFoobar2(metaclass=bMeta):
    pass

class bFoobar3(metaclass=bCallableMeta2):
    pass


bFoobar
#<class '__main__.bFoobar'>
type(bFoobar)
#<class '__main__.bMeta'>

bFoobar2
#<class '__main__.bMeta'>
try:
    bFoobar.dummy1
except Exception as e:
    print( e.__class__.__name__ + ": " + str(e) )
bFoobar.dummy2
bFoobar.dummy3
type(bFoobar2)
#<class '__main__.bMeta'>
bFoobar2.dummy1
bFoobar2.dummy2

bFoobar3
#<class '__main__.bMeta'>
type(bFoobar3)
#<class 'type'>
print('\n')

###############################
# class may inherit from multiple metaclasses as long
# as there is a common leaf
#
# for example, this would throw a metaclass conflict TypeError:
#
print( '\033[4mMetaclass multiple inheritance\033[0m' )
class cMeta1(type):
    pass

class cMeta2(type):
    pass

class cBase1(metaclass=cMeta1):
    pass

class cBase2(metaclass=cMeta2):
    pass

try:
    class cFoobar(cBase1, cBase2):
        pass
except Exception as e:
    print( e.__class__.__name__ + ": " + str(e) )

class dMeta(type):
    pass

class dSubMeta(dMeta):
    pass

class dBase1(metaclass=dMeta):
    pass

class dBase2(metaclass=dSubMeta):
    pass

class dFoobar(dBase1, dBase2):
    pass

type(dFoobar)
#<class '__main__.SubMeta'>
