import unittest

from parameterized import parameterized

from scripts.KeyCodeImporter import KeyCodeImporter


class KeyCodeImporterTest(unittest.TestCase):
    @parameterized.expand([
        ("data/tests/KeyCodeImporter/Empty.txt", []),
        ("data/tests/KeyCodeImporter/One.txt", [("KEY_A", 65)]),
        ("data/tests/KeyCodeImporter/Many.txt", [("KEY_A", 65), ("KEY_B", 66), ("KEY_C", 67)]),
    ])
    def testEmptyKeyCodeImporter(self, filename, codes):
        sut = KeyCodeImporter()
        sut.setFilename(filename)
        sut.makeItSo()
        self.assertEqual(codes, sut.codes())

if __name__ == "__main__":
    unittest.main()