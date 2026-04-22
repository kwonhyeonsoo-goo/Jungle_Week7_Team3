#pragma once
#include "Render/Execute/Passes/Base/MeshPassBase.h"
struct FRenderPipelineContext;
class FPrimitiveSceneProxy;
/*
    ê¸°ì¦ˆëª??„ë¡?œë? ë³„ë„ ?íƒœë¡??Œë”ë§í•˜???ë””???¨ìŠ¤?…ë‹ˆ??
*/
class FGizmoPass : public FMeshPassBase
{
public:
    void PrepareInputs(FRenderPipelineContext& Context) override;
    void PrepareTargets(FRenderPipelineContext& Context) override;
    // Gizmo pass??gizmo proxyë§?ê°œë³„?ìœ¼ë¡?ì²˜ë¦¬?œë‹¤.
    void BuildDrawCommands(FRenderPipelineContext& Context) override { (void)Context; }
    void BuildDrawCommands(FRenderPipelineContext& Context, const FPrimitiveSceneProxy& Proxy) override;
    void SubmitDrawCommands(FRenderPipelineContext& Context) override;
};
