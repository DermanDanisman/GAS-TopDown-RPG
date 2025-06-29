// © 2025 Heathrow (Derman). All rights reserved.This project is the intellectual property of Heathrow (Derman) and is protected by copyright law. Unauthorized reproduction, distribution, or use of this material is strictly prohibited.Unreal Engine and its associated trademarks are used under license from Epic Games.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/HighlightTraceStrategy.h"
#include "VRControllerTraceStrategy.generated.h"

/**
 * Traces a ray from a VR motion controller component.
 */
UCLASS()
class HIGHLIGHTACTOR_API UVRControllerTraceStrategy : public UHighlightTraceStrategy
{
	GENERATED_BODY()

public:
	virtual bool PerformTrace_Implementation(APlayerController* PC, FHitResult& OutHit) override;
};
