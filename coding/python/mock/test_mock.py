# Mock allows you to replace a part of your application with a dummy at 
# test time. Depending on usage, it will even provide a context "with" that
# limits the scope of the dummy function to within the context only. 
#
# Instead of calling the actual function, it calls the substitute function.
# It is useful in the following cases:
#   1. Speed: you may have a test that takes long time to run, but is 
#             inconsequential to what you are actually testing. 
#   2. Undesirable side effects: an external API call that is not deterministic
#
# One thing to be aware off with mock is that at some point, when importing,
# python reads in the entire class. This including constructing default values.
# Therefore to mock default values, you must mock the attribute affected by the
# default value in the class, not the default value itself. 


import unittest
from unittest.mock import patch
import time
import os

# Basic Usage
m = unittest.mock.Mock()
m.foo = 'bar'
m.configure_mock( bar='baz' )
assert m.bar == 'baz'

# Return values can be configured on Mock calls or side effects can be used to
# redirect calls to custom functions.
m.return_value = 42
assert m() == 42

m.side_effect = [ 'foo', 'bar', 'baz' ]
assert m() == 'foo'
assert m() == 'bar'
assert m() == 'baz'
try:
    m()
except StopIteration:
    assert True
else:
    assert False

m.side_effect = RuntimeError( 'Boom' )
try:
    m()
except RuntimeError:
    assert True
else:
    assert False

# assert_*...can be used to assert that mock has been called n times
# call_*...can be used to retrieve number of calls and its arguments

# @patch: replace sum function with a mock sum function that always returns 9.
#         It then returns the mock sum function as a parameter to the unittest.
class Calculator:
    def sum( self, a, b ):
        time.sleep(10)
        return a + b

def mock_sum( a, b ):
    # mock sum function without the long running time.sleep
    return a + b

class TestCalculator( unittest.TestCase ):
    # setUp() runs at the beginning of 
    def setUp( self ):
        print( "Setting up..." )
        self.calc = Calculator()

    def tearDown( self ):
        print( "Tearing Down..." )

    # For unittest.main() to find unit tests, test_* must be pre-appended.
    @patch('__main__.Calculator.sum', return_value=9)
    def test_sum_mock1( self, sum ):
        answer = self.calc.sum( 2, 3 )
        self.assertEqual( answer, 9 )

    @patch( '__main__.Calculator.sum', side_effect=mock_sum )
    def test_sum_mock2( self, sum ):
        self.assertEqual( sum( 2, 3 ), 5 )
        self.assertEqual( sum( 7, 3 ), 10 )

# @patch returns a mock class object example 1
class Blog( object ):
    def __init__( self, name ):
        self.name = name

    def posts( self ):
        reponse = requests.get( "https://jsonplaceholder.typicode.com/posts" )
        return response.json()

    def __repr__( self ):
        return '<Blog: {}>'.format( self.name )

class TestBlog( unittest.TestCase ):
    @patch( '__main__.Blog' )
    def test_blog_posts( self, MockBlog ):
        blog = MockBlog()
        
        # other parameters that may be set include
        #   'assert_any_call', 
        #   'assert_called', 
        #   'assert_called_once', 
        #   'assert_called_once_with', 
        #   'assert_called_with', 
        #   'assert_has_calls', 
        #   'assert_not_called', 
        #   'attach_mock', 
        #   'call_args',
        #   'call_args_list',
        #   'call_count',
        #   'called',
        #   'configure_mock',
        #   'method_calls',
        #   'mock_add_spec',
        #   'mock_calls',
        #   'reset_mock',
        #   'return_value',
        #   'side_effect']
        blog.posts.return_value = [
            {
                'userId': 1,
                'id': 1,
                'title': 'Test Title',
                'body': 'Far out...'
            }
        ]

        response = blog.posts()
        self.assertIsNotNone( response )
        self.assertIsInstance( response[0], dict )

# @patch returns a mock class object example 2, calls on mock returns another mock
class Helper:
    DISCOUNT = 0.5
    def __init__( self, path ):
        self.path = path
    def get_path( self ):
        base_path = os.getcwd()
        return os.path.join( base_path, self.path )
    def another_path( self ):
        return "another_path"

class Worker:
    def __init__( self ):
        self.helper = Helper( 'db' )
    def work( self ):
        path = self.helper.get_path()
        return path
    def work2( self ):
        return self.helper.another_path()

class TestWorkerModule( unittest.TestCase ):
    @patch( '__main__.Helper' )
    def test_patching_class1( self, mockHelper ):
        mockHelper.return_value.get_path.return_value = 'testing'
        worker = Worker()
        self.assertEqual( worker.work(), 'testing' )
   
    # If attributes don't exist in mock object, it is created.
    @patch( '__main__.Helper' )
    def test_patching_class2( self, mockHelper ):
        mockHelper.nonExistantAttr()
    
    # Limit mock objects with autospec=True so that it only has attributes of the
    # object it is mocking.
    @patch( '__main__.Helper', autospec=True )
    def test_patching_class3( self, mockHelper ):
        try:
            mockHelper.nonExistantAttr()
        except AttributeError:
            assert True
        else:
            assert False
    
    # Partial class mocking (e.g., only get_path is mocked in Helper with remainder
    # of class unchanged )
    @patch.object( Helper, 'get_path', return_value='testing' )
    def test_patching_class4( self, mockHelper ):
        worker = Worker()
        self.assertEqual( worker.work(), 'testing' )
        self.assertEqual( worker.work2(), 'another_path' )

    # Patch a class attribute
    def test_patch_class_attribute( self ):
        with patch.object( Helper, 'DISCOUNT', 1 ):
            helper = Helper('db')
            self.assertEqual( helper.DISCOUNT, 1 )

    @unittest.expectedFailure
    def test_patch_class_attribute_fail( self ):
        helper = Helper('db')
        self.assertEqual( helper.DISCOUNT, 1 )

# @patch imports
class TestOS( unittest.TestCase ):
    @patch( 'os.getcwd', return_value='testing' )
    def test_getcwd( self, mockOS ):
        self.assertEqual( os.getcwd(), 'testing' )

# @patch nested, environment variables, override builtin functions
def work_on():
    path = os.path.join( os.getcwd(), os.environ[ 'MY_VAR' ] )
    print( "Working on " + path )
    return path

class TestBuiltin( unittest.TestCase ):
    @patch( '__main__.print' )
    @patch( 'os.getcwd', return_value='/home/' )
    @patch.dict( 'os.environ', {'MY_VAR': 'testing'} ) 
    def test_patch_builtin( self, mockCwd, mockPrint ):
        self.assertEqual( work_on(), '/home/testing' )
        mockPrint.assert_called_once_with( 'Working on /home/testing' )

# Instead of decoraters, you can create a patch context. The patch exists only 
# within the scope. 
class TestBuiltinContext( unittest.TestCase ):
    def test_patch_builtin( self ):
        with patch( '__main__.print' ) as mockPrint:
            with patch( 'os.getcwd', return_value='/home' ):
                with patch.dict( 'os.environ', {'MY_VAR': 'testing'} ):
                    self.assertEqual( work_on(), '/home/testing' )
                    mockPrint.assert_called_once_with( 'Working on /home/testing' )

# @patch mock context manager
def size_of():
    with open('text.txt') as f:
        contents = f.read()
    return len(contents)

class TestContextmanager( unittest.TestCase ):
    @patch( '__main__.open' )
    def test_context_manager( self, mockOpen ):
        from io import StringIO
        mockOpen.return_value.__enter__.return_value = StringIO('testing')
        self.assertEqual( size_of(), 7 )

if __name__ == '__main__':
    unittest.main()

# Reference
# Example code is taken from
# 1. https://semaphoreci.com/community/tutorials/getting-started-with-mocking-in-python
# For full disambiguation see
# 2. https://docs.python.org/3/library/unittest.mock.html
# Alternatively, this provides a cheatsheet with shoutouts to 'pretend' and pytest
# 'monkeypatch'.
# 3. https://medium.com/@yeraydiazdiaz/what-the-mock-cheatsheet-mocking-in-python-6a71db997832
