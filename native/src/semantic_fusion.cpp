#include "mobile_spatial_ai/semantic_fusion.hpp"
namespace msai {
SpatialObservation SemanticFusion::fuse(const Detection2D& detection,const DepthFrameView& depth,const CameraIntrinsics& intrinsics,const Pose& camera_pose) const {
    const RobustDepth robust=depth_postprocessor_.estimateForDetection(depth,detection); if(!robust.valid) return {};
    const float px=(detection.left+detection.right)*0.5F, py=(detection.top+detection.bottom)*0.5F;
    const Vec3 camera_position=unproject(px,py,robust.depth_m,intrinsics);
    return {detection.label,detection.confidence,robust.confidence,camera_position,transformPoint(camera_pose,camera_position),depth.timestamp_ns};
}
}  // namespace msai
