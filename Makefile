CC=g++
CFLAGS=-c -Wall
LDFLAGS=-L/usr/lib/x86_64-linux-gnu -lboost_regex
SOURCES=Main.cc \
		src/server.cc \
		src/abor.cc \
		src/cdup.cc \
		src/cmdfactory.cc \
		src/command.cc \
		src/cwd.cc \
		src/dele.cc \
		src/epsv.cc \
		src/help.cc \
		src/list.cc \
		src/mdtm.cc \
		src/mkd.cc \
		src/nlst.cc \
		src/noop.cc \
		src/pass.cc \
		src/pasv.cc \
		src/port.cc \
		src/pwd.cc \
		src/quit.cc \
		src/rein.cc \
		src/retr.cc \
		src/rmd.cc \
		src/size.cc \
		src/stor.cc \
		src/syst.cc \
		src/user.cc \
		src/type.cc 
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=lftpd

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC)  $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@



