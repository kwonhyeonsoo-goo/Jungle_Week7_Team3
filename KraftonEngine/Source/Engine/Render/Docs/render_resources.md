# Render Resources

| 구분 | 내용 |
|---|---|
| 최초 작성자 | 김연하 |
| 최초 작성일 | 2026-04-22 |
| 최근 수정자 | 김연하 |
| 최근 수정일 | 2026-04-23 |
| 상태 | Draft |
| 버전 | 1.1 |

## 1. 개요

렌더 리소스 구조의 핵심 축은 다음 세 가지다.

1. `FFrameRenderResources`  
   프레임 공용 리소스

2. `FViewportRenderTargets`  
   뷰포트 출력 및 복사 리소스

3. `FSceneViewModeSurfaces`  
   surface 기반 intermediate 리소스

## 2. 분류

### 2.1 Context

현재 프레임/뷰 실행에 필요한 문맥이다.  
리소스를 직접 소유하거나 참조할 수 있다.

| 타입 | 역할 |
|---|---|
| `FRenderPipelineContext` | 패스 실행에 필요한 현재 프레임/뷰/리소스 문맥 |
| `FRenderCollectContext` | render item 수집 단계에서 사용하는 문맥 |
| `FSceneView` | 현재 camera, matrix, viewport, show flag를 담는 view 문맥 |
| `FViewportRenderTargets` | 현재 viewport용 RTV/DSV 및 scene color/depth copy를 관리하는 문맥 |
| `FFrameRenderResources` | 프레임 공용 constant buffer, sampler, light buffer 등을 관리하는 문맥 |
| `FLODUpdateContext` | LOD 계산 시 사용하는 view 기반 문맥 |

### 2.2 Resource

실제 렌더링 중 읽고 쓰는 GPU/프레임 자원이다.

| 타입 | 역할 |
|---|---|
| `FSceneViewModeSurfaces` | Opaque/Decal/Lighting 경로에서 사용하는 intermediate surface 묶음 |
| `FConstantBufferPool` | constant buffer 재사용 풀 |
| `FConstantBufferBinding` | draw 시점에 constant buffer를 바인딩하기 위한 정보 |
| `FPerObjectConstants` | object별 상수 버퍼 payload |
| `FFrameConstants` | 프레임 공용 상수 버퍼 payload |
| `FGizmoConstants` | gizmo 렌더링용 상수 버퍼 payload |
| `FOutlinePostProcessConstants` | outline post-process용 상수 버퍼 payload |
| `FFogConstants` | fog pass용 상수 버퍼 payload |
| `FFXAAConstants` | FXAA pass용 상수 버퍼 payload |

### 2.3 Preset

패스별로 미리 정해둔 고정 설정값이다.  
리소스가 아니라 실행 정책에 가깝다.

| 타입 | 역할 |
|---|---|
| `FRenderPassPreset` | 패스별 고정 preset 묶음 |
| `FRenderPassDrawPreset` | 패스의 depth/blend/rasterizer/topology preset |
| `FRenderPipelineDesc` | pipeline 트리 구조를 정의하는 preset |
| `FRenderPipelinePassDesc` | pipeline 내부 pass 구성 정보를 정의하는 preset |
| `FViewModePassConfig` | view mode별 pass/shader 사용 구성을 정의하는 preset |
| `FShaderVariantDesc` | shader variant 조합을 정의하는 preset |

### 2.4 Registry

패스 타입, preset 같은 고정 데이터를 등록·조회하는 계층이다.

| 타입 | 역할 |
|---|---|
| `FRenderPassRegistry` | render pass와 pass preset을 등록·조회 |
| `FRenderPipelineRegistry` | render pipeline 트리 구조를 등록·조회 |
| `FViewModePassRegistry` | view mode별 pass/shader 구성을 등록·조회 |
| `ERenderPass` | render pass 식별자 |
| `FRenderNodeRef` | pipeline tree에서 child pass/pipeline을 가리키는 참조 |

## 3. 수명 (LifeTime)

| 수명 | 설명 | 예 |
|---|---|---|
| Frame lifetime | 프레임 전체 동안 유지 | `FFrameRenderResources` 내부 버퍼 |
| Viewport lifetime | 뷰포트 크기/유효성에 종속 | `FViewportRenderTargets`, `FSceneViewModeSurfaces` |
| Persistent lifetime | 여러 프레임에 걸쳐 재사용 | `FConstantBufferPool` |

## 4. 접근 패턴 (Access Pattern)

| 패턴 | 설명 |
|---|---|
| Read-only | 이전 pass 결과를 이후 pass가 읽기만 함 |
| Write-only | 현재 pass가 새 결과를 기록함 |
| Read-Modify-Write | 기존 결과를 읽고 수정한 뒤 다른 target에 기록함 |
| Producer / Consumer | 한 pass가 쓰고 다음 pass가 읽음 |

대표 예:
- `DepthPre` → `DepthCopySRV` → `Decal`, `Fog`, `Outline`
- `Opaque` → `BaseColor/Surface` → `Decal`, `Lighting`
- `Decal` → `ModifiedSurface` → `Lighting`
