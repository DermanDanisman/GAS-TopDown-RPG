// © 2025 Heathrow (Derman). All rights reserved.This project is the intellectual property of Heathrow (Derman) and is protected by copyright law. Unauthorized reproduction, distribution, or use of this material is strictly prohibited.Unreal Engine and its associated trademarks are used under license from Epic Games.

#pragma once

#include "CoreMinimal.h"
#include "Core/Character/GASCharacterBase.h"
#include "GASCharacter.generated.h"

/**
 * AGASCharacter
 *
 * Primary gameplay character class for the GAS TopDown RPG project.
 * - Inherits from AGASCharacterBase, which provides GAS integration.
 * - Handles movement, possession, and GAS initialization for both server and clients.
 */
UCLASS()
class GAS_TOPDOWN_RPG_API AGASCharacter : public AGASCharacterBase
{
	GENERATED_BODY()

public:
	
	AGASCharacter();

	/** Called when this character is possessed by a new controller (server-side).
	  * Initializes the Ability System with the new controller and state.
	  * @param NewController The controller (player or AI) that now possesses this character.
	  */
	virtual void PossessedBy(AController* NewController) override;

	/** Called when the replicated PlayerState is updated (client-side).
	  * Ensures Ability System is re-initialized for the new PlayerState.
	  */
	virtual void OnRep_PlayerState() override;
	
protected:

	virtual void BeginPlay() override;

	virtual void InitializeAbilityActorInfo() override;
};
