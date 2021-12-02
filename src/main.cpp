#include "cpu.h"

int main(int argc, char **argv)
{
    Log::Init();
    INFO("CPU INITIALIZED");
    
    CPU* cpu = new CPU();
    char* instrFile = NULL;

    if (argc==2)
    {
        instrFile = argv[1];
        if(!cpu->LoadInstructions(instrFile))
        {
            exit(-1);
        }
    }
    else
    {
        // Fill memory with NO-OP code
        cpu->FillMem(0x00);
    }

    cpu->DumpMem(0x100, 0x100 + 10);

    cpu->Cycle();

    //cpu->~I8080();
    delete cpu;

    exit(1);
}