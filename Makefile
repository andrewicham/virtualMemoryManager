all: virtualMemoryManager

virtualMemoryManager: virtualMemoryManager.c
	gcc virtualMemoryManager.c -lpthread -o virtualMemoryManager

clean:
	rm -rf virtualMemoryManager

run: 
	./virtualMemoryManager