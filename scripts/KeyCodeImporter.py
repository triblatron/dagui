import re
import sys


class KeyCodeImporter(object):
    def __init__(self):
        self._filename = ""
        self._codes = []

    def setFilename(self, filename):
        self._filename = filename

    def codes(self):
        return self._codes

    def makeItSo(self):
        with open(self._filename, "r") as f:
            pattern = re.compile(r'#define (\w+) (\d+)')
            code = f.read()
            lines = code.split('\n')
            for line in lines:
                match = pattern.match(line)
                if match:
                    name = match.group(1)
                    key = match.group(2)
                    self._codes.append((name, int(key)))

if __name__ == '__main__':
    if len(sys.argv) < 2:
        raise ValueError("Usage: KeyCodeImporter.py <filename>")

    importer = KeyCodeImporter()
