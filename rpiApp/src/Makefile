TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

USR_LDFLAGS_Linux +=-lwiringPi

#=============================
# Build the IOC application

PROD_IOC = rpi
# rpi.dbd will be created and installed
DBD += rpi.dbd

# rpi.dbd will be made up from these files:
rpi_DBD += base.dbd

# Include dbd files from all support applications:
rpi_DBD += rpi_extra.dbd

# Add all the support libraries needed by this IOC
#rpi_LIBS += wiringPi

# rpi_registerRecordDeviceDriver.cpp derives from rpi.dbd
rpi_SRCS += rpi_registerRecordDeviceDriver.cpp
rpi_SRCS += rpi_extra.c

# Build the main IOC entry point on workstation OSs.
rpi_SRCS_DEFAULT += rpiMain.cpp
rpi_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#rpi_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
rpi_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

