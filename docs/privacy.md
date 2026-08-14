# Privacy model

MobileSpatialAI is local-first.

## Defaults

- no camera frame upload;
- no cloud inference requirement;
- no telemetry upload by the native core;
- models loaded from application-private storage or packaged assets;
- generated maps remain local unless an explicit export feature is invoked.

Future network/model download features must be isolated from the spatial core and clearly visible to the user. Optional VLM/cloud experimentation must be opt-in and must not change the default local path.
