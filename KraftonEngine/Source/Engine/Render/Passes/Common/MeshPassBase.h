#pragma once

#include "Render/Passes/RenderPass.h"
#include "Render/Passes/Common/RenderPassContext.h"
#include "Render/Submission/Commands/DrawCommandList.h"

class FMeshPassBase : public FRenderPass
{
public:
    virtual ~FMeshPassBase() = default;

    void Execute(FRenderPassContext& Context) final
    {
        if (!IsEnabled(Context))
        {
            return;
        }

        PrepareInputs(Context);
        PrepareTargets(Context);
        SubmitDrawCommands(Context);
        Cleanup(Context);
    }

    void BuildDrawCommands(FRenderPassContext& Context) override
    {
        (void)Context;
    }

protected:
    virtual bool IsEnabled(const FRenderPassContext& Context) const
    {
        (void)Context;
        return true;
    }

    virtual void Cleanup(FRenderPassContext& Context)
    {
        (void)Context;
    }

    void BindViewportTarget(FRenderPassContext& Context) const
    {
        ID3D11RenderTargetView* RTV = Context.GetViewportRTV();
        Context.Context->OMSetRenderTargets(1, &RTV, Context.GetViewportDSV());
    }

    void SubmitPassRange(FRenderPassContext& Context, ERenderPass Pass) const
    {
        if (!Context.DrawCommandList)
        {
            return;
        }

        uint32 Start = 0;
        uint32 End = 0;
        Context.DrawCommandList->GetPassRange(Pass, Start, End);
        if (Start < End)
        {
            Context.DrawCommandList->SubmitRange(Start, End, *Context.Device, Context.Context, *Context.StateCache);
        }
    }
};
