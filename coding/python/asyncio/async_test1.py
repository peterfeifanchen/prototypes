#!/usr/bin/python

# async is combination of event loop, tasks and coroutines. 
# The event loop runs tasks one after another, at any given time, only one task is
# running. On blocking calls, the active task yields control to the event loop. 
# This is achieved through the use of coroutines, which provide the semantics for
# cooperative scheduling. It is basically a stateful function that can yield 
# control flow to the event loop. When yield occurs, the coroutine is "paused".
# When it is invoked again, execution "resumes". 

# Coroutines pause using 'await'. When 'await another_routine' is called from one 
# subroutine on another, that subroutine is executed immediately. As event loops
# schedule tasks, when `await another_routine` is called, it wraps the coroutine
# object in a task, ran immediately. 

import asyncio
import time

async def coroutine1():
    print( 'coroutine1 running in event loop' )
    # model a blocking call that causes coroutine to yield. In real life this can be
    # a network request, db query etc...
    # async.sleep is special in that it creates a coroutine object that waits the
    # number of seconds and schedules a wake up. However this does not mean that it
    # will be for sure woken up in those number of seconds as another coroutine can
    # make a blocking call without await (e.g., coroutine2 below).
    print( 'coroutine1 paused' )
    await asyncio.sleep(1)

    print( 'coroutine1 resumed' )

async def coroutine2():
    print( 'coroutine2 running in event loop' )

    time.sleep(5)
    print( 'coroutine2 paused' )
    await asyncio.sleep(1)

    print ('coroutine2 resumed' )

# event loop
loop = asyncio.get_event_loop()

# schedule both coroutines to run on the event loop
loop.run_until_complete( asyncio.gather( coroutine1(), coroutine2() ) )

# Reference
# 1. https://hackernoon.com/a-simple-introduction-to-pythons-asyncio-595d9c9ecf8c
