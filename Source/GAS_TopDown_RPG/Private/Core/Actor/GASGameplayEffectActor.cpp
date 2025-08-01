// © 2025 Heathrow (Derman). All rights reserved.
// This project is the intellectual property of Heathrow (Derman) and is protected by copyright law.
// Unauthorized reproduction, distribution, or use of this material is strictly prohibited.
// Unreal Engine and its associated trademarks are used under license from Epic Games.


#include "Core/Actor/GASGameplayEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


AGASGameplayEffectActor::AGASGameplayEffectActor()
{
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootComponent")));
}

void AGASGameplayEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AGASGameplayEffectActor::ApplyEffectToTargetActor(AActor* InTargetActor, const TSubclassOf<UGameplayEffect> InGameplayEffectClass) const
{
	// Try to get the target's AbilitySystemComponent (required for applying effects).
	UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InTargetActor);
	if (TargetAbilitySystemComponent == nullptr) return;

	// Validate that the effect class is provided.
	checkf(InGameplayEffectClass, TEXT("Gameplay Effect Class IS NOT VALID!"));
	
	// Create a context for the effect, marking this actor as the source.
	FGameplayEffectContextHandle GameplayEffectContextHandle = TargetAbilitySystemComponent->MakeEffectContext();
	GameplayEffectContextHandle.AddSourceObject(this);
	
	// Create a specification for the outgoing effect (level 1 by default).
	const FGameplayEffectSpecHandle GameplayEffectSpecHandle = TargetAbilitySystemComponent->MakeOutgoingSpec(
		InGameplayEffectClass, 1.f, GameplayEffectContextHandle);

	// Apply the effect spec to the target itself.
	const FGameplayEffectSpec GameplayEffectSpec = *GameplayEffectSpecHandle.Data;
	TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(GameplayEffectSpec);
}
