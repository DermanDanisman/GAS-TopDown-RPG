// © 2025 Heathrow (Derman). All rights reserved.This project is the intellectual property of Heathrow (Derman) and is protected by copyright law. Unauthorized reproduction, distribution, or use of this material is strictly prohibited.Unreal Engine and its associated trademarks are used under license from Epic Games.


#include "GameplayAbilities/SequentialStackingDurationGameplayEffect.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemLog.h"
#include "Engine/World.h"
#include "TimerManager.h"


USequentialStackingDurationGameplayEffect::USequentialStackingDurationGameplayEffect()
{
	// Set reasonable defaults
	TransitionDelay = 0.1f;
	bShowQueuedStacksInUI = true;
}

int32 USequentialStackingDurationGameplayEffect::GetQueuedStackCount(const TSubclassOf<UGameplayEffect> EffectClass) const
{
	if (const int32* Count = QueuedStackCounts.Find(EffectClass.GetDefaultObject()))
	{
		return *Count;
	}
	return 0;
}

bool USequentialStackingDurationGameplayEffect::CanGameplayEffectApply(
	const FActiveGameplayEffectsContainer& ActiveGEContainer, const FGameplayEffectSpec& GESpec) const
{
	UE_LOG(LogTemp, Warning, TEXT("SequentialStacking: CanGameplayEffectApply called for %s"), *GetNameSafe(GESpec.Def));
	
	// Only allow if this is a duration-based effect
	if (GESpec.GetDuration() <= 0.0f || GESpec.GetDuration() == UGameplayEffect::INFINITE_DURATION)
	{
		UE_LOG(LogGameplayEffects, Warning, 
			TEXT("SequentialStackingComponent: Effect %s is not duration-based. Sequential stacking only supports duration-based effects."), 
			*GetNameSafe(GESpec.Def));
		return false;
	}

	// Always allow application - we'll handle the queuing logic in OnActiveGameplayEffectAdded
	return true;
}

bool USequentialStackingDurationGameplayEffect::OnActiveGameplayEffectAdded(
	FActiveGameplayEffectsContainer& ActiveGEContainer, FActiveGameplayEffect& ActiveGE) const
{
	UE_LOG(LogTemp, Warning, TEXT("SequentialStacking: OnActiveGameplayEffectAdded called for %s"), *GetNameSafe(ActiveGE.Spec.Def));
	
	UAbilitySystemComponent* ASC = ActiveGEContainer.Owner;
	if (!ASC || !ASC->IsOwnerActorAuthoritative())
	{
		// Only server processes sequential stacking logic
		return true;
	}
	
	// Find existing active effect of the same type
	FActiveGameplayEffect* ExistingEffect = FindExistingSequentialEffect(ActiveGEContainer, ActiveGE.Spec.Def);

	if (ExistingEffect)
	{
		// There's already an active effect - add this to the queue
		TWeakObjectPtr<const UGameplayEffect> EffectKey = ActiveGE.Spec.Def;
		int32& QueuedCount = QueuedStackCounts.FindOrAdd(EffectKey, 0);
		QueuedCount += ActiveGE.Spec.GetStackCount();

		UE_LOG(LogGameplayEffects, Log, 
			TEXT("SequentialStacking: Queued %d stacks of %s. Total queued: %d"), 
			ActiveGE.Spec.GetStackCount(), 
			*GetNameSafe(ActiveGE.Spec.Def), 
			QueuedCount);

		// Update the existing effect's visual stack count for UI purposes
		if (bShowQueuedStacksInUI)
		{
			int32 TotalStacks = ExistingEffect->Spec.GetStackCount() + QueuedCount;
			ExistingEffect->Spec.SetStackCount(TotalStacks);
			ActiveGEContainer.MarkItemDirty(*ExistingEffect);
		}

		// Don't allow this new effect to become active
		return false;
	}
	else
	{
		// This is the first effect of this type - let it activate normally
		// But bind to its removal event to handle the queue
		// IMPORTANT: Cast away const for delegate binding
		ActiveGE.EventSet.OnEffectRemoved.AddUObject(
			const_cast<USequentialStackingDurationGameplayEffect*>(this), 
			&USequentialStackingDurationGameplayEffect::OnCurrentStackExpired
		);

		UE_LOG(LogGameplayEffects, Log, 
			TEXT("SequentialStacking: Activated first stack of %s"), 
			*GetNameSafe(ActiveGE.Spec.Def));

		return true;
	}
}

void USequentialStackingDurationGameplayEffect::OnCurrentStackExpired(const FGameplayEffectRemovalInfo& RemovalInfo)
{
	// Validate the removal info contains valid data
	if (!RemovalInfo.ActiveEffect)
	{
		UE_LOG(LogGameplayEffects, Warning, TEXT("SequentialStacking: OnCurrentStackExpired called with null ActiveEffect"));
		return;
	}
	
	// Get the ASC that owned the expired effect
	UAbilitySystemComponent* ASC = RemovalInfo.ActiveEffect->Handle.GetOwningAbilitySystemComponent();
	if (!ASC || !ASC->IsOwnerActorAuthoritative())
	{
		// Only server processes the queue
		return;
	}

	// Extract effect information from the removal data
	const FActiveGameplayEffect* ExpiredEffect = RemovalInfo.ActiveEffect;
	TWeakObjectPtr<const UGameplayEffect> EffectKey = ExpiredEffect->Spec.Def;
	
	// Check if there are any queued stacks for this effect type
	int32* QueuedCount = QueuedStackCounts.Find(EffectKey);

	if (QueuedCount && *QueuedCount > 0)
	{
		// Process the next stack in the queue
		(*QueuedCount)--; // Decrement queue count

		// Create a new spec based on the expired effect
		// This preserves the original effect configuration but resets timing
		FGameplayEffectSpec NextSpec = ExpiredEffect->Spec;
		NextSpec.SetStackCount(1); // Each sequential stack is applied as a single instance
        
		// Reset dynamic capture data that shouldn't carry over between stacks
		// This ensures each stack captures fresh attribute and tag data
		NextSpec.CapturedSourceTags.GetActorTags().Reset();
		NextSpec.CapturedTargetTags.GetActorTags().Reset();

		UE_LOG(LogGameplayEffects, Log, 
			TEXT("SequentialStacking: Applying next stack of %s. Remaining queued: %d"), 
			*GetNameSafe(ExpiredEffect->Spec.Def), 
			*QueuedCount);

		// Apply the next stack with optional delay for clean transitions
		if (TransitionDelay > 0.0f)
		{
			// Use timer system for delayed application
			// This ensures clean networking and prevents frame timing issues
			FTimerHandle& TimerHandle = PendingStackTimers.FindOrAdd(ExpiredEffect->Handle);
			FTimerDelegate TimerDelegate = FTimerDelegate::CreateUFunction(
				(this), 
				FName("ApplyNextSequentialStack"), 
				ASC, 
				NextSpec
			);

			ASC->GetWorld()->GetTimerManager().SetTimer(
				TimerHandle, 
				TimerDelegate, 
				TransitionDelay, 
				false // Don't loop
			);
		}
		else
		{
			// Apply immediately if no delay is configured
			ApplyNextSequentialStack(ASC, NextSpec);
		}

		// Clean up queue tracking if no more stacks remain
		if (*QueuedCount <= 0)
		{
			QueuedStackCounts.Remove(EffectKey);
		}
	}
	else
	{
		// No more queued stacks - sequential chain is complete
		UE_LOG(LogGameplayEffects, Log, 
			TEXT("SequentialStacking: No more queued stacks for %s. Sequential chain complete."), 
			*GetNameSafe(ExpiredEffect->Spec.Def));
	}

	// Clean up timer tracking for this expired effect
	PendingStackTimers.Remove(ExpiredEffect->Handle);
}

void USequentialStackingDurationGameplayEffect::ApplyNextSequentialStack(
	UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& NextGameplayEffectSpec)
{
	if (AbilitySystemComponent && AbilitySystemComponent->IsOwnerActorAuthoritative())
	{
		// Apply the next stack - this will automatically replicate to clients
		FActiveGameplayEffectHandle NewHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(NextGameplayEffectSpec);
        
		if (NewHandle.IsValid())
		{
			UE_LOG(LogGameplayEffects, Log, 
				TEXT("SequentialStacking: Successfully applied next stack %s"), 
				*NewHandle.ToString());
		}
	}
}

FActiveGameplayEffect* USequentialStackingDurationGameplayEffect::FindExistingSequentialEffect(
	FActiveGameplayEffectsContainer& ActiveGEContainer, const UGameplayEffect* GameplayEffectDef) const
{
	for (auto It = ActiveGEContainer.CreateIterator(); It; ++It)
	{
		FActiveGameplayEffect& Effect = *It;
		if (Effect.Spec.Def == GameplayEffectDef && !Effect.IsPendingRemove)
		{
			return &Effect;
		}
	}
	return nullptr;
}

