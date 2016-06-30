# Project imports
import pump
import tests

class TestLong(tests.PumpTestCase):

    def testLong(self):
        toTest = [
            0L,
            1L,
            -1L,
            1234567L,
            -1234567L,
            long(2**63 - 1),
            -long(2**63 - 1),
            1238751298037598013268071263059871290387901283750781236876123847980127357806132904872198037478012635786132980750129837,
            -1238751298037598013268071263059871290387901283750781236876123847980127357806132904872198037478012635786132980750129837
        ]

        for i in toTest:
            deflated = pump.deflate(i)
            self.assertEqual(i, pump.inflate(deflated))