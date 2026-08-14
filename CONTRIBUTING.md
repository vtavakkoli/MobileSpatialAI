# Contributing

MobileSpatialAI accepts focused pull requests with reproducible validation.

## Native changes

Run:

```bash
./scripts/benchmark-native.sh
```

Hot-path changes should include before/after p50 and p95 evidence. Avoid introducing per-frame heap allocation when a reusable buffer is practical.

## Android changes

Build with the pinned AGP/JDK/NDK versions described in the README. Keep Android lifecycle/UI code out of the native world-model APIs.

## Models

Do not commit large model weights. Add model metadata, source, license, checksum and conversion instructions before enabling automatic download or packaging.
