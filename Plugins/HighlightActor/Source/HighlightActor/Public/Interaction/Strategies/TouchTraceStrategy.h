// © 2025 Heathrow (Derman). All rights reserved.This project is the intellectual property of Heathrow (Derman) and is protected by copyright law. Unauthorized reproduction, distribution, or use of this material is strictly prohibited.Unreal Engine and its associated trademarks are used under license from Epic Games.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/HighlightTraceStrategy.h"
#include "TouchTraceStrategy.generated.h"

/**
 * Traces a ray from the first touch point when pressed.
 */
UCLASS()
class HIGHLIGHTACTOR_API UTouchTraceStrategy : public UHighlightTraceStrategy
{
	GENERATED_BODY()

public:
	virtual bool PerformTrace_Implementation(APlayerController* PC, FHitResult& OutHit) override;
};
