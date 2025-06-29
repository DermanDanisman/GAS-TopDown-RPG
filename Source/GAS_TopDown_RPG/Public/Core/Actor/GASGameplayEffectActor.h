// © 2025 Heathrow (Derman). All rights reserved.This project is the intellectual property of Heathrow (Derman) and is protected by copyright law. Unauthorized reproduction, distribution, or use of this material is strictly prohibited.Unreal Engine and its associated trademarks are used under license from Epic Games.

#pragma once

#include "CoreMinimal.h"
#include "GASGameplayEffectActor.generated.h"

class UGameplayEffect;

/**
 * 
 */
UCLASS()
class GAS_TOPDOWN_RPG_API AGASGameplayEffectActor : public AActor
{
	GENERATED_BODY()

public:

	AGASGameplayEffectActor();

protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Gameplay Effect Actor")
	void ApplyEffectToTarget(AActor* InTarget, TSubclassOf<UGameplayEffect> InGameplayEffectClass);

	UPROPERTY(EditAnywhere, Category = "Gameplay Effects | Instant")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
	
};
