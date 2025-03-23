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
    
    // Humanization settings
    void SetHumanizationLevel(int level);
    int GetHumanizationLevel() const;
    void SetJitterAmount(int amount);
    int GetJitterAmount() const;
    void SetMovementDelay(int microseconds);
    int GetMovementDelay() const;
    void SetPathDeviation(double deviation);
    double GetPathDeviation() const;
    void SetMovementStyle(int style);
    int GetMovementStyle() const;

private:
    void WorkerThread();
    bool OptimizedMouseInput(int deltaX, int deltaY);
    bool FastDirectAccess(int x, int y);
    bool InjectMouseInput(int deltaX, int deltaY, DWORD flags);
    bool DirectUserObjectsAccess(int x, int y);
    bool EmulateHardwareInput(int deltaX, int deltaY);
    
    // New humanized movement methods
    bool HumanizedMovement(int deltaX, int deltaY);
    bool BezierCurveMovement(int startX, int startY, int endX, int endY);
    std::vector<POINT> GenerateBezierCurve(int startX, int startY, int endX, int endY, int controlPoints);
    int ApplyJitter(int value);
    
    bool isInitialized;
    bool isEnabled;
    
    // Humanization parameters
    int humanizationLevel;    // 0-10, 0 = none, 10 = maximum
    int jitterAmount;         // 0-10, pixels of random movement
    int movementDelay;        // microseconds between movements
    double pathDeviation;     // 0.0-1.0, how much to deviate from straight line
    int movementStyle;        // 0 = direct, 1 = bezier, 2 = random
    
    std::thread workerThread;
    MovementQueue movementQueue;
    std::mutex previousMovementsMutex;
    std::vector<POINT> previousMovements;
}; 