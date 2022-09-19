
EE_OBJS = src/Hyperblock.o src/Pad.o
EE_OBJS += src/screens/MainMenu.o src/screens/MemcardStatus.o src/screens/MemcardFormat.o src/screens/MemcardUnformat.o src/screens/FileWrite.o src/screens/Superblock.o 
EE_BIN = hyperblock.elf
EE_LIBS = -lkernel -lgraph -lpad -lmc -lstdc++ -ldebug

all: $(EE_BIN)

clean:
	rm -f $(EE_BIN) $(EE_OBJS) 

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
