# Project imports
import pump
import tests

class TestTuple(tests.PumpTestCase):

    def testTuple(self):
        toTest = [
            tuple(),
            ('',),
            (0,),
            (1,),
            (u'\u2208',),
            (1, 2, 3),
            (-1L,),
            tuple(range(900000)),
            ((1, 2), (4, 5), ((6, 7), (8,), tuple())),
            ((((((tuple(),),),),),),),
            ([], [], [], None, [tuple()])
        ]

        self._test(toTest)
