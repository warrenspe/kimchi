#!/usr/bin/python

# Standard imports
import unittest, argparse, sys, os

sys.path.append('.')
sys.path.append('..')

# Projet imports
import pump

# Globals
TEST_DIR = os.path.split(os.path.dirname(os.path.realpath(__file__)))[-1]

class PumpTestCase(unittest.TestCase):

    def _test(self, toTest):
        for obj in toTest:
            try:
                deflated = pump.deflate(obj)
            except Exception as e:
                self.fail("Error deflating %s:\n%s" % (repr(obj), str(e)))

            try:
                inflated = pump.inflate(deflated)
            except Exception as e:
                self.fail("Error inflating %s (deflated: %r):\n%s" % (repr(obj), repr(deflated), str(e)))
            self.assertEqual(obj, inflated)

    @classmethod
    def setUpClass(cls):
        pass

    @classmethod
    def tearDownClass(cls):
        pass

def run(verbosity=1):
    loader = unittest.TestLoader()
    suite = loader.discover(TEST_DIR)
    _runSuite(suite, verbosity)

def runSelective(testFiles, verbosity):
    loader = unittest.TestLoader()
    suite = loader.loadTestsFromNames(["%s.test_%s" % (TEST_DIR, testFile) for testFile in testFiles])
    _runSuite(suite, verbosity)

def _runSuite(testSuite, verbosity):
    unittest.TextTestRunner(verbosity=verbosity).run(testSuite)
    sys.stdout.write("\nPress enter to close.")
    sys.stdin.readline()

def main():
    parser = argparse.ArgumentParser(description="Execute pump Unit Tests")
    parser.add_argument("testFiles", nargs="*")
    parser.add_argument("--verbosity", nargs="?", choices=['1', '2'], default=1)
    args = parser.parse_args()

    if len(args.testFiles):
        runSelective(args.testFiles, int(args.verbosity))
    else:
        run(int(args.verbosity))

if __name__ == '__main__':
    main()

