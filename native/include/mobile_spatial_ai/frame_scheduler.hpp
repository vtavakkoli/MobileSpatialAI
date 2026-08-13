#pragma once
#include <atomic>
#include <cstdint>
namespace msai {
class FrameScheduler {
public:
    class Lease {
    public:
        Lease() = default; explicit Lease(FrameScheduler* owner) noexcept : owner_(owner) {}
        Lease(const Lease&) = delete; Lease& operator=(const Lease&) = delete;
        Lease(Lease&& other) noexcept : owner_(other.owner_) { other.owner_ = nullptr; }
        Lease& operator=(Lease&& other) noexcept { if (this != &other) { release(); owner_=other.owner_; other.owner_=nullptr; } return *this; }
        ~Lease() { release(); }
        [[nodiscard]] explicit operator bool() const noexcept { return owner_ != nullptr; }
    private: void release() noexcept; FrameScheduler* owner_{nullptr};
    };
    [[nodiscard]] Lease tryAcquire() noexcept;
    [[nodiscard]] std::uint64_t accepted() const noexcept { return accepted_.load(); }
    [[nodiscard]] std::uint64_t dropped() const noexcept { return dropped_.load(); }
private:
    friend class Lease; void release() noexcept { busy_.clear(std::memory_order_release); }
    std::atomic_flag busy_ = ATOMIC_FLAG_INIT; std::atomic<std::uint64_t> accepted_{0}; std::atomic<std::uint64_t> dropped_{0};
};
}  // namespace msai
