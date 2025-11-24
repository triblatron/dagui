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
