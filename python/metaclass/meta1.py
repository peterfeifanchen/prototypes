#!/usr/bin/python

###################################
# type as a function
class Foobar(object):
    foobar = 'value'
    pass

type(Foobar)
# <class 'type'>

foo = Foobar()
type(foo)
# <class '__main__.Foobar'>

# when you evaluate object lookup 'foo.foobar', this equates to 
#   1) return value if it exists in 'Foobar.__dict__'
#   2) return 'Foobar.__getattr__('foobar')

MyClass = type( 'MyClass', (), {} )
type(MyClass)
# <class 'type' >
MyClass
# <class '__main__.MyClass'>

###################################
# structure is instance ->  class -> metaclass
#                          (Complex)   (Meta)

class Meta(type):
    foobar = 'value'
    pass

class Complex(metaclass=Meta):
    pass

type(Complex)
#<class '__main__.Meta'>

Complex.__init__
#<slot wrapper '__init__' of 'object' objects>
Complex.__base__
#<class 'object' >
type(Complex)
#<class '__main__.Meta'>

Meta.__init__
#<slot wrapper '__init__' of 'type' objects>
Meta.__base__
#<class 'type'>
type(Meta)
#<class 'type'>


# slots are the fields in a struct for each magic method define
# for a class. These struct fields are pointers to each of these
# magic methods.
#
# this is not to be confused with __slots__, which will create 
# instances without __dict__ and a more restricted set of attributes


# when you evaluate class lookup 'Complex.foobar', this equates to 
#   1) return value if it exists in 'Complex.__dict__'
#   2) return 'Complex.__getattr__('foobar')

# in essence, just as foo is an instance of class Foobar, Complex is
# an instance of the metaclass Meta. 
####################################
