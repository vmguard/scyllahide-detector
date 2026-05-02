#include "scyllahidedetector.h"
#include <windows.h>
#include <cstdint>

static bool IsOriginalSyscallStub(const std::uint8_t* addr) noexcept {
    if (!addr)
        return false;
    /*
        4c 8B D1       mov r10, rcx
        B8 XX XX XX mov eax, SYSCALL_INDEX
    */
    return (addr[0] == 0x4C && addr[1] == 0x8B && addr[2] == 0xD1 && addr[3] == 0xB8);
}

static bool CheckNtdllHooks() noexcept {
    HMODULE hNtdll = GetModuleHandleA("Ntdll.dll");
    if (!hNtdll)
        return false;
    static const char* s_TargetFunctions[] = {
        "NtQueryInformationProcess",
        "NtSetInformationThread",
        "NtQuerySystemInformation",
        "NtQueryObject",
        "NtYieldExecution",
        "NtGetContextThread",
        "NtSetContextThread",
        "NtContinue",
        "NtClose",
        "NtDuplicateObject",
        "NtCreateThreadEx",
        "NtQueryPerformanceCounter",
        "NtSetInformationProcess",
        "NtQueryInformationThread",
        "NtQueryVirtualMemory",
        "NtProtectVirtualMemory",

    };

    for (const char* szFunc : s_TargetFunctions) {
        FARPROC pFunc = GetProcAddress(hNtdll, szFunc);
        if (!pFunc)
            continue;
        if (!IsOriginalSyscallStub(reinterpret_cast<const std::uint8_t*>(pFunc)))
            return true;
    }
    return false;

}

static bool CheckScyllaHidePipe() noexcept {
    HANDLE hPipe = CreateFileA(
        "\\\\.\\pipe\\ScyllaHide",
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        OPEN_EXISTING,
        0,
        nullptr
    );


    if (hPipe != INVALID_HANDLE_VALUE) {
        CloseHandle(hPipe);
        return true;

    }
    return false;
}
bool IsScyllaHidePresent() {
    return CheckNtdllHooks() || CheckScyllaHidePipe();
}
