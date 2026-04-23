# Render Overview

렌더러 문서의 진입점이다. 문서는 구현 규칙을 빠르게 확인하는 용도로 유지한다.

## Documents

| File | Purpose |
|---|---|
| `render_execution_structure.md` | 루트 파이프라인, ViewMode별 실행 경로, pass 실행 방식 |
| `render_resources.md` | 주요 렌더 리소스와 수명 |
| `render_conventions_binding.md` | HLSL register slot 규칙 |
| `render_conventions.md` | 이름 규칙 |
| `render_pass_authoring.md` | 새 pass/pipeline 추가 절차 |

## Current Render Roots

| Root | Use |
|---|---|
| `DefaultRootPipeline` | 게임/기본 렌더 경로 |
| `EditorRootPipeline` | 에디터 viewport 렌더 경로 |

`PresentPipeline`은 `EditorRootPipeline`에 포함된다. `PresentPass`는 viewport target이 없으면 no-op이다.
