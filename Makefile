MCU = at90usb162 
FCPU = 8000000

CC = avr-gcc
LD = avr-gcc
FLAGS = -mmcu=$(MCU)
CFLAGS = -Wall $(FLAGS) -DF_CPU=$(FCPU) -Os
LDFLAGS = $(FLAGS)

PROGRAMMER = dfu-programmer

TARGET = key
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET).hex

clean:
	rm -f *.o $(TARGET).hex $(TARGET)

$(TARGET): $(OBJECTS)

$(TARGET).hex: $(TARGET)
	avr-objcopy -j .text -j .data -O ihex $(TARGET) $(TARGET).hex

upload: $(TARGET).hex
	$(PROGRAMMER) $(MCU) erase
	$(PROGRAMMER) $(MCU) flash $(TARGET).hex
	$(PROGRAMMER) $(MCU) reset

size: $(TARGET)
	avr-size --format=avr --mcu=$(MCU) $(TARGET)
