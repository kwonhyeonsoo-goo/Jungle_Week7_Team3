#pragma once

#include "Render/Pipelines/ViewModePassConfig.h"

struct FRenderPassContext;
class FDrawCommandList;

class FFullscreenDrawCommandBuilder
{
public:
    static void Build(
        ERenderPass Pass,
        FRenderPassContext& Context,
        FDrawCommandList& OutList,
        EViewModePostProcessVariant PostProcessVariant = EViewModePostProcessVariant::None);
};
