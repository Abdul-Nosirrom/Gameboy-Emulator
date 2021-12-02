#include "cpu.h"

/*
    All instructions listed below - PC increments likely incorrect
*/
void CPU::INSTR_ADD(byte& dest, byte source, bool addCFlag)
{
    byte carryAdd = m_Flags.C * addCFlag;
    byte toAdd = source + carryAdd;
    word resultTest = dest + toAdd;
    bool carryTest = resultTest > 0xFF;
    bool auxCarry = ((dest & 0xf) + (source & 0xf) + carryAdd > 0xf);

    ResetFlags();

    SetFlag_AuxCarry(true);
    SetFlag_Carry(carryTest);
    SetFlag_Zero(dest == 0);
    SetFlag_Sign(false);

    dest += toAdd;
}

void CPU::INSTR_SUB(byte& dest, byte source, bool subCFlag)
{
    byte toSub = source + m_Flags.C * subCFlag;
    bool neg = toSub > dest;

    ResetFlags();

    SetFlag_AuxCarry((dest & 0xf) - (source & 0xf) > 0xf);
    SetFlag_Carry(dest < toSub);
    SetFlag_Zero(dest == 0);
    SetFlag_Sign(neg);

    dest -= toSub;
}

void CPU::INSTR_CMP(byte dest, byte source)
{
    // Here we just pass in copies of dest into the sub instructions which will handle our flags
    INSTR_SUB(dest, source, false);
}

void CPU::INSTR_INC(byte& dest)
{
    byte pre_inc = dest;
    dest++;

    ResetFlags();

    SetFlag_AuxCarry(pre_inc & 0xf == 0xf);
    SetFlag_Zero(dest == 0);
}

void CPU::INSTR_DEC(byte& dest)
{
    byte pre_inc = dest;
    dest--;

    ResetFlags();

    SetFlag_AuxCarry(pre_inc & 0xf == 0xf);
    SetFlag_Zero(dest == 0);
    SetFlag_Sign(true);
}

void CPU::INSTR_INC_16BIT(word& dest)
{
    dest++;
}

void CPU::INSTR_DEC_16BIT(word& dest)
{
    dest--;
}

void CPU::INSTR_INC_MEM(word address)
{
    byte val = ReadByte(address);
    INSTR_INC(val);
    WriteByte(address, val);
}

void CPU::INSTR_DEC_MEM(word address)
{
    byte val = ReadByte(address);
    INSTR_DEC(val);
    WriteByte(address, val);
}

void CPU::INSTR_AND(byte& dest, byte source)
{
    dest &= source;
    
    ResetFlags();
    
    if (dest == 0)
        m_Flags.Z = 0b1;
}

void CPU::INSTR_OR(byte& dest, byte source)
{
    dest |= source;
    
    ResetFlags();
    
    if (dest == 0)
        m_Flags.Z = 0b1;
}

void CPU::INSTR_XOR(byte& dest, byte source)
{
    dest ^= source;
    
    ResetFlags();
    
    if (dest == 0)
        m_Flags.Z = 0b1;    
}

void CPU::INSTR_LOAD(byte& dest, byte source)
{
    dest = source;
    m_Registers.PC.reg += 2;
}

void CPU::INSTR_LOAD_WORD(word& dest, word source)
{
    dest = source;
    m_Registers.PC.reg += 2;
}

void CPU::INSTR_LOAD_MEM(byte& dest, word address)
{
    dest = ReadWord(address);
}

void CPU::INSTR_WRITE_MEM(word address, byte source)
{
    WriteByte(address, source);
}

void CPU::INSTR_JUMP(Conditions condition, bool condiStatus)
{
    word jumpPoint = ReadWord(m_Registers.PC.reg);
    m_Registers.PC.reg += 2;

    if (condition == NONE)
    {
        m_Registers.PC.reg = jumpPoint;
    }
    else if (GetCondFlag(condition) == condiStatus)
    {
        m_Registers.PC.reg = jumpPoint;
    }
}

void CPU::INSTR_JUMP_IM(Conditions condition, bool condiStatus)
{
    Sbyte jumpVal = (Sbyte)ReadByte(m_Registers.PC.reg);

    if (condition == NONE)
    {
        m_Registers.PC.reg += jumpVal;
    }
    else if (GetCondFlag(condition) == condiStatus)
    {
        m_Registers.PC.reg += jumpVal;
    }
    
    m_Registers.PC.reg++;
}

void CPU::INSTR_CALL(Conditions condition, bool condiStatus)
{
    word jumpPoint = ReadWord(m_Registers.PC.reg);
    m_Registers.PC.reg += 2;

    if (condition == NONE)
    {
        StackPush(m_Registers.PC.reg);
        m_Registers.PC.reg = jumpPoint;
    }
    else if (GetCondFlag(condition) == condiStatus)
    {
        StackPush(m_Registers.PC.reg);
        m_Registers.PC.reg = jumpPoint;
    }
}

void CPU::INSTR_RETURN(Conditions condition, bool condiStatus)
{
    if (condition == NONE || GetCondFlag(condition) == condiStatus) 
        m_Registers.PC.reg = StackPop();
}