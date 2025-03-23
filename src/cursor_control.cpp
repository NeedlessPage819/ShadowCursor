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

CursorControl::CursorControl() : isInitialized(false), isEnabled(true),
    humanizationLevel(5), jitterAmount(2), movementDelay(100), 
    pathDeviation(0.3), movementStyle(1) {
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
        
        // Choose movement method based on settings and randomness
        if (abs(deltaX) <= 5 && abs(deltaY) <= 5) {
            if (humanizationLevel > 0) {
                HumanizedMovement(deltaX, deltaY);
            } else {
                SetCursorPos(currentPos.x + deltaX, currentPos.y + deltaY);
            }
            continue;
        }
        
        // Choose movement method based on style and randomness
        int randomChoice = std::rand() % 10;
        if (randomChoice < humanizationLevel) {
            HumanizedMovement(deltaX, deltaY);
        } else if (std::rand() % 2 == 0) {
            OptimizedMouseInput(deltaX, deltaY);
        } else {
            FastDirectAccess(currentPos.x + deltaX, currentPos.y + deltaY);
        }
        
        // Add variable delay based on humanization level
        if (movementDelay > 0) {
            int actualDelay = movementDelay;
            if (humanizationLevel > 0) {
                // Add some randomness to the delay
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
    
    // Apply humanization if enabled
    if (humanizationLevel > 0) {
        // Apply jitter based on humanization level
        if (jitterAmount > 0) {
            deltaX = ApplyJitter(deltaX);
            deltaY = ApplyJitter(deltaY);
        }
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
    
    // If humanization is enabled and we're moving a significant distance, use bezier curve
    if (humanizationLevel > 0 && movementStyle == 1) {
        int dx = x - currentPos.x;
        int dy = y - currentPos.y;
        
        if (abs(dx) > 10 || abs(dy) > 10) {
            return BezierCurveMovement(currentPos.x, currentPos.y, x, y);
        }
    }
    
    // Otherwise use standard movement
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

// New humanization methods
bool CursorControl::HumanizedMovement(int deltaX, int deltaY) {
    POINT currentPos = GetCurrentPosition();
    int targetX = currentPos.x + deltaX;
    int targetY = currentPos.y + deltaY;
    
    // Add slight randomness to movement
    int jitterX = (std::rand() % (jitterAmount + 1)) - (jitterAmount / 2);
    int jitterY = (std::rand() % (jitterAmount + 1)) - (jitterAmount / 2);
    
    // Choose a random movement method based on humanization level
    int method = std::rand() % 3;
    
    switch (method) {
        case 0:
            EmulateHardwareInput(deltaX + jitterX, deltaY + jitterY);
            break;
        case 1:
            OptimizedMouseInput(deltaX + jitterX, deltaY + jitterY);
            break;
        case 2:
            FastDirectAccess(targetX + jitterX, targetY + jitterY);
            break;
    }
    
    // Add a small random delay to simulate human reaction time
    std::this_thread::sleep_for(std::chrono::microseconds(50 + (std::rand() % 50)));
    
    return true;
}

bool CursorControl::BezierCurveMovement(int startX, int startY, int endX, int endY) {
    // Generate a bezier curve with control points
    int numPoints = 10 + (std::rand() % 10) * (humanizationLevel / 2);
    std::vector<POINT> curve = GenerateBezierCurve(startX, startY, endX, endY, 2);
    
    // Move along the curve
    for (size_t i = 1; i < curve.size(); i++) {
        // Add jitter based on humanization level
        int jitterX = (std::rand() % (jitterAmount + 1)) - (jitterAmount / 2);
        int jitterY = (std::rand() % (jitterAmount + 1)) - (jitterAmount / 2);
        
        SetCursorPos(curve[i].x + jitterX, curve[i].y + jitterY);
        
        // Variable delay between points
        int delay = movementDelay + (std::rand() % movementDelay);
        std::this_thread::sleep_for(std::chrono::microseconds(delay));
    }
    
    // Ensure we end at the target position
    SetCursorPos(endX, endY);
    
    return true;
}

std::vector<POINT> CursorControl::GenerateBezierCurve(int startX, int startY, int endX, int endY, int controlPoints) {
    std::vector<POINT> curve;
    
    // Create control points with some randomness
    std::vector<POINT> controls;
    controls.push_back({startX, startY});
    
    for (int i = 0; i < controlPoints; i++) {
        // Create control points that deviate from the straight line
        double t = (i + 1.0) / (controlPoints + 1.0);
        int lineX = startX + static_cast<int>(t * (endX - startX));
        int lineY = startY + static_cast<int>(t * (endY - startY));
        
        // Add deviation based on pathDeviation setting
        int maxDeviation = static_cast<int>((abs(endX - startX) + abs(endY - startY)) * pathDeviation);
        int deviationX = (std::rand() % (maxDeviation * 2 + 1)) - maxDeviation;
        int deviationY = (std::rand() % (maxDeviation * 2 + 1)) - maxDeviation;
        
        controls.push_back({lineX + deviationX, lineY + deviationY});
    }
    
    controls.push_back({endX, endY});
    
    // Generate points along the curve
    int numPoints = 20 + humanizationLevel * 3;
    for (int i = 0; i <= numPoints; i++) {
        double t = static_cast<double>(i) / numPoints;
        
        // De Casteljau's algorithm for bezier curve
        std::vector<POINT> temp = controls;
        for (int j = controls.size() - 1; j > 0; j--) {
            for (int k = 0; k < j; k++) {
                temp[k].x = static_cast<int>((1 - t) * temp[k].x + t * temp[k + 1].x);
                temp[k].y = static_cast<int>((1 - t) * temp[k].y + t * temp[k + 1].y);
            }
        }
        
        curve.push_back(temp[0]);
    }
    
    return curve;
}

int CursorControl::ApplyJitter(int value) {
    if (jitterAmount <= 0) return value;
    
    // Apply jitter proportional to the value and jitter amount
    int maxJitter = std::min(jitterAmount, abs(value) / 2 + 1);
    int jitter = (std::rand() % (maxJitter * 2 + 1)) - maxJitter;
    return value + jitter;
}

// Humanization getters and setters
void CursorControl::SetHumanizationLevel(int level) {
    humanizationLevel = std::max(0, std::min(10, level));
}

int CursorControl::GetHumanizationLevel() const {
    return humanizationLevel;
}

void CursorControl::SetJitterAmount(int amount) {
    jitterAmount = std::max(0, std::min(10, amount));
}

int CursorControl::GetJitterAmount() const {
    return jitterAmount;
}

void CursorControl::SetMovementDelay(int microseconds) {
    movementDelay = std::max(0, std::min(10000, microseconds));
}

int CursorControl::GetMovementDelay() const {
    return movementDelay;
}

void CursorControl::SetPathDeviation(double deviation) {
    pathDeviation = std::max(0.0, std::min(1.0, deviation));
}

double CursorControl::GetPathDeviation() const {
    return pathDeviation;
}

void CursorControl::SetMovementStyle(int style) {
    movementStyle = std::max(0, std::min(2, style));
}

int CursorControl::GetMovementStyle() const {
    return movementStyle;
} 