# Project imports
import pump
import tests

class TestBool(tests.PumpTestCase):

    def testBool(self):
        toTest = [
            False,
            True
        ]

        self._test(toTest)
