#pragma once
#include "Render/Execute/Passes/Base/MeshPassBase.h"
struct FRenderPipelineContext;
class FPrimitiveSceneProxy;
/*
    Additive ë¸”ë Œ???°ì¹¼???¬ì— ?„ì ?˜ëŠ” ë©”ì‹œ ?¨ìŠ¤?…ë‹ˆ??
*/
class FAdditiveDecalPass : public FMeshPassBase
{
public:
    void PrepareInputs(FRenderPipelineContext& Context) override;
    void PrepareTargets(FRenderPipelineContext& Context) override;
    // Additive decal?€ ?„ë¡??ê¸°ë°˜ ê²½ë¡œë§??¬ìš©?œë‹¤.
    void BuildDrawCommands(FRenderPipelineContext& Context) override { (void)Context; }
    void BuildDrawCommands(FRenderPipelineContext& Context, const FPrimitiveSceneProxy& Proxy) override;
    void SubmitDrawCommands(FRenderPipelineContext& Context) override;
};
