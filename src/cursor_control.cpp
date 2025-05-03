// Header files needed for mouse control functionality
#include "cursor_control.h"
#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include "driver.hpp"

// Initialize movement queue with termination flag set to false
MovementQueue::MovementQueue() : terminate(false) {}

// Add new mouse movement to the queue
void MovementQueue::push(int deltaX, int deltaY) {
    std::unique_lock<std::mutex> lock(mutex);
    movements.push(std::make_pair(deltaX, deltaY));
    cv.notify_one();
}

// Get next movement from queue, returns false if queue is empty and terminated
bool MovementQueue::pop(int& deltaX, int& deltaY) {
    std::unique_lock<std::mutex> lock(mutex);
    cv.wait(lock, [this] { return !movements.empty() || terminate; });

    if (terminate && movements.empty()) {
        return false;
    }

    auto movement = movements.front();
    movements.pop();
    deltaX = movement.first;
    deltaY = movement.second;
    return true;
}

// Signal thread termination and wake up waiting threads
void MovementQueue::stop() {
    terminate = true;
    cv.notify_all();
}

// Initialize cursor control with default settings for natural movement
CursorControl::CursorControl() : isInitialized(false), isEnabled(true),
    humanizationLevel(5), jitterAmount(2), movementDelay(100), 
    pathDeviation(0.3), movementStyle(1), driverInitialized(false) {
    driverComms.TryInitDriver();
    driverInitialized = driverComms.IsConnected();
}

// Clean up resources and stop worker thread
CursorControl::~CursorControl() {
    if (workerThread.joinable()) {
        movementQueue.stop();
        workerThread.join();
    }
}

// Set up cursor control system and start worker thread
bool CursorControl::Initialize() {
    isInitialized = true;
    previousMovements.clear();
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    workerThread = std::thread(&CursorControl::WorkerThread, this);
    return true;
}

// Main processing loop for mouse movements
void CursorControl::WorkerThread() {
    int deltaX, deltaY;
    
    while (isInitialized && movementQueue.pop(deltaX, deltaY)) {
        if (!isEnabled) continue;
        POINT currentPos = GetCurrentPosition();
        
        // Handle small movements differently
        if (abs(deltaX) <= 5 && abs(deltaY) <= 5) {
            if (humanizationLevel > 0) {
                HumanizedMovement(deltaX, deltaY);
            } else {
                SetCursorPos(currentPos.x + deltaX, currentPos.y + deltaY);
            }
            continue;
        }
        
        // Randomly choose movement style based on humanization settings
        int randomChoice = std::rand() % 10;
        if (randomChoice < humanizationLevel) {
            HumanizedMovement(deltaX, deltaY);
        } else if (std::rand() % 2 == 0) {
            OptimizedMouseInput(deltaX, deltaY);
        } else {
            FastDirectAccess(currentPos.x + deltaX, currentPos.y + deltaY);
        }
        
        // Add natural delay between movements
        if (movementDelay > 0) {
            int actualDelay = movementDelay;
            if (humanizationLevel > 0) {
                actualDelay += std::rand() % (movementDelay * humanizationLevel / 5);
            }
            std::this_thread::sleep_for(std::chrono::microseconds(actualDelay));
        }
        
        std::this_thread::yield();
    }
}

bool CursorControl::MoveCursor(int deltaX, int deltaY) {

    if (!isInitialized) {

        return false;

    }

    

    // Limit movement size for better control (smallest unit = 1 pixel)

    int limitedDeltaX = (deltaX == 0) ? 0 : (deltaX > 0 ? 1 : -1);

    int limitedDeltaY = (deltaY == 0) ? 0 : (deltaY > 0 ? 1 : -1);

    

    // Just queue the movement and return immediately

    // This prevents input lag by not blocking the calling thread

    movementQueue.push(limitedDeltaX, limitedDeltaY);

    

    // Store this movement for future reference (no need to wait for the worker)

    POINT currentPos = GetCurrentPosition();

    POINT newPos = {currentPos.x + limitedDeltaX, currentPos.y + limitedDeltaY};

    

    // Use lock to safely update the shared resource

    {

        std::lock_guard<std::mutex> lock(previousMovementsMutex);

        previousMovements.push_back(newPos);

        

        // Keep only the last 10 movements

        if (previousMovements.size() > 10) {

            previousMovements.erase(previousMovements.begin());

        }

    }

    

    return true;

}



bool CursorControl::MoveCursorTo(int x, int y) {

    if (!isInitialized) {

        return false;

    }

    

    POINT currentPos = GetCurrentPosition();

    

    // Calculate the path towards the target

    int dx = x - currentPos.x;

    int dy = y - currentPos.y;

    

    // For small distances, do direct movement

    if (abs(dx) <= 5 && abs(dy) <= 5) {

        return MoveCursor(dx, dy);

    }

    

    // For larger distances, move in steps

    int stepX = (dx == 0) ? 0 : (dx > 0 ? 2 : -2);

    int stepY = (dy == 0) ? 0 : (dy > 0 ? 2 : -2);

    

    // Queue a single step in the desired direction

    return MoveCursor(stepX, stepY);

}



POINT CursorControl::GetCurrentPosition() {

    POINT p;

    GetCursorPos(&p);

    return p;

}



// Python interface utility functions

std::pair<int, int> CursorControl::GetPosition() {

    POINT pos = GetCurrentPosition();

    return std::make_pair(pos.x, pos.y);

}



bool CursorControl::SetPosition(int x, int y) {

    return MoveCursorTo(x, y);

}



bool CursorControl::IsEnabled() const {

    return isEnabled;

}



void CursorControl::SetEnabled(bool enabled) {

    isEnabled = enabled;

}



bool CursorControl::OptimizedMouseInput(int deltaX, int deltaY) {

    // Faster alternative to InjectMouseInput with less delay

    

    // Use a single, quick movement with minimal delay

    // Small random jitter to avoid detection patterns

    int jitterX = (std::rand() % 3) - 1;  // -1, 0, or 1

    int jitterY = (std::rand() % 3) - 1;  // -1, 0, or 1

    

    // Combine movement with jitter for a more natural appearance

    INPUT input = {0};

    input.type = INPUT_MOUSE;

    input.mi.dx = deltaX + jitterX;

    input.mi.dy = deltaY + jitterY;

    input.mi.dwFlags = MOUSEEVENTF_MOVE;

    SendInput(1, &input, sizeof(INPUT));

    

    // Minimal delay (0.1-0.2ms maximum)

    std::this_thread::sleep_for(std::chrono::microseconds(100 + (std::rand() % 100)));

    

    return true;

}



bool CursorControl::FastDirectAccess(int x, int y) {

    // Fast direct positioning optimized for responsiveness

    

    POINT current;

    GetCursorPos(&current);

    

    // Move directly to position with a single call

    SetCursorPos(x, y);

    

    // No delay for maximum responsiveness

    return true;

}



bool CursorControl::InjectMouseInput(int deltaX, int deltaY, DWORD flags) {

    // Legacy method, not used in high-performance path

    // Keeping for compatibility

    

    // Use a single, direct call to the API

    mouse_event(MOUSEEVENTF_MOVE, deltaX, deltaY, 0, 0);

    

    return true;

}



bool CursorControl::DirectUserObjectsAccess(int x, int y) {

    // Legacy method, not used in high-performance path

    // Keeping for compatibility

    

    // Direct call to Windows API

    SetCursorPos(x, y);

    

    return true;

}



bool CursorControl::EmulateHardwareInput(int deltaX, int deltaY) {

    // Legacy method, not used in high-performance path

    // Keeping for compatibility

    

    // Simplified for responsiveness - direct input

    INPUT input = {0};

    input.type = INPUT_MOUSE;

    input.mi.dx = deltaX;

    input.mi.dy = deltaY;

    input.mi.dwFlags = MOUSEEVENTF_MOVE;

    SendInput(1, &input, sizeof(INPUT));

    

    return true;

} 
