#pragma once

#include "Render/Execute/Context/Scene/ViewTypes.h"

/*
    Î∑?Î™®ÎìúÎ•??§Ï†ú ?∞Ïù¥??Î™®Îç∏Í≥??®Ïä§ ?®Í≥ÑÎ°??¥ÏÑù?????¨Ïö©?òÎäî Í≥µÏö© ?Ä??Î™®Ïùå?ÖÎãà??
    ViewMode -> ShadingModel Î≥Ä??Í∑úÏπô?????åÏùº?êÏÑú ?®Íªò Í¥ÄÎ¶¨Ìï©?àÎã§.
*/
enum class EShadingModel : uint8
{
    Gouraud = 0,
    Lambert,
    BlinnPhong,
    Unlit,
    WorldNormal,
    Count
};

enum class EViewModeStage : uint8
{
    Opaque = 0,
    Decal,
    Lighting,
    Count
};

inline EShadingModel GetShadingModelFromViewMode(EViewMode ViewMode)
{
    switch (ViewMode)
    {
    case EViewMode::Wireframe:
    case EViewMode::SceneDepth:
        return EShadingModel::Unlit;
    case EViewMode::Lit_Lambert:
        return EShadingModel::Lambert;
    case EViewMode::Lit_Phong:
        return EShadingModel::BlinnPhong;
    case EViewMode::Unlit:
        return EShadingModel::Unlit;
    case EViewMode::WorldNormal:
        return EShadingModel::WorldNormal;
    default:
        return EShadingModel::Gouraud;
    }
}

inline bool IsLitShadingModel(EShadingModel Model)
{
    return Model == EShadingModel::Gouraud || Model == EShadingModel::Lambert || Model == EShadingModel::BlinnPhong;
}

inline const char* GetShadingModelName(EShadingModel Model)
{
    switch (Model)
    {
    case EShadingModel::Gouraud:
        return "Gouraud";
    case EShadingModel::Lambert:
        return "Lambert";
    case EShadingModel::BlinnPhong:
        return "BlinnPhong";
    case EShadingModel::Unlit:
        return "Unlit";
    case EShadingModel::WorldNormal:
        return "WorldNormal";
    default:
        return "Unknown";
    }
}
