#pragma once

#include <Windows.h>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <atomic>

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

    bool Initialize();
    bool MoveCursor(int deltaX, int deltaY);
    bool MoveCursorTo(int x, int y);
    POINT GetCurrentPosition();
    std::pair<int, int> GetPosition();
    bool SetPosition(int x, int y);
    bool IsEnabled() const;
    void SetEnabled(bool enabled);

private:
    void WorkerThread();
    bool OptimizedMouseInput(int deltaX, int deltaY);
    bool FastDirectAccess(int x, int y);
    bool InjectMouseInput(int deltaX, int deltaY, DWORD flags);
    bool DirectUserObjectsAccess(int x, int y);
    bool EmulateHardwareInput(int deltaX, int deltaY);
    
    bool isInitialized;
    bool isEnabled;
    
    std::thread workerThread;
    MovementQueue movementQueue;
    std::mutex previousMovementsMutex;
    std::vector<POINT> previousMovements;
}; 