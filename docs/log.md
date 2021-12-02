# Project Log

You should use this file to document all your meetings and general progress/notes.

Each entry should have a date and assocaited notes eg.


## DATE: Mon Sept 13, 2021

### Notes
We could not meet since we don't exist as a group yet ... but hopefully soon ;-)
I am sure are project is going to be really cool!

### Progress

So far we have gotten nothing done

### Goals / Next steps

1. Meet each other and talk about what project we would like to do
2. Setup a meeting with the professor to chat about our project
3. Settle on the project
4. Starting writting up the proposal

## DATE: Mon Sept 27, 2021
### Meeting with professor

- WebAssembly as possible target, test it out

- Suggestions:
  1. Start with a simple loop for the FEL:
    - start with a simple cpu.c program and test fetch decode execute functions in loop
  2. Write a small z80/6502 assembly program (look for those CPUs manuals) for testing
  3. Hard code pc to some value,  and put one OPcode there in memory, to bootstrap our fetch
  4. Slowly build up the instruction set as you test out specific ROMs
  5. Consider memcpy on z80 page as an initial fixed program to test on
  6. Find a toolchain for the gameboy (assembler, or C compiler for it) so we could test our own program
  7. Start with a 8080 as a base - toolchains are easily accessible

- Things for cpu.c test program
  1. PC
  2. IR
  3. Address Buffer

- Kazojc 8080 datasheet, look it up

## DATE: Wed Sept 29, 2021
### JA: Comment
- You might want to look at sdcc (Small Device C Compiler) https://sourceforge.net/projects/sdcc/ as a toolchain for building binary memory images for your simulator

## DATE: Thur Oct 14, 2021
### First piece of code written (CPU class)
- Began working on the 8080 emulator portion
- What was written was just a base header file highlighting the structure of our CPU emulator
- Still a bit apprehensive about how to represent the registers and OPCode flags (don't quite know much about these yet -Abdul)
- Currently consists of:
  1. Registers and Flags represented 
  2. Some basic function defnitions for reading/writing to memory, executing an opcode, and cycle

- NOTE: Might be a better way to represent BDH registers since they're pairs, so rather than individual B,C,D,E,H,L we could define them as pairs instead?

## DATE: Mon Oct 25, 2021
### Working Instruction Loop, Computer and Main
- Implemented C verison of computer&CPU struct as well as instruction cycle, allows for simulation of computer memory and instruction cycle
- Need to clarify that my execute function is performed within my decode stage 
- Need to converse more with group to ensure we are all the same page for implementation and strategy
- to do:
  1. load a basic operation into ram see how cpu executes 
  2. work on better understanding how flags function
  3. implement a few other opcodes for testing

## DATE Thurs Nov 4, 2021
### Implemetation of General Opcodes and Memory for testing
- Abdul implemented a lot of different general cases for various opcodes for explain, add, sub, load, jump, or, xor and began to apply them for each individual case 
- Abdul as well created his own version for loading and dumping memory
- Griffin and Professor created a file that consists of all the general cases of the opcodes for the z80, he has began to try and implement these into his already working code but is a little confused about how to generalize, after looking at Abduls code he better understands what to needs to be done and see how this implementation works 
- to do: 
  1. continue to implement opcodes as they arise
  2. modify code to meet the opcodes and instruction syntax for the z80
  3. continue to collaborate with partners to ensure we have a mutual understanding of what must be done

## DATE Thurs Nov 18, 2021
### Implementing Z80 CPU and opcodes with flags
- created a new struct to represent the z80 cpu and appropriate flags 
- working on implementing opcodes and setting flags accordingly 
- throughly reading through the z80 instruction manual to figure out the complexities of the processor
