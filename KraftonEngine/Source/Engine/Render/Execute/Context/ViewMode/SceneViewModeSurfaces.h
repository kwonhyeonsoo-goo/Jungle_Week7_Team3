#pragma once

#include "Core/CoreTypes.h"
#include "Render/Execute/Passes/Scene/ShadingTypes.h"
#include "Render/RHI/D3D11/Textures/SurfaceTexture.h"

/*
    ë·?ëª¨ë“œ ?Œì´?„ë¼?¸ì´ ?¬ìš©?˜ëŠ” ì¤‘ê°„ ?œë©´ ?¬ë¡¯?…ë‹ˆ??
    Opaque, Decal, Lighting/Resolve ?¨ìŠ¤ê°€ ê°™ì? ?¬ë¡¯ ?˜ë?ë¥?ê³µìœ ?˜ë„ë¡?ê³ ì •???ˆì´?„ì›ƒ???œê³µ?©ë‹ˆ??
*/
enum class ESceneViewModeSurfaceSlot : uint8
{
    BaseColor = 0,
    Surface1,
    Surface2,
    ModifiedBaseColor,
    ModifiedSurface1,
    ModifiedSurface2,
    Count
};

/*
    ??ë·°ëª¨???Œì´?„ë¼?¸ì—???¬ìš©?˜ëŠ” ì¤‘ê°„ ?œë©´ ë¬¶ìŒ?…ë‹ˆ??
    Opaque??ê¸°ë³¸ ?œë©´??ê¸°ë¡?˜ê³ , Decal?€ Modified ?œë©´??ê¸°ë¡?˜ë©°, Lighting/Resolve???´ë? SRVë¡??½ìŠµ?ˆë‹¤.
*/
class FSceneViewModeSurfaces
{
public:
    bool Initialize(ID3D11Device* Device, uint32 InWidth, uint32 InHeight);
    void Resize(ID3D11Device* Device, uint32 InWidth, uint32 InHeight);
    void Release();

    void ClearBaseTargets(ID3D11DeviceContext* Ctx, EShadingModel Model);
    void ClearModifiedTargets(ID3D11DeviceContext* Ctx, EShadingModel Model);

    void BindOpaqueTargets(ID3D11DeviceContext* Ctx, EShadingModel Model, ID3D11DepthStencilView* DSV);
    void BindDecalTargets(ID3D11DeviceContext* Ctx, EShadingModel Model, ID3D11DepthStencilView* DSV);

    ID3D11ShaderResourceView* GetSRV(ESceneViewModeSurfaceSlot Slot) const;
    ID3D11RenderTargetView* GetRTV(ESceneViewModeSurfaceSlot Slot) const;

private:
    bool CreateSurface(ID3D11Device* Device, ESceneViewModeSurfaceSlot Slot, DXGI_FORMAT Format, uint32 InWidth, uint32 InHeight);
    void ReleaseSurface(FSurfaceTexture& Surface);

private:
    FSurfaceTexture Surfaces[static_cast<uint32>(ESceneViewModeSurfaceSlot::Count)] = {};
    uint32 Width = 0;
    uint32 Height = 0;
};
