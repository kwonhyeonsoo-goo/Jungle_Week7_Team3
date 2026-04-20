#pragma once

#include "Render/Passes/Common/FullscreenPassBase.h"

class FPrimitiveSceneProxy;
struct FRenderPassContext;

class FViewModeResolvePass : public FFullscreenPassBase
{
public:
    void PrepareInputs(FRenderPassContext& Context) override;
    void PrepareTargets(FRenderPassContext& Context) override;
    void BuildDrawCommands(FRenderPassContext& Context) override;
    // ViewMode post-process는 fullscreen path만 사용한다.
    void BuildDrawCommands(FRenderPassContext& Context, const FPrimitiveSceneProxy& Proxy) override { (void)Context; (void)Proxy; }
    void SubmitDrawCommands(FRenderPassContext& Context) override;
};
