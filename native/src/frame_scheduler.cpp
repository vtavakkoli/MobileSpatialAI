#include "mobile_spatial_ai/frame_scheduler.hpp"
namespace msai {
void FrameScheduler::Lease::release() noexcept { if (owner_ != nullptr) { owner_->release(); owner_ = nullptr; } }
FrameScheduler::Lease FrameScheduler::tryAcquire() noexcept {
    if (busy_.test_and_set(std::memory_order_acquire)) { dropped_.fetch_add(1,std::memory_order_relaxed); return {}; }
    accepted_.fetch_add(1,std::memory_order_relaxed); return Lease(this);
}
}  // namespace msai
