// © 2025 Heathrow (Derman). All rights reserved.This project is the intellectual property of Heathrow (Derman) and is protected by copyright law. Unauthorized reproduction, distribution, or use of this material is strictly prohibited.Unreal Engine and its associated trademarks are used under license from Epic Games.


#include "Interaction/Strategies/VRControllerTraceStrategy.h"

#include "HighlightActor.h"
#include "MotionControllerComponent.h"

bool UVRControllerTraceStrategy::PerformTrace_Implementation(APlayerController* PC, FHitResult& OutHit)
{
	// Find the pawn's motion controller
	APawn* Pawn = PC->GetPawn();
	if (!Pawn) return false;
	UMotionControllerComponent* MC = Pawn->FindComponentByClass<UMotionControllerComponent>();
	if (!MC) return false;

	// Trace from controller forward
	FVector Start = MC->GetComponentLocation();
	FVector End   = Start + MC->GetForwardVector() * 10000.f;
	return PC->GetWorld()->LineTraceSingleByChannel(
		OutHit,
		Start,
		End,
		HIGHLIGHTABLE,
		FCollisionQueryParams(NAME_None, true)
	);
}
