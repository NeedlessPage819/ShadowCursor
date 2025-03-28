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

[Rest of code continues unchanged...]
