#pragma once
#include <cmath>

struct InterpolatedTransform {
    float currentX = 0.0f, currentY = 0.0f, currentZ = 0.0f;
    float targetX = 0.0f, targetY = 0.0f, targetZ = 0.0f;
    float currentRotY = 0.0f, targetRotY = 0.0f;
    
    // Adjust accumulation factors based on network latency
    const float POSITION_LERP_FACTOR = 0.25f; 
    const float ROTATION_LERP_FACTOR = 0.30f;

    void SetTarget(float x, float y, float z, float rotY, bool teleport) {
        targetX = x;
        targetY = y;
        targetZ = z;
        targetRotY = rotY;

        if (teleport) {
            currentX = x;
            currentY = y;
            currentZ = z;
            currentRotY = rotY;
        }
    }

    void Step(float* outPos, float* outRotY) {
        // Linear interpolation for spatial vectors
        currentX += (targetX - currentX) * POSITION_LERP_FACTOR;
        currentY += (targetY - currentY) * POSITION_LERP_FACTOR;
        currentZ += (targetZ - currentZ) * POSITION_LERP_FACTOR;

        // Angular interpolation (handling 0 to 360-degree wrap-around boundaries)
        float diffY = targetRotY - currentRotY;
        while (diffY < -180.0f) diffY += 360.0f;
        while (diffY >  180.0f) diffY -= 360.0f;
        currentRotY += diffY * ROTATION_LERP_FACTOR;

        // Output back directly into the engine's pointer offsets
        outPos[0] = currentX;
        outPos[1] = currentY;
        outPos[2] = currentZ;
        *outRotY = currentRotY;
    }
};