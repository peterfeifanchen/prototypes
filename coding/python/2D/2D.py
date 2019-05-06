#!/usr/bin/python


# When you create a 2D array in Python as a list of nxm filled
# by zeros. This is wrong, as each list [0]*3 is a reference to
# a list of m zeros. This is why modifying a[0][0]=5 also changes
# a[1][0]=5.

a = [[0]*3]*4
a[0][0] = 2
print("We only set a[0][0]=5, the rest should be 0's but...")
print("a[0][0]=", a[0][0])
print("a[1][0]=", a[1][0])

# The easiest way is to use a generator
a = [[0] * 3 for i in range(4)]
a[0][0] = 2
print("We only set a[0][0]=5, the rest is still 0 when we initiate a 2D array correctly")
print("a[0][0]=", a[0][0])
print("a[1][0]=", a[1][0])
