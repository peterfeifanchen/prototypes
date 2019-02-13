#!/usr/bin/python

# import can be overriden and customized. For example, the following imports
# a shadow module if `import myModule`, `import myModule.mySubModule` or 
# `from myModule.mySubModule import myClass` is called and redirects it to 
# my customModule.

# We override the import function to insert our own shadowModule. ShadowModule
# contains a subclass of myModule.mySubModule.myClass which replaces 
# myModule.mySubModule.myClass. However, other attr in mySubModule and myClass
# remain available.

# We present two methods:
#   1) replace mySubModule1 and myClass entirely. This requires using getters on
#      mySubModule1 by the shadowModule import interception to create missing
#      attributes and redirect get calls to the real mySubModule1. Further, to
#      replicate the hierarchical module relationship using nested classes, we
#      use metaclasses. This is because __getattr__ are only called on instances
#      not types. By using metaclasses, our class now because an instance of
#      the metaclass.
#   2) using setattr to override myClass methods


# Comment shadowModule1 or shadowModule2 to see run() of 
# myModule1.mySubModule1.myClass run different functions
import shadowModule1
#import shadowModule2

import myModule1.mySubModule1
from myModule1.mySubModule1 import myClass, func2
from myModule1 import func3

breakpoint()
widget = myModule1.mySubModule1.myClass()
# Calls 'run' method of shadowModule1
widget.run()
# Calls 'printName' method of myModule1
widget.printName()
# We are still able to call the attributes in myModule1 even though it is a
# shadowModule.
myModule1.mySubModule1.func1()

# Calls 'run' method of shadowModule
myClass().run()
# We can still call the attributes in mySubModule1
func2()
# And we can still call the attributes of myModule1
func3()
myModule1.func4()
