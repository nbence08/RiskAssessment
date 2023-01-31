import argparse
import os

from IOTest.IOTest import IOTester
from CreationTest.CreationTester import CreationTester
from OperationTest.OperationTester import OperationTester
from RolesDialogTest.RolesDialogTest import RolesDialogTester
from EntityLPrivsDialogTest.EntityLPrivsDialogTest import EntityLPrivsDialogTester

# print(dir(self.app.RiskAssessment.Load.wrapper_object()))
# self.app.RiskAssessment.Load.print_control_identifiers()

from pywinauto.application import Application


def prepareForDir(nextDir: str):
    while not os.path.basename(os.getcwd()) == "RiskAssessmentAutoTester":
        os.chdir("./..")
    os.chdir(nextDir)


parser = argparse.ArgumentParser()
parser.add_argument("-d", "--directory", nargs=1, type=str, dest="directory")
args = parser.parse_args()
directory = args.directory[0]

path = os.path.join(directory, "RiskAssessment.exe")

if not os.listdir(directory).__contains__("RiskAssessment.exe"):
    exit(-1)

app = Application(backend="uia").start(path)
app.RiskAssessment.wait('visible')


# executing tests
'''prepareForDir(OperationTester.DIRECTORY)
opTester = OperationTester(app)
opTester.operationTest()'''

prepareForDir(IOTester.DIRECTORY)
io = IOTester(app)
io.testIO()

prepareForDir(CreationTester.DIRECTORY)
create = CreationTester(app)
create.testCreation()

prepareForDir(RolesDialogTester.DIRECTORY)
rolesDialogTester = RolesDialogTester(app)
rolesDialogTester.testRolesDialog()

prepareForDir(EntityLPrivsDialogTester.DIRECTORY)
entityLPrivsDialogTester = EntityLPrivsDialogTester(app)
entityLPrivsDialogTester.testEntityLPrivsDialog()

app.kill()
