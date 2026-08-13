#pragma once
#include "mobile_spatial_ai/frame_scheduler.hpp"
#include "mobile_spatial_ai/performance.hpp"
#include "mobile_spatial_ai/semantic_fusion.hpp"
#include "mobile_spatial_ai/spatial_world.hpp"
namespace msai {
class SpatialCore {
public:
  [[nodiscard]] std::uint64_t observe3D(const std::string&label,float confidence,const Vec3&position,std::int64_t timestamp_ns){ScopedTimer timer(performance_,"world_update");return world_.observe({label,confidence,1.0F,position,position,timestamp_ns});}
  [[nodiscard]] std::uint64_t fuseDetection(const Detection2D&d,const DepthFrameView&depth,const CameraIntrinsics&k,const Pose&pose){ScopedTimer timer(performance_,"semantic_fusion");const auto o=fusion_.fuse(d,depth,k,pose);return o.label.empty()?0:world_.observe(o);}
  [[nodiscard]] NearestResult nearest(const std::string&label,const Vec3&origin={})const{return world_.nearest(label,origin);}
  [[nodiscard]] std::vector<SpatialObject> behind(const Pose&pose,float max_m)const{return world_.behind(pose,max_m);}
  [[nodiscard]] std::size_t objectCount()const{return world_.size();}
  [[nodiscard]] FrameScheduler& scheduler()noexcept{return scheduler_;}
  [[nodiscard]] PerformanceMonitor& performance()noexcept{return performance_;}
private:SpatialWorld world_{};SemanticFusion fusion_{};FrameScheduler scheduler_{};PerformanceMonitor performance_{};
};
}  // namespace msai
