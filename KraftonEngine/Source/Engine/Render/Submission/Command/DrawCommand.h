#pragma once

#include "Render/Execute/Passes/Base/RenderPassTypes.h"
#include "Render/RHI/D3D11/Common/D3D11API.h"
#include "Render/Execute/Context/PipelineStateTypes.h"
#include "Math/Vector.h"
#include "Core/CoreTypes.h"

class FShader;
class FMeshBuffer;
class FConstantBuffer;
struct ID3D11ShaderResourceView;
struct ID3D11Buffer;

/*
    FDrawCommand ???œë¡œ?°ì½œ 1ê°œì— ?„ìš”??ëª¨ë“  ?•ë³´ë¥?ìº¡ìŠ?”í•©?ˆë‹¤.
    UE5??FMeshDrawCommand ?¨í„´??ì°¨ìš©?˜ì—¬,
    PSO ?íƒœ + Geometry + Bindings + ?•ë ¬ ?¤ë? ?˜ë‚˜??êµ¬ì¡°ì²´ë¡œ ?µí•©?©ë‹ˆ??
*/
struct FDrawCommand
{
    // ===== PSO (Pipeline State Object) =====
    FShader*                 Shader       = nullptr;
    EDepthStencilState       DepthStencil = EDepthStencilState::Default;
    EBlendState              Blend        = EBlendState::Opaque;
    ERasterizerState         Rasterizer   = ERasterizerState::SolidBackCull;
    D3D11_PRIMITIVE_TOPOLOGY Topology     = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    uint8                    StencilRef   = 0;

    // ===== Geometry =====
    FMeshBuffer* MeshBuffer  = nullptr; // VB + IB (nullptr ??RawVB ?ëŠ” SV_VertexID ê¸°ë°˜ ?œë¡œ??
    uint32       FirstIndex  = 0;       // ?¸ë±???œì‘ ?¤í”„??
    uint32       IndexCount  = 0;       // DrawIndexed ?¸ë±????
    uint32       VertexCount = 0;       // IB ?†ì„ ??Draw(VertexCount, 0)
    int32        BaseVertex  = 0;       // DrawIndexed BaseVertexLocation

    // ===== Raw Buffer (?™ì  ì§€?¤ë©”?¸ë¦¬????MeshBufferê°€ nullptr?????¬ìš©) =====
    ID3D11Buffer* RawVB       = nullptr;
    uint32        RawVBStride = 0;
    ID3D11Buffer* RawIB       = nullptr;

    // ===== Bindings =====
    FConstantBuffer*          PerObjectCB    = nullptr; // b1: Model + Color
    FConstantBuffer*          PerShaderCB[2] = {};      // [0]=b2 (PerShader0), [1]=b3 (PerShader1)
    FConstantBuffer*          LightCB        = nullptr; // b4: Global Lights Constant Buffer
    ID3D11ShaderResourceView* DiffuseSRV     = nullptr; // t0: Base / Diffuse ?ìŠ¤ì²?
    ID3D11ShaderResourceView* NormalSRV      = nullptr; // t1: Normal map ?ìŠ¤ì²?
    ID3D11ShaderResourceView* SpecularSRV    = nullptr; // t2: Specular map ?ìŠ¤ì²?
    ID3D11ShaderResourceView* LocalLightSRV  = nullptr; // t6: LocalLights StructuredBuffer

    // ===== Sort =====
    uint64 SortKey = 0; // ?•ë ¬ ??(Pass ??Shader ??MeshBuffer ??SRV)

    // ===== Debug =====
    ERenderPass Pass      = ERenderPass::Opaque; // ?Œì† ?¨ìŠ¤ (?”ë²„ê·??µê³„??
    const char* DebugName = nullptr;             // ?”ë²„ê·??´ë¦„

    // ===== SortKey ?ì„± ? í‹¸ë¦¬í‹° =====
    // Pass(4bit) | ShaderHash(16bit) | MeshHash(16bit) | SRVHash(16bit) | UserBits(12bit)
    static uint64 BuildSortKey(ERenderPass InPass, const FShader* InShader,
                               const FMeshBuffer* InMeshBuffer, const ID3D11ShaderResourceView* InSRV,
                               uint16 UserBits = 0)
    {
        auto PtrHash16 = [](const void* Ptr) -> uint16
        {
            // ?¬ì¸?°ë? 16ë¹„íŠ¸ë¡?ì¶•ì†Œ ???íƒœ ?„í™˜ ê·¸ë£¹?‘ìš©?´ë?ë¡?ì¶©ëŒ ?ˆìš©
            uintptr_t Val = reinterpret_cast<uintptr_t>(Ptr);
            return static_cast<uint16>((Val >> 4) ^ (Val >> 20));
        };

        uint64 Key = 0;
        Key |= (static_cast<uint64>(InPass) & 0xF) << 60;            // [63:60] Pass
        Key |= (static_cast<uint64>(PtrHash16(InShader))) << 44;     // [59:44] Shader
        Key |= (static_cast<uint64>(PtrHash16(InMeshBuffer))) << 28; // [43:28] MeshBuffer
        Key |= (static_cast<uint64>(PtrHash16(InSRV))) << 12;        // [27:12] SRV
        Key |= (static_cast<uint64>(UserBits) & 0xFFF);              // [11:0]  User
        return Key;
    }
};
