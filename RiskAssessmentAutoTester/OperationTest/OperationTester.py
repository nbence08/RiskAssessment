from pywinauto.keyboard import send_keys
from pywinauto.mouse import press, release
from CreationTest.CreationTester import CreationTester
import utility


class OperationTester:
    DIRECTORY = "OperationTest"

    def __init__(self, app):
        self.app = app

        self.editArea = app.RiskAssessment.GroupBox3
        self.lzcoords = (370, 130)
        self.pzcoords = (525, 130)

        self.selectButton = app.RiskAssessment.Select

    def __editAreaCoordsInGlobal(self, coords:tuple[int,int]):
        rectangle = self.editArea.rectangle()
        return coords[0] + rectangle.left, coords[1] + rectangle.top

    def moveDeleteTest(self):
        self.execMoveTest()
        self.checkMoveTest()

        self.execDeleteTest()
        self.checkDeleteTest()

    def checkDeleteTest(self):
        utility.performStandardCheck(self.app, "DeleteTest")
        utility.execClear(self.app)

    def checkMoveTest(self):
        utility.performStandardCheck(self.app, "MoveTest")
        utility.execClear(self.app)

    def checkClearTest(self):
        utility.performStandardCheck(self.app, "ClearTest")
        utility.execClear(self.app)

    def execMoveTest(self):
        self.selectButton.click_input()
        press(coords=self.__editAreaCoordsInGlobal(self.pzcoords))
        release(coords=self.__editAreaCoordsInGlobal((self.pzcoords[0] + 100, self.pzcoords[1])))

    def execDeleteTest(self):
        self.selectButton.click_input()
        self.editArea.click_input(coords=self.lzcoords)
        send_keys("{DELETE}")

    def clearTest(self):
        self.execClearTest()
        self.checkClearTest()

    def execClearTest(self):
        tester = CreationTester(self.app)
        tester.execTestCreation()

        self.app.RiskAssessment.Menu2.File.click_input()
        self.app.RiskAssessment.Menu2.File.New.click_input()

    def operationTest(self):
        self.moveDeleteTest()
        self.clearTest()


