# Makefile для программы "Угадай число"

CC = gcc
CFLAGS = -Wall -Wextra
TARGET1 = ex1
TARGET2 = ex2

# Исходные файлы
SRC1 = Ex1.c
SRC2 = Ex2.c

# Правила сборки
all: $(TARGET1) $(TARGET2)

$(TARGET1): $(SRC1)
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET2): $(SRC2)
	$(CC) $(CFLAGS) -o $@ $^

# Очистка скомпилированных файлов
clean:
	rm -f $(TARGET1) $(TARGET2)

# Правила для запуска
run_ex1: $(TARGET1)
	./$(TARGET1) 80  # Замените 100 на желаемое значение N

run_ex2: $(TARGET2)
	./$(TARGET2) 80 # Замените 100 на желаемое значение N

.PHONY: all clean run_ex1 run_ex2
