include ../../makefile.defs
SRCDIRS = .
SRCEXTS = .cpp
EXE= KCPServer

LIBDIR= ../../..
SODIR=-L../../Bin/

INCLUDE= -I../../Net/inc/
SHARELIBDIR=

SHARELIBS= -lpthread -levent -lNetEvent -lkcp -lNetFilter
STATICLIBS= 

SOURCES = ServerMain.cpp  TestDataTrans.cpp  TestIOEvent.cpp
OBJS = $(foreach x, $(SRCEXTS), $(patsubst %$(x), %.o, $(filter %$(x), $(SOURCES))))

$(EXE): $(OBJS) $(STATICLIBS)
	$(CXX) $(INCLUDE) $(SODIR) -o $@ $(OBJS) $(STATICLIBS) $(AM_CXXFLAGS) $(SHARELIBS) $(SHARELIBDIR)

objs: $(OBJS)
%.o : %.cpp
	$(CXX) $(AM_CXXFLAGS) $(CXXFLAGS) $(INCLUDE) -c $< -o $@ -pthread

clean:
	rm -rf $(OBJS) $(EXE)
	rm -rf log/*
