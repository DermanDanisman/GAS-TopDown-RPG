// © 2025 Heathrow (Derman). All rights reserved.This project is the intellectual property of Heathrow (Derman) and is protected by copyright law. Unauthorized reproduction, distribution, or use of this material is strictly prohibited.Unreal Engine and its associated trademarks are used under license from Epic Games.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayEffectComponent.h"
#include "SequentialStackingDurationGameplayEffect.generated.h"

class UAbilitySystemComponent;
struct FActiveGameplayEffectHandle;

/**
 * Component that enables sequential stacking for duration-based GameplayEffects.
 * 
 * Sequential stacking means that when multiple instances of the same effect are applied,
 * they don't stack simultaneously but instead form a queue where each effect waits
 * for the previous one to complete before starting.
 * 
 * Example Timeline:
 * T=0s:  Apply Bleed Stack 1 → ACTIVE (-5 HP/sec for 5s)
 * T=1s:  Apply Bleed Stack 2 → QUEUED  
 * T=3s:  Apply Bleed Stack 3 → QUEUED
 * T=5s:  Stack 1 expires → Stack 2 becomes ACTIVE (-5 HP/sec for 5s)
 * T=10s: Stack 2 expires → Stack 3 becomes ACTIVE (-5 HP/sec for 5s)
 * T=15s: Stack 3 expires → All effects complete
 * 
 * Requirements:
 * - Only works with duration-based effects (not instant or infinite)
 * - Must be added to a GameplayEffect that has DurationPolicy = HasDuration
 * - Server-authoritative (queuing logic only runs on server)
 */
UCLASS(BlueprintType, meta=(DisplayName="Sequential Stacking (Duration-Based)"))
class GASCORE_API USequentialStackingDurationGameplayEffect : public UGameplayEffectComponent
{
	GENERATED_BODY()

public:
	USequentialStackingDurationGameplayEffect();

	/**
	 * Small delay between stack transitions to ensure clean networking.
	 * Prevents potential frame timing issues when one effect expires and the next begins.
	 * Range: 0.0 to 1.0 seconds
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequential Stacking", 
		meta = (ClampMin = "0.0", ClampMax = "1.0", ToolTip = "Delay in seconds between effect transitions"))
	float TransitionDelay = 0.1f;

	/**
	 * Whether to show the total number of stacks (active + queued) in UI elements.
	 * When true: UI shows "Poison x3" (1 active + 2 queued)
	 * When false: UI shows "Poison x1" (only active effect)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sequential Stacking",
		meta = (ToolTip = "Show total stacks including queued ones in UI"))
	bool bShowQueuedStacksInUI = true;

	UFUNCTION(BlueprintCallable, Category = "Sequential Stacking", CallInEditor)
	int32 GetQueuedStackCount(TSubclassOf<UGameplayEffect> EffectClass) const;

	/**
	 * Validates that the effect can use sequential stacking.
	 * Only duration-based effects are supported.
	 * 
	 * @param ActiveGEContainer - The container holding active effects
	 * @param GESpec - The gameplay effect spec being validated
	 * @return true if the effect can be applied with sequential stacking
	 */
	virtual bool CanGameplayEffectApply(const FActiveGameplayEffectsContainer& ActiveGEContainer, const FGameplayEffectSpec& GESpec) const override;
	
	/**
	 * Main logic for handling sequential stacking when a new effect is applied.
	 * Determines whether to queue the effect or let it become active immediately.
	 * 
	 * @param ActiveGEContainer - The container holding active effects
	 * @param ActiveGE - The new active gameplay effect being added
	 * @return true to allow the effect to become active, false to queue it
	 */
	virtual bool OnActiveGameplayEffectAdded(FActiveGameplayEffectsContainer& ActiveGEContainer, FActiveGameplayEffect& ActiveGE) const override;

protected:
	/**
	 * Callback triggered when the current active stack expires naturally or is removed.
	 * Handles applying the next queued stack if any exist.
	 * 
	 * @param RemovalInfo - Information about the effect that was removed
	 */
	UFUNCTION()
	void OnCurrentStackExpired(const FGameplayEffectRemovalInfo& RemovalInfo);

	/**
	 * Applies the next sequential stack after the transition delay.
	 * This function is called by the timer system to ensure proper timing.
	 * 
	 * @param AbilitySystemComponent - The ASC to apply the effect to
	 * @param NextGameplayEffectSpec - The spec for the next effect to apply
	 */
	UFUNCTION()
	void ApplyNextSequentialStack(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& NextGameplayEffectSpec);

	/**
	 * Searches for an existing active effect of the same type.
	 * Used to determine if a new application should be queued or applied immediately.
	 * 
	 * @param ActiveGEContainer - The container to search in
	 * @param GameplayEffectDef - The effect definition to look for
	 * @return Pointer to existing effect, or nullptr if none found
	 */
	FActiveGameplayEffect* FindExistingSequentialEffect(FActiveGameplayEffectsContainer& ActiveGEContainer, const UGameplayEffect* GameplayEffectDef) const;
	
private:
	/**
	 * Tracks the number of queued stacks for each effect type.
	 * Key: WeakPtr to the GameplayEffect class
	 * Value: Number of stacks waiting in queue
	 * 
	 * Marked mutable because it needs to be modified from const member functions
	 * (base class OnActiveGameplayEffectAdded is const)
	 */
	UPROPERTY()
	mutable TMap<TWeakObjectPtr<const UGameplayEffect>, int32> QueuedStackCounts;

	/**
	 * Tracks timer handles for delayed stack applications.
	 * Key: Handle of the effect that expired
	 * Value: Timer handle for the delayed application
	 * 
	 * Used to manage the TransitionDelay between effect transitions
	 */
	UPROPERTY()
	mutable TMap<FActiveGameplayEffectHandle, FTimerHandle> PendingStackTimers;
};