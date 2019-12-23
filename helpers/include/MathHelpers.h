#pragma once

using Radians = float;
using Degrees = float;

constexpr float  PI_F = 3.14159265358979f;

inline constexpr Degrees RadiansToDegrees(Radians radians) { return radians / PI_F * 180.f; }
inline constexpr Radians DegreesToRadians(Degrees degrees) { return degrees / 180.f * PI_F; }