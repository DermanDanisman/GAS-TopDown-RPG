// © 2025 Heathrow (Derman). All rights reserved.This project is the intellectual property of Heathrow (Derman) and is protected by copyright law. Unauthorized reproduction, distribution, or use of this material is strictly prohibited.Unreal Engine and its associated trademarks are used under license from Epic Games.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HighlightTraceStrategy.generated.h"

struct FHitResult;
class APlayerController;

/**
 * Abstract base strategy for highlight tracing.
 * Derive from this to implement different input modalities.
 */
UCLASS(Abstract, Blueprintable)
class HIGHLIGHTACTOR_API UHighlightTraceStrategy : public UObject
{
	GENERATED_BODY()

public:
	
	/**
	 * Perform a trace to determine the actor under the input.
	 * @param PC The player controller context.
	 * @param OutHit The hit result from the trace.
	 * @return true if a valid actor was hit; false otherwise.
	 */
	UFUNCTION(BlueprintNativeEvent, Category="Highlight")
	bool PerformTrace(APlayerController* PC, FHitResult& OutHit);
	virtual bool PerformTrace_Implementation(APlayerController* PC, FHitResult& OutHit) { return false; }
};
