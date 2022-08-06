#include "Mem.h"

uintptr_t Mem::findSig(const char* sig){
    const char* pattern = sig;
	uintptr_t firstMatch = 0;
	static const uintptr_t rangeStart = (uintptr_t)GetModuleHandleA("Minecraft.Windows.exe");
	static MODULEINFO miModInfo;
	static bool init = false;
	if (!init) {
		init = true;
		GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
	}
	static const uintptr_t rangeEnd = rangeStart + miModInfo.SizeOfImage;

    BYTE patByte = GET_BYTE(pattern);
	const char* oldPat = pattern;

    for (uintptr_t pCur = rangeStart; pCur < rangeEnd; pCur++){
        if (!*pattern)
            return firstMatch;
        
        while (*(PBYTE)pattern == ' ')
            pattern++;
        
        if (!*pattern)
            return firstMatch;
        
        if (oldPat != pattern){
            oldPat = pattern;
            if (*(PBYTE)pattern != '\?')
                patByte = GET_BYTE(pattern);
        };

        if (*(PBYTE)pattern == '\?' || *(BYTE*)pCur == patByte){
            if (!firstMatch)
                firstMatch = pCur;
            
            if (!pattern[2])
                return firstMatch;
            
            pattern += 2;
        }
        else {
            pattern = sig;
            firstMatch = 0;
        };
    };
    return NULL;
};

uintptr_t Mem::findMultiLvlPtr(uintptr_t baseAddr, std::vector<unsigned int> offsets){
    uintptr_t addr = baseAddr;
    
    for (int I = 0; I < offsets.size(); I++){
        addr = *(uintptr_t*)(addr);
        if ((uintptr_t*)(addr) == nullptr)
            return addr;
        addr += offsets[I];
    };

    return addr;
};
