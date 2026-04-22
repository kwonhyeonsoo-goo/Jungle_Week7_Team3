#pragma once

#include "Render/Execute/Context/PipelineStateTypes.h"
#include "Render/Execute/Passes/Base/RenderPassTypes.h"
#include "Render/RHI/D3D11/Common/D3D11API.h"

/*
    ê°??Œë” ?¨ìŠ¤ê°€ ê¸°ë³¸?ìœ¼ë¡??¬ìš©???íƒœ ê¸°ìˆ ?œì…?ˆë‹¤.
    ?¨ìŠ¤ë³?ê¸°ë³¸ Depth / Blend / Rasterizer / Topology ì¡°í•©????êµ°ë°?ì„œ ?•ì˜?©ë‹ˆ??
*/
struct FPassRenderStateDesc
{
    EDepthStencilState DepthStencil;
    EBlendState Blend;
    ERasterizerState Rasterizer;
    D3D11_PRIMITIVE_TOPOLOGY Topology;
};

void InitializeDefaultPassRenderStateDescs(FPassRenderStateDesc (&OutStateDescs)[(uint32)ERenderPass::MAX]);
