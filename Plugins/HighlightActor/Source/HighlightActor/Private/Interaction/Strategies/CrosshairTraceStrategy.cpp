// © 2025 Heathrow (Derman). All rights reserved.This project is the intellectual property of Heathrow (Derman) and is protected by copyright law. Unauthorized reproduction, distribution, or use of this material is strictly prohibited.Unreal Engine and its associated trademarks are used under license from Epic Games.


#include "Interaction/Strategies/CrosshairTraceStrategy.h"

#include "HighlightActor.h"

bool UCrosshairTraceStrategy::PerformTrace_Implementation(APlayerController* PC, FHitResult& OutHit)
{
    int32 VPX, VPY;
    PC->GetViewportSize(VPX, VPY);
    FVector2D Center(VPX * 0.5f, VPY * 0.5f);

    // Trace from screen center
    return PC->GetHitResultAtScreenPosition(
        Center,
        HIGHLIGHTABLE,
        true,
        OutHit
    );
}
