import json
from jsonschema import *
from argparse import ArgumentParser
from json import JSONDecoder
import os
from swiplserver import PrologMQI


def validateJson(instance, schema):
    try:
        validate(instance=instance, schema=schema)
        return True
    except exceptions.ValidationError:
        return False


def createPlMQI():
    mqi = PrologMQI()
    mqi.start()
    return mqi


def createPlThread(mqi):
    cwd = os.getcwd()

    thread = mqi.create_thread()
    cwd = cwd.replace('\\', '/')

    cmd = "working_directory(_,'" + cwd + "')."
    thread.start()
    thread.query(cmd)
    return thread


args = ArgumentParser()
args.add_argument("-f", "--file", nargs=1, type=str, dest="file")
args.add_argument("-v", "--validate", action="store_true")
args = args.parse_args()
file = args.file[0]
validate_only = args.validate

with open("schema.json", "rt") as handle:
    raw = str(handle.read())
    schema = JSONDecoder().decode(raw)
with open(file, "rt") as handle:
    raw = handle.read()
    instance = JSONDecoder().decode(raw)

if validateJson(instance, schema):
    print("JSON appears valid")
else:
    print("JSON appears invalid")
if validate_only:
    exit(0)

mqi = createPlMQI()
thread = createPlThread(mqi)

connectableTypes = ["Actor", "Device", "Employee", "PhysicalZone", "LogicalZone", "Service", "Operation", "Role"]
allowedSingleConnections = ["PZoneContainsDevice", "LZoneContainsDevice"]
allowedPluralConnections = ["HasLPriv", "HasPAccess", "HasConnection", "ServiceClassContainsService",
                            "HasRole", "DeviceHasService", "OperationContainsConnection",
                            "OperationContainsPAccess", "PZoneHasNeighbor", "ServiceContainsOperation"]

reversedList = ["PZoneContainsDevice", "LZoneContainsDevice"]

with open("facts.pl", "wt") as handle:
    items = []

    connecteds = []

    data = json.loads(raw)

    for objectType in data:
        if objectType not in connectableTypes:
            continue
        for element in data[objectType]:
            name = element["name"]
            elementType = element["type"].lower()
            items.append(elementType + "(" + name + ").\n")

            for connName in allowedSingleConnections:
                if connName in element:
                    c1 = connName.lower() + "({},{}).\n".format(name, element[connName]) if connName not in reversedList \
                        else connName.lower() + "({},{}).\n".format(element[connName], name)

                    if c1 not in connecteds:
                        connecteds.append(c1)

            for connName in allowedPluralConnections:
                if connName in element:
                    for conn in element[connName]:
                        c1 = connName.lower() + "({},{}).\n".format(name, conn) if connName not in reversedList \
                            else connName.lower() + "({},{}).\n".format(conn, name)

                        if c1 not in connecteds:
                            connecteds.append(c1)

    for unary in items:
        handle.write(unary)
    for val in connecteds:
        handle.write(val)

thread.query("[facts].")
thread.stop()
mqi.stop()
