// © 2025 Heathrow (Derman). All rights reserved.This project is the intellectual property of Heathrow (Derman) and is protected by copyright law. Unauthorized reproduction, distribution, or use of this material is strictly prohibited.Unreal Engine and its associated trademarks are used under license from Epic Games.


#include "Core/Character/GASEnemy.h"

#include "HighlightActor.h"
#include "Core/AbilitySystem/GASAbilitySystemComponent.h"
#include "Core/AbilitySystem/GASAttributeSet.h"
#include "GAS_TopDown_RPG/GAS_TopDown_RPG.h"

AGASEnemy::AGASEnemy()
{
	// Set mesh collision to respond to the custom highlight channel.
	GetMesh()->SetCollisionResponseToChannel(HIGHLIGHTABLE, ECR_Block);

	// Create the Ability System Component for the AI enemy.
	// Unlike player characters, AI own their own ASC and AttributeSet.
	AbilitySystemComponent = CreateDefaultSubobject<UGASAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true); // Enable replication for multiplayer.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	// Create the Attribute Set for this AI character.
	AttributeSet = CreateDefaultSubobject<UGASAttributeSet>("AttributeSet");
}

void AGASEnemy::HighlightActor()
{
	// Mark as highlighted for internal logic or UI.
	bHighlighted = true;

	// Enable custom depth rendering for outline/FX.
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

	// Also highlight the weapon mesh.
	WeaponMesh->SetRenderCustomDepth(true);
	WeaponMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AGASEnemy::UnHighlightActor()
{
	// Unmark as highlighted.
	bHighlighted = false;

	// Disable custom depth rendering.
	GetMesh()->SetRenderCustomDepth(false);
	WeaponMesh->SetRenderCustomDepth(false);
}

void AGASEnemy::BeginPlay()
{
	Super::BeginPlay();

	// Initialize the Ability System with this actor as both owner and avatar.
	// For AI, this is done here since AI owns its own ASC/AttributeSet.
	InitializeAbilityActorInfo();
}

void AGASEnemy::InitializeAbilityActorInfo()
{
	// --- AI-controlled character setup ---
	// AI characters own their own ASC and AttributeSet.
	if (AbilitySystemComponent)
	{
		// Initialize the ASC's actor info, using this character as both owner and avatar.
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
	
}
