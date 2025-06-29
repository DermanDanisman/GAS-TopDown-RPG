// © 2025 Heathrow (Derman). All rights reserved.This project is the intellectual property of Heathrow (Derman) and is protected by copyright law. Unauthorized reproduction, distribution, or use of this material is strictly prohibited.Unreal Engine and its associated trademarks are used under license from Epic Games.


#include "Core/Character/GASCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

AGASCharacter::AGASCharacter()
{
	// Orient rotation to movement direction (character faces where it moves).
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// Set the rotation rate for smooth turning.
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	// Lock movement to a plane (X/Y for top-down).
	GetCharacterMovement()->bConstrainToPlane = true;
	// Snap the character to the movement plane at spawn.
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Do not use controller rotation for pitch, yaw, or roll (handled by movement).
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AGASCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Initialize Ability System Actor Info for the server context.
	InitializeAbilityActorInfo();
}

void AGASCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Initialize Ability System Actor Info for the client context.
	InitializeAbilityActorInfo();
}

void AGASCharacter::BeginPlay()
{
	Super::BeginPlay();
	// Additional initialization logic can go here.
}
