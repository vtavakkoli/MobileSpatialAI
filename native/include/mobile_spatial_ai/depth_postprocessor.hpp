#pragma once
#include <vector>
#include "mobile_spatial_ai/types.hpp"
namespace msai {
struct RobustDepth { bool valid{false}; float depth_m{0.0F}; float confidence{0.0F}; std::size_t samples{0}; };
class DepthPostprocessor {
public:
    [[nodiscard]] RobustDepth estimateForDetection(const DepthFrameView& frame, const Detection2D& detection) const;
private:
    static thread_local std::vector<float> depth_scratch_;
    static thread_local std::vector<float> confidence_scratch_;
};
}  // namespace msai
