#pragma once
#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>
#include <mutex>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>
namespace msai {
struct LatencySummary { std::size_t samples{0}; double mean_ms{0}; double p50_ms{0}; double p95_ms{0}; double max_ms{0}; };
class PerformanceMonitor {
public:
  static constexpr std::size_t kWindow=512;
  void record(const std::string& stage,double ms){std::lock_guard lock(mutex_);auto& r=rings_[stage];r.values[r.cursor]=ms;r.cursor=(r.cursor+1)%kWindow;r.count=std::min(r.count+1,kWindow);}
  [[nodiscard]] LatencySummary summary(const std::string& stage) const {std::vector<double>s;{std::lock_guard lock(mutex_);auto it=rings_.find(stage);if(it==rings_.end()||it->second.count==0)return{};s.assign(it->second.values.begin(),it->second.values.begin()+static_cast<std::ptrdiff_t>(it->second.count));}std::sort(s.begin(),s.end());double sum=std::accumulate(s.begin(),s.end(),0.0);auto p=[&](double q){return s[static_cast<std::size_t>(static_cast<double>(s.size()-1U)*q)];};return{s.size(),sum/static_cast<double>(s.size()),p(.50),p(.95),s.back()};}
private:
  struct Ring{std::array<double,kWindow>values{};std::size_t cursor{0};std::size_t count{0};};mutable std::mutex mutex_;std::unordered_map<std::string,Ring>rings_;
};
class ScopedTimer{public:ScopedTimer(PerformanceMonitor&m,std::string s):m_(m),s_(std::move(s)),start_(Clock::now()){}~ScopedTimer(){m_.record(s_,std::chrono::duration<double,std::milli>(Clock::now()-start_).count());}private:using Clock=std::chrono::steady_clock;PerformanceMonitor&m_;std::string s_;Clock::time_point start_;};
}  // namespace msai
