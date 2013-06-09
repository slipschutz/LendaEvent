ltCXX=$(shell root-config --cxx)
CFLAGS=-c -g -Wall $(shell root-config --cflags) -I./src -I ./include
LDLIBS=$(shell root-config --glibs)
LDFLAGS=$(shell root-config --ldflags)
#SOURCES=./src/SL_Event.cc ./src/FileManager.cc ./src/Filter.cc

EXTRASOURCE=Introspective.cc CorrectionInfo.cc
EXTRAOBJS=$(patsubst %.cc,%.o,$(EXTRASOURCE))

ROOTCINT=rootcint

NAME=LendaEvent

DICS=LendaEvent #Introspective CorrectionInfo

temp= $(addprefix lib,$(NAME))
temp2=$(addsuffix $(LDFLAGS),$(temp))
LIBRARY= $(addsuffix .so,$(temp2))
DICTNAME=$(addsuffix Dictionary,$(DICS))
DICTOBJ=$(addsuffix .o,$(DICTNAME))
OBJECTS=$(addsuffix Dict.o, $(NAME))

.PHONY: clean get put all test sclean

all: $(LIBRARY) 

$(LIBRARY) : $(OBJECTS) $(EXTRAOBJS) $(DICTOBJ)
	@echo "Building Library"
	$(CXX) `root-config --cflags` -fPIC -shared -o $@ $^;
	@echo "Build succeed"

%Dictionary.o : %.hh %LinkDef.h
	@echo "Generating Dictionary"
	@$(ROOTCINT) -f $(patsubst %.o,%.cc,$@) -c $^;
	@echo "Compiling Dicionary"
	$(CXX) -p -fPIC $(CFLAGS) -c $(patsubst %.o,%.cc,$@)

%Dict.o : %.cc %.hh
	@echo "Making mapped file for" $< "..."
	./Parse $(patsubst %.cc,%.hh,$<)
	@echo "Compiling mapped file" $@
	$(CXX) $(CFLAGS) -fPIC $(patsubst %.o,%.cc,$@) -o $@ 

%.o : %.cc
	@echo "Compiling" $< "..."
	$(CXX) $(CFLAGS) -fPIC $< -o $@ 

test:
	@echo $(LIBRARY) : $(DICTOBJ) $(OBJECTS)
	echo $(EXTRAOBJS)


clean:
	-rm -f $(DICTOBJ) $(OBJECTS) $(EXTRAOBJS)
	-rm -f *Dictionary.h
	-rm -f *Dictionary.cc
	-rm -f $(LIBRARY)
	-rm -f *Dict.cc
sclean:
	-rm *~
	make clean