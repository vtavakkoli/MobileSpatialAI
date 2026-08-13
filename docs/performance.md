# Performance engineering

MobileSpatialAI optimizes for sustained mobile performance, not a short peak-FPS demo.

## Current native techniques

- C++17 release `-O3`.
- link-time optimization when supported.
- position-independent/hidden native symbols.
- thread-local reusable depth scratch memory.
- central ROI depth sampling to reduce work and background contamination.
- bounded object store.
- shared-reader/exclusive-writer world locking.
- lock-free frame admission counters.
- rolling fixed-size latency windows.

## Metrics

Every performance-sensitive stage should report:

- FPS / throughput;
- p50 and p95 latency;
- maximum latency;
- resident memory;
- dropped frames;
- thermal state;
- energy/battery delta when available;
- accelerator selected (CPU/GPU/NPU).

## Mobile scheduling

The camera path must favor recency. If inference cannot keep up with acquisition, stale frames are dropped and telemetry records the drop rate. The SLAM/VIO path may have a separate policy when sequential observations are mandatory; this must not silently block the preview/UI path.

## Fast math

`MSAI_ENABLE_FAST_MATH` is OFF by default. Enabling it can improve some numeric kernels but changes IEEE behavior; any accuracy effect must be included in benchmark reports.
