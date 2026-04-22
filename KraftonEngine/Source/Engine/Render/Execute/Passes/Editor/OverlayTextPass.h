#pragma once
#include "Render/Execute/Passes/Base/RenderPass.h"
struct FRenderPipelineContext;
class FPrimitiveSceneProxy;
/*
    ?¤ë²„?ˆì´ ?ìŠ¤??ë°°ì¹˜ë¥??”ë©´???©ì„±?˜ëŠ” ?ë””???¨ìŠ¤?…ë‹ˆ??
*/
class FOverlayTextPass : public FRenderPass
{
public:
    void PrepareInputs(FRenderPipelineContext& Context) override;
    void PrepareTargets(FRenderPipelineContext& Context) override;
    void BuildDrawCommands(FRenderPipelineContext& Context) override;
    // Overlay text???ìŠ¤??ë°°ì¹˜ë§??Œë¹„?˜ë?ë¡??„ë¡???…ë ¥???¬ìš©?˜ì? ?ŠëŠ”??
    void BuildDrawCommands(FRenderPipelineContext& Context, const FPrimitiveSceneProxy& Proxy) override { (void)Context; (void)Proxy; }
    void SubmitDrawCommands(FRenderPipelineContext& Context) override;
};
