
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/time.h>

#include <vector>
#include <string>
#include <memory>
#include <iostream>

#include "log.h"

// Establish some system macros
#define MEMSIZE (1<<16)
// Some memory information like which addresses belong to what here as well, not yet though


/*
 * Typedefs for clarity
 */
typedef uint8_t     byte;
typedef int8_t      Sbyte;
typedef uint16_t    word;
typedef uint32_t    doubleword;
typedef uint64_t    quadword;

/*
  Size to pass through read and write memory methods
*/
enum memtypes
{
    BYTE,
    WORD,
    DOUBLEWORD,
    QUADWORD
};

/*
  REGISTERS:
  8080 has seven 8-bit registers [A,B,C,D,E,H,L]
   - A: 8-bit accumulator 
   - BC, DE, HL: BDH accumulators can be used as 16-bit register pairs (Confused by this)
  Then you've got your stack pointer [SP] and program counter [PC] which are 16-bit

    (Comment out further explaining the purpose of each register when you actually understand)
*/
union Register
{
    word reg;
    struct 
    {
      byte low;
      byte high;
    };
};


typedef struct registers
{
    Register A;
    Register BC;
    Register DE;
    Register HL;

    // Stack pointer and Program Counter
    Register SP;
    Register PC;
} registers;


/*
  OPCODE FLAGS:
  Flag bits which indicate results of arithmetic and logical instructions
    (Don't really know what they are just yet)
  One bit for flags (hence :1) except AC 
*/
typedef struct OPflags
{
    byte Z  : 1;     // (Zero) Set if result is zero
    byte S  : 1;     // (Sign) Set if result is negative
    byte P  : 1;     // (Parity) Set if results' number of 1-bits is even
    byte C  : 1;     // (Carry) Set if addition operation resulted in carry or subtraction in borrow
    byte AC : 1;     // (Auxillary Carry) I don't know yet
} OPflags;

/*
  Flag Conditions - useful for cycling through Jump conditions
*/
typedef enum Conditions{ Z,S,C,AC, NONE } Conditions;

class CPU
{
    public:
                                    CPU             ();
                                    ~CPU            ();
        bool                        LoadInstructions  (const std::string fileName);
        void                        FillMem           (byte instr);
        void                        DumpMem           (word start, word end);
        void                        Cycle             ();

    private:
        registers                   m_Registers;
        OPflags                     m_Flags;
        byte                        m_Memory[MEMSIZE];

        // Execute given opcode
        byte                        Fetch           ();
        bool                        Execute         (byte opcode);

        // Read from
        byte                        ReadByte        (word address);
        word                        ReadWord        (word address);

        void                        WriteByte       (word address, byte val);
        void                        WriteWord       (word address, word val);

        // Stack Related Shit
        void                        StackPush       (word val);
        word                        StackPop        ();

        void                        ResetRegisters  ();
        void                        ResetFlags      ();

        // Flag Setters
        void                        SetFlag_Carry   (bool val);
        void                        SetFlag_AuxCarry(bool val);
        void                        SetFlag_Zero    (bool val);
        void                        SetFlag_Sign    (bool val);
        bool                        GetCondFlag     (Conditions condition);


        // Actual instructions
        void                        INSTR_ADD        (byte& dest, byte source, bool addCFlag);
        void                        INSTR_LOAD       (byte& dest, byte source);
        void                        INSTR_LOAD_MEM   (byte& dest, word address);
        void                        INSTR_WRITE_MEM  (word address, byte source);
        void                        INSTR_SUB        (byte& dest, byte source, bool subCFlag);
        void                        INSTR_AND        (byte& dest, byte source);
        void                        INSTR_OR         (byte& dest, byte source);
        void                        INSTR_XOR        (byte& dest, byte source);
        void                        INSTR_CMP        (byte dest, byte source);
        void                        INSTR_INC        (byte& dest);
        void                        INSTR_INC_16BIT  (word& dest);
        void                        INSTR_INC_MEM    (word address);
        void                        INSTR_DEC        (byte& dest);
        void                        INSTR_DEC_16BIT  (word& dest);
        void                        INSTR_DEC_MEM    (word address);
        void                        INSTR_JUMP       (Conditions condition, bool condiStatus);
        void                        INSTR_JUMP_IM    (Conditions condition, bool condiStatus);
        void                        INSTR_CALL       (Conditions condition, bool condiStatus);
        void                        INSTR_RETURN     (Conditions condition, bool condiStatus);
        // Word Instructions
        void                        INSTR_LOAD_WORD  (word& dest, word source);

};