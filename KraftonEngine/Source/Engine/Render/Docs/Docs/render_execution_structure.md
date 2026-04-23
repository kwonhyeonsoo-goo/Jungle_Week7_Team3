# Render Execution Structure

## Node Model

렌더 실행 그래프는 두 종류의 node로 구성된다.

| Node | Meaning |
|---|---|
| Pipeline | pass 또는 다른 pipeline을 순서대로 묶는 node |
| Pass | 실제 draw, dispatch, fullscreen 작업을 수행하는 leaf node |

정의 위치:

| Type | File |
|---|---|
| `ERenderPipelineType` | `Execute/Registry/RenderPipelineType.h` |
| `FRenderPipelineRegistry` | `Execute/Registry/RenderPipelineRegistry.*` |
| `FRenderPipelineRunner` | `Execute/Runner/RenderPipelineRunner.*` |

## Pipeline Tree

```text
EditorRootPipeline
├─ ScenePipeline
│  ├─ Lit:         DepthPre → LightCulling → Opaque → Decal → Lighting
│  ├─ Unlit:       DepthPre → Opaque → Decal
│  ├─ WorldNormal: DepthPre → Opaque → Decal → NonLitView
│  ├─ SceneDepth:  DepthPre → NonLitView
│  └─ PostProcess: HeightFog → FXAA
├─ OverlayPipeline
│  ├─ LightHitMap
│  ├─ DebugLine
│  ├─ Outline: SelectionMask → Outline
│  ├─ OverlayBillboard
│  ├─ Gizmo
│  └─ OverlayText
└─ PresentPipeline
   └─ Present
```

`ScenePipeline`은 위 child를 모두 가지고 있지만 `FRenderPipelineRunner::ShouldExecutePipeline`이 현재 `EViewMode`에 맞는 scene child만 실행한다.

## ViewMode Mapping

| ViewMode | Pipeline path |
|---|---|
| `Lit_Gouraud`, `Lit_Lambert`, `Lit_Phong` | `Lit → PostProcess` |
| `Unlit` | `Unlit → PostProcess` |
| `WorldNormal` | `WorldNormal` |
| `SceneDepth` | `SceneDepth` |
| `Wireframe` | `Unlit` |

`PostProcess`의 pass는 `FViewModePassRegistry` 플래그로 다시 걸러진다. 예를 들어 `WorldNormal`과 `SceneDepth`는 `HeightFog`, `FXAA`를 실행하지 않는다.

## Execution Flow

1. `Renderer.BeginCollect`가 draw collector와 frame-local batch를 초기화한다.
2. `DrawCollector`가 scene primitive, light, overlay 데이터를 수집한다.
3. `Renderer.BuildDrawCommands`가 pass별 draw command를 만든다.
4. `Renderer.RunRootPipeline`이 `PreparePipelineExecution` 후 runner를 호출한다.
5. `FRenderPipelineRunner`가 pipeline tree를 순회하며 pass를 실행한다.
6. 각 pass는 `PrepareInputs → PrepareTargets → SubmitDrawCommands` 순서로 실행된다.

## Important Rule

새 pass를 등록하면 GPU marker 이름도 `RenderPipelineRunner.cpp`의 `GetRenderPassMarkerName`에 추가한다. marker가 없으면 GPU 캡처와 프로파일링에서 pass 경계가 흐려진다.
