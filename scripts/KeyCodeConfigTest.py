import unittest
from io import StringIO

from parameterized import parameterized

from scripts.KeyCodeConfig import KeyCodeConfig


class KeyCodeConfigTest(unittest.TestCase):
    @parameterized.expand([
        ([], "root=\n{\n}\n"),
        ([("Key_A", 65)], "root=\n{\n    Key_A=65,\n}\n"),
    ])
    def testMakeItSo(self, input, expected):
        sut = KeyCodeConfig()
        sut.setCodes(input)
        output = StringIO()
        sut.setOutput(output)
        sut.makeItSo()
        self.assertEqual(expected, output.getvalue())