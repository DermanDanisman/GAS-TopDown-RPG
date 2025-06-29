// © 2025 Heathrow (Derman). All rights reserved.This project is the intellectual property of Heathrow (Derman) and is protected by copyright law. Unauthorized reproduction, distribution, or use of this material is strictly prohibited.Unreal Engine and its associated trademarks are used under license from Epic Games.


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

void AGASGameplayEffectActor::ApplyEffectToTarget(AActor* InTarget, const TSubclassOf<UGameplayEffect> InGameplayEffectClass)
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InTarget);
	if (TargetAbilitySystemComponent == nullptr) return;
	checkf(InGameplayEffectClass, TEXT("Gameplay Effect Class IS NOT VALID!"));
	
	FGameplayEffectContextHandle GameplayEffectContextHandle = TargetAbilitySystemComponent->MakeEffectContext();
	GameplayEffectContextHandle.AddSourceObject(this);
	
	FGameplayEffectSpecHandle GameplayEffectSpecHandle = TargetAbilitySystemComponent->MakeOutgoingSpec(
		InGameplayEffectClass, 1.f, GameplayEffectContextHandle);

	FGameplayEffectSpec GameplayEffectSpec = *GameplayEffectSpecHandle.Data;
	TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(GameplayEffectSpec);
}
