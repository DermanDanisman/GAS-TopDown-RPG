// © 2025 Heathrow (Derman). All rights reserved.This project is the intellectual property of Heathrow (Derman) and is protected by copyright law. Unauthorized reproduction, distribution, or use of this material is strictly prohibited.Unreal Engine and its associated trademarks are used under license from Epic Games.


#include "Core/Player/GASPlayerState.h"

#include "AbilitySystemComponent.h"
#include "Core/AbilitySystem/GASAbilitySystemComponent.h"
#include "Core/AbilitySystem/GASAttributeSet.h"

AGASPlayerState::AGASPlayerState()
{
	// Increase net update frequency for responsive attribute and state replication.
	SetNetUpdateFrequency(100.f);

	// Create the GAS AbilitySystemComponent as a default subobject.
	// PlayerState is the authoritative owner for player-controlled characters.
	AbilitySystemComponent = CreateDefaultSubobject<UGASAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true); // Enable replication for multiplayer.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// Create the GAS AttributeSet as a default subobject.
	AttributeSet = CreateDefaultSubobject<UGASAttributeSet>("AttributeSet");
}

void AGASPlayerState::BeginPlay()
{
	Super::BeginPlay();

}

UAbilitySystemComponent* AGASPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AGASPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

