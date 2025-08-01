// © 2025 Heathrow (Derman). All rights reserved.
// This project is the intellectual property of Heathrow (Derman) and is protected by copyright law.
// Unauthorized reproduction, distribution, or use of this material is strictly prohibited.
// Unreal Engine and its associated trademarks are used under license from Epic Games.

#pragma once

#include "CoreMinimal.h"
#include "GASGameplayEffectActor.generated.h"

class UGameplayEffect;

/**
 * AGASGameplayEffectActor
 * 
 * An actor designed to apply Gameplay Effects (GAS) to target actors in the world.
 * Typically used for things like pickups (health potions, buffs, etc.) or environmental triggers.
 */
UCLASS()
class GAS_TOPDOWN_RPG_API AGASGameplayEffectActor : public AActor
{
	GENERATED_BODY()

public:

	AGASGameplayEffectActor();

protected:

	virtual void BeginPlay() override;

	/**
	 * Applies the specified Gameplay Effect class to the provided target actor.
	 * @param InTargetActor - The actor to receive the effect (must have an AbilitySystemComponent).
	 * @param InGameplayEffectClass - The Gameplay Effect class to apply (should be a subclass of UGameplayEffect).
	 */
	UFUNCTION(BlueprintCallable, Category = "Gameplay Effect Actor")
	void ApplyEffectToTargetActor(AActor* InTargetActor, TSubclassOf<UGameplayEffect> InGameplayEffectClass) const;

	/** The default instant Gameplay Effect class to apply (configurable in editor or Blueprint). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Effects | Instant")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	/** The default instant Gameplay Effect class to apply (configurable in editor or Blueprint). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Effects | Duration")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;
	
};
