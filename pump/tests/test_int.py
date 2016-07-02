# Project imports
import pump
import tests

class TestInteger(tests.PumpTestCase):

    def testInteger(self):
        toTest = [
            0,
            1,
            -1,
            1234567,
            -1234567,
            int(2**63 - 1)
            -int(2**63 - 1)
        ]

        self._test(toTest)
