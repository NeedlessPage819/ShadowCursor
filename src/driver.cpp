#include "driver.hpp"
#include <fstream>
#include <filesystem>
#include <TlHelp32.h>

// Function to generate a random string of specified length
std::string RandomString(const int len)
{
    // Define a set of alphanumeric characters to choose from
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len); // Reserve space for the string to improve performance

    // Loop to generate random characters
    for (int i = 0; i < len; ++i)
    {
        // Append a random character from the alphanumeric set
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return tmp_s; // Return the generated random string
}

// Function to get the process ID by its name
DWORD GetProcessPidByName(const wchar_t* ProcessName)
{
    // Take a snapshot of all processes running on the system
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    // Check if the snapshot is valid
    if (!hSnapshot || hSnapshot == INVALID_HANDLE_VALUE || hSnapshot == ((HANDLE)(LONG_PTR)ERROR_BAD_LENGTH))
    {
        return 0; // Return 0 if the snapshot is invalid
    }

    DWORD Pid; // Variable to store the process ID
    PROCESSENTRY32W ProcessEntry; // Structure to hold process information
    ProcessEntry.dwSize = sizeof(ProcessEntry); // Set the size of the structure
    // Start iterating through the processes in the snapshot
    if (Process32FirstW(hSnapshot, &ProcessEntry)) // Use wide-character version
    {
        // Loop through the processes until we find a match
        while (_wcsicmp(ProcessEntry.szExeFile, ProcessName)) // Compare process names
        {
            // Move to the next process in the snapshot
            if (!Process32NextW(hSnapshot, &ProcessEntry)) // Use wide-character version
            {
                CloseHandle(hSnapshot); // Close the snapshot handle
                return 0; // Return 0 if no matching process is found
            }
        }

        Pid = ProcessEntry.th32ProcessID; // Store the found process ID
    }
    else
    {
        CloseHandle(hSnapshot); // Close the snapshot handle
        return 0; // Return 0 if no processes were found
    }

    CloseHandle(hSnapshot); // Close the snapshot handle
    return Pid; // Return the found process ID
}

namespace Driver
{

    // Function to create a device driver handle
    void Comms::CreateDeviceDrv()
    {
        // Attempt to open the device driver
        hDriver = CreateFileA(("\\\\.\\cla300"), GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING,
            NULL, NULL);
    }

    // Function to try initializing the driver
    void Comms::TryInitDriver()
    {
        CreateDeviceDrv(); // Create the device driver
        // Check if the driver is not connected
        if (!IsConnected())
        {
            UDMapper(); // Call user-mode mapper if not connected
            CreateDeviceDrv(); // Try to create the device driver again
        }
    }

    // Function for user-mode mapping (currently empty)
    void Comms::UDMapper()
    {
        // Implementation for user-mode mapping would go here
    }

    // Function to disconnect the driver
    void Comms::Disconnect()
    {
        // Check if the driver handle is valid before closing
        if (hDriver != INVALID_HANDLE_VALUE)
            CloseHandle(hDriver); // Close the driver handle
    }

    // Function to send a mouse event to the driver
    bool Comms::MouseEvent(double x, double y, MouseFlags ButtonFlags)
    {
        // Check if the driver handle is valid
        if (!hDriver || hDriver == INVALID_HANDLE_VALUE)
            return false; // Return false if the driver is not connected

        NF_MOUSE_REQUEST MouseRequest; // Structure to hold mouse request data
        MouseRequest.x = (int)x; // Set the x-coordinate
        MouseRequest.y = (int)y; // Set the y-coordinate
        MouseRequest.ButtonFlags = (int)ButtonFlags; // Set the button flags

        // Send the mouse event to the driver and return the result
        return DeviceIoControl(hDriver, IO_SEND_MOUSE_EVENT, &MouseRequest, sizeof(NF_MOUSE_REQUEST), nullptr, NULL, nullptr, nullptr);
    }
}