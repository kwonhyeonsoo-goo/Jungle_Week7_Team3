#pragma once
#include "Render/Execute/Passes/Base/FullscreenPassBase.h"
struct FRenderPipelineContext;
class FPrimitiveSceneProxy;
/*
    ë·?ëª¨ë“œ ?œë©´???½ì–´ ìµœì¢… ì¡°ëª… ê²°ê³¼ë¥??©ì„±?˜ëŠ” fullscreen ?¨ìŠ¤?…ë‹ˆ??
*/
class FLightingPass : public FFullscreenPassBase
{
public:
    void PrepareInputs(FRenderPipelineContext& Context) override;
    void PrepareTargets(FRenderPipelineContext& Context) override;
    void BuildDrawCommands(FRenderPipelineContext& Context) override;
    // Lighting?€ fullscreen ?©ì„± pass?¼ì„œ ?„ë¡???…ë ¥???¬ìš©?˜ì? ?ŠëŠ”??
    void BuildDrawCommands(FRenderPipelineContext& Context, const FPrimitiveSceneProxy& Proxy) override { (void)Context; (void)Proxy; }
    void SubmitDrawCommands(FRenderPipelineContext& Context) override;
};
