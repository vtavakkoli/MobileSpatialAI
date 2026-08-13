# MobileSpatialAI

**Privacy-preserving, on-device spatial intelligence for Android.**

MobileSpatialAI is a research-grade spatial-computing platform designed to turn an ordinary Android phone into a persistent semantic 3D observer: estimate depth, localize the device, place detected objects in world coordinates, maintain a local world model, and answer spatial questions without sending camera frames to the cloud by default.

> **Status:** v1.0 performance foundation. The hot path is implemented in C++17 and exposed to Android through a deliberately thin JNI layer. Depth, VIO/SLAM, semantic-model and LiteRT accelerator adapters are the next integration milestones.

## Why a native core?

The project is intentionally **C++ first for performance-sensitive work**:

- geometry, coordinate transforms and depth post-processing;
- semantic 2D→3D fusion;
- persistent object-map updates and spatial queries;
- frame admission/backpressure and bounded queues;
- latency telemetry and benchmark instrumentation;
- future VIO/SLAM, pose-graph and dense-map kernels;
- future LiteRT C++ CPU/GPU/NPU model adapters.

Kotlin is reserved for Android lifecycle, permissions, sensors, CameraX integration and presentation. This avoids pushing large image/depth buffers repeatedly through managed allocations.

## v1.0 target

```text
CameraX RGB ─────────┐
                     ├──> Native C++ spatial pipeline ──> SemanticWorldModel
Accelerometer/Gyro ──┤          │                              │
                     │          ├─ DepthEngine                 ├─ nearest(object)
Depth model ─────────┤          ├─ VIO/SLAM                   ├─ behind(camera)
                     │          ├─ SemanticEngine             ├─ distance(object)
Detector/segmenter ──┘          ├─ 3D fusion                  └─ navigation tools
                                └─ performance telemetry
```

## Implemented in this foundation

- `SpatialCore`: native orchestration facade.
- `FrameScheduler`: lock-free single-flight frame admission with explicit dropped-frame accounting.
- `DepthPostprocessor`: robust central-ROI depth estimation using reusable thread-local scratch buffers.
- `SemanticFusion`: projects 2D detections through depth + camera intrinsics + pose into world coordinates.
- `SpatialWorld`: bounded, thread-safe persistent object store with confidence-weighted observation fusion.
- spatial queries: nearest object and objects behind the current camera pose.
- `PerformanceMonitor`: fixed-size rolling p50/p95/max latency instrumentation.
- stable `InferenceBackend` interface for future LiteRT CPU/GPU/NPU implementations.
- JNI facade and Android native smoke screen.
- Linux native correctness tests and release microbenchmark.
- Android/NDK and native CI workflows.

## Performance principles

1. **Native hot path.** Geometry, mapping and model post-processing remain in C++.
2. **Drop stale frames.** Spatial perception prefers the newest frame over queue growth.
3. **Reuse memory.** Per-frame scratch allocations are retained and reused.
4. **Bound state.** The world model has a configurable maximum object count.
5. **Measure tails.** p95 latency is a first-class metric; average FPS is not enough.
6. **No unsafe math by default.** `-ffast-math` is opt-in because spatial geometry needs predictable numerical behavior.
7. **Release LTO.** Interprocedural optimization is enabled when supported.
8. **ARM64 first.** Android release builds target `arm64-v8a`; `x86_64` remains available for emulator/dev testing.

## Build the native core

```bash
./scripts/benchmark-native.sh
```

The command configures a Release build, runs correctness tests and writes:

```text
results/native-benchmark.json
```

## Android build

The project is pinned to Android Gradle Plugin 9.3, JDK 17, compile/target SDK 37 and NDK 28.2.

```bash
gradle :app:assembleDebug
```

The current Android screen verifies that the C++ world model is loaded and can answer a native spatial query. The camera/depth/SLAM Spatial Lab UI is intentionally left for a follow-up PR so that this foundation remains reviewable and benchmarkable.

## Planned pipeline

### 1. Real-time depth

- Depth Anything 3 Small as the first mobile target.
- LiteRT native CPU/GPU/NPU adapters.
- confidence/uncertainty maps.
- FPS, p50/p95 latency, RAM, thermal and energy measurements.

### 2. Visual-inertial SLAM

- camera + timestamp-aligned gyro/accelerometer input.
- keyframes, local mapping, loop closure and pose graph.
- persistent maps and relocalization.
- reuse algorithms from `MobileSLAM` through a clean native adapter rather than copying its Android UI.

### 3. Semantic 3D mapping

- object detection + optional segmentation.
- depth-aware 3D localization.
- persistent object identity and room/scene graph.
- occupancy and obstacle maps.

### 4. Spatial AI agent

The agent will call deterministic spatial tools instead of inventing geometry:

- “Where is the closest chair?”
- “How far is the door?”
- “What objects are behind me?”
- “Find an obstacle-free path.”

### 5. Privacy

- camera frames remain on device by default;
- local models and local map storage;
- no network permission required by the spatial core;
- optional local VLM/embeddings behind explicit user-controlled modules.

### 6. Research benchmark

The benchmark suite will compare both models and end-to-end pipelines:

| Pipeline / model | FPS | p50 | p95 | RAM | Energy | Accuracy / error |
|---|---:|---:|---:|---:|---:|---:|
| Depth only | | | | | | |
| SLAM only | | | | | | |
| Depth + SLAM | | | | | | |
| Depth + SLAM + semantics | | | | | | |
| Full Spatial AI | | | | | | |

### 7. Spatial Lab demo

The flagship UI will expose RGB, depth, semantic overlay, 3D reconstruction, camera trajectory, object map, navigation and a performance dashboard from the same synchronized pipeline.

## Repository layout

```text
MobileSpatialAI/
├── app/                      # thin Android/JNI application layer
├── native/                   # C++17 performance-critical spatial core
│   ├── include/mobile_spatial_ai/
│   ├── src/
│   ├── tests/
│   └── bench/
├── models/                   # model registry/checksum metadata (no large weights in Git)
├── docs/                     # architecture, performance and privacy design
├── scripts/                  # reproducible native benchmark commands
├── results/                  # generated benchmark evidence (ignored except docs/examples)
└── .github/workflows/        # native + Android CI
```

## Safety and scope

MobileSpatialAI is a research and development platform. Monocular depth and mobile visual localization can be uncertain or fail under poor texture, motion blur, reflective surfaces, occlusion, low light and unseen environments. Do not use the current software for safety-critical autonomous navigation or accessibility guidance without application-specific validation and appropriate fail-safe design.

## License

Apache License 2.0. Third-party models and optional adapters retain their own licenses; model metadata must record those terms before distribution.
