#include "mobile_spatial_ai/geometry.hpp"
namespace msai {
Vec3 rotate(const Quaternion& q, const Vec3& v) noexcept {
    const Vec3 qv{q.x,q.y,q.z};
    const Vec3 uv{qv.y*v.z-qv.z*v.y,qv.z*v.x-qv.x*v.z,qv.x*v.y-qv.y*v.x};
    const Vec3 uuv{qv.y*uv.z-qv.z*uv.y,qv.z*uv.x-qv.x*uv.z,qv.x*uv.y-qv.y*uv.x};
    return v + uv*(2.0F*q.w) + uuv*2.0F;
}
Vec3 transformPoint(const Pose& pose,const Vec3& point) noexcept { return rotate(pose.rotation,point)+pose.translation; }
Vec3 forwardVector(const Pose& pose) noexcept { return rotate(pose.rotation,{0.0F,0.0F,1.0F}); }
Vec3 unproject(float px,float py,float d,const CameraIntrinsics& k) noexcept {
    if (d<=0.0F || k.fx==0.0F || k.fy==0.0F) return {};
    return {(px-k.cx)*d/k.fx,(py-k.cy)*d/k.fy,d};
}
}  // namespace msai
