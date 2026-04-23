# Render Binding Conventions

HLSL register slot은 고정 규칙을 따른다. 새 shader를 추가할 때 기존 slot 의미를 바꾸지 않는다.

## Constant Buffers

| Slot | Owner | Use |
|---|---|---|
| `b0` | Frame | view/projection, camera, time, wireframe state |
| `b1` | Object/Material | per-object matrix 또는 material shared data |
| `b2` | Pass/Shader | pass-local constants |
| `b3` | Material | optional material parameter buffer |

`b2`는 pass마다 의미가 달라질 수 있다. pass 실행 후 필요한 경우 SRV/CB를 명시적으로 unbind한다.

## SRV

| Slot | Use |
|---|---|
| `t0` | primary texture or fullscreen input |
| `t1` | normal texture or auxiliary pass input |
| `t2` | specular/material texture |
| `t3` | modified base color |
| `t4` | modified surface1 |
| `t5` | modified surface2 |
| `t6` | local light buffer |
| `t7` | per-tile light mask |
| `t8` | light debug hit map |
| `t10` | scene depth copy |
| `t11` | scene color copy |
| `t13` | stencil copy |

## Samplers

| Slot | Name |
|---|---|
| `s0` | `LinearClampSampler` |
| `s1` | `LinearWrapSampler` |
| `s2` | `PointClampSampler` |

## UAV

| Slot | Use |
|---|---|
| `u0` | light culling output |
| `u1` | lighting debug/evaluation counter |

## Source of Truth

| File | Role |
|---|---|
| `Render/Resources/Bindings/RenderBindingSlots.h` | C++ side slot constants |
| `Shaders/Common/Resources/SystemResources.hlsl` | common SRV declarations |
| `Shaders/Common/Resources/SystemSamplers.hlsl` | common sampler declarations |
| `Shaders/Common/Resources/ConstantBuffers.hlsl` | common CB declarations |
