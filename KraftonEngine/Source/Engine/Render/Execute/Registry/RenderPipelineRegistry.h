#pragma once

#include "Core/CoreTypes.h"

#include "Render/Execute/Registry/RenderPassRegistry.h"
#include "Render/Execute/Registry/RenderPipelineType.h"

/*
    ?Œì´?„ë¼??ê·¸ë˜?„ì—???ì‹ ?¸ë“œê°€ ???¤ë¥¸ ?Œì´?„ë¼?¸ì¸ì§€, ?¨ì¼ ?¨ìŠ¤?¸ì? êµ¬ë¶„?©ë‹ˆ??
*/
enum class ERenderNodeKind
{
    Pipeline,
    Pass,
};

/*
    ?Œì´?„ë¼??ê·¸ë˜?„ì˜ ?ì‹ ?¸ë“œ 1ê°œë? ?œí˜„?©ë‹ˆ??
*/
struct FRenderNodeRef
{
    ERenderNodeKind Kind;
    int32 TypeValue;
};

/*
    ?˜ë‚˜???Œë” ?Œì´?„ë¼?¸ê³¼ ê·??ì‹ ?¸ë“œ ëª©ë¡???•ì˜?©ë‹ˆ??
*/
struct FRenderPipelineDesc
{
    ERenderPipelineType Type;
    TArray<FRenderNodeRef> Children;
};

/*
    ë£¨íŠ¸/?œë¸Œ ?Œì´?„ë¼??êµ¬ì„±???±ë¡???ëŠ” ?ˆì??¤íŠ¸ë¦¬ì…?ˆë‹¤.
    Renderer?????ˆì??¤íŠ¸ë¦¬ë? ?µí•´ Scene, OverlayPipeline ê°™ì? ?Œì´?„ë¼???œì„œë¥?ì¡°íšŒ?©ë‹ˆ??
*/
class FRenderPipelineRegistry
{
public:
    void Initialize();
    void Release();

    const FRenderPipelineDesc* FindPipeline(ERenderPipelineType Type) const;

private:
    TMap<int32, FRenderPipelineDesc> Pipelines;
};
