// Shared memory manager class
#include <iostream>
#include <windows.h>
#include "SharedMemStruct.h" // Include the struct definition from SharedMemStruct.h

class MemManager {
private:
    size_t size_;
    const wchar_t* name_;
    HANDLE file_handle_;

public:
    SharedMemStruct* data;

    MemManager(const wchar_t* name) : name_(name), file_handle_(nullptr), data(nullptr) {
        size_ = sizeof(SharedMemStruct); // Implicitly set the size from the struct size
    }

    ~MemManager() {
        Disconnect();
    }

    // Create a new memory-mapped file
    bool Create() {
        file_handle_ = CreateFileMappingW(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, size_, name_);
        if (file_handle_ == nullptr) {
            std::cerr << "CreateFileMappingW failed: " << GetLastError() << std::endl;
            return false;
        }

        data = static_cast<SharedMemStruct*>(MapViewOfFile(file_handle_, FILE_MAP_ALL_ACCESS, 0, 0, size_));
        if (data == nullptr) {
            std::cerr << "MapViewOfFile failed: " << GetLastError() << std::endl;
            CloseHandle(file_handle_);
            return false;
        }

        return true;
    }

    // Connect to an existing memory-mapped file
    bool Connect() {
        file_handle_ = OpenFileMappingW(FILE_MAP_ALL_ACCESS, FALSE, name_);
        if (file_handle_ == nullptr) {
            std::cerr << "OpenFileMappingW failed: " << GetLastError() << std::endl;
            return false;
        }

        data = static_cast<SharedMemStruct*>(MapViewOfFile(file_handle_, FILE_MAP_ALL_ACCESS, 0, 0, size_));
        if (data == nullptr) {
            std::cerr << "MapViewOfFile failed: " << GetLastError() << std::endl;
            CloseHandle(file_handle_);
            return false;
        }

        return true;
    }

    // Disconnect from the memory-mapped file
    void Disconnect() {
        if (data != nullptr) {
            UnmapViewOfFile(data);
            data = nullptr;
        }

        if (file_handle_ != nullptr) {
            CloseHandle(file_handle_);
            file_handle_ = nullptr;
        }
    }

    SharedMemStruct* GetData() const {
        return data;
    }
};