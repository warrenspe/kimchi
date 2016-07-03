# Project imports
import pump
import tests

class TestDict(tests.PumpTestCase):

    def testDict(self):
        toTest = [
            dict(),
            {1: 2},
            dict([(a, a + 1) for a in range(15000)]),
            {3: {2: {1: {0: -1}}}}
        ]

        self._test(toTest)
