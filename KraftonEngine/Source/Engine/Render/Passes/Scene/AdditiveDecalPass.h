#pragma once
#include "Render/Passes/Common/MeshPassBase.h"
struct FSceneView; using FFrameContext = FSceneView;
struct FRenderPassContext;
class FPrimitiveSceneProxy;
class FAdditiveDecalPass : public FMeshPassBase
{
public:
    void PrepareInputs(FRenderPassContext& Context) override;
    void PrepareTargets(FRenderPassContext& Context) override;
    // Additive decal은 프록시 기반 경로만 사용한다.
    void BuildDrawCommands(FRenderPassContext& Context) override { (void)Context; }
    void BuildDrawCommands(FRenderPassContext& Context, const FPrimitiveSceneProxy& Proxy) override;
    void SubmitDrawCommands(FRenderPassContext& Context) override;
};
