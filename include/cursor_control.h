#pragma once

#include <Windows.h>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <atomic>
#include "driver.hpp"

// Class to manage a queue of cursor movements
class MovementQueue {
private:
    std::queue<std::pair<int, int>> movements; // Queue to store movements as (deltaX, deltaY) pairs
    std::mutex mutex; // Mutex to protect access to the queue
    std::condition_variable cv; // Condition variable for thread synchronization
    std::atomic<bool> terminate; // Atomic flag to signal termination

public:
    MovementQueue(); // Constructor to initialize the movement queue
    
    // Push a new movement into the queue
    void push(int deltaX, int deltaY);
    
    // Pop a movement from the queue
    bool pop(int& deltaX, int& deltaY);
    
    // Stop processing movements
    void stop();
};

// Class to control cursor movements
class CursorControl {
public:
    CursorControl(); // Constructor to initialize cursor control
    ~CursorControl(); // Destructor to clean up resources

    // Initialize the cursor control system
    bool Initialize();
    
    // Move the cursor by specified deltas
    bool MoveCursor(int deltaX, int deltaY);
    
    // Move the cursor to an absolute position
    bool MoveCursorTo(int x, int y);
    
    // Get the current cursor position
    POINT GetCurrentPosition();
    
    // Get the current position as a pair of integers
    std::pair<int, int> GetPosition();
    
    // Set the cursor to a specific position
    bool SetPosition(int x, int y);
    
    // Check if cursor control is enabled
    bool IsEnabled() const;
    
    // Enable or disable cursor control
    void SetEnabled(bool enabled);
    
    // Humanization settings for more natural movements
    void SetHumanizationLevel(int level); // Set the level of humanization
    int GetHumanizationLevel() const; // Get the current humanization level
    void SetJitterAmount(int amount); // Set the amount of jitter
    int GetJitterAmount() const; // Get the current jitter amount
    void SetMovementDelay(int microseconds); // Set delay between movements
    int GetMovementDelay() const; // Get the current movement delay
    void SetPathDeviation(double deviation); // Set how much to deviate from a straight line
    double GetPathDeviation() const; // Get the current path deviation
    void SetMovementStyle(int style); // Set the style of movement
    int GetMovementStyle() const; // Get the current movement style

    // Check if the driver is initialized
    bool IsDriverInitialized() const;

private:
    void WorkerThread(); // Thread function to process movements
    bool OptimizedMouseInput(int deltaX, int deltaY); // Optimized method for mouse input
    bool FastDirectAccess(int x, int y); // Direct access method for cursor positioning
    bool InjectMouseInput(int deltaX, int deltaY, DWORD flags); // Inject mouse input events
    bool DirectUserObjectsAccess(int x, int y); // Access user objects directly for cursor control
    bool EmulateHardwareInput(int deltaX, int deltaY); // Emulate hardware input for cursor movements
    
    // New methods for humanized movement
    bool HumanizedMovement(int deltaX, int deltaY); // Perform humanized movement
    bool BezierCurveMovement(int startX, int startY, int endX, int endY); // Move cursor using Bezier curves
    std::vector<POINT> GenerateBezierCurve(int startX, int startY, int endX, int endY, int controlPoints); // Generate points for Bezier curve
    int ApplyJitter(int value); // Apply jitter to a movement value
    
    bool isInitialized; // Flag to check if the system is initialized
    bool isEnabled; // Flag to check if cursor control is enabled
    
    // Parameters for humanization
    int humanizationLevel;    // 0-10, where 0 = none and 10 = maximum humanization
    int jitterAmount;         // 0-10, representing pixels of random movement
    int movementDelay;        // Delay in microseconds between movements
    double pathDeviation;     // 0.0-1.0, indicating how much to deviate from a straight line
    int movementStyle;        // 0 = direct, 1 = bezier, 2 = random
    
    std::thread workerThread; // Thread for processing movements
    MovementQueue movementQueue; // Queue for managing movements
    std::mutex previousMovementsMutex; // Mutex for protecting previous movements
    std::vector<POINT> previousMovements; // Store previous cursor movements

    bool driverInitialized; // Flag to check if the driver is initialized
    Driver::Comms driverComms; // Communication object for driver interactions
}; 