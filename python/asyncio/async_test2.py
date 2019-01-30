#/usr/bin/python

# tasks on the event loop can also be scheduled using ensure_future or 
# AbstractEventLoop.create_task().

import asyncio

async def fake_request( request ):
    print( 'making request: ' + request )
    # simulate delay
    await asyncio.sleep(1)

    return 'got response: ' + request

async def web_server_handler():
    
    # Two ways to create tasks. When get_event_loop is called from a subroutine,
    # it returns the event loop running the coroutine. 
    task1 = asyncio.ensure_future( fake_request( 'one' ) )
    task2 = asyncio.get_event_loop().create_task( fake_request( 'two' ) )  

    # simulate non-blocking tasks
    await asyncio.sleep(0.5)

    print( "work in progress..." )
    # coordinate with completion of task1 and task2
    await asyncio.wait( [task1, task2] )
    print( "task1 and task2 are complete..." )

    print( task1.result() )
    print( task2.result() )

# There is one global default policy that define a context for the current thread's
# event loop. This policy manages one event loop per thread that calls asyncio. If
# no event loop is associated with a thread, RuntimeError is returned. A new event
# loop can be created with new_event_loop() and set_event_loop(). Custom global 
# policies can be accessed/set using get_event_loop_policy(), 
# set_event_loop_policy( policy ) where policy is a subclass of 
# asyncio.DefaultEventLoopPolicy
loop = asyncio.get_event_loop()
loop.run_until_complete( asyncio.ensure_future( web_server_handler() ) )
