# TinyOBJLoader

This folder contains the single-header TinyOBJLoader dependency used by the OBJ asset loader.

Source header:
https://raw.githubusercontent.com/tinyobjloader/tinyobjloader/release/tiny_obj_loader.h

Upstream repository:
https://github.com/tinyobjloader/tinyobjloader

Branch:
release

Copied on:
2026-04-24

Usage:
`TINYOBJLOADER_IMPLEMENTATION` is defined in exactly one `.cc` file in the asset-loading module.

`TINYOBJLOADER_DISABLE_FAST_FLOAT` is also defined there to avoid MSVC constexpr issues in TinyOBJLoader's bundled fast-float path.
