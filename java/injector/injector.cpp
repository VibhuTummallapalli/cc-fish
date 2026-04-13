/*
 * Fish DLL Injector
 * 
 * Injects Fish.dll into a running Minecraft process.
 * Must be run as Administrator if Minecraft is running elevated.
 * 
 * Usage: FishInjector.exe [process_name]
 *   Default process: javaw.exe (Minecraft)
 */

#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <string>
#include <filesystem>

// ANSI color codes for Windows 10+
namespace color {
    const char* reset = "\033[0m";
    const char* red = "\033[91m";
    const char* green = "\033[92m";
    const char* yellow = "\033[93m";
    const char* blue = "\033[94m";
    const char* magenta = "\033[95m";
    const char* cyan = "\033[96m";
}

void enable_ansi_colors() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

void print_banner() {
    std::cout << color::cyan;
    std::cout << R"(
    _____ _     _     
   |  ___(_)___| |__  
   | |_  | / __| '_ \ 
   |  _| | \__ \ | | |
   |_|   |_|___/_| |_|
                      
   Minecraft Auto-Fisher
)" << color::reset << std::endl;
}

DWORD find_process_id(const std::wstring& process_name) {
    DWORD pid = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    
    if (snapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    PROCESSENTRY32W pe32;
    pe32.dwSize = sizeof(pe32);

    if (Process32FirstW(snapshot, &pe32)) {
        do {
            if (_wcsicmp(pe32.szExeFile, process_name.c_str()) == 0) {
                pid = pe32.th32ProcessID;
                break;
            }
        } while (Process32NextW(snapshot, &pe32));
    }

    CloseHandle(snapshot);
    return pid;
}

bool inject_dll(DWORD pid, const std::wstring& dll_path) {
    HANDLE hProcess = OpenProcess(
        PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | 
        PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ,
        FALSE, pid
    );

    if (!hProcess) {
        std::cout << color::red << "[-] Failed to open process. Error: " 
                  << GetLastError() << color::reset << std::endl;
        std::cout << color::yellow << "[!] Try running as Administrator" << color::reset << std::endl;
        return false;
    }

    // Allocate memory in target process
    size_t path_size = (dll_path.size() + 1) * sizeof(wchar_t);
    LPVOID remote_mem = VirtualAllocEx(hProcess, nullptr, path_size, 
                                        MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    if (!remote_mem) {
        std::cout << color::red << "[-] Failed to allocate memory. Error: " 
                  << GetLastError() << color::reset << std::endl;
        CloseHandle(hProcess);
        return false;
    }

    // Write DLL path to target process
    if (!WriteProcessMemory(hProcess, remote_mem, dll_path.c_str(), path_size, nullptr)) {
        std::cout << color::red << "[-] Failed to write memory. Error: " 
                  << GetLastError() << color::reset << std::endl;
        VirtualFreeEx(hProcess, remote_mem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    // Get LoadLibraryW address
    HMODULE kernel32 = GetModuleHandleW(L"kernel32.dll");
    LPTHREAD_START_ROUTINE load_library = reinterpret_cast<LPTHREAD_START_ROUTINE>(
        GetProcAddress(kernel32, "LoadLibraryW")
    );

    if (!load_library) {
        std::cout << color::red << "[-] Failed to get LoadLibraryW address" << color::reset << std::endl;
        VirtualFreeEx(hProcess, remote_mem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    // Create remote thread to load DLL
    HANDLE hThread = CreateRemoteThread(hProcess, nullptr, 0, load_library, remote_mem, 0, nullptr);

    if (!hThread) {
        std::cout << color::red << "[-] Failed to create remote thread. Error: " 
                  << GetLastError() << color::reset << std::endl;
        VirtualFreeEx(hProcess, remote_mem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    std::cout << color::blue << "[*] Waiting for DLL to load..." << color::reset << std::endl;

    // Wait for injection to complete
    WaitForSingleObject(hThread, 5000);

    // Check if DLL was loaded
    DWORD exit_code = 0;
    GetExitCodeThread(hThread, &exit_code);

    CloseHandle(hThread);
    VirtualFreeEx(hProcess, remote_mem, 0, MEM_RELEASE);
    CloseHandle(hProcess);

    return exit_code != 0;
}

int wmain(int argc, wchar_t* argv[]) {
    enable_ansi_colors();
    print_banner();

    // Default target process
    std::wstring target_process = L"javaw.exe";
    
    if (argc > 1) {
        target_process = argv[1];
    }

    // Get DLL path (same directory as injector)
    wchar_t exe_path[MAX_PATH];
    GetModuleFileNameW(nullptr, exe_path, MAX_PATH);
    
    std::filesystem::path dll_path = std::filesystem::path(exe_path).parent_path() / L"Fish.dll";

    std::wcout << color::blue << L"[*] DLL Path: " << color::reset << dll_path.wstring() << std::endl;
    std::wcout << color::blue << L"[*] Target Process: " << color::reset << target_process << std::endl;

    // Check if DLL exists
    if (!std::filesystem::exists(dll_path)) {
        std::cout << color::red << "[-] Fish.dll not found!" << color::reset << std::endl;
        std::cout << color::yellow << "[!] Make sure Fish.dll is in the same directory as the injector" << color::reset << std::endl;
        std::cout << std::endl << "Press Enter to exit...";
        std::cin.get();
        return 1;
    }

    // Find Minecraft process
    std::cout << color::blue << "[*] Searching for " << color::reset;
    std::wcout << target_process << L"..." << std::endl;

    DWORD pid = find_process_id(target_process);

    if (pid == 0) {
        std::cout << color::red << "[-] Process not found!" << color::reset << std::endl;
        std::cout << color::yellow << "[!] Make sure Minecraft is running" << color::reset << std::endl;
        std::cout << std::endl << "Press Enter to exit...";
        std::cin.get();
        return 1;
    }

    std::cout << color::green << "[+] Found process with PID: " << color::reset << pid << std::endl;

    // Inject DLL
    std::cout << color::blue << "[*] Injecting DLL..." << color::reset << std::endl;

    if (inject_dll(pid, dll_path.wstring())) {
        std::cout << color::green << "[+] DLL injected successfully!" << color::reset << std::endl;
        std::cout << color::green << "[+] Check the Minecraft window for the Fish console" << color::reset << std::endl;
    } else {
        std::cout << color::red << "[-] DLL injection failed!" << color::reset << std::endl;
    }

    std::cout << std::endl << "Press Enter to exit...";
    std::cin.get();
    return 0;
}
