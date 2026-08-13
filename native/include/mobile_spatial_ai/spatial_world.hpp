#pragma once
#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstddef>
#include <limits>
#include <mutex>
#include <shared_mutex>
#include <string_view>
#include <vector>
#include "mobile_spatial_ai/geometry.hpp"
#include "mobile_spatial_ai/types.hpp"
namespace msai {
class SpatialWorld {
public:
  explicit SpatialWorld(float merge_distance_m=0.65F,std::size_t max_objects=4096):merge_distance_sq_(merge_distance_m*merge_distance_m),max_objects_(max_objects){objects_.reserve(std::min<std::size_t>(max_objects,512));}
  [[nodiscard]] std::uint64_t observe(const SpatialObservation&o){if(o.label.empty()||o.timestamp_ns<=0)return 0;const float c=std::clamp(o.detection_confidence*o.depth_confidence,0.0F,1.0F);std::unique_lock lock(mutex_);SpatialObject*best=nullptr;float best_d=merge_distance_sq_;for(auto&x:objects_){if(x.label!=o.label)continue;const float d=d2(x.position,o.world_position);if(d<=best_d){best_d=d;best=&x;}}if(best){const float ow=std::max(.05F,best->confidence)*static_cast<float>(best->observations),nw=std::max(.05F,c),t=ow+nw;best->position={(best->position.x*ow+o.world_position.x*nw)/t,(best->position.y*ow+o.world_position.y*nw)/t,(best->position.z*ow+o.world_position.z*nw)/t};best->confidence=std::clamp((best->confidence*ow+c*nw)/t,0.0F,1.0F);++best->observations;best->last_seen_ns=o.timestamp_ns;return best->id;}if(objects_.size()>=max_objects_){auto oldest=std::min_element(objects_.begin(),objects_.end(),[](const auto&a,const auto&b){return a.last_seen_ns<b.last_seen_ns;});if(oldest!=objects_.end())objects_.erase(oldest);}const auto id=next_id_.fetch_add(1,std::memory_order_relaxed);objects_.push_back({id,o.label,o.world_position,c,1,o.timestamp_ns,o.timestamp_ns});return id;}
  [[nodiscard]] std::vector<SpatialObject> snapshot()const{std::shared_lock lock(mutex_);return objects_;}
  [[nodiscard]] NearestResult nearest(std::string_view label,const Vec3&origin)const{std::shared_lock lock(mutex_);NearestResult r{};float best=std::numeric_limits<float>::infinity();for(const auto&o:objects_){if(o.label!=label)continue;const float d=d2(o.position,origin);if(d<best){best=d;r.found=true;r.object=o;}}if(r.found)r.distance_m=std::sqrt(best);return r;}
  [[nodiscard]] std::vector<SpatialObject> behind(const Pose&pose,float max_m)const{const Vec3 f=forwardVector(pose);const float max2=max_m*max_m;std::vector<SpatialObject>r;std::shared_lock lock(mutex_);r.reserve(std::min<std::size_t>(objects_.size(),32));for(const auto&o:objects_){const Vec3 rel=o.position-pose.translation;if(d2(o.position,pose.translation)<=max2&&dot(f,rel)<0.0F)r.push_back(o);}return r;}
  [[nodiscard]] std::size_t size()const{std::shared_lock lock(mutex_);return objects_.size();}
  void clear(){std::unique_lock lock(mutex_);objects_.clear();}
private:
  [[nodiscard]] static float d2(const Vec3&a,const Vec3&b)noexcept{const float x=a.x-b.x,y=a.y-b.y,z=a.z-b.z;return x*x+y*y+z*z;}
  const float merge_distance_sq_;const std::size_t max_objects_;mutable std::shared_mutex mutex_;std::vector<SpatialObject>objects_;std::atomic<std::uint64_t>next_id_{1};
};
}  // namespace msai
