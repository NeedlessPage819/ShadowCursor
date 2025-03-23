#pragma once

#include <Windows.h>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <atomic>

// Thread-safe queue for movement commands
class MovementQueue {
private:
    std::queue<std::pair<int, int>> movements;
    std::mutex mutex;
    std::condition_variable cv;
    std::atomic<bool> terminate;

public:
    MovementQueue();
    
    void push(int deltaX, int deltaY);
    bool pop(int& deltaX, int& deltaY);
    void stop();
};

class CursorControl {
public:
    CursorControl();
    ~CursorControl();

    // Initialize cursor control
    bool Initialize();

    // Move cursor using undetectable methods
    bool MoveCursor(int deltaX, int deltaY);
    
    // Move cursor to specific coordinates
    bool MoveCursorTo(int x, int y);
    
    // Get current cursor position
    POINT GetCurrentPosition();
    
    // Utility functions for Python interface
    std::pair<int, int> GetPosition();
    bool SetPosition(int x, int y);
    
    // Check if arrow key control is enabled
    bool IsEnabled() const;
    
    // Enable/disable arrow key control
    void SetEnabled(bool enabled);

private:
    // Thread worker function for movement processing
    void WorkerThread();

    // Optimized movement methods for better performance
    bool OptimizedMouseInput(int deltaX, int deltaY);
    bool FastDirectAccess(int x, int y);
    
    // Legacy methods (kept for compatibility)
    bool InjectMouseInput(int deltaX, int deltaY, DWORD flags);
    bool DirectUserObjectsAccess(int x, int y);
    bool EmulateHardwareInput(int deltaX, int deltaY);
    
    // State variables
    bool isInitialized;
    bool isEnabled;
    
    // Thread-related members
    std::thread workerThread;
    MovementQueue movementQueue;
    std::mutex previousMovementsMutex;
    
    // Track previous movement data for natural acceleration
    std::vector<POINT> previousMovements;
}; 