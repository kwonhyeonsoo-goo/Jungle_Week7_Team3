#pragma once
#include "Render/Execute/Passes/Base/RenderPass.h"
struct FRenderPipelineContext;
class FPrimitiveSceneProxy;
/*
    ?”ë²„ê·??¼ì¸ ë°°ì¹˜ë¥??”ë©´???œì¶œ?˜ëŠ” ?ë””???¨ìŠ¤?…ë‹ˆ??
*/
class FDebugLinePass : public FRenderPass
{
public:
    void PrepareInputs(FRenderPipelineContext& Context) override;
    void PrepareTargets(FRenderPipelineContext& Context) override;
    void BuildDrawCommands(FRenderPipelineContext& Context) override;
    // Debug line pass???„ë¡?œê? ?„ë‹ˆ???¼ì¸ ë²„í¼ë§??Œë¹„?œë‹¤.
    void BuildDrawCommands(FRenderPipelineContext& Context, const FPrimitiveSceneProxy& Proxy) override { (void)Context; (void)Proxy; }
    void SubmitDrawCommands(FRenderPipelineContext& Context) override;
};
