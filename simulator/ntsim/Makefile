TOPDIR=../..

SOURCES = \
	./cpp/ConnectionNotifier.cpp\
	./cpp/Dispatcher.cpp\
	./cpp/DsClient.cpp\
	./cpp/EntryNotifier.cpp\
	./cpp/InstanceImpl.cpp\
	./cpp/LoggerImpl.cpp\
	./cpp/Message.cpp\
	./cpp/NetworkConnection.cpp\
	./cpp/networktables/NetworkTable.cpp\
	./cpp/networktables/NetworkTableEntry.cpp\
	./cpp/networktables/NetworkTableInstance.cpp\
	./cpp/networktables/RpcCall.cpp\
	./cpp/ntcore_c.cpp\
	./cpp/ntcore_cpp.cpp\
	./cpp/ntcore_test.cpp\
	./cpp/RpcServer.cpp\
	./cpp/SequenceNumber.cpp\
	./cpp/Storage.cpp\
	./cpp/Storage_load.cpp\
	./cpp/Storage_save.cpp\
	./cpp/tables/ITableListener.cpp\
	./cpp/Value.cpp\
	./cpp/WireDecoder.cpp\
	./cpp/WireEncoder.cpp

SUPPRESS_DEBUG_DEFINE=true

TARGET=ntsim

LOCAL_CFLAGS += -Iinclude -I$(TOPDIR)/simulator/wpiutilsim/include

include $(TOPDIR)/makefiles/buildlib.mk
