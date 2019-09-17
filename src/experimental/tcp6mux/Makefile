# Be sure to set LD_LIBRARY_PATH, this build uses dynamic libs

DOORS_BUILD_DIR=/share/prj/doors/bill/newtests/tcp6mux/doors

DOORS_INCLUDES_DIR = -I$(DOORS_BUILD_DIR)/include/

DOORS_LIB_DIRS = -L$(DOORS_BUILD_DIR)/lib/ 

DOORS_BIN_DIR = $(DOORS_BUILD_DIR)/bin/

DOORS_LIBS = -ldoors_tools -ldoors_protocols -ldoors_base

CXXFLAGS= -g $(DOORS_INCLUDES_DIR)

CXX = CC

TCP6MUXOBJS = tcp6mux_main.o tcp6mux.o tcp6muxesm.o tcp6muxcsm.o tcp6muxent.o tcp6muxconn.o 

all: tcp6multiplex

tcp6multiplex: $(TCP6MUXOBJS)
	$(CXX) $(CXXFLAGS) -o tcp6multiplex $(TCP6MUXOBJS) $(DOORS_LIB_DIRS) $(DOORS_LIBS)

tcp6mux_main.C: tcp6muxent.h

tcp6muxent.C: tcp6muxent.h

tcp6muxent.h: tcp6muxesm.h tcp6mux.h

tcp6mux.h: tcp6mux.sap
	$(DOORS_BIN_DIR)/dsapg --cc-file tcp6mux.C --hh-file tcp6mux.h tcp6mux.sap

tcp6mux.C: tcp6mux.h

tcp6muxesm.C: tcp6muxesm.h

tcp6muxesm.h: tcp6muxesm.sm
	$(DOORS_BIN_DIR)/dsmg --cc-file tcp6muxesm.C \
	--hh-file tcp6muxesm.h tcp6muxesm.sm

tcp6muxcsm.C: tcp6muxcsm.h

tcp6muxcsm.h: tcp6muxcsm.sm
	$(DOORS_BIN_DIR)/dsmg --cc-file tcp6muxcsm.C \
	--hh-file tcp6muxcsm.h tcp6muxcsm.sm

clean:
	rm -rf SunWS_cache/ tcp6multiplex core *.o tcp6muxcsm.? tcp6muxesm.? tcp6mux.? *~
