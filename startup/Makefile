BUILDDIR= ../build
SRC= $(wildcard *.s)
OBJ= $(SRC:.s=.o)
TAR = startup.a
INC_DIR	= ./include

CFLAGS += -I$(INC_DIR) -I$(INC_DIR)/$(SOC) -I$(INC_DIR)/csky	-I$(INC_DIR)/lib

all: $(TAR)


%.o: %.s 
	$(AS) $(AFLAGS) $< -o $@

$(TAR): $(OBJ)
	$(AR) r $(BUILDDIR)/$(TAR) $(OBJ)
	mv $(OBJ) $(BUILDDIR)

clean:
	$(RM) -rf $(BUILDDIR)/*.o 
	$(RM) -rf $(BUILDDIR)/*.a
	$(RM) -rf $(BUILDDIR)/*.elf

