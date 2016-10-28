BUILDDIR= ../../build
SRC=  $(CASE_NAME).c main.c
OBJ= $(SRC:.c=.o)
TAR= user.a

INC_DIR	= ../../include
INC1_DIR = ../../freertosv8.2.3/FreeRTOS

CFLAGS += -I$(INC_DIR) \
		  -I$(INC_DIR)/csky \
		  -I$(INC_DIR)/lib \
		  -I$(INC_DIR)/$(SOC) \
		  -I$(INC1_DIR)/Source/include \
		  -I$(INC1_DIR)/Source/portable/GCC/$(TARGET_CPU)


all: $(TAR)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

$(TAR):$(OBJ)
	$(AR) r $(BUILDDIR)/$(TAR) $(OBJ)
	mv $(OBJ) $(BUILDDIR)

clean:
	$(RM) -rf *.o
	$(RM) -rf *.a
	$(RM) -rf *.elf

