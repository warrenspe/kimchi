# Project imports
import pump
import tests

class TestList(tests.PumpTestCase):

    def testList(self):
        toTest = [
            [],
            [''],
            [0],
            [1],
            [u'\u2208'],
            [1, 2, 3],
            [-1L],
            list(range(900000)),
            [[1, 2], [4, 5], [[6, 7], [8], []]],
            [[[[[[[[[[[[[None]]]]]]]]]]]]]
        ]

        self._test(toTest)
