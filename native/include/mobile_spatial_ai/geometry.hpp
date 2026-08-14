#pragma once
#include "mobile_spatial_ai/types.hpp"
namespace msai {
[[nodiscard]] Vec3 rotate(const Quaternion& q, const Vec3& v) noexcept;
[[nodiscard]] Vec3 transformPoint(const Pose& pose, const Vec3& point) noexcept;
[[nodiscard]] Vec3 forwardVector(const Pose& pose) noexcept;
[[nodiscard]] Vec3 unproject(float pixel_x, float pixel_y, float depth_m, const CameraIntrinsics& intrinsics) noexcept;
}  // namespace msai
