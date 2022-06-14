#Definição do nome do executavel 
PROG = blinky

#Variaveis para definir o compilador
CC = arm-none-eabi-gcc
LD = arm-none-eabi-gcc
CP = arm-none-eabi-objcopy

#Flags de compilação
CFLAGS = -mcpu=cortex-m4 -mthumb
LFLAGS = -nostdlib -T stm32f411-rom.ld

#Padronização dos prerequisites
OBJS = startup.o \
       main.o

#Criação do executavel
all: $(PROG).elf

$(PROG).elf: $(OBJS)
	$(LD) $(LFLAGS) $^ -o $@
	$(CP) $(PROG).elf -O binary $(PROG).bin

#Compilação dos .c
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

#Apaga arquivos
clean:
	rm -f $(OBJS) $(PROG).elf