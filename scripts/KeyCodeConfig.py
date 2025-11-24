import sys

from KeyCodeImporter import KeyCodeImporter


class KeyCodeConfig:
    def __init__(self):
        self._codes = []
        self._output = None

    def setCodes(self, codes):
        self._codes = codes

    def setOutput(self, output):
        self._output = output

    def makeItSo(self):
        if self._output is not None:
            self._output.write("root=\n")
            self._output.write("{\n")
            for code in self._codes:
                self._output.write("    {}={},\n".format(code[0],code[1]))
            self._output.write("}\n")

if __name__ == '__main__':
    if len(sys.argv) != 3:
        raise ValueError("Usage: KeyCodeConfig.py <inputFilename> <outputFilename>")

    inputFilename = sys.argv[1]
    outputFilename = sys.argv[2]
    importer = KeyCodeImporter()
    importer.setFilename(inputFilename)
    importer.makeItSo()
    exporter = KeyCodeConfig()
    exporter.setCodes(importer.codes())
    exporter.setOutput(open(outputFilename, "w"))
    exporter.makeItSo()

