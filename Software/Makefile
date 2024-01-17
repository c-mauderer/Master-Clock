MCU = attiny402
BUILDDIR = build
SRCDIR = src
DEPDIR := .d
TARGET = $(BUILDDIR)/master_clock

SRC =
SRC += $(SRCDIR)/main.c

REV = $(shell git rev-parse --short HEAD)
DATE = $(shell date "+%Y-%m-%d %H:%M:%S")
CFLAGS = -DREVISION="\"$(REV)\"" -DBUILDDATE="\"$(DATE)\"" -mmcu=$(MCU) -Wall -Wextra -std=c11 -O2 -g
LDFLAGS =

AVRDUDE_FLAGS = -p $(MCU)
DW_FLAGS = -c dragon_dw
HV_FLAGS = -c dragon_hvsp
AVARICE_FLAGS = --dragon --jtag usb --debugwire

CC = avr-gcc
GDB = avr-gdb
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
NM = avr-nm
AVRDUDE = avrdude
AVARICE = avarice

OBJ = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SRC))
DEPS = $(patsubst $(SRCDIR)/%.c,$(DEPDIR)/%.d,$(SRC))

# dependency generation
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

all: elf hex eep

elf: $(TARGET).elf
hex: $(TARGET).hex
eep: $(TARGET).eep

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)
	rm -rf $(DEPDIR)

.PHONY: ctags
ctags:
	ctags --recurse $(SRCDIR) /usr/avr/include

.PHONY: reset
reset:
	avarice $(AVARICE_FLAGS)

flash: $(TARGET).hex
	@echo "*****************************"
	@echo "don't forget to set the fuses"
	@echo "*****************************"
	@echo " Fuses in the elf:"
	@ $(OBJDUMP) -s --section=.fuse $(TARGET).elf
	@echo "*****************************"
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(DW_FLAGS) -U flash:w:$(TARGET).hex

fuses: $(TARGET).elf
	@echo "HV PROGRAMMING NECESSARY. SEE AVR Dragon Manual Chapter 6.4"
	$(eval FUSES := $(shell $(OBJDUMP) -s --section=.fuse $(TARGET).elf | grep 820000 | cut -d' ' -f3))
	$(eval LFUSE := $(shell echo -n $(FUSES) | head -c2))
	$(eval HFUSE := $(shell echo -n $(FUSES) | tail -c2))
	avrdude $(AVRDUDE_FLAGS) $(HV_FLAGS) -U lfuse:w:0x$(LFUSE):m -U hfuse:w:0x$(HFUSE):m

eeprom: $(TARGET).eep
	$(AVRDUDE) $(AVRDUDE_FLAGS) -U eeprom:w:$(TARGET).eep

debugserver:
	$(AVARICE) $(AVARICE_FLAGS) --part $(MCU) --jtag-bitrate 2000 localhost:1234

debug: $(TARGET).elf
	$(GDB) -ex "target remote localhost:1234" $<

%.hex : %.elf
	$(OBJCOPY) -O ihex -R .eeprom -R .fuse $< $@

%.eep : %.elf
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 -O ihex $< $@

$(TARGET).elf: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) --output $@ $(LDFLAGS)

$(BUILDDIR)/%.o : $(SRCDIR)/%.c $(DEPDIR)/%.d
	mkdir -p $(dir $@)
	mkdir -p $(dir $(patsubst $(SRCDIR)/%.c,$(DEPDIR)/%.d,$<))
	$(CC) $(DEPFLAGS) $(CFLAGS) -c $< -o $@
	mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

-include $(DEPS)
