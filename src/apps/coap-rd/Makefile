# Makefile for altbit
#
# NOTE: Because dynamic DOORS libraries are used, the library path must
#       exist in LD_LIBRARY_PATH which can be used in adding extra library
#       directories. For example:
#
#       tcsh: setenv LD_LIBRARY_PATH /share/faster/doors/install/lib
#       bash: export LD_LIBRARY_PATH=/share/faster/doors/install/lib
#       zsh: Who cares?
 

#CXX = g++
DOORS_INCLUDES = -I/home/bilhanan/openwrt-experiments/doors/buffalo-ap/include
DOORS_LIBS = -L/home/bilhanan/openwrt-experiments/doors/buffalo-ap/lib -ldoors_tools -ldoors_protocols -ldoors_base
CXXFLAGS = $(DOORS_INCLUDES)
COAPCLIENTOBJS = coappdu.o coapsap.o coapclient.o coapclienttask.o client.o options.o 
COAPSERVEROBJS = coappdu.o coapsap.o coapserver.o coapservertask.o server.o options.o 


all: server

client: $(COAPCLIENTOBJS)
	$(CXX) $(CXXFLAGS) -o coapclient $(COAPCLIENTOBJS) $(DOORS_LIBS)

server: $(COAPSERVEROBJS)
	$(CXX) $(CXXFLAGS) -o coapserver $(COAPSERVEROBJS) $(DOORS_LIBS)

#options.cpp: options:h

coapclient.cpp: coapclienttask.hh
coapserver.cpp: coapservertask.hh

coapclienttask.cc: coapclienttask.hh
coapclienttask.hh: coapclient.hh coappdu.hh coapsap.hh

coapservertask.cc: coapservertask.hh
coapservertask.hh: coapserver.hh coappdu.hh coapsap.hh

coapsap.cc: coapsap.hh

coapclient.cc: coapclient.hh

coapserver.cc: coapserver.hh

coappdu.cc: coappdu.hh options.hh

clean:
	rm -rf *.o coapClient coapServer

