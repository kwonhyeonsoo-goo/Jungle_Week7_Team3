#include "Render/Passes/Scene/AlphaBlendPass.h"
#include "Render/Passes/Common/RenderPassContext.h"
#include "Render/Submission/Commands/DrawCommandList.h"
#include "Render/Submission/Builders/MeshDrawCommandBuilder.h"
#include "Render/Scene/Proxies/Primitive/PrimitiveSceneProxy.h"

void FAlphaBlendPass::PrepareInputs(FRenderPassContext& Context)
{
    (void)Context;
}

void FAlphaBlendPass::PrepareTargets(FRenderPassContext& Context)
{
    BindViewportTarget(Context);
}

void FAlphaBlendPass::BuildDrawCommands(FRenderPassContext& Context, const FPrimitiveSceneProxy& Proxy)
{
    FMeshDrawCommandBuilder::Build(Proxy, ERenderPass::AlphaBlend, Context, *Context.DrawCommandList);
}

void FAlphaBlendPass::SubmitDrawCommands(FRenderPassContext& Context)
{
    SubmitPassRange(Context, ERenderPass::AlphaBlend);
}