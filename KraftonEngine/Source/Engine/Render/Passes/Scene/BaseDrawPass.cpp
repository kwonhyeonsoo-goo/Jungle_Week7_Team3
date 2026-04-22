#include "Render/Passes/Scene/BaseDrawPass.h"
#include "Render/Pipelines/Context/RenderPipelineContext.h"
#include "Render/Submission/Command/DrawCommandList.h"
#include "Render/Submission/Command/BuildDrawCommand.h"
#include "Render/Scene/Proxies/Primitive/PrimitiveSceneProxy.h"
#include "Render/Pipelines/Context/ViewMode/SceneViewModeSurfaces.h"
#include "Render/Pipelines/Registry/ViewModePassRegistry.h"
#include "Render/Resources/RenderResources.h"
#include "Render/Visibility/TileBasedLightCulling.h"

// Gouraud Light Culling에서 VS t7/b5 바인딩 여부를 확인하는 헬퍼
static bool NeedsGouraudLightCulling(const FRenderPipelineContext& Context)
{
    return Context.LightCulling &&
           Context.LightCulling->IsInitialized() &&
           Context.ViewModePassRegistry &&
           Context.ViewModePassRegistry->GetShadingModel(Context.ActiveViewMode) == EShadingModel::Gouraud;
}

void FBaseDrawPass::PrepareInputs(FRenderPipelineContext& Context)
{
    ID3D11ShaderResourceView* NullSRVs[6] = {};
    Context.Context->PSSetShaderResources(0, ARRAY_SIZE(NullSRVs), NullSRVs);

    ID3D11ShaderResourceView* NullSystemSRV = nullptr;
    Context.Context->PSSetShaderResources(ESystemTexSlot::SceneDepth, 1, &NullSystemSRV);
    Context.Context->PSSetShaderResources(ESystemTexSlot::SceneColor, 1, &NullSystemSRV);
    Context.Context->PSSetShaderResources(ESystemTexSlot::Stencil, 1, &NullSystemSRV);
    Context.Context->PSSetShaderResources(ESystemTexSlot::LocalLights, 1, &NullSystemSRV);

    if (Context.StateCache)
    {
        Context.StateCache->DiffuseSRV = nullptr;
        Context.StateCache->NormalSRV = nullptr;
        Context.StateCache->LocalLightSRV = nullptr;
        Context.StateCache->bForceAll = true;
    }

    // Gouraud Light Culling: DepthPrePass 결과 타일 마스크를 VS에 바인딩
    // - t7 (VS): LightCullingPass가 채운 타일별 조명 비트마스크
    // - b5 (VS): LightCullingParams (ScreenSize, TileSize) — b2는 StaticMeshMaterial이 점유
    if (NeedsGouraudLightCulling(Context))
    {
        ID3D11ShaderResourceView* TileMaskSRV = Context.LightCulling->GetPerTileMaskSRV();
        Context.Context->VSSetShaderResources(7, 1, &TileMaskSRV);

        ID3D11Buffer* LightCullingParamsCB = Context.LightCulling->GetLightCullingParamsCB();
        Context.Context->VSSetConstantBuffers(5, 1, &LightCullingParamsCB);
    }
    else
    {
        ID3D11ShaderResourceView* NullVSSRV = nullptr;
        Context.Context->VSSetShaderResources(7, 1, &NullVSSRV);

        ID3D11Buffer* NullVSCB = nullptr;
        Context.Context->VSSetConstantBuffers(5, 1, &NullVSCB);
    }
}

void FBaseDrawPass::PrepareTargets(FRenderPipelineContext& Context)
{
    const bool bUseViewModeSurfaces =
        Context.ActiveViewSurfaces &&
        Context.ViewModePassRegistry &&
        Context.ViewModePassRegistry->HasConfig(Context.ActiveViewMode) &&
        Context.ActiveViewMode != EViewMode::Wireframe;

    if (bUseViewModeSurfaces)
    {
        const EShadingModel ShadingModel = Context.ViewModePassRegistry->GetShadingModel(Context.ActiveViewMode);
        Context.ActiveViewSurfaces->ClearBaseTargets(Context.Context, ShadingModel);
        Context.ActiveViewSurfaces->BindBaseDrawTargets(Context.Context, ShadingModel, Context.GetViewportDSV());
    }
    else
    {
        ID3D11RenderTargetView* RTV = Context.GetViewportRTV();
        Context.Context->OMSetRenderTargets(1, &RTV, Context.GetViewportDSV());
    }
}

void FBaseDrawPass::BuildDrawCommands(FRenderPipelineContext& Context, const FPrimitiveSceneProxy& Proxy)
{
    DrawCommandBuilder::BuildMeshDrawCommand(Proxy, ERenderPass::Opaque, Context, *Context.DrawCommandList);
}

void FBaseDrawPass::SubmitDrawCommands(FRenderPipelineContext& Context)
{
    SubmitPassRange(Context, ERenderPass::Opaque);
}

void FBaseDrawPass::Cleanup(FRenderPipelineContext& Context)
{
    // Gouraud Light Culling용 VS 전용 리소스 해제 (이후 패스와의 슬롯 충돌 방지)
    ID3D11ShaderResourceView* NullSRV = nullptr;
    Context.Context->VSSetShaderResources(7, 1, &NullSRV);

    ID3D11Buffer* NullCB = nullptr;
    Context.Context->VSSetConstantBuffers(5, 1, &NullCB);
}
