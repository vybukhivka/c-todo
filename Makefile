CC = gcc
CFLAGS = -O3
TARGET = todo
INSTALL_PATH = /usr/local/bin/$(TARGET)

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) main.c -o $(TARGET)

install: $(TARGET)
	@echo "Installing $(TARGET) to /usr/local/bin..."
	sudo mv $(TARGET) $(INSTALL_PATH)
	@echo "Installation complete. Try run: 'todo'"

uninstall: 
	@echo "Removing $(TARGET) from /usr/local/bin..."
	sudo rm -f $(INSTALL_PATH)
	@echo "Uninstalled"

clear-data:
	rm -f ~/.todo_data.txt

clean:
	rm -f $(TARGET)
