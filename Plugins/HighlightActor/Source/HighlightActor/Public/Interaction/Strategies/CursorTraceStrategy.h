// © 2025 Heathrow (Derman). All rights reserved.This project is the intellectual property of Heathrow (Derman) and is protected by copyright law. Unauthorized reproduction, distribution, or use of this material is strictly prohibited.Unreal Engine and its associated trademarks are used under license from Epic Games.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/HighlightTraceStrategy.h"
#include "CursorTraceStrategy.generated.h"

/**
 * Traces a ray from the mouse cursor into the world.
 */
UCLASS()
class HIGHLIGHTACTOR_API UCursorTraceStrategy : public UHighlightTraceStrategy
{
	GENERATED_BODY()

public:
	
	/**
	 * Deprojects the mouse screen position and performs a line trace.
	 */
	virtual bool PerformTrace_Implementation(APlayerController* PC, FHitResult& OutHit) override;
};
