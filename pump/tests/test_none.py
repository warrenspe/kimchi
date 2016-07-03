# Project imports
import pump
import tests

class TestNone(tests.PumpTestCase):

    def testNone(self):
        toTest = [
            None
        ]

        self._test(toTest)
