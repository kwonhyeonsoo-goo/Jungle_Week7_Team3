
# Render Conventions

| 구분 | 내용 |
|---|---|
| 최초 작성자 | 김연하 |
| 최초 작성일 | 2026-04-22 |
| 최근 수정자 | 김연하 |
| 최근 수정일 | 2026-04-23 |
| 상태 | Draft |
| 버전 | 1.0 |


## 2. Binding Conventions

### 2.1 Slot Conventions

| 구분 | 표기 | 의미 |
|---|---|---|
| Constant Buffer | `b#` | 상수 버퍼 슬롯 |
| Shader Resource View | `t#` | 읽기 전용 리소스 슬롯 |
| Sampler | `s#` | 샘플러 슬롯 |
| Unordered Access View | `u#` | 읽기/쓰기 가능한 리소스 슬롯 |

### 2.2 Binding Scope Conventions

| 단위 | 설명 |
|---|---|
| Frame 단위 | 프레임 전체에서 공통으로 유지되는 바인딩 |
| Pass 단위 | 특정 pass 실행 동안 공통으로 유지되는 바인딩 |
| Draw 단위 | 개별 draw call마다 바뀌는 바인딩 |

### 2.3 State Separation Conventions

| 구분 | 설명 |
|---|---|
| Global binding | 여러 pass가 공통으로 참조하는 바인딩 |
| Pass-local binding | 특정 pass 안에서만 유효한 바인딩 |
| Material binding | 머티리얼 단위로 바뀌는 바인딩 |
| Per-object binding | 오브젝트마다 바뀌는 바인딩 |

### 2.4 Comment Conventions

| 항목 | 규칙 |
|---|---|
| 슬롯 주석 | `b#`, `t#`, `s#`, `u#`의 의미를 적는다 |
| 바인딩 주체 | 어떤 C++ 코드가 바인딩하는지 적는다 |
| 유효 범위 | 어느 pass에서 유효한지 적는다 |
| 후속 사용처 | 다음 pass가 무엇을 읽는지 적는다 |

## 3. Naming Conventions

### 3.1 Render Node Naming

| 대상 | 규칙 | 예시 |
|---|---|---|
| Pass | 역할이 직접 드러나는 이름 사용 | `OpaquePass`, `LightingPass` |
| Pipeline | 조합 단위 역할이 드러나는 이름 사용 | `ScenePipeline`, `LitPipeline` |
| RootPipeline | 특정 문맥의 최상위 엔트리에만 사용 | `DefaultRootPipeline`, `EditorRootPipeline` |

### 3.2 Rules for Using Root

| 규칙 | 내용 |
|---|---|
| 규칙 1 | 재사용 가능한 단위 이름에는 `Root`를 넣지 않는다 |
| 규칙 2 | 특정 문맥의 최상위 엔트리 이름에만 `Root`를 붙인다 |
| 규칙 3 | `RootPipeline` 단독 이름은 쓰지 않는다 |
