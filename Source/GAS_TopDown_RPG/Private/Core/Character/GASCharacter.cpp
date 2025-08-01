// © 2025 Heathrow (Derman). All rights reserved.This project is the intellectual property of Heathrow (Derman) and is protected by copyright law. Unauthorized reproduction, distribution, or use of this material is strictly prohibited.Unreal Engine and its associated trademarks are used under license from Epic Games.


#include "Core/Character/GASCharacter.h"

#include "Core/AbilitySystem/GASAbilitySystemComponent.h"
#include "Core/AbilitySystem/GASAttributeSet.h"
#include "Core/Player/GASPlayerController.h"
#include "Core/Player/GASPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/HUD/GASHeadsUpDisplay.h"

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

void AGASCharacter::InitializeAbilityActorInfo()
{
	if (AController* CharacterController = GetController())
	{
		if (CharacterController->IsPlayerController())
		{
			// --- Player-controlled character setup ---

			// Get the custom PlayerState class (must be AGASPlayerState).
			AGASPlayerState* GASPlayerState = GetPlayerState<AGASPlayerState>();
			if (GASPlayerState)
			{
				// The ASC and AttributeSet live on the PlayerState for player characters.
				AbilitySystemComponent = Cast<UGASAbilitySystemComponent>(GASPlayerState->GetAbilitySystemComponent());
				AttributeSet = Cast<UGASAttributeSet>(GASPlayerState->GetAttributeSet());

				// Initialize the ASC's actor info, specifying the player state as the owner and this character as the avatar.
				if (AbilitySystemComponent)
				{
					AbilitySystemComponent->InitAbilityActorInfo(GASPlayerState, this);
				}
			}

			// In multiplayer, only the locally controlled character on each client will have a valid PlayerController pointer.
			// For other (non-local) characters on that client, PlayerController will be nullptr.
			// This is normal and expected in Unreal multiplayer, so always check for validity before using the pointer.
			AGASPlayerController* GASPlayerController = Cast<AGASPlayerController>(CharacterController);
			if (GASPlayerController)
			{
				// Safe to use GASPlayerController here
				// Get the custom HUD and initialize it with all gameplay references.
				AGASHeadsUpDisplay* GASHUD = Cast<AGASHeadsUpDisplay>(GASPlayerController->GetHUD());
				if (GASHUD)
				{
					GASHUD->InitializeHUD(GASPlayerController, GASPlayerState, AbilitySystemComponent, AttributeSet);
				}
			}
		}
	}
}
