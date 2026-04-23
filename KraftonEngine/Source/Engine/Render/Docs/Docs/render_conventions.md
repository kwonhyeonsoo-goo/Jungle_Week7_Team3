# Render Naming Conventions

## Pipelines

| Rule | Example |
|---|---|
| Root pipeline은 `*RootPipeline`로 끝낸다. | `EditorRootPipeline` |
| Shared grouping pipeline은 역할 + `Pipeline`을 쓴다. | `ScenePipeline`, `OverlayPipeline` |
| ViewMode branch pipeline은 화면 모드 이름을 그대로 쓴다. | `Lit`, `Unlit`, `WorldNormal`, `SceneDepth` |

## Passes

| Rule | Example |
|---|---|
| C++ pass class는 `F*Pass`를 쓴다. | `FOpaquePass` |
| pass node enum은 class 이름에서 `F`만 뺀다. | `OpaquePass` |
| pass marker는 enum 이름과 같은 사람이 읽는 이름을 쓴다. | `L"OpaquePass"` |

## Shaders

| Category | Rule | Example |
|---|---|---|
| Scene pass shader | pass 이름과 맞춘다. | `OpaquePass.hlsl` |
| ViewMode shader | scene pass 내부의 view mode 폴더에 둔다. | `Passes/Scene/ViewModes/UberLit.hlsl` |
| Common include | 도메인 폴더에 둔다. | `Common/Lighting/LightingCommon.hlsli` |
| Material helper | material 의미가 드러나게 둔다. | `Common/Material/DecalCommon.hlsli` |

## Shader Directories

```text
Shaders/
├─ Common/
│  ├─ Geometry/
│  ├─ Resources/
│  ├─ Surface/
│  ├─ Lighting/
│  ├─ Material/
│  └─ Utils/
├─ Passes/
│  ├─ Scene/
│  │  └─ ViewModes/
│  ├─ PostProcess/
│  └─ Visibility/
├─ Materials/
└─ Editor/
```

Top-level `Shaders/ViewModes`는 사용하지 않는다. ViewMode용 shader는 해당 scene pass 아래에 둔다.
