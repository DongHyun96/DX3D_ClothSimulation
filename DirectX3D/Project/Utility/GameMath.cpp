#include "Framework.h"
#include "GameMath.h"

float GameMath::Clamp(const float& value, const float& minValue, const float& maxValue)
{
    return max(minValue, min(maxValue, value));
}

float GameMath::Saturate(const float& value)
{
    return Clamp(value, 0.f, 1.f);
}

Vector3 GameMath::ClosestPointOnLine(const Vector3& start, const Vector3& end, const Vector3& point)
{
    Vector3 line    = end - start;
    Vector3 A       = point - start;

    float x = Vector3::Dot(line, A);
    float y = Vector3::Dot(line, line);

    float t = Clamp(x / y, 0, 1);

    return start + line * t;
}
