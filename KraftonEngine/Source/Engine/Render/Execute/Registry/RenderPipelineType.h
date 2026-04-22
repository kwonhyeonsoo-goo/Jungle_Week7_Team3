#pragma once

// Render/Execute에서 조립되는 파이프라인 노드 종류입니다.
enum class ERenderPipelineType
{
    DefaultRootPipeline,
    EditorRootPipeline,
    ScenePipeline,
    LitPipeline,
    NonLitPipeline,
    DepthOnlyPipeline,
    PostProcessPipeline,
    OverlayPipeline,
    PresentPipeline,
    Outline
};