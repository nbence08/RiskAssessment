import utility
from pywinauto.keyboard import send_keys
from pywinauto.mouse import press, release

class Tester:
    def __init__(self, app):
        self.app = app

        self.editArea = app.RiskAssessment.GroupBox3

        self.createDeviceButton = self.app.RiskAssessment.GroupBox1.Button1
        self.createServiceButton = self.app.RiskAssessment.GroupBox1.Button2
        self.createOperationButton = self.app.RiskAssessment.GroupBox1.Button3
        self.createRoleButton = self.app.RiskAssessment.GroupBox1.Button4
        self.createActorButton = self.app.RiskAssessment.GroupBox1.Button5
        self.createEmployeeButton = self.app.RiskAssessment.GroupBox1.Button6
        self.createLZoneButton = self.app.RiskAssessment.GroupBox1.Button7
        self.createPZoneButton = self.app.RiskAssessment.GroupBox1.Button8
        self.createServiceClassButton = self.app.RiskAssessment.GroupBox1.Button9

        self.selectButton = self.app.RiskAssessment.Select
        self.grantLPrivButton = self.app.RiskAssessment.Grant_LPriv
        self.grantPAccessButton = self.app.RiskAssessment.Grant_PAccess
        self.connectWithZoneButton = self.app.RiskAssessment.Connect_With_Zone
        self.grantConnectionButton = self.app.RiskAssessment.Grant_Connection
        self.connectToButton = self.app.RiskAssessment.Connect_To

    def _inEditAreaCoords(self, coords: tuple[int, int] = (0, 0)):
        rect = self.editArea.rectangle()

        return rect.left + coords[0], rect.top + coords[1]

    def lClickOnEditArea(self, coords:tuple[int, int]):
        self.editArea.click_input(coords=coords)

    def pressOnEditArea(self, coords:tuple[int, int]):
        press(coords=utility.inGlobalCoords(coords, self.editArea.rectangle()))

    def releaseOnEditArea(self, coords:tuple[int, int]):
        release(coords=utility.inGlobalCoords(coords, self.editArea.rectangle()))

    def editAreaCenter(self):
        return utility.rectCenter(self.editArea.rectangle())

    def deleteDefaultZones(self):
        self.editArea.set_focus()
        self.editArea.click_input(coords=(490, 170))
        send_keys("{DELETE}")
        self.editArea.set_focus()
        self.editArea.click_input(coords=(690, 170))
        send_keys("{DELETE}")
