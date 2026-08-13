# Architecture

## Design rule

The Android application is a shell around a native spatial runtime. The system avoids model-specific or UI-specific logic inside the world model.

```text
Android/Kotlin
  CameraX | Sensors | UI | permissions
             |
             | timestamped buffers / JNI
             v
C++ Spatial Runtime
  FrameScheduler
      |
      +--> DepthEngine -----------+
      +--> TrackingEngine --------+--> SpatialFusion --> SpatialWorld
      +--> SemanticEngine --------+                       |
                                                         +--> QueryEngine
                                                         +--> Navigation
                                                         +--> Persistence
```

## Coordinate contract

All modules must use an explicit coordinate frame and timestamp. `Pose` transforms camera-local positions into the persistent world frame. Models may have different tensor conventions, but adapters must normalize before entering `SpatialFusion`.

## Threading

- camera producer: Android/CameraX thread;
- sensor producer: Android sensor callback thread;
- inference workers: native worker pool in future adapters;
- world updates: short exclusive lock;
- queries/rendering: shared lock;
- stale camera analysis frames: dropped rather than queued indefinitely.

## Native boundaries

`InferenceBackend` is intentionally independent from LiteRT headers. A LiteRT adapter owns runtime objects and exposes normalized tensor views/stats to the spatial core. This keeps the core unit-testable on Linux and makes accelerator backends replaceable.

## SLAM licensing

External SLAM systems are architectural references or optional adapters unless their licenses are compatible with this repository. Source code from strong-copyleft implementations must not be copied into the Apache-2.0 core without an explicit licensing decision.
