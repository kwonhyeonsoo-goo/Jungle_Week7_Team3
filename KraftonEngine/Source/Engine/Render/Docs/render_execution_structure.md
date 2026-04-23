# Render Execution Structure

| 구분 | 내용 |
|---|---|
| 최초 작성자 | 김연하 |
| 최초 작성일 | 2026-04-22 |
| 최근 수정자 | 김연하 |
| 최근 수정일 | 2026-04-23 |
| 상태 | Draft |
| 버전 | 1.0 |

## 1. 개요

이 구조는 렌더러를 **재사용 가능한 실행 단위** 중심으로 구성한다.  
실행 단위는 실제 GPU 작업을 수행하는 단위인 `Pass`와, Pass 및 다른 Pipeline을 묶어 실행하는 단위인 `Pipeline`으로 나뉜다.

`Pass`와 `Pipeline`는 모두 렌더 노드로 취급한다. 따라서 `Pipeline` 내부에는 `Pass`뿐 아니라 다른 `Pipeline`도 들어갈 수 있으며, 전체 실행 구조는 트리로 표현된다.

이 문서의 목적은 렌더러가 어떤 실행 단위들로 구성되고, 그 단위들이 어떤 문맥과 순서로 실행되는지 설명하는 것이다.

## 2. 핵심 설계 원칙

### 2.1 재사용 가능한 실행 단위: "Render Node"

렌더러는 큰 단일 함수가 아니라, 실행 단위인 "Render Node"를 조합해서 구성한다.

- Pass는 가장 작은 실행 단위다.
- Pipeline은 "Pass"를 묶는 조합 단위이자 실행 단위다.
- Pipeline 안에는 Pass 혹은 Pipeline이 일렬로 나열될 수 있다.
- Pass와 Pipeline은 모두 Render Node로 취급한다.
    - Pass는 **실행의 주체**가 되는 Leaf Node이다.
    - Pipeline은 **Child Node 리스트를 갖는** 조합 노드이다.

이 구조 덕분에 특정 파이프라인을 독립적으로 조합하거나 교체하기 쉬워진다.

| 구분 | 역할 |
|---|---|
| Pass | 실제 draw, dispatch, fullscreen 작업 수행 |
| Pipeline | Pass/Pipeline을 묶어 실행 순서와 구조를 표현 |


### 2.2 실행 정책 및 실행 문맥(Context)

**실행 정책**은 렌더러가 가진다.  
렌더러는 어떤 경로를 실행할지, 어떤 순서로 실행할지, 현재 ViewMode에서 어떤 Pipeline을 선택할지를 결정한다.

실행 중 필요한 공유 상태인 **실행 문맥**은 `Context` 계층에서 관리한다.  
Context는 각 Pass와 Pipeline이 공통으로 참조하는 실행 상태이며 scene 정보, view mode 정보, 공용 상태 객체 등을 담는다.

## 3. 전체 흐름

렌더러의 흐름은 크게 다음과 같다.

1. `Scene`과 `Submission`에서 렌더할 대상을 준비한다.
2. `Execute/Pipelines`에서 현재 프레임의 실행 트리를 구성한다.
3. `Runner`가 트리를 순회하며 실행한다.
4. 각 Leaf 단계는 `Pass`에서 실제 GPU 작업을 수행한다.
5. 실행 도중 필요한 상태는 `Context`가 제공한다.
6. Pass/ViewMode 매핑과 선택 규칙은 `Registry`가 관리한다.

즉, 렌더 대상 준비와 실행 구조는 분리되어 있으며, 실행 자체는 `Execute` 계층이 담당한다.

## 4. 디렉토리 구조

```text
Render/
├─ Execute/
│  ├─ Passes/
│  ├─ Pipelines/
│  ├─ Context/
│  ├─ Runner/
│  └─ Registry/
├─ Scene/
├─ Submission/
└─ Resources/
```

* `Execute/`: 렌더 실행 구조를 담당하는 핵심 계층이다.
* `Scene/`: 렌더 대상이 되는 scene 데이터와 proxy를 두는 계층이다.
* `Submission/`: scene에서 수집한 데이터를 실제 렌더 가능한 형태로 정리하는 계층이다. visible 수집, 분류, batching, overlay용 데이터 준비 등을 담당한다.
* `Resources/`: 버퍼, 텍스처, 상태 객체 등 렌더 실행에 필요한 GPU 자원을 관리하는 계층이다.


### 4.1. `Render/Execute/`

#### `/Passes`

가장 작은 실행 단위를 둔다.  
Pass는 실제 draw, dispatch, fullscreen 작업을 담당하는 Leaf Node다.

#### `/Pipelines`

Pass 또는 다른 Pipeline을 묶는 재사용 가능한 실행 단위인 Pipeline을 기술(decript)한다.

#### `/Context`

실행 중 공유되는 상태와 타입을 둔다.  
Context는 실행 노드들이 공통으로 참조하는 문맥이다.

#### `/Runner`

실행 트리를 실제로 순회하고 호출하는 실행기를 둔다.  
Runner는 “무엇을 실행할지”보다 “어떻게 실행할지”를 담당한다.

#### `/Registry`

Pass, ViewMode, shader variant 매핑 규칙을 둔다.  
Registry는 실행 시 필요한 선택 규칙을 제공한다.

### 4.2. `Render/Execute/Context`

#### `PipelineContext`

파이프라인 실행의 공통 문맥을 둔다.
전체 실행 흐름에서 공용으로 참조되는 상태를 담는다.

#### `ScenePipelineContext`

scene 렌더에 필요한 공통 문맥을 둔다.
scene 관련 렌더 대상, visible 결과, scene resource 참조를 담는다.

#### `ViewModePipelineContext`

현재 ViewMode에 따른 분기 정보와 해석 문맥을 둔다. 
Lit, UnLit, WorldNormal, SceneDepth 경로 선택과 shader variant 결정에 필요한 정보를 담는다.


## 5. 실행 트리

### 5.1 DefaultRootPipeline

기본 렌더 경로는 아래처럼 구성된다.

```text
ascii 트리 작성해야함
```

이 트리에서 `ScenePipeline`은 씬 자체를 렌더하는 주 경로이고, `OverlayPipeline`은 씬 위에 겹쳐지는 요소를 처리하며, `PresentPipeline`은 최종 결과를 출력한다.

### 5.2 EditorRootPipeline


```text
ascii 트리 작성해야함
```


## 6. ViewMode별 실행 경로

ViewMode는 개별 pass를 직접 고르는 개념이라기보다, 어떤 Pipeline 경로를 선택할지 결정하는 기준이다.

```
Lit_*       : DepthPre → LightCulling → Opaque → Decal → Lighting → PostProcess
Unlit       : DepthPre → Opaque → Decal → PostProcess
WorldNormal : DepthPre → Opaque → Decal → NonLitView
SceneDepth  : DepthPre → NonLitView
```
