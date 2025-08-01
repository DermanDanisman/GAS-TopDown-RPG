// © 2025 Heathrow (Derman). All rights reserved.This project is the intellectual property of Heathrow (Derman) and is protected by copyright law. Unauthorized reproduction, distribution, or use of this material is strictly prohibited.Unreal Engine and its associated trademarks are used under license from Epic Games.


#include "Interaction/Strategies/CursorTraceStrategy.h"

#include "HighlightActor.h"

bool UCursorTraceStrategy::PerformTrace_Implementation(APlayerController* PC, FHitResult& OutHit)
{
	// Convert mouse to world direction
	FVector WorldLoc, WorldDir;
	if (!PC->DeprojectMousePositionToWorld(WorldLoc, WorldDir))
		return false;

	// Trace forward from the cursor
	return PC->GetWorld()->LineTraceSingleByChannel(
		OutHit,
		WorldLoc,
		WorldLoc + WorldDir * 10000.f,
		HIGHLIGHTABLE,
		FCollisionQueryParams(NAME_None, true)
	);
}
