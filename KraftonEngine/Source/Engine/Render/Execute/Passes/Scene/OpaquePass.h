#pragma once
#include "Render/Execute/Passes/Base/MeshPassBase.h"
struct FRenderPipelineContext;
class FPrimitiveSceneProxy;
/*
    ?¬ì˜ ê¸°ë³¸ G-Buffer/?œë©´ ?•ë³´ë¥?ê¸°ë¡?˜ëŠ” ?µì‹¬ ë©”ì‹œ ?¨ìŠ¤?…ë‹ˆ??
*/
class FOpaquePass : public FMeshPassBase
{
public:
    void PrepareInputs(FRenderPipelineContext& Context) override;
    void PrepareTargets(FRenderPipelineContext& Context) override;
    // Base draw???„ë¡???¨ìœ„ë¡œë§Œ ì»¤ë§¨?œë? ?ì„±?œë‹¤.
    void BuildDrawCommands(FRenderPipelineContext& Context) override { (void)Context; }
    void BuildDrawCommands(FRenderPipelineContext& Context, const FPrimitiveSceneProxy& Proxy) override;
    void SubmitDrawCommands(FRenderPipelineContext& Context) override;
};
