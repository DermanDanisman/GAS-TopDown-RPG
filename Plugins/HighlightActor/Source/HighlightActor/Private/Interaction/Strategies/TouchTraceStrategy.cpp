// © 2025 Heathrow (Derman). All rights reserved.This project is the intellectual property of Heathrow (Derman) and is protected by copyright law. Unauthorized reproduction, distribution, or use of this material is strictly prohibited.Unreal Engine and its associated trademarks are used under license from Epic Games.


#include "Interaction/Strategies/TouchTraceStrategy.h"

#include "HighlightActor.h"

bool UTouchTraceStrategy::PerformTrace_Implementation(APlayerController* PC, FHitResult& OutHit)
{
	// Read touch state
	float X, Y;
	bool bPressed;
	PC->GetInputTouchState(ETouchIndex::Touch1, X, Y, bPressed);
	if (!bPressed)
		return false;

	// Trace from touch position
	return PC->GetHitResultAtScreenPosition(
		FVector2D(X, Y),
		HIGHLIGHTABLE,
		true,
		OutHit
	);
}
