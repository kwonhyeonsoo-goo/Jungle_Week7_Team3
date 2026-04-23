# Render Resources

## Frame Resources

| Resource | Owner | Use |
|---|---|---|
| `FrameBuffer` | `FFrameRenderResources` | per-frame constants |
| `GlobalLightBuffer` | `FFrameRenderResources` | ambient/directional light constants |
| `LocalLightBuffer/SRV` | `FFrameRenderResources` | local light structured buffer |
| per-object CB pool | `FFrameRenderResources` | object matrices and material data |

## Viewport Resources

| Resource | Owner | Use |
|---|---|---|
| viewport RTV | `FViewport` | scene color output |
| viewport DSV | `FViewport` | depth/stencil output |
| scene color copy | `FViewport` | fog, FXAA, outline input |
| depth copy | `FViewport` | decal, scene depth, fog, lighting input |
| stencil copy | `FViewport` | selection/outline input |

`FViewportRenderTargets` is a lightweight reference bundle. It does not own the D3D resources.

## ViewMode Surfaces

| Surface | Use |
|---|---|
| `BaseColor` | opaque base color |
| `Surface1` | normal or Gouraud lighting |
| `Surface2` | material params |
| `ModifiedBaseColor` | decal-modified base color |
| `ModifiedSurface1` | decal-modified normal/surface data |
| `ModifiedSurface2` | decal-modified material params |

`OpaquePass` writes base surfaces. `DecalPass` writes modified surfaces. `LightingPass` resolves base and modified values.

## Light Culling Resources

| Resource | Use |
|---|---|
| per-tile light mask | lighting shader local light lookup |
| debug hit map | `LightHitMapPass` visualization |
| light culling params CB | tile size, screen size, culling mode |

Compute shader path: `Shaders/Passes/Visibility/TileBasedLightCullingCS.hlsl`.
