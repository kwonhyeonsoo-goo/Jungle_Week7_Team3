#pragma once
#include "Render/Execute/Passes/Base/MeshPassBase.h"
struct FRenderPipelineContext;
class FPrimitiveSceneProxy;
/*
    ê¹Šì´ë§?ë¨¼ì? ê¸°ë¡???´í›„ ?¨ìŠ¤??early-z ?¨ìœ¨???’ì´???¨ìŠ¤?…ë‹ˆ??
*/
class FDepthPrePass : public FMeshPassBase
{
public:
    void PrepareInputs(FRenderPipelineContext& Context) override;
    void PrepareTargets(FRenderPipelineContext& Context) override;
    // Depth pre-pass??ê°œë³„ ?„ë¡??ê²½ë¡œë§??¬ìš©?œë‹¤.
    void BuildDrawCommands(FRenderPipelineContext& Context) override { (void)Context; }
    // ?„ì¬ Depth pre-pass??ë³„ë„ draw commandë¥?ë§Œë“¤ì§€ ?ŠëŠ”??
    void BuildDrawCommands(FRenderPipelineContext& Context, const FPrimitiveSceneProxy& Proxy) override { (void)Context; (void)Proxy; }
    void SubmitDrawCommands(FRenderPipelineContext& Context) override;
};
