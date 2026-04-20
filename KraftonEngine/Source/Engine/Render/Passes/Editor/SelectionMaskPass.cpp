#include "Render/Passes/Editor/SelectionMaskPass.h"
#include "Render/Passes/Common/RenderPassContext.h"
#include "Render/Submission/Commands/DrawCommandList.h"
#include "Render/Submission/Builders/MeshDrawCommandBuilder.h"
#include "Render/Scene/Proxies/Primitive/PrimitiveSceneProxy.h"

void FSelectionMaskPass::PrepareInputs(FRenderPassContext& Context)
{
    (void)Context;
}

void FSelectionMaskPass::PrepareTargets(FRenderPassContext& Context)
{
    BindViewportTarget(Context);
}

void FSelectionMaskPass::BuildDrawCommands(FRenderPassContext& Context, const FPrimitiveSceneProxy& Proxy)
{
    FMeshDrawCommandBuilder::Build(Proxy, ERenderPass::SelectionMask, Context, *Context.DrawCommandList);
}

void FSelectionMaskPass::SubmitDrawCommands(FRenderPassContext& Context)
{
    SubmitPassRange(Context, ERenderPass::SelectionMask);
}