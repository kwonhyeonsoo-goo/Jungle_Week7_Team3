#pragma once
#include "Render/Execute/Passes/Base/MeshPassBase.h"
struct FRenderPipelineContext;
class FPrimitiveSceneProxy;
/*
    ? íƒ???„ë¡?œë? ë§ˆìŠ¤??ë²„í¼??ê¸°ë¡?˜ëŠ” ?ë””???¨ìŠ¤?…ë‹ˆ??
*/
class FSelectionMaskPass : public FMeshPassBase
{
public:
    void PrepareInputs(FRenderPipelineContext& Context) override;
    void PrepareTargets(FRenderPipelineContext& Context) override;
    // Selection mask??? íƒ???„ë¡??ê²½ë¡œ?ì„œë§?ì»¤ë§¨?œë? ë§Œë“ ??
    void BuildDrawCommands(FRenderPipelineContext& Context) override { (void)Context; }
    void BuildDrawCommands(FRenderPipelineContext& Context, const FPrimitiveSceneProxy& Proxy) override;
    void SubmitDrawCommands(FRenderPipelineContext& Context) override;
};
