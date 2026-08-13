#include "mobile_spatial_ai/depth_postprocessor.hpp"
#include <algorithm>
#include <cmath>
namespace msai {
thread_local std::vector<float> DepthPostprocessor::depth_scratch_{};
thread_local std::vector<float> DepthPostprocessor::confidence_scratch_{};
RobustDepth DepthPostprocessor::estimateForDetection(const DepthFrameView& frame,const Detection2D& detection) const {
    if (!frame.valid()) return {};
    const auto clampX=[width=frame.width](float v){return std::clamp(static_cast<int>(std::lround(v)),0,width-1);};
    const auto clampY=[height=frame.height](float v){return std::clamp(static_cast<int>(std::lround(v)),0,height-1);};
    int left=clampX(detection.left), right=clampX(detection.right), top=clampY(detection.top), bottom=clampY(detection.bottom);
    if(right<left) std::swap(right,left); if(bottom<top) std::swap(bottom,top);
    const int width=right-left+1, height=bottom-top+1; left+=width/4; right-=width/4; top+=height/4; bottom-=height/4;
    depth_scratch_.clear(); confidence_scratch_.clear();
    const std::size_t reserve=static_cast<std::size_t>(std::max(1,(right-left+1)*(bottom-top+1)));
    if(depth_scratch_.capacity()<reserve) depth_scratch_.reserve(reserve); if(confidence_scratch_.capacity()<reserve) confidence_scratch_.reserve(reserve);
    for(int y=top;y<=bottom;++y){const auto row=static_cast<std::size_t>(y*frame.width); for(int x=left;x<=right;++x){const auto i=row+static_cast<std::size_t>(x); const float d=frame.depth[i]; if(!std::isfinite(d)||d<=0.05F||d>100.0F) continue; depth_scratch_.push_back(d); if(frame.confidence) confidence_scratch_.push_back(std::clamp(frame.confidence[i],0.0F,1.0F));}}
    if(depth_scratch_.empty()) return {};
    const auto mid=depth_scratch_.begin()+static_cast<std::ptrdiff_t>(depth_scratch_.size()/2); std::nth_element(depth_scratch_.begin(),mid,depth_scratch_.end());
    float conf=1.0F; if(!confidence_scratch_.empty()){float sum=0.0F; for(float v:confidence_scratch_) sum+=v; conf=sum/static_cast<float>(confidence_scratch_.size());}
    return {true,*mid,conf,depth_scratch_.size()};
}
}  // namespace msai
