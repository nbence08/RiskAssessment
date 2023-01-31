import math

import pywinauto
from pywinauto.keyboard import send_keys
from pywinauto.mouse import press, release, _perform_click_input, move
import utility

from OperationTest.OperationTester import OperationTester


class RolesDialogTester:
    DIRECTORY = "RolesDialogTest"

    def __init__(self, app):
        self.app = app

        self.operations = OperationTester(app)

        self.editArea = app.RiskAssessment.GroupBox3

        self.createDeviceButton = app.RiskAssessment.Button4
        self.createServiceButton = app.RiskAssessment.Button5
        self.createOperationButton = app.RiskAssessment.Button6
        self.createRoleButton = app.RiskAssessment.Button7
        self.createActorButton = app.RiskAssessment.Button8
        self.createEmployeeButton = app.RiskAssessment.Button9
        self.createLZoneButton = app.RiskAssessment.Button10
        self.createPZoneButton = app.RiskAssessment.Button11
        self.createServiceClassButton = app.RiskAssessment.Button12

        self.selectButton = app.RiskAssessment.Select
        self.grantLPrivButton = app.RiskAssessment.Grant_LPriv
        self.grantPAccessButton = app.RiskAssessment.Grant_PAccess
        self.connectWithZoneButton = app.RiskAssessment.Connect_With_Zone
        self.grantConnectionButton = app.RiskAssessment.Grant_Connection
        self.connectToButton = app.RiskAssessment.Connect_To

    def openRolesDialog(self):
        self.app.RiskAssessment.Menu2.Edit.click_input()
        self.app.RiskAssessment.Menu2.Edit.Edit_Roles.click_input()

    def okRolesDialog(self):
        self.app.RiskAssessment.Edit_Roles.OK.click_input()

    def createRoleAndOperation(self):
        editAreaCenter = utility.rectCenter(self.editArea.rectangle())

        opCoords = (editAreaCenter[0] + 25, editAreaCenter[1] + 25)
        self.createOperationButton.click_input()
        self.editArea.click_input(coords=editAreaCenter)

        roleCreateCoords = (editAreaCenter[0] + 300, editAreaCenter[1])
        roleCoords = (roleCreateCoords[0] + 25, roleCreateCoords[1] + 25)
        self.createRoleButton.click_input()
        self.editArea.click_input(coords=roleCreateCoords)

        return opCoords, roleCoords

    def execAddPriv(self):
        self.createRoleAndOperation()
        self.openRolesDialog()

        rolesDialog = self.app.Roles_Dialog

        rolesList = rolesDialog.RolesListBox
        lprivsList = rolesDialog.LPrivsListBox
        lprivsInRoleList = rolesDialog.LPrivs_in_RoleListBox

        assert rolesList.item_count() == 1
        assert lprivsList.item_count() == 1
        assert lprivsInRoleList.item_count() == 0

        role = rolesList.items()[0]
        roleText = role.window_text()
        role.select()

        assert rolesList.item_count() == 1
        assert lprivsList.item_count() == 1
        assert lprivsInRoleList.item_count() == 0

        operation = lprivsList.items()[0]
        operationText = operation.window_text()
        operation.select()

        assert rolesList.item_count() == 1
        assert lprivsList.item_count() == 1
        assert lprivsInRoleList.item_count() == 0

        assert rolesList.items()[0].window_text() == roleText

        rolesDialog.LPrivs_in_RoleButton2.click_input()

        assert rolesList.item_count() == 1
        assert lprivsList.item_count() == 0
        assert lprivsInRoleList.item_count() == 1
        assert rolesList.items()[0].window_text() == roleText
        assert lprivsInRoleList.items()[0].window_text() == operationText

        self.okRolesDialog()

    def execRemovePriv(self):
        opCoords, roleCoords = self.createRoleAndOperation()

        self.grantLPrivButton.click_input()

        press(coords=utility.inGlobalCoords(roleCoords, self.editArea.rectangle()))
        release(coords=utility.inGlobalCoords(opCoords, self.editArea.rectangle()))

        self.openRolesDialog()
        rolesDialog = self.app.Roles_Dialog

        rolesList = rolesDialog.RolesListBox
        lprivsList = rolesDialog.LPrivsListBox
        lprivsInRoleList = rolesDialog.LPrivs_in_RoleListBox

        assert rolesList.item_count() == 1
        assert lprivsList.item_count() == 1
        assert lprivsInRoleList.item_count() == 0

        role = rolesList.items()[0]
        roleText = role.window_text()
        role.select()

        assert rolesList.item_count() == 1
        assert lprivsList.item_count() == 0
        assert lprivsInRoleList.item_count() == 1

        operation = lprivsInRoleList.items()[0]
        operationText = operation.window_text()
        operation.select()

        assert rolesList.items()[0].window_text() == roleText
        rolesDialog.LPrivs_in_RoleButton.click_input()

        assert rolesList.item_count() == 1
        assert lprivsList.item_count() == 1
        assert lprivsInRoleList.item_count() == 0
        assert rolesList.items()[0].window_text() == roleText
        assert lprivsList.items()[0].window_text() == operationText

        self.okRolesDialog()

    def checkAddPriv(self):
        utility.performStandardCheck(self.app, "AddPriv")
        utility.execClear(self.app)

    def checkRemovePriv(self):
        utility.performStandardCheck(self.app, "RemovePriv")
        utility.execClear(self.app)

    def testRolesDialog(self):

        self.execAddPriv()
        self.checkAddPriv()

        self.execRemovePriv()
        self.checkRemovePriv()
