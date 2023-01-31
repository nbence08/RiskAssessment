import utility
from Tester import Tester


class EntityLPrivsDialogTester(Tester):
    DIRECTORY = "EntityLPrivsDialogTest"

    def __init__(self, app):
        super().__init__(app)

        actorPos = self.editAreaCenter()
        self.actorPos = (actorPos[0] - 200, actorPos[1] - 200)

        empPos = self.editAreaCenter()
        self.empPos = (empPos[0], empPos[1] - 400)

        rolePos = self.editAreaCenter()
        self.rolePos = (rolePos[0] + 200, rolePos[1] - 200)

        firstOpPos = self.editAreaCenter()
        self.firstOpPos = (firstOpPos[0] - 200, firstOpPos[1] + 200)
        secondOpPos = self.editAreaCenter()
        self.secondOpPos = (secondOpPos[0] + 200, secondOpPos[1] + 200)

    def openEntityLPrivsDialog(self):
        self.app.RiskAssessment.Menu2.Edit.click_input()
        self.app.RiskAssessment.Menu2.Edit.Entity_Privs_and_Roles.click_input()

    def closeEntityLPrivsDialog(self):
        self.app.RiskAssessment.Edit_Entity_LPrivs.close()

    def createBasicLayout(self):
        self.deleteDefaultZones()

        self.createActorButton.click_input()
        self.lClickOnEditArea((self.actorPos[0]-25, self.actorPos[1]-25))

        self.createEmployeeButton.click_input()
        self.lClickOnEditArea((self.empPos[0]-25, self.empPos[1]-25))

        self.createRoleButton.click_input()
        self.lClickOnEditArea((self.rolePos[0]-25, self.rolePos[1]-25))

        self.createOperationButton.click_input()
        self.lClickOnEditArea((self.firstOpPos[0]-25, self.firstOpPos[1]-25))
        self.lClickOnEditArea((self.secondOpPos[0]-25, self.secondOpPos[1]-25))

        self.grantLPrivButton.click_input()
        self.pressOnEditArea(self.rolePos)
        self.releaseOnEditArea(self.firstOpPos)

    def execAddPrivToEntityTest(self):
        self.createBasicLayout()

        self.openEntityLPrivsDialog()
        addPrivToEntityButton = self.app.RiskAssessment.Edit_Entity_LPrivs.LPrivs_in_EntityButton2

        entitiesListBox = self.app.RiskAssessment.Edit_Entity_LPrivs.EntitiesListBox
        rolesInEntityListBox = self.app.RiskAssessment.Edit_Entity_LPrivs.Roles_in_EntityListBox
        allRolesListBox = self.app.RiskAssessment.Edit_Entity_LPrivs.All_RolesListbox
        allPrivsListBox = self.app.RiskAssessment.Edit_Entity_LPrivs.All_LPrivsListBox
        privsInEntityListBox = self.app.RiskAssessment.Edit_Entity_LPrivs.LPrivs_in_EntityListBox
        assert entitiesListBox.item_count() == 2
        assert privsInEntityListBox.item_count() == 0
        assert rolesInEntityListBox.item_count() == 0
        assert allPrivsListBox.item_count() == 2
        assert allRolesListBox.item_count() == 1

        firstEntityText = entitiesListBox.items()[0].window_text()
        secondEntityText = entitiesListBox.items()[1].window_text()
        firstPrivText = allPrivsListBox.items()[0].window_text()
        secondPrivText = allPrivsListBox.items()[1].window_text()
        roleText = allRolesListBox.items()[0].window_text()

        entitiesListBox.items()[0].click_input()
        assert entitiesListBox.item_count() == 2
        assert privsInEntityListBox.item_count() == 0
        assert rolesInEntityListBox.item_count() == 0
        assert allPrivsListBox.item_count() == 2
        assert allRolesListBox.item_count() == 1

        allPrivsListBox.items()[0].click_input()
        assert entitiesListBox.item_count() == 2
        assert privsInEntityListBox.item_count() == 0
        assert rolesInEntityListBox.item_count() == 0
        assert allPrivsListBox.item_count() == 2
        assert allRolesListBox.item_count() == 1

        addPrivToEntityButton.click_input()
        assert entitiesListBox.item_count() == 2
        assert privsInEntityListBox.item_count() == 1
        assert rolesInEntityListBox.item_count() == 0
        assert allPrivsListBox.item_count() == 1
        assert allRolesListBox.item_count() == 1
        assert privsInEntityListBox.items()[0].window_text() == firstPrivText
        assert allPrivsListBox.items()[0].window_text() == secondPrivText
        assert entitiesListBox.items()[0].window_text() == firstEntityText
        assert entitiesListBox.items()[1].window_text() == secondEntityText
        assert allRolesListBox.items()[0].window_text() == roleText

        self.closeEntityLPrivsDialog()

    def execRemovePrivFromEntityTest(self):
        self.createBasicLayout()
        self.grantLPrivButton.click_input()
        self.pressOnEditArea(self.actorPos)
        self.releaseOnEditArea(self.secondOpPos)

        self.openEntityLPrivsDialog()
        removePrivFromEntityButton = self.app.RiskAssessment.Edit_Entity_LPrivs.LPrivs_in_EntityButton

        entitiesListBox = self.app.RiskAssessment.Edit_Entity_LPrivs.EntitiesListBox
        rolesInEntityListBox = self.app.RiskAssessment.Edit_Entity_LPrivs.Roles_in_EntityListBox
        allRolesListBox = self.app.RiskAssessment.Edit_Entity_LPrivs.All_RolesListbox
        allPrivsListBox = self.app.RiskAssessment.Edit_Entity_LPrivs.All_LPrivsListBox
        privsInEntityListBox = self.app.RiskAssessment.Edit_Entity_LPrivs.LPrivs_in_EntityListBox
        assert entitiesListBox.item_count() == 2
        assert privsInEntityListBox.item_count() == 0
        assert rolesInEntityListBox.item_count() == 0
        assert allPrivsListBox.item_count() == 2
        assert allRolesListBox.item_count() == 1

        firstListElementText = allPrivsListBox.items()[0].window_text()
        secondListElementText = allPrivsListBox.items()[1].window_text()
        toBeRemovedPrivIndex = 1 if firstListElementText.split("_")[1] > secondListElementText.split("_")[1] else 0

        firstEntityText = entitiesListBox.items()[0].window_text()
        secondEntityText = entitiesListBox.items()[1].window_text()
        unassignedPrivText = firstListElementText if toBeRemovedPrivIndex == 1 else secondListElementText
        toBeRemovedPrivText = firstListElementText if toBeRemovedPrivIndex == 0 else secondListElementText
        roleText = allRolesListBox.items()[0].window_text()

        toBeRemovedIndex = 0 if firstEntityText.split('_')[0] == "AR" else 1

        entitiesListBox.items()[toBeRemovedIndex].click_input()
        assert entitiesListBox.item_count() == 2
        assert privsInEntityListBox.item_count() == 1
        assert rolesInEntityListBox.item_count() == 0
        assert allPrivsListBox.item_count() == 1
        assert allRolesListBox.item_count() == 1

        privsInEntityListBox.items()[0].click_input()
        assert entitiesListBox.item_count() == 2
        assert privsInEntityListBox.item_count() == 1
        assert rolesInEntityListBox.item_count() == 0
        assert allPrivsListBox.item_count() == 1
        assert allRolesListBox.item_count() == 1

        removePrivFromEntityButton.click_input()
        assert entitiesListBox.item_count() == 2
        assert privsInEntityListBox.item_count() == 0
        assert rolesInEntityListBox.item_count() == 0
        assert allPrivsListBox.item_count() == 2
        assert allRolesListBox.item_count() == 1
        assert allPrivsListBox.items()[0].window_text() == unassignedPrivText or allPrivsListBox.items()[1].window_text() == unassignedPrivText
        assert allPrivsListBox.items()[1].window_text() == toBeRemovedPrivText or allPrivsListBox.items()[0].window_text() == toBeRemovedPrivText
        assert entitiesListBox.items()[0].window_text() == firstEntityText
        assert entitiesListBox.items()[1].window_text() == secondEntityText
        assert allRolesListBox.items()[0].window_text() == roleText

        self.closeEntityLPrivsDialog()

    def execAddRoleToEntityTest(self):
        self.createBasicLayout()

        self.openEntityLPrivsDialog()
        addRoleToEntityButton = self.app.RiskAssessment.Edit_Entity_LPrivs.Roles_in_EntityButton2

        entitiesListBox = self.app.RiskAssessment.Edit_Entity_LPrivs.EntitiesListBox
        rolesInEntityListBox = self.app.RiskAssessment.Edit_Entity_LPrivs.Roles_in_EntityListBox
        allRolesListBox = self.app.RiskAssessment.Edit_Entity_LPrivs.All_RolesListbox
        allPrivsListBox = self.app.RiskAssessment.Edit_Entity_LPrivs.All_LPrivsListBox
        privsInEntityListBox = self.app.RiskAssessment.Edit_Entity_LPrivs.LPrivs_in_EntityListBox
        assert entitiesListBox.item_count() == 2
        assert privsInEntityListBox.item_count() == 0
        assert rolesInEntityListBox.item_count() == 0
        assert allPrivsListBox.item_count() == 2
        assert allRolesListBox.item_count() == 1

        firstEntityText = entitiesListBox.items()[0].window_text()
        secondEntityText = entitiesListBox.items()[1].window_text()
        firstPrivText = allPrivsListBox.items()[0].window_text()
        secondPrivText = allPrivsListBox.items()[1].window_text()
        roleText = allRolesListBox.items()[0].window_text()

        entityToAddToIndex = 0 if firstEntityText.split("_")[0] == "EM" else 1
        entitiesListBox.items()[entityToAddToIndex].click_input()
        assert entitiesListBox.item_count() == 2
        assert privsInEntityListBox.item_count() == 0
        assert rolesInEntityListBox.item_count() == 0
        assert allPrivsListBox.item_count() == 2
        assert allRolesListBox.item_count() == 1

        allRolesListBox.items()[0].click_input()
        assert entitiesListBox.item_count() == 2
        assert privsInEntityListBox.item_count() == 0
        assert rolesInEntityListBox.item_count() == 0
        assert allPrivsListBox.item_count() == 2
        assert allRolesListBox.item_count() == 1

        addRoleToEntityButton.click_input()
        assert entitiesListBox.item_count() == 2
        assert privsInEntityListBox.item_count() == 0
        assert rolesInEntityListBox.item_count() == 1
        assert allPrivsListBox.item_count() == 2
        assert allRolesListBox.item_count() == 0
        assert allPrivsListBox.items()[0].window_text() == firstPrivText
        assert allPrivsListBox.items()[1].window_text() == secondPrivText
        assert entitiesListBox.items()[0].window_text() == firstEntityText
        assert entitiesListBox.items()[1].window_text() == secondEntityText
        assert rolesInEntityListBox.items()[0].window_text() == roleText

        self.closeEntityLPrivsDialog()

    def execRemoveRoleFromEntityTest(self):
        self.createBasicLayout()
        self.grantLPrivButton.click_input()
        self.pressOnEditArea(self.empPos)
        self.releaseOnEditArea(self.rolePos)

        self.openEntityLPrivsDialog()
        removePrivFromEntityButton = self.app.RiskAssessment.Edit_Entity_LPrivs.LPrivs_in_EntityButton
        addPrivToEntityButton = self.app.RiskAssessment.Edit_Entity_LPrivs.LPrivs_in_EntityButton2
        removeRoleFromEntityButton = self.app.RiskAssessment.Edit_Entity_LPrivs.Roles_in_EntityButton
        addRoleToEntityButton = self.app.RiskAssessment.Edit_Entity_LPrivs.Roles_in_EntityButton2

        entitiesListBox = self.app.RiskAssessment.Edit_Entity_LPrivs.EntitiesListBox
        rolesInEntityListBox = self.app.RiskAssessment.Edit_Entity_LPrivs.Roles_in_EntityListBox
        allRolesListBox = self.app.RiskAssessment.Edit_Entity_LPrivs.All_RolesListbox
        allPrivsListBox = self.app.RiskAssessment.Edit_Entity_LPrivs.All_LPrivsListBox
        privsInEntityListBox = self.app.RiskAssessment.Edit_Entity_LPrivs.LPrivs_in_EntityListBox
        assert entitiesListBox.item_count() == 2
        assert privsInEntityListBox.item_count() == 0
        assert rolesInEntityListBox.item_count() == 0
        assert allPrivsListBox.item_count() == 2
        assert allRolesListBox.item_count() == 1

        firstEntityText = entitiesListBox.items()[0].window_text()
        secondEntityText = entitiesListBox.items()[1].window_text()
        firstPrivText = allPrivsListBox.items()[0].window_text()
        secondPrivText = allPrivsListBox.items()[1].window_text()
        roleText = allRolesListBox.items()[0].window_text()

        entityToRemoveFromIndex = 0 if firstEntityText.split("_")[0] == "EM" else 1
        entitiesListBox.items()[entityToRemoveFromIndex].click_input()
        assert entitiesListBox.item_count() == 2
        assert privsInEntityListBox.item_count() == 0
        assert rolesInEntityListBox.item_count() == 1
        assert allPrivsListBox.item_count() == 2
        assert allRolesListBox.item_count() == 0

        rolesInEntityListBox.items()[0].click_input()
        assert entitiesListBox.item_count() == 2
        assert privsInEntityListBox.item_count() == 0
        assert rolesInEntityListBox.item_count() == 1
        assert allPrivsListBox.item_count() == 2
        assert allRolesListBox.item_count() == 0

        removeRoleFromEntityButton.click_input()
        assert entitiesListBox.item_count() == 2
        assert privsInEntityListBox.item_count() == 0
        assert rolesInEntityListBox.item_count() == 0
        assert allPrivsListBox.item_count() == 2
        assert allRolesListBox.item_count() == 1
        assert allPrivsListBox.items()[0].window_text() == firstPrivText
        assert allPrivsListBox.items()[1].window_text() == secondPrivText
        assert entitiesListBox.items()[0].window_text() == firstEntityText
        assert entitiesListBox.items()[1].window_text() == secondEntityText
        assert allRolesListBox.items()[0].window_text() == roleText

        self.closeEntityLPrivsDialog()

    def checkAddRoleToEntityTest(self):
        utility.performStandardCheck(self.app, "AddRoleToEntityTest")
        utility.execClear(self.app)

    def checkRemoveRoleFromEntityTest(self):
        utility.performStandardCheck(self.app, "RemoveRoleFromEntityTest")
        utility.execClear(self.app)

    def checkRemovePrivFromEntityTest(self):
        utility.performStandardCheck(self.app, "RemovePrivFromEntityTest")
        utility.execClear(self.app)

    def checkAddPrivToEntityTest(self):
        utility.performStandardCheck(self.app, "AddPrivToEntityTest")
        utility.execClear(self.app)

    def testEntityLPrivsDialog(self):
        self.execAddPrivToEntityTest()
        self.checkAddPrivToEntityTest()

        self.execAddRoleToEntityTest()
        self.checkAddRoleToEntityTest()

        self.execRemovePrivFromEntityTest()
        self.checkRemovePrivFromEntityTest()

        self.execRemoveRoleFromEntityTest()
        self.checkRemoveRoleFromEntityTest()

