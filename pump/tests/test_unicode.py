# Project imports
import pump
import tests

class TestUnicode(tests.PumpTestCase):

    def testUnicode(self):
        toTest = [
            u'',
            u'\u2208',
            u'adsjgfasjrqkwerkqjwekrhqwerjkw',
            u'\x00',
            u'\xFF',
            u'\u2208\u9208\u5208\u0208\ua208\u6208\u5208\u4408',
        ]

        self._test(toTest)
