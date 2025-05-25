# === USER CONFIGURATION ===
MCU     = atmega328p
F_CPU   = 16000000UL
TARGET  = main
SRC     = $(wildcard *.c)

# === Tools ===
CC       = avr-gcc
OBJCOPY  = avr-objcopy
SIZE     = avr-size
OBJDUMP  = avr-objdump
AVRDUDE  = avrdude

FORMAT   = ihex
BAUD     = 115200
PROGRAMMER = arduino

# === Auto-detect USB port (macOS) ===
PORT := $(shell ls /dev/cu.usbmodem* /dev/cu.usbserial* 2>/dev/null | head -n 1)

# === Directories ===
OBJDIR = build

# === Flags ===
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall -std=gnu99
LDFLAGS = -mmcu=$(MCU)

# === Build Targets ===

all: $(OBJDIR) $(OBJDIR)/$(TARGET).hex

$(OBJDIR):
	@mkdir -p $@

$(OBJDIR)/$(TARGET).elf: $(SRC)
	@echo "🔧 Compiling..."
	$(CC) $(CFLAGS) $(SRC) -o $@

$(OBJDIR)/$(TARGET).hex: $(OBJDIR)/$(TARGET).elf
	@echo "📦 Generating HEX..."
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

flash: $(OBJDIR)/$(TARGET).hex
	@if [ -z "$(PORT)" ]; then \
		echo "❌ Error: No USB serial port found. Plug in your board."; \
		exit 1; \
	fi
	@echo "🔌 Flashing to $(PORT)..."
	$(AVRDUDE) -c $(PROGRAMMER) -p $(MCU) -P $(PORT) -b $(BAUD) -U flash:w:$<

size: $(OBJDIR)/$(TARGET).elf
	$(SIZE) -C --mcu=$(MCU) $<

disasm: $(OBJDIR)/$(TARGET).elf
	@echo "🪓 Disassembling..."
	$(OBJDUMP) -d -S $< > $(OBJDIR)/$(TARGET).lss
	@echo "📄 Output: $(OBJDIR)/$(TARGET).lss"

clean:
	@echo "🧹 Cleaning..."
	rm -rf $(OBJDIR)

.PHONY: all flash clean size disasm

