#pragma once

#include "Render/Execute/Passes/Scene/FogParams.h"
#include "Render/Scene/Proxies/Effects/SceneEffectProxy.h"

class UHeightFogComponent;

/*
    FFogSceneProxy??HeightFogComponent???Œë” ?Œë¼ë¯¸í„°ë¥?Scene effect ê³„ì¸µ??ë³´ê??©ë‹ˆ??
    ?„ì¬??HeightFogPassê°€ ì²?ë²ˆì§¸ fog ?„ë¡?œì˜ ?Œë¼ë¯¸í„°ë¥??½ì–´ ?¬ìš©?©ë‹ˆ??
*/
class FFogSceneProxy : public FSceneEffectProxy
{
public:
    FFogSceneProxy(const UHeightFogComponent* InOwner, const FFogParams& InParams)
        : Owner(InOwner), Params(InParams)
    {
    }

    void UpdateParams(const FFogParams& InParams)
    {
        Params = InParams;
        DirtyFlags = EDirtyFlag::All;
    }

    const UHeightFogComponent* GetOwner() const { return Owner; }
    const FFogParams& GetFogParams() const { return Params; }

private:
    const UHeightFogComponent* Owner = nullptr;
    FFogParams Params;
};
