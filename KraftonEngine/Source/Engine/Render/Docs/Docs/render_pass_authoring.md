# 렌더 패스 추가 가이드

새로운 렌더 패스나 파이프라인 분기를 추가할 때 아래 체크리스트를 사용합니다.

## 패스 추가하기

1. `Execute/Passes/<Area>/` 아래에 새 클래스를 추가합니다.
2. `RenderPassRegistry.h`에 `ERenderPassNodeType` 값을 추가합니다.
3. `FRenderPassRegistry::Initialize`에서 해당 클래스를 등록합니다.
4. `FRenderPipelineRegistry::Initialize`에서 파이프라인에 해당 패스를 추가합니다.
5. `RenderPipelineRunner.cpp::GetRenderPassMarkerName`에 GPU 마커를 추가합니다.
6. draw command를 제출하는 패스라면 `PassRenderState.cpp`에 패스 상태를 추가합니다.
7. 대응되는 `Shaders/Passes/...` 폴더 아래에 셰이더 파일을 추가합니다.
8. `.vcxproj`와 `.vcxproj.filters`도 함께 갱신합니다.

## 뷰 모드 분기 추가하기

1. 필요하면 `ViewTypes.h`의 `EViewMode`를 추가하거나 수정합니다.
2. `ViewModePassRegistry.cpp`에 pass flag를 추가합니다.
3. `BuildViewMode*PassDesc`에서 셰이더 variant 매크로를 추가하거나 수정합니다.
4. `ERenderPipelineType`에 분기 파이프라인 이름을 추가합니다.
5. `ShouldExecutePipeline`에 실행 조건 분기를 추가합니다.

## 마커 작성 규칙

등록되는 모든 패스는 반드시 마커 문자열을 가져야 합니다.  
마커 이름은 패스 enum과 동일하게 맞추는 것을 권장합니다. 예를 들면 다음과 같습니다.

```cpp
case ERenderPassNodeType::OpaquePass: return L"OpaquePass";