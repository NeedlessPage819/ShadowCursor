#include "cursor_control.h"
#include <iostream>
#include <random>
#include <chrono>
#include <thread>

MovementQueue::MovementQueue() : terminate(false) {}

void MovementQueue::push(int deltaX, int deltaY) {
    std::unique_lock<std::mutex> lock(mutex);
    movements.push(std::make_pair(deltaX, deltaY));
    cv.notify_one();
}

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

void MovementQueue::stop() {
    terminate = true;
    cv.notify_all();
}

CursorControl::CursorControl() : isInitialized(false), isEnabled(true) {
}

CursorControl::~CursorControl() {
    if (workerThread.joinable()) {
        movementQueue.stop();
        workerThread.join();
    }
}

bool CursorControl::Initialize() {
    isInitialized = true;
    previousMovements.clear();
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    workerThread = std::thread(&CursorControl::WorkerThread, this);
    return true;
}

void CursorControl::WorkerThread() {
    int deltaX, deltaY;
    
    while (isInitialized && movementQueue.pop(deltaX, deltaY)) {
        if (!isEnabled) continue;
        POINT currentPos = GetCurrentPosition();
        if (abs(deltaX) <= 5 && abs(deltaY) <= 5) {
            SetCursorPos(currentPos.x + deltaX, currentPos.y + deltaY);
            continue;
        }
        if (std::rand() % 2 == 0) {
            OptimizedMouseInput(deltaX, deltaY);
        } else {
            FastDirectAccess(currentPos.x + deltaX, currentPos.y + deltaY);
        }
        std::this_thread::yield();
    }
}

bool CursorControl::MoveCursor(int deltaX, int deltaY) {
    if (!isInitialized) {
        return false;
    }
    
    int limitedDeltaX = (deltaX == 0) ? 0 : (deltaX > 0 ? 1 : -1);
    int limitedDeltaY = (deltaY == 0) ? 0 : (deltaY > 0 ? 1 : -1);
    
    movementQueue.push(limitedDeltaX, limitedDeltaY);
    
    POINT currentPos = GetCurrentPosition();
    POINT newPos = {currentPos.x + limitedDeltaX, currentPos.y + limitedDeltaY};
    
    {
        std::lock_guard<std::mutex> lock(previousMovementsMutex);
        previousMovements.push_back(newPos);
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
    int dx = x - currentPos.x;
    int dy = y - currentPos.y;
    
    if (abs(dx) <= 5 && abs(dy) <= 5) {
        return MoveCursor(dx, dy);
    }
    
    int stepX = (dx == 0) ? 0 : (dx > 0 ? 2 : -2);
    int stepY = (dy == 0) ? 0 : (dy > 0 ? 2 : -2);
    
    return MoveCursor(stepX, stepY);
}

POINT CursorControl::GetCurrentPosition() {
    POINT p;
    GetCursorPos(&p);
    return p;
}

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
    int jitterX = (std::rand() % 3) - 1;
    int jitterY = (std::rand() % 3) - 1;
    
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dx = deltaX + jitterX;
    input.mi.dy = deltaY + jitterY;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    SendInput(1, &input, sizeof(INPUT));
    
    std::this_thread::sleep_for(std::chrono::microseconds(100 + (std::rand() % 100)));
    
    return true;
}

bool CursorControl::FastDirectAccess(int x, int y) {
    POINT current;
    GetCursorPos(&current);
    SetCursorPos(x, y);
    return true;
}

bool CursorControl::InjectMouseInput(int deltaX, int deltaY, DWORD flags) {
    mouse_event(MOUSEEVENTF_MOVE, deltaX, deltaY, 0, 0);
    return true;
}

bool CursorControl::DirectUserObjectsAccess(int x, int y) {
    SetCursorPos(x, y);
    return true;
}

bool CursorControl::EmulateHardwareInput(int deltaX, int deltaY) {
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dx = deltaX;
    input.mi.dy = deltaY;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    SendInput(1, &input, sizeof(INPUT));
    return true;
} 