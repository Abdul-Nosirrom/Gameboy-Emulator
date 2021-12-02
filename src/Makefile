sim: cpu.h main.cpp cpu.o log.cpp
	g++ -g main.cpp log.o cpu.o cpu.opcodes.o -o $@

cpu.o:cpu.cpp cpu.opcodes.cpp cpu.h
	g++ -g cpu.cpp cpu.opcodes.cpp -c

log.o: log.cpp log.h
	g++ -g log.cpp -c

clean:
	rm cpu.o cpu.opcodes.o