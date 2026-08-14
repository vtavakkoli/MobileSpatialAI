#pragma once
#include "mobile_spatial_ai/depth_postprocessor.hpp"
#include "mobile_spatial_ai/geometry.hpp"
#include "mobile_spatial_ai/types.hpp"
namespace msai {
class SemanticFusion {
public:
    [[nodiscard]] SpatialObservation fuse(const Detection2D& detection, const DepthFrameView& depth, const CameraIntrinsics& intrinsics, const Pose& camera_pose) const;
private:
    DepthPostprocessor depth_postprocessor_{};
};
}  // namespace msai
