MCU = attiny402
BUILDDIR = build
SRCDIR = src
DEPDIR := .d
TARGET = $(BUILDDIR)/master_clock

SRC =
SRC += $(SRCDIR)/main.c

REV = $(shell git rev-parse --short HEAD)
DATE = $(shell date "+%Y-%m-%d %H:%M:%S")
CFLAGS = -DREVISION="\"$(REV)\"" -DBUILDDATE="\"$(DATE)\"" -mmcu=$(MCU) -Wall -Wextra -std=c11 --param=min-pagesize=0 -O2 -g
LDFLAGS =

AVRDUDE_FLAGS = -p $(MCU) -c snap_updi

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

all: elf hex

elf: $(TARGET).elf
hex: $(TARGET).hex

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)
	rm -rf $(DEPDIR)

.PHONY: ctags
ctags:
	ctags --recurse $(SRCDIR) /usr/avr/include

flash: $(TARGET).hex
	@echo "*****************************"
	@echo "don't forget to set the fuses"
	@echo "*****************************"
	@echo " Fuses in the elf:"
	@ $(OBJDUMP) -s --section=.fuse $(TARGET).elf
	@echo "*****************************"
	$(AVRDUDE) $(AVRDUDE_FLAGS) -U flash:w:$<

fuses: $(TARGET).fuses.hex
	avrdude $(AVRDUDE_FLAGS) -U fuses:w:$<

debugserver:
	$(AVARICE) $(AVARICE_FLAGS) --part $(MCU) --jtag-bitrate 2000 localhost:1234

debug: $(TARGET).elf
	$(GDB) -ex "target remote localhost:1234" $<

%.hex : %.elf
	$(OBJCOPY) -O ihex -R .eeprom -R .fuse $< $@

%.fuses.hex : %.elf
	-$(OBJCOPY) -j .fuse --change-section-lma .fuse=0 -O ihex $< $@

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
