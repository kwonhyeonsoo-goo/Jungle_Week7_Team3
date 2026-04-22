#pragma once
#include "Render/Execute/Passes/Base/MeshPassBase.h"
struct FRenderPipelineContext;
class FPrimitiveSceneProxy;
/*
    ?¬ëª… ?¤ë¸Œ?íŠ¸ë¥??ŒíŒŒ ë¸”ë Œ?©ìœ¼ë¡?ê·¸ë¦¬??ë©”ì‹œ ?¨ìŠ¤?…ë‹ˆ??
*/
class FAlphaBlendPass : public FMeshPassBase
{
public:
    void PrepareInputs(FRenderPipelineContext& Context) override;
    void PrepareTargets(FRenderPipelineContext& Context) override;
    // Alpha blend pass???„ë¡???¨ìœ„ submitë§??¬ìš©?œë‹¤.
    void BuildDrawCommands(FRenderPipelineContext& Context) override { (void)Context; }
    void BuildDrawCommands(FRenderPipelineContext& Context, const FPrimitiveSceneProxy& Proxy) override;
    void SubmitDrawCommands(FRenderPipelineContext& Context) override;
};
