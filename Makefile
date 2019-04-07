all: virtualMemoryManager

virtualMemoryManager: virtualMemoryManager.c
	gcc virtualMemoryManager.c -o virtualMemoryManager

clean:
	rm -rf virtualMemoryManager

run: 
	./virtualMemoryManager addresses.txt