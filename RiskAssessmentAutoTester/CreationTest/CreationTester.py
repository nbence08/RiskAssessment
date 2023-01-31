import math

from pywinauto.keyboard import send_keys
from pywinauto.mouse import press, release, move, click
from Tester import Tester
import utility


class CreationTester(Tester):
    DIRECTORY = "CreationTest"

    def __init__(self, app):
        super().__init__(app)

        self.radius = 500

        self.placementCounter = 0.0
        self.placementCounterMax = 11.0

    def moveMouseToEditAreaCoords(self, coords:tuple[int, int]):
        move(self._inEditAreaCoords(coords))

    def getNextEditAreaCoord(self):
        rect = self.editArea.rectangle()

        center = ((rect.right-rect.left)/2.0 - 50, (rect.bottom-rect.top)/2.0 - 50)

        arc = math.radians(360.0 * self.placementCounter / self.placementCounterMax)
        x = math.cos(arc)
        y = math.sin(arc)
        self.placementCounter += 1

        currx = int(x * self.radius + center[0])
        curry = int(y * self.radius + center[1])

        return currx, curry

    def createItem(self, createButton, coords:tuple[int, int]):
        createButton.click_input()
        self.lClickOnEditArea(coords)

        return coords[0]+25, coords[1]+25

    def createConnection(self, start:tuple[int, int], end:tuple[int, int]):
        press(coords=self._inEditAreaCoords(start))
        release(coords=self._inEditAreaCoords(end))

    def testCreation(self):
        self.execTestCreation()
        self.checkTestCreation()

    def execTestCreation(self):
        deviceCoords = self.getNextEditAreaCoord()
        serviceCoords = self.getNextEditAreaCoord()
        opCoords = self.getNextEditAreaCoord()
        op2Coords = self.getNextEditAreaCoord()
        roleCoords = self.getNextEditAreaCoord()
        actorCoords = self.getNextEditAreaCoord()
        empCoords = self.getNextEditAreaCoord()
        lzoneCoords = self.getNextEditAreaCoord()
        pzoneCoords = self.getNextEditAreaCoord()
        pzone2Coords = self.getNextEditAreaCoord()
        serviceClassCoords = self.getNextEditAreaCoord()

        self.deleteDefaultZones()

        deviceCoords = self.createItem(self.createDeviceButton, deviceCoords)
        serviceCoords = self.createItem(self.createServiceButton, serviceCoords)
        opCoords = self.createItem(self.createOperationButton, opCoords)
        op2Coords = self.createItem(self.createOperationButton, op2Coords)
        roleCoords = self.createItem(self.createRoleButton, roleCoords)
        actorCoords = self.createItem(self.createActorButton, actorCoords)
        empCoords = self.createItem(self.createEmployeeButton, empCoords)
        lzoneCoords = self.createItem(self.createLZoneButton, lzoneCoords)
        pzoneCoords = self.createItem(self.createPZoneButton, pzoneCoords)
        pzone2Coords = self.createItem(self.createPZoneButton, pzone2Coords)
        serviceClassCoords = self.createItem(self.createServiceClassButton, serviceClassCoords)

        self.grantLPrivButton.click_input()
        self.createConnection(actorCoords, opCoords)
        self.createConnection(empCoords, opCoords)
        self.createConnection(roleCoords, opCoords)
        # self.createConnection(opCoords, op2Coords) # contained LPrivs
        self.createConnection(empCoords, roleCoords)
        self.createConnection(actorCoords, roleCoords)

        self.grantPAccessButton.click_input()
        self.createConnection(actorCoords, pzoneCoords)
        self.createConnection(empCoords, pzoneCoords)
        self.createConnection(opCoords, pzoneCoords)

        self.grantConnectionButton.click_input()
        self.createConnection(opCoords, serviceCoords)
        self.createConnection(actorCoords, serviceCoords)
        self.createConnection(empCoords, serviceCoords)

        self.connectWithZoneButton.click_input()
        self.createConnection(deviceCoords, lzoneCoords)
        self.createConnection(deviceCoords, pzoneCoords)
        self.createConnection(pzoneCoords, pzone2Coords)

        self.connectToButton.click_input()
        self.createConnection(deviceCoords, serviceCoords)
        self.createConnection(serviceClassCoords, serviceCoords)
        self.createConnection(serviceCoords, op2Coords)

    def checkTestCreation(self):
        utility.performStandardCheck(self.app, "CreationTest")
        utility.execClear(self.app)
