# Project imports
import kimchi
import tests

class TestLong(tests.KimchiTestCase):

    def testLong(self):
        toTest = [
            long(0),
            long(1),
            long(-1),
            long(1234567),
            long(-1234567),
            long(2**63 - 1),
            -long(2**63 - 1),
            1238751298037598013268071263059871290387901283750781236876123847980127357806132904872198037478012635786132980750129837,
            -1238751298037598013268071263059871290387901283750781236876123847980127357806132904872198037478012635786132980750129837
        ]

        self._test(toTest)