#pragma once
 
#include <Windows.h>
 
namespace Driver
{
    // Enum for different mouse button flags
    enum MouseFlags
    {
        None = 0,
        LeftButtonDown = 1,
        LeftButtonUp = 2,
        RightButtonDown = 4,
        RightButtonUp = 8,
        MiddleButtonDown = 16,
        MiddleButtonUp = 32,
        XButton1Down = 64,
        XButton1Up = 128,
        XButton2Down = 256,
        XButton2Up = 512,
        MouseWheel = 1024,
        MouseHorizontalWheel = 2048
    };
 
 
    class Comms
    {
        // Struct for mouse request data
        struct NF_MOUSE_REQUEST
        {
            int x;
            int y;
            short ButtonFlags;
        };
 
        // Struct for general info data 
        struct INFO_T
        {
            int Pid;
            uintptr_t Address;
            uintptr_t Value;
            int Size;
            uintptr_t Data;
        };
 
    public:
        // Constructor and destructor
        Comms() : hDriver(INVALID_HANDLE_VALUE), bIsConnected(false) { }
        ~Comms() { }
 
        // Functions to create and disconnect the driver
        void CreateDeviceDrv();
        void Disconnect();
 
        // Functions to initialize the driver and map user data
        void TryInitDriver();
        void UDMapper();
 
        // Function to send a mouse event
        bool MouseEvent(double x, double y, MouseFlags ButtonFlags);
 
        // Function to check if driver is connected
        bool IsConnected()
        {
            if (!hDriver || hDriver == INVALID_HANDLE_VALUE)
                return false;
 
            return true;
        }
 
    private:
        // IO control code for sending mouse events
        DWORD IO_SEND_MOUSE_EVENT = 0x23FACC00;
 
    private:
        HANDLE hDriver;
        bool bIsConnected;
    };
}