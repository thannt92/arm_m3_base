CPPFLAGS	+= -I./sources/rf_protocols/RF24Network
CPPFLAGS	+= -I./sources/rf_protocols/RF24

VPATH += sources/rf_protocols/RF24Network

# CPP source files
SOURCES_CPP += sources/rf_protocols/RF24Network/RF24Network.cpp
SOURCES_CPP += sources/rf_protocols/RF24Network/Sync.cpp

