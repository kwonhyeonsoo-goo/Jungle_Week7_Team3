#pragma once

#include "Render/Pipelines/ViewModePassConfig.h"

class FPrimitiveSceneProxy;
struct FRenderPassContext;
class FDrawCommandList;

class FMeshDrawCommandBuilder
{
public:
    static void Build(const FPrimitiveSceneProxy& Proxy, ERenderPass Pass, FRenderPassContext& Context, FDrawCommandList& OutList);
};
