#pragma once

namespace GameMath
{
	float Clamp(const float& value, const float& minValue, const float& maxValue);
	float Saturate(const float& value);

	Vector3 ClosestPointOnLine(const Vector3& start, const Vector3& end, const Vector3& point);

	int   Random(const int&   min, const int&   max);
	float Random(const float& min, const float& max);
}
