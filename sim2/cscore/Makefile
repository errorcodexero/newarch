TOPDIR=../..

include $(TOPDIR)/makefiles/defaults.mk

SOURCES = \
	./cpp/ConfigurableSourceImpl.cpp\
	./cpp/cscore_c.cpp\
	./cpp/cscore_cpp.cpp\
	./cpp/cscore_oo.cpp\
	./cpp/CvSinkImpl.cpp\
	./cpp/CvSourceImpl.cpp\
	./cpp/Frame.cpp\
	./cpp/HttpCameraImpl.cpp\
	./cpp/Instance.cpp\
	./cpp/JpegUtil.cpp\
	./cpp/MjpegServerImpl.cpp\
	./cpp/Notifier.cpp\
	./cpp/PropertyContainer.cpp\
	./cpp/PropertyImpl.cpp\
	./cpp/RawSinkImpl.cpp\
	./cpp/RawSourceImpl.cpp\
	./cpp/SinkImpl.cpp\
	./cpp/SourceImpl.cpp\
	./cpp/Telemetry.cpp\
	./cpp/UsbCameraImplCommon.cpp


TARGET=cscore

NEED_WPIUTIL=true
NEED_NTCORE=true

LOCAL_CFLAGS += -Iinclude -Icpp

include $(TOPDIR)/makefiles/buildlib.mk

