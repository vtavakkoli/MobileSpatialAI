#pragma once
#include <cstddef>
#include <cstdint>
#include <string_view>
namespace msai {
enum class Accelerator { kCpu, kGpu, kNpu };
struct TensorView { void* data{nullptr}; std::size_t bytes{0}; const std::int32_t* shape{nullptr}; std::size_t rank{0}; };
struct InferenceStats { double preprocess_ms{0.0}; double inference_ms{0.0}; double postprocess_ms{0.0}; std::size_t resident_bytes{0}; Accelerator accelerator{Accelerator::kCpu}; };
class InferenceBackend {
public:
    virtual ~InferenceBackend() = default;
    virtual bool initialize(std::string_view model_path) = 0;
    virtual bool invoke(const TensorView& input, TensorView& output, InferenceStats& stats) = 0;
    [[nodiscard]] virtual Accelerator accelerator() const noexcept = 0;
    [[nodiscard]] virtual std::string_view name() const noexcept = 0;
};
}  // namespace msai
