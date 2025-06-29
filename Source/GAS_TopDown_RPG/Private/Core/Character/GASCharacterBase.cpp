// © 2025 Heathrow (Derman). All rights reserved.This project is the intellectual property of Heathrow (Derman) and is protected by copyright law. Unauthorized reproduction, distribution, or use of this material is strictly prohibited.Unreal Engine and its associated trademarks are used under license from Epic Games.


#include "Core/Character/GASCharacterBase.h"

#include "Core/AbilitySystem/GASAbilitySystemComponent.h"
#include "Core/AbilitySystem/GASAttributeSet.h"
#include "Core/Player/GASPlayerController.h"
#include "Core/Player/GASPlayerState.h"
#include "UI/HUD/GASHeadsUpDisplay.h"

AGASCharacterBase::AGASCharacterBase()
{
	// Disable tick by default for performance. Subclasses can enable as needed.
	PrimaryActorTick.bCanEverTick = false;

	// Create the skeletal mesh component for the weapon.
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	// Attach the weapon mesh to the main character mesh at the "WeaponHandSocket" socket.
	WeaponMesh->SetupAttachment(GetMesh(), "WeaponHandSocket");
	// Disable collision on the weapon mesh (can be enabled in subclasses if needed).
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGASCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGASCharacterBase::InitializeAbilityActorInfo()
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
		else
		{
			// --- AI-controlled character setup ---
			// AI characters own their own ASC and AttributeSet.
			if (AbilitySystemComponent)
			{
				// Initialize the ASC's actor info, using this character as both owner and avatar.
				AbilitySystemComponent->InitAbilityActorInfo(this, this);
			}
		}
	}
}

UAbilitySystemComponent* AGASCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AGASCharacterBase::GetAttributeSet() const
{
	return AttributeSet;
}

