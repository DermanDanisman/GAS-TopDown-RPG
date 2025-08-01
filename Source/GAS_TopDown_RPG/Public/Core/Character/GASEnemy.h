// © 2025 Heathrow (Derman). All rights reserved.This project is the intellectual property of Heathrow (Derman) and is protected by copyright law. Unauthorized reproduction, distribution, or use of this material is strictly prohibited.Unreal Engine and its associated trademarks are used under license from Epic Games.

#pragma once

#include "CoreMinimal.h"
#include "Core/Character/GASCharacterBase.h"
#include "Interaction/HighlightInterface.h"
#include "GASEnemy.generated.h"

/**
 * AGASEnemy
 *
 * Enemy character class with GAS (Gameplay Ability System) and highlight interaction.
 * Inherits from AGASCharacterBase (provides Ability System, Attribute Set) and supports IHighlightInterface.
 */
UCLASS()
class GAS_TOPDOWN_RPG_API AGASEnemy : public AGASCharacterBase, public IHighlightInterface
{
	GENERATED_BODY()

public:

	AGASEnemy();

	/** Sets the actor as highlighted (visual feedback, e.g. for mouseover). */
	virtual void HighlightActor() override;

	/** Clears the highlight state from the actor. */
	virtual void UnHighlightActor() override;

	/** Whether this enemy is currently highlighted (visible to Blueprints/UI). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interactable)
	bool bHighlighted;

protected:

	virtual void BeginPlay() override;

	virtual void InitializeAbilityActorInfo() override;
	
};
