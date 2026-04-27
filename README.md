# scyllahide-detector

A lightweight x64 C++ PoC for detecting scyllahide at runtime.

### How it works

### 1. Hook Detection

On windows, every syscall stub in ntdll.dll begins with a consistent 4 byte prologue

```
4C 8B D1    mov r10, rcx    ; kernel expects syscall params in r10
B8 XX ...   mov eax, index  ; syscall number
```

ScyllaHide patches these stubs these stubs with an inline JMP to redirect execution  to
its own code which breaks the consistent pattern above

The detector reads the first bytes of ntdll exports with GetProcAddress and checks for
the unexpected pattern caused by scyllahide.

### 2. Named Pipe Detection

ScyllaHide creates a named pipe \\.\pipe\ScyllaHide for internal communication. A CreateFile 
call with OPEN_EXISTING will succeed if the pipe is present, acting as a fingerprint check.

Either detection method returning true is treated as a positive detection.

## Notes/warnings

- x64 windows and apps only, might release x86 version later if I feel like it.
- Tested with latest scyllahide release, could break in the future.
- Educational and software protection ONLY

## Sources 

- https://github.com/x64dbg/ScyllaHide
- https://learn.microsoft.com/en-us/windows/win32/api/winternl/nf-winternl-ntqueryinformationprocess]
