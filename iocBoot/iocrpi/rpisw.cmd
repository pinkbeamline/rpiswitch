#!../../bin/linux-arm/rpi

## You may have to change rpi to something else
## everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/rpi.dbd"
rpi_registerRecordDeviceDriver pdbbase

## Load record instances
dbLoadRecords("db/rpi.db","BL=PINK,DEV=RPISW")

cd "${TOP}/iocBoot/${IOC}"

dbLoadTemplate("rpi.subst")

iocInit

## Start any sequence programs
#seq sncxxx,"user=epics"
