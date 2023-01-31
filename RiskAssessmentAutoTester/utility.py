from types import NoneType

from PIL import Image, ImageChops, ImageOps
from os import path

from enum import Enum


class CompareResult(Enum):
    MATCH = 0
    DIFFERENT = 1
    NONEXISTENT_REFERENCE = 2


def saveActual(window, folder=None):
    if folder is None:
        window.capture_as_image().save("res/actual.png")
    else:
        window.capture_as_image().save(path.join(folder, "actual.png"))


def compareToReference(window, tolerance=0.999, actualPath: NoneType | str = None, referencePath: NoneType | str = None,
                       diffPath: str = "res/difference.png"):
    actual = window.capture_as_image() if actualPath is None else Image.open(actualPath)
    reference = Image.open("res/reference.png").convert('RGB') if referencePath is None else Image.open(referencePath).convert('RGB')

    return compare(actual, reference, tolerance, diffPath=diffPath)


def compare(actual: Image.Image | str, reference: Image.Image | str, tolerance=0.99, attennuation=0.2,
            diffPath: str = "res/difference.png"):
    if type(actual) == str:
        actual = Image.open(actual).convert("RGB")
    if type(reference) == str:
        if not path.exists(reference):
            return CompareResult.NONEXISTENT_REFERENCE
        reference = Image.open(reference).convert("RGB")

    diff = ImageChops.difference(actual, reference)
    grayscale = ImageOps.grayscale(diff)
    diff.save(diffPath)

    histogram = grayscale.histogram()[0:256]
    pixNum = diff.height * diff.width

    for x in range(grayscale.width):
        for y in range(grayscale.height):
            pixel = grayscale.getpixel((x, y))
            grayscale.putpixel((x, y), min(pixel*4, 255))
    diffNameUnits = diffPath.split(".")
    diffNameUnits[-2] += "_intense"
    intenseName = '.'.join(diffNameUnits)
    grayscale.save(intenseName)

    diffValue = pixNum

    for i in range(256):
        diffValue -= int(attennuation * float(i * histogram[i]))

    if diffValue < tolerance * pixNum:
        return CompareResult.DIFFERENT
    else:
        return CompareResult.MATCH


def inGlobalCoords(coords: [int, int], rect):
    return rect.left + coords[0], rect.top + coords[1]


def performStandardCheck(app, testName):
    app.RiskAssessment.set_focus()
    app.RiskAssessment.wait("visible")
    saveActual(app.RiskAssessment, testName)

    result = compare(tolerance=0.995,
                     actual=path.join(testName, "actual.png"),
                     reference=path.join(testName, "reference.png"),
                     diffPath=path.join(testName, "difference.png"))
    if result == CompareResult.MATCH:
        print(testName + " matches.")
    elif result == CompareResult.DIFFERENT:
        print(testName + " does not match.")
    else:
        print("Reference for test "+testName+" could not be found.")



def rectCenter(rect):
    return int((rect.right - rect.left) / 2.0), int((rect.bottom - rect.top) / 2.0)

def execClear(app):
    app.RiskAssessment.Menu2.File.click_input()
    app.RiskAssessment.Menu2.File.New.click_input()
    app.RiskAssessment.Select.click_input()
