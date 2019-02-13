#!/usr/bin/python

# import is a placeholder for calling __builtin__.__import__.
#
# For
#
#   import os
#
# Call
os = __import__( 'os', globals(), locals(), [], 0 )
breakpoint()
print(os)
# <module 'os' from 
#  '/Library/Frameworks/Python.framework/Versions/3.7/lib/python3.7/os.py'>

# For 
#
#   import os.path
#
# Call
os = __import__( 'os.path', globals(), locals(), [], 0 )
print(os)
# <module 'os' from 
#  '/Library/Frameworks/Python.framework/Versions/3.7/lib/python3.7/os.py'>
print(os.path)
# <module 'posixpath' from 
#  '/Library/Frameworks/Python.framework/Versions/3.7/lib/python3.7/posixpath.py'>

# For
#
#   from os.path import basename, dirname
#
# Call
_temp = __import__( 'os.path', globals(), locals(), [ 'basename', 'dirname' ], 0 )
basename = _temp.basename
dirname = _temp.dirname
print(_temp)
# <module 'posixpath' from 
#  '/Library/Frameworks/Python.framework/Versions/3.7/lib/python3.7/posixpath.py'>
print(basename)
# <function basename at 0x10fb3d2f0>
print(dirname)
# <function dirname at 0x10fb3d378>
