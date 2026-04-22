#pragma once
#include "Render/Execute/Passes/Base/RenderPass.h"
struct FRenderPipelineContext;
class FPrimitiveSceneProxy;

/*
    ?”ë“œ ê·¸ë¦¬?œì? ?”ë“œ ì¶•ì„ ?Œë”ë§í•˜??ë°°ê²½ ?¨ìŠ¤?…ë‹ˆ??
    Opaque ?¨ìŠ¤ ?´ì „???¤í–‰?˜ì–´ ë¬¼ì²´ ?¤ì— ê¹”ë¦¬ê²??©ë‹ˆ??
*/
class FGridPass : public FRenderPass
{
public:
    void PrepareInputs(FRenderPipelineContext& Context) override;
    void PrepareTargets(FRenderPipelineContext& Context) override;
    void BuildDrawCommands(FRenderPipelineContext& Context) override;
    void BuildDrawCommands(FRenderPipelineContext& Context, const FPrimitiveSceneProxy& Proxy) override { (void)Context; (void)Proxy; }
    void SubmitDrawCommands(FRenderPipelineContext& Context) override;
};
