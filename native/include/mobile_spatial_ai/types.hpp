#pragma once

#include <cmath>
#include <cstdint>
#include <limits>
#include <string>

namespace msai {
struct Vec3 { float x{0.0F}; float y{0.0F}; float z{0.0F}; [[nodiscard]] float norm() const noexcept { return std::sqrt(x*x+y*y+z*z); } };
inline Vec3 operator+(const Vec3& a,const Vec3& b) noexcept { return {a.x+b.x,a.y+b.y,a.z+b.z}; }
inline Vec3 operator-(const Vec3& a,const Vec3& b) noexcept { return {a.x-b.x,a.y-b.y,a.z-b.z}; }
inline Vec3 operator*(const Vec3& a,float s) noexcept { return {a.x*s,a.y*s,a.z*s}; }
inline float dot(const Vec3& a,const Vec3& b) noexcept { return a.x*b.x+a.y*b.y+a.z*b.z; }
struct Quaternion { float w{1.0F}; float x{0.0F}; float y{0.0F}; float z{0.0F}; };
struct Pose { Vec3 translation{}; Quaternion rotation{}; };
struct CameraIntrinsics { float fx{1.0F}; float fy{1.0F}; float cx{0.0F}; float cy{0.0F}; };
struct Detection2D { std::string label; float confidence{0.0F}; float left{0.0F}; float top{0.0F}; float right{0.0F}; float bottom{0.0F}; };
struct DepthFrameView { const float* depth{nullptr}; const float* confidence{nullptr}; int width{0}; int height{0}; std::int64_t timestamp_ns{0}; [[nodiscard]] bool valid() const noexcept { return depth != nullptr && width > 0 && height > 0; } };
struct SpatialObservation { std::string label; float detection_confidence{0.0F}; float depth_confidence{0.0F}; Vec3 camera_position{}; Vec3 world_position{}; std::int64_t timestamp_ns{0}; };
struct SpatialObject { std::uint64_t id{0}; std::string label; Vec3 position{}; float confidence{0.0F}; std::uint32_t observations{0}; std::int64_t first_seen_ns{0}; std::int64_t last_seen_ns{0}; };
struct NearestResult { bool found{false}; SpatialObject object{}; float distance_m{std::numeric_limits<float>::infinity()}; };
}  // namespace msai
