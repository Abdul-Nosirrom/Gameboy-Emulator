#include "cpu.h"

CPU::CPU()
{
    // Initiate memory
    memset(m_Memory, 0, sizeof(m_Memory));
    // Reset Registers
    ResetRegisters();
}

CPU::~CPU()
{
    exit(1);
}

/*
    Reset registers to some default value (unsure if there is a default value
    so I'm setting it all to zero for now)
*/
void CPU::ResetRegisters()
{
    m_Registers.A.low   = 0x00;
    m_Registers.A.high  = 0x00;
    m_Registers.BC.low  = 0x00;
    m_Registers.BC.high = 0x00;
    m_Registers.DE.low  = 0x00;
    m_Registers.DE.high = 0x00;
    m_Registers.HL.low  = 0x00;
    m_Registers.HL.high = 0x00;
    m_Registers.PC.reg  = 0x100;
    m_Registers.SP.reg  = 0x00;
}

void CPU::ResetFlags()
{
    m_Flags.AC  = 0;
    m_Flags.C   = 0;
    m_Flags.P   = 0;
    m_Flags.S   = 0;
    m_Flags.Z   = 0;
}

/*
    Flag Setters
*/
void CPU::SetFlag_Carry       (bool val) { m_Flags.C  = val; }
void CPU::SetFlag_AuxCarry    (bool val) { m_Flags.AC = val; }
void CPU::SetFlag_Zero        (bool val) { m_Flags.Z  = val; }
void CPU::SetFlag_Sign        (bool val) { m_Flags.S  = val; }

/* Get Flag of specified condition */
bool CPU::GetCondFlag(Conditions condition)
{
    switch (condition)
    {
        case Z:     return m_Flags.Z;
        case S:     return m_Flags.S;
        case C:     return m_Flags.C;
        case AC:    return m_Flags.AC;
    }
    return false;
}


/*
    Load instructions into m_memory
*/
bool CPU::LoadInstructions(const std::string fileName)
{
    FILE *fp;
    fp = fopen(fileName.c_str(), "rb");
    if (fp == NULL)
    {
        perror("Instruction File Open Failed:");
        return false;
    }
    fread(m_Memory, 1, MEMSIZE, fp);
    fclose(fp);
    return true;
}

void CPU::FillMem(byte val)
{
    memset(m_Memory, val, sizeof(m_Memory));
}

void CPU::DumpMem(word start, word end)
{
    word i;
    for(i=start; i< end; i++)
    {
        if ((i%16)==0 && (i != start)){
            printf("\n");
        }
        printf("%02X ", m_Memory[i]);
    }
    printf("\n");
}

void CPU::WriteByte(word address, byte val)
{
    m_Memory[address] = val;
}

void CPU::WriteWord(word address, word val)
{
    byte low = val & 0xFF;
    byte high = (val >> 8) & 0xFF;

    m_Memory[address] = low;
    m_Memory[address+1] = high;
}

byte CPU::ReadByte(word address)
{
    return m_Memory[address];
}

word CPU::ReadWord(word address)
{
    return (m_Memory[address + 1] << 8) | m_Memory[address];
}

/*
    Stack Accessor Methods
*/
void CPU::StackPush(word val)
{
    m_Registers.SP.reg -= 2;
    WriteWord(m_Registers.SP.reg, val);
}


word CPU::StackPop()
{
    word val = ReadByte(m_Registers.SP.reg);
    m_Registers.SP.reg += 2;

    return val;
}

/*
    Cycle - our loop basically
*/
void CPU::Cycle()
{
    byte curOp = Fetch();
    while (Execute(curOp))
    {
        sleep(1);
        curOp = Fetch();
    }
}

/*
*  Memory Reading related methods
*/

/*
    Fetch Method, self explanatory, get OPCode from memory
*/
byte CPU::Fetch()
{
    byte opCode = m_Memory[m_Registers.PC.reg];
    m_Registers.PC.reg++;
    return opCode;
}


bool CPU::Execute(byte opcode)
{
    INFO("Executing {:X}", opcode);
    switch (opcode)
    {   
        case 0x00: m_Registers.PC.reg++; break;

        case 0x10: m_Registers.PC.reg++; break;

        // Load Instructions

        // 8 Bit Immediate
		case 0x06: INSTR_LOAD(m_Registers.BC.high, ReadByte(m_Registers.PC.reg)); break;
		case 0x0E: INSTR_LOAD(m_Registers.BC.low, ReadByte(m_Registers.PC.reg));  break;
		case 0x16: INSTR_LOAD(m_Registers.DE.high, ReadByte(m_Registers.PC.reg)); break;
		case 0x1E: INSTR_LOAD(m_Registers.DE.low, ReadByte(m_Registers.PC.reg));  break;
		case 0x26: INSTR_LOAD(m_Registers.HL.high, ReadByte(m_Registers.PC.reg)); break;
		case 0x2E: INSTR_LOAD(m_Registers.HL.low, ReadByte(m_Registers.PC.reg));  break;

        // 16 Bit Immediate
        case 0x01: INSTR_LOAD_WORD(m_Registers.BC.reg, ReadWord(m_Registers.PC.reg)); break;
        case 0x11: INSTR_LOAD_WORD(m_Registers.DE.reg, ReadWord(m_Registers.PC.reg)); break;
        case 0x21: INSTR_LOAD_WORD(m_Registers.HL.reg, ReadWord(m_Registers.PC.reg)); break;
        case 0x31: INSTR_LOAD_WORD(m_Registers.SP.reg, ReadWord(m_Registers.PC.reg)); break;
        case 0xF9: INSTR_LOAD_WORD(m_Registers.SP.reg, m_Registers.HL.reg);           break;

		case 0x7F: INSTR_LOAD(m_Registers.A.high, m_Registers.A.high);  break;
		case 0x78: INSTR_LOAD(m_Registers.A.high, m_Registers.BC.high); break;
		case 0x79: INSTR_LOAD(m_Registers.A.high, m_Registers.BC.low);  break;
		case 0x7A: INSTR_LOAD(m_Registers.A.high, m_Registers.DE.high); break;
		case 0x7B: INSTR_LOAD(m_Registers.A.high, m_Registers.DE.low);  break;
		case 0x7C: INSTR_LOAD(m_Registers.A.high, m_Registers.HL.high); break;
		case 0x7D: INSTR_LOAD(m_Registers.A.high, m_Registers.HL.low);  break;
		case 0x40: INSTR_LOAD(m_Registers.BC.high, m_Registers.BC.high);break;
		case 0x41: INSTR_LOAD(m_Registers.BC.high, m_Registers.BC.low); break;
		case 0x42: INSTR_LOAD(m_Registers.BC.high, m_Registers.DE.high);break;
		case 0x43: INSTR_LOAD(m_Registers.BC.high, m_Registers.DE.low); break;
		case 0x44: INSTR_LOAD(m_Registers.BC.high, m_Registers.HL.high);break;
		case 0x45: INSTR_LOAD(m_Registers.BC.high, m_Registers.HL.low); break;
		case 0x48: INSTR_LOAD(m_Registers.BC.low, m_Registers.BC.high); break;
		case 0x49: INSTR_LOAD(m_Registers.BC.low, m_Registers.BC.low);  break;
		case 0x4A: INSTR_LOAD(m_Registers.BC.low, m_Registers.DE.high); break;
		case 0x4B: INSTR_LOAD(m_Registers.BC.low, m_Registers.DE.low);  break;
		case 0x4C: INSTR_LOAD(m_Registers.BC.low, m_Registers.HL.high); break;
		case 0x4D: INSTR_LOAD(m_Registers.BC.low, m_Registers.HL.low);  break;
		case 0x50: INSTR_LOAD(m_Registers.DE.high, m_Registers.BC.high);break;
		case 0x51: INSTR_LOAD(m_Registers.DE.high, m_Registers.BC.low); break;
		case 0x52: INSTR_LOAD(m_Registers.DE.high, m_Registers.DE.high);break;
		case 0x53: INSTR_LOAD(m_Registers.DE.high, m_Registers.DE.low); break;
		case 0x54: INSTR_LOAD(m_Registers.DE.high, m_Registers.HL.high);break;
		case 0x55: INSTR_LOAD(m_Registers.DE.high, m_Registers.HL.low); break;
		case 0x58: INSTR_LOAD(m_Registers.DE.low, m_Registers.BC.high); break;
		case 0x59: INSTR_LOAD(m_Registers.DE.low, m_Registers.BC.low);  break;
		case 0x5A: INSTR_LOAD(m_Registers.DE.low, m_Registers.DE.high); break;
		case 0x5B: INSTR_LOAD(m_Registers.DE.low, m_Registers.DE.low);  break;
		case 0x5C: INSTR_LOAD(m_Registers.DE.low, m_Registers.HL.high); break;
		case 0x5D: INSTR_LOAD(m_Registers.DE.low, m_Registers.HL.low);  break;
		case 0x60: INSTR_LOAD(m_Registers.HL.high, m_Registers.BC.high);break;
		case 0x61: INSTR_LOAD(m_Registers.HL.high, m_Registers.BC.low); break;
		case 0x62: INSTR_LOAD(m_Registers.HL.high, m_Registers.DE.high);break;
		case 0x63: INSTR_LOAD(m_Registers.HL.high, m_Registers.DE.low); break;
		case 0x64: INSTR_LOAD(m_Registers.HL.high, m_Registers.HL.high);break;
		case 0x65: INSTR_LOAD(m_Registers.HL.high, m_Registers.HL.low); break;
		case 0x68: INSTR_LOAD(m_Registers.HL.low, m_Registers.BC.high); break;
		case 0x69: INSTR_LOAD(m_Registers.HL.low, m_Registers.BC.low);  break;
		case 0x6A: INSTR_LOAD(m_Registers.HL.low, m_Registers.DE.high); break;
		case 0x6B: INSTR_LOAD(m_Registers.HL.low, m_Registers.DE.low);  break;
		case 0x6C: INSTR_LOAD(m_Registers.HL.low, m_Registers.HL.high); break;
		case 0x6D: INSTR_LOAD(m_Registers.HL.low, m_Registers.HL.low);  break;

        // Load from memory pointed to by source register
		case 0x7E: INSTR_LOAD_MEM(m_Registers.A.high, m_Registers.HL.reg);          break;
		case 0x46: INSTR_LOAD_MEM(m_Registers.BC.high, m_Registers.HL.reg);         break;
		case 0x4E: INSTR_LOAD_MEM(m_Registers.BC.low, m_Registers.HL.reg);          break;
		case 0x56: INSTR_LOAD_MEM(m_Registers.DE.high, m_Registers.HL.reg);         break;
		case 0x5E: INSTR_LOAD_MEM(m_Registers.DE.low, m_Registers.HL.reg);          break;
		case 0x66: INSTR_LOAD_MEM(m_Registers.HL.high, m_Registers.HL.reg);         break;
		case 0x6E: INSTR_LOAD_MEM(m_Registers.HL.low, m_Registers.HL.reg);          break;
		case 0x0A: INSTR_LOAD_MEM(m_Registers.A.high, m_Registers.BC.reg);          break;
		case 0x1A: INSTR_LOAD_MEM(m_Registers.A.high, m_Registers.DE.reg);          break;

        // Write to memory of dest address
		case 0x70: INSTR_WRITE_MEM(m_Registers.HL.reg, m_Registers.BC.high); break;
		case 0x71: INSTR_WRITE_MEM(m_Registers.HL.reg, m_Registers.BC.low);  break;
		case 0x72: INSTR_WRITE_MEM(m_Registers.HL.reg, m_Registers.DE.high); break;
		case 0x73: INSTR_WRITE_MEM(m_Registers.HL.reg, m_Registers.DE.low);  break;
		case 0x74: INSTR_WRITE_MEM(m_Registers.HL.reg, m_Registers.HL.high); break;
		case 0x75: INSTR_WRITE_MEM(m_Registers.HL.reg, m_Registers.HL.low);  break;



		// Increment
		case 0x3C: INSTR_INC(m_Registers.A.high); break ;
		case 0x04: INSTR_INC(m_Registers.BC.high); break ;
		case 0x0C: INSTR_INC(m_Registers.BC.low); break ;
		case 0x14: INSTR_INC(m_Registers.DE.high); break ;
		case 0x1C: INSTR_INC(m_Registers.DE.low); break ;
		case 0x24: INSTR_INC(m_Registers.HL.high); break ;
		case 0x2C: INSTR_INC(m_Registers.HL.low); break ;
		case 0x34: INSTR_INC_MEM(m_Registers.HL.reg); break ;

		// Decrement
		case 0x3D: INSTR_DEC(m_Registers.A.high); break ;
		case 0x05: INSTR_DEC(m_Registers.BC.high); break ;
		case 0x0D: INSTR_DEC(m_Registers.BC.low); break ;
		case 0x15: INSTR_DEC(m_Registers.DE.high); break ;
		case 0x1D: INSTR_DEC(m_Registers.DE.low); break ;
		case 0x25: INSTR_DEC(m_Registers.HL.high); break ;
		case 0x2D: INSTR_DEC(m_Registers.HL.low); break ;
		case 0x35: INSTR_DEC_MEM(m_Registers.HL.reg); break ;

		// 16-Bit Increment
		case 0x03: INSTR_INC_16BIT( m_Registers.BC.reg) ; break ;
		case 0x13: INSTR_INC_16BIT( m_Registers.DE.reg) ; break ;
		case 0x23: INSTR_INC_16BIT( m_Registers.HL.reg) ; break ;
		case 0x33: INSTR_INC_16BIT( m_Registers.SP.reg) ; break ;

		// 16-Bit Decrement
		case 0x0B: INSTR_DEC_16BIT( m_Registers.BC.reg) ; break ;
		case 0x1B: INSTR_DEC_16BIT( m_Registers.DE.reg) ; break ;
		case 0x2B: INSTR_DEC_16BIT( m_Registers.HL.reg) ; break ;
		case 0x3B: INSTR_DEC_16BIT( m_Registers.SP.reg) ; break ;

        // Write to memory
		case 0x02: INSTR_WRITE_MEM(m_Registers.BC.reg, m_Registers.A.high);          break;
		case 0x12: INSTR_WRITE_MEM(m_Registers.DE.reg, m_Registers.A.high);          break;
		case 0x77: INSTR_WRITE_MEM(m_Registers.HL.reg, m_Registers.A.high);          break;
		case 0xE2: INSTR_WRITE_MEM((0xFF00+m_Registers.BC.low), m_Registers.A.high); break;


        // Add Instructions
        case 0x80: INSTR_ADD(m_Registers.A.high, m_Registers.BC.high, false); break;
        case 0x81: INSTR_ADD(m_Registers.A.high, m_Registers.BC.low, false);  break;
        case 0x82: INSTR_ADD(m_Registers.A.high, m_Registers.DE.high, false); break;
        case 0x83: INSTR_ADD(m_Registers.A.high, m_Registers.DE.low, false);  break;
        case 0x84: INSTR_ADD(m_Registers.A.high, m_Registers.HL.high, false); break;
        case 0x85: INSTR_ADD(m_Registers.A.high, m_Registers.HL.low, false);  break;
        case 0x86: INSTR_ADD(m_Registers.A.high, ReadByte(m_Registers.HL.reg), false); break;
        case 0x87: INSTR_ADD(m_Registers.A.high, m_Registers.A.high, false);  break;

        // Add while adding carry flag
        case 0x88: INSTR_ADD(m_Registers.A.high, m_Registers.BC.high, true); break;
        case 0x89: INSTR_ADD(m_Registers.A.high, m_Registers.BC.low, true);  break;
        case 0x8A: INSTR_ADD(m_Registers.A.high, m_Registers.DE.high, true); break;
        case 0x8B: INSTR_ADD(m_Registers.A.high, m_Registers.DE.low, true);  break;
        case 0x8C: INSTR_ADD(m_Registers.A.high, m_Registers.HL.high, true); break;
        case 0x8D: INSTR_ADD(m_Registers.A.high, m_Registers.HL.low, true);  break;
        case 0x8E: INSTR_ADD(m_Registers.A.high, ReadByte(m_Registers.HL.reg), true); break;
        case 0x8F: INSTR_ADD(m_Registers.A.high, m_Registers.A.high, true);  break;

        // Sub Instructions
        case 0x90: INSTR_SUB(m_Registers.A.high, m_Registers.BC.high, false); break;
        case 0x91: INSTR_SUB(m_Registers.A.high, m_Registers.BC.low, false);  break;
        case 0x92: INSTR_SUB(m_Registers.A.high, m_Registers.DE.high, false); break;
        case 0x93: INSTR_SUB(m_Registers.A.high, m_Registers.DE.low, false);  break;
        case 0x94: INSTR_SUB(m_Registers.A.high, m_Registers.HL.high, false); break;
        case 0x95: INSTR_SUB(m_Registers.A.high, m_Registers.HL.low, false);  break;
        case 0x96: INSTR_SUB(m_Registers.A.high, ReadByte(m_Registers.HL.reg), false); break;
        case 0x97: INSTR_SUB(m_Registers.A.high, m_Registers.A.high, false);  break;

        // Sub while subtracting carry flag
        case 0x98: INSTR_SUB(m_Registers.A.high, m_Registers.BC.high, true); break;
        case 0x99: INSTR_SUB(m_Registers.A.high, m_Registers.BC.low, true);  break;
        case 0x9A: INSTR_SUB(m_Registers.A.high, m_Registers.DE.high, true); break;
        case 0x9B: INSTR_SUB(m_Registers.A.high, m_Registers.DE.low, true);  break;
        case 0x9C: INSTR_SUB(m_Registers.A.high, m_Registers.HL.high, true); break;
        case 0x9D: INSTR_SUB(m_Registers.A.high, m_Registers.HL.low, true);  break;
        case 0x9E: INSTR_SUB(m_Registers.A.high, ReadByte(m_Registers.HL.reg), true); break;
        case 0x9F: INSTR_SUB(m_Registers.A.high, m_Registers.A.high, true);  break;

        // Bitwise instructions
        case 0xA0: INSTR_AND(m_Registers.A.high, m_Registers.BC.high); break;
        case 0xA1: INSTR_AND(m_Registers.A.high, m_Registers.BC.high); break;
        case 0xA2: INSTR_AND(m_Registers.A.high, m_Registers.DE.high); break;
        case 0xA3: INSTR_AND(m_Registers.A.high, m_Registers.DE.low);  break;
        case 0xA4: INSTR_AND(m_Registers.A.high, m_Registers.HL.high); break;
        case 0xA5: INSTR_AND(m_Registers.A.high, m_Registers.HL.low);  break;
        case 0xA6: INSTR_AND(m_Registers.A.high, ReadByte(m_Registers.HL.reg)); break;
        case 0xA7: INSTR_AND(m_Registers.A.high, m_Registers.A.high);  break;
        case 0xE6: INSTR_AND(m_Registers.A.high, ReadByte(m_Registers.PC.reg)); m_Registers.PC.reg++; break;

        case 0xA8: INSTR_XOR(m_Registers.A.high, m_Registers.BC.high); break;
        case 0xA9: INSTR_XOR(m_Registers.A.high, m_Registers.BC.high); break;
        case 0xAA: INSTR_XOR(m_Registers.A.high, m_Registers.DE.high); break;
        case 0xAB: INSTR_XOR(m_Registers.A.high, m_Registers.DE.low);  break;
        case 0xAC: INSTR_XOR(m_Registers.A.high, m_Registers.HL.high); break;
        case 0xAD: INSTR_XOR(m_Registers.A.high, m_Registers.HL.low);  break;
        case 0xAE: INSTR_XOR(m_Registers.A.high, ReadByte(m_Registers.HL.reg)); break;
        case 0xAF: INSTR_XOR(m_Registers.A.high, m_Registers.A.high);  break;

        case 0xB0: INSTR_OR(m_Registers.A.high, m_Registers.BC.high); break;
        case 0xB1: INSTR_OR(m_Registers.A.high, m_Registers.BC.high); break;
        case 0xB2: INSTR_OR(m_Registers.A.high, m_Registers.DE.high); break;
        case 0xB3: INSTR_OR(m_Registers.A.high, m_Registers.DE.low);  break;
        case 0xB4: INSTR_OR(m_Registers.A.high, m_Registers.HL.high); break;
        case 0xB5: INSTR_OR(m_Registers.A.high, m_Registers.HL.low);  break;
        case 0xB6: INSTR_OR(m_Registers.A.high, ReadByte(m_Registers.HL.reg)); break;
        case 0xB7: INSTR_OR(m_Registers.A.high, m_Registers.A.high);  break;

        // Jump Instructions
        case 0xC3: INSTR_JUMP(NONE, false); break;
        case 0xC2: INSTR_JUMP(Z, false);    break;
        case 0xCA: INSTR_JUMP(Z, true);     break;
        case 0xD2: INSTR_JUMP(C, false);    break;
        case 0xDA: INSTR_JUMP(C, true);     break;
        // Weird jump instruction thats different from the rest
        case 0xE9: m_Registers.PC.reg = m_Registers.HL.reg; break;

        case 0x18: INSTR_JUMP_IM(NONE, false);  break;
        case 0x20: INSTR_JUMP_IM(Z, false);     break;
        case 0x28: INSTR_JUMP_IM(Z, true);      break;
        case 0x30: INSTR_JUMP_IM(C, false);     break;
        case 0x38: INSTR_JUMP_IM(C, true);      break;


        // Call Instructions
        case 0xCD: INSTR_CALL(NONE, false); break;
        case 0xC4: INSTR_CALL(Z, false);    break;
        case 0xCC: INSTR_CALL(Z, true);     break;
        case 0xD4: INSTR_CALL(C, false);    break;
        case 0xDC: INSTR_CALL(C, true);     break;

        // Return Instructions
        case 0xC9: INSTR_RETURN(NONE, false);  break;
        case 0xC0: INSTR_RETURN(Z, false);     break;
        case 0xC8: INSTR_RETURN(Z, true);      break;
        case 0xD0: INSTR_RETURN(C, false);     break;
        case 0xD8: INSTR_RETURN(C, true);      break;

        case 0xFB: break; // ENABLES INTERRUPTS
        case 0xF3: break; // DISABLE INTERRUPTS

        // UNDEFINED INSTRUCTIONS
        case 0xDD: break;
        case 0xED: break; 
        case 0xE3: break;
        case 0xF2: break;
        case 0xF4: break;

        default:
            WARN("Error: Unimplemented OPcode {:X} Address {:X}\n", opcode, m_Registers.PC.reg);
            return false;
            break;
    }
    return true;
}