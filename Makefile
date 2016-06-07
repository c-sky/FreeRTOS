NAME   = freertos
CDIR   = .
BUILDDIR= $(CDIR)/build

export CASE_NAME 

CASE_NAME = user

TARGET_LIB_OS_ONLY = $(BUILDDIR)/startup.a \
			 $(BUILDDIR)/freertos.a \
			 $(BUILDDIR)/driver.a \
			 $(BUILDDIR)/lib.a \
             


LINKFILE = ckcpu.ld

BUILD_OBJ = ./build/$(CASE_NAME).o ./build/main.o

TARGET_CPU = ck610

SOC	= ck5a6

export TARGET_CPU SOC

ifeq ($(TARGET_CPU), ck610)
CC      = csky-elf-gcc
LD      = csky-elf-ld
AR      = csky-elf-ar
AS      = csky-elf-as
GS      = csky-elf-gasp
OBJDUMP = csky-elf-objdump
else
CC      = csky-abiv2-elf-gcc
LD      = csky-abiv2-elf-ld
AR      = csky-abiv2-elf-ar
AS      = csky-abiv2-elf-as
GS      = csky-abiv2-elf-gasp
OBJDUMP = csky-abiv2-elf-objdump
endif
RM      = rm
MV      = mv


ENDIAN = -mlittle-endian
CFLAGS += -c -g -Os -ffunction-sections -Wall
CFLAGS += $(ENDIAN)
CFLAGS += -mcpu=$(TARGET_CPU)
ifeq ($(ENDIAN),-mbig-endian)
AFLAGS = -EB -mcpu=$(TARGET_CPU)
else
AFLAGS = -EL -mcpu=$(TARGET_CPU)
endif

export CC AS AR LD GS RM OBJDUMP CFLAGS AFLAGS MV

all: os_only

target_lib:
	make -C ./freertosv8.2.3/FreeRTOS/Source/
	make -C ./freertosv8.2.3/drivers/$(SOC)
	make -C ./freertosv8.2.3/lib

target_obj:
	make -C ./freertosv8.2.3/startup
	make -C ./users/

build_dir:
	if [ ! -d $(BUILDDIR) ]; \
	then mkdir $(BUILDDIR); \
	fi


os_only: build_dir target_obj target_lib 
	echo $(BUILD_OBJ)
	$(CC) $(ENDIAN) -mcpu=$(TARGET_CPU) -static \
	-nostartfiles -o $(BUILDDIR)/freertos.elf \
	$(BUILD_OBJ) $(TARGET_LIB_OS_ONLY) \
	-Wl,-T./freertosv8.2.3/ld/$(LINKFILE) \
	-lm -lc -lgcc -Wl,-gc-sections
	$(OBJDUMP) -S $(BUILDDIR)/freertos.elf > $(BUILDDIR)/freertos.asm


clean:
	$(RM) -rf $(BUILDDIR)
	find . -type f -name '*.o' -o -name '*.a' -o -name '*.bin' \
	| xargs rm -f

