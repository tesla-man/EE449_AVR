BUILDDIR = build
UTILS = /home/teslaman/Documents/AVR/EE449/utils
F_CPU = 18432000UL
#F_CPU = 8000000UL
SOURCES += controller.c

SOURCES += $(UTILS)/lcd/lcd.c \
		   $(UTILS)/i2c/i2c.c

OBJECTS = $(addprefix $(BUILDDIR)/, $(addsuffix .o, $(basename $(SOURCES))))

INCLUDES += -I$(UTILS)/i2c \
			-I$(UTILS)/lcd \
			-I.



ELF = $(BUILDDIR)/program.elf
HEX = $(BUILDDIR)/program.hex
BIN = $(BUILDDIR)/program.bin

CC = avr-gcc
OBJCOPY = avr-objcopy
DUDE = avrdude
CFLAGS = -g  -Os -mmcu=atmega328p $(INCLUDES) -Wall -D F_CPU=$(F_CPU)
OBJFLAGS = -j .text -j .data -O ihex
DUDEFLAGS = -p ATmega328P -c avrispmkii -v


#all: main.hex $(CMDLINE)

#	gcc  -O -Wall

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

$(HEX): $(ELF)
	$(OBJCOPY) $(OBJFLAGS) $< $@

$(ELF): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

$(BUILDDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILDDIR)/%.o: %.s
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

flash: $(BIN)
	$(DUDE) $(DUDEFLAGS) -U flash:w:$<

clean:
	rm -rf build
