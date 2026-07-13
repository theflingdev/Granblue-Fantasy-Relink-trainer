#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <windows.h>
#include <string>
#include <vector>

struct ProcessModuleInfo {
    std::wstring moduleName;
    uintptr_t baseAddress;
    size_t moduleSize;
};

class MemoryManager {
private:
    DWORD processId;
    HANDLE hProcess;
    std::vector<ProcessModuleInfo> moduleCache;

    bool VerifyHandle();

public:
    MemoryManager();
    ~MemoryManager();
    
    bool AttachProcess(const std::wstring& processName);
    void DetachProcess();
    uintptr_t GetModuleBaseAddress(const std::wstring& moduleName);
    bool CacheModules();
    uintptr_t FindPattern(const std::wstring& moduleName, const char* pattern, const char* mask);
    uintptr_t ResolvePointerChain(uintptr_t baseAddress, const std::vector<ptrdiff_t>& offsets);

    template <typename T>
    T ReadMemory(uintptr_t address) {
        T buffer{};
        if (!VerifyHandle() || address == 0) return buffer;
        SIZE_T bytesRead = 0;
        ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(address), &buffer, sizeof(T), &bytesRead);
        return buffer;
    }

    template <typename T>
    bool WriteMemory(uintptr_t address, T value) {
        if (!VerifyHandle() || address == 0) return false;
        SIZE_T bytesWritten = 0;
        BOOL result = WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(address), &value, sizeof(T), &bytesWritten);
        return result && (bytesWritten == sizeof(T));
    }

    template <typename T>
    bool ProtectMemory(uintptr_t address, size_t size, DWORD newProtect, DWORD& oldProtect) {
        if (!VerifyHandle() || address == 0) return false;
        return VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(address), size, newProtect, &oldProtect);
    }
};

#endif