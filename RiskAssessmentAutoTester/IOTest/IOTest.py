import json
import os

import pywinauto
from pywinauto.keyboard import send_keys
import utility


class IOTester:
    DIRECTORY = "IOTest"

    def __init__(self, app: pywinauto.application.Application):
        self.app = app

    def testIO(self):
        self.execBasicIOTest()
        self.checkBasicIOTest()

    def execBasicIOTest(self):
        cwd = os.getcwd()
        jsonPath = os.path.join(cwd, "res")

        self.app.RiskAssessment.Menu2.File.click_input()
        self.app.RiskAssessment.Menu2.File.Open.click_input()
        self.app.RiskAssessment.Load.Toolbar4.click_input('right')
        self.app.Context.Edit_address.click_input()
        send_keys(jsonPath + '{ENTER}', pause=0.01)
        self.app.RiskAssessment.Load.ListBox.base_json.click_input()
        self.app.RiskAssessment.Load.OpenButton3.click_input('left')

        self.app.RiskAssessment.Menu2.File.click_input()
        self.app.RiskAssessment.Menu2.File.Save_as.click_input()
        self.app.RiskAssessment.Save.Toolbar5.click_input('right')
        self.app.Context.Edit_address.click_input()
        send_keys(jsonPath + '{ENTER}', pause=0.01)

        if os.path.exists("res/actual.json"):
            os.remove("res/actual.json")

        self.app.RiskAssessment.Save.Pane0.ComboBox0.Edit.click_input()
        send_keys("actual.json")
        self.app.RiskAssessment.Save.Save.click_input()

    def checkBasicIOTest(self):
        utility.saveActual(self.app.RiskAssessment, "res")

        result = utility.compareToReference(self.app.RiskAssessment)
        if result == utility.CompareResult.MATCH:
            print("basicIOTest matches")
        else:
            print("basicIOTest does not match")

        referenceFile = open("res/reference.json", "r")
        referenceText = referenceFile.read()

        actualFile = open("res/actual.json", "r")
        actualText = actualFile.read()

        referenceJson = json.loads(referenceText)
        actualJson = json.loads(actualText)

        if referenceJson == actualJson:
            print("basicIOTest json matches")
        else:
            print("basicIOTest json does not match")

        utility.execClear(self.app)


