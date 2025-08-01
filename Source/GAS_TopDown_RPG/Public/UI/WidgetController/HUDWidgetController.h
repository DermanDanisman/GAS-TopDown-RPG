// © 2025 Heathrow (Derman). All rights reserved.This project is the intellectual property of Heathrow (Derman) and is protected by copyright law. Unauthorized reproduction, distribution, or use of this material is strictly prohibited.Unreal Engine and its associated trademarks are used under license from Epic Games.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/GASWidgetController.h"
#include "HUDWidgetController.generated.h"

struct FOnAttributeChangeData;

// Declare multicast delegates for different HUD attribute changes
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangedSignature, float, NewMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangedSignature, float, NewMaxMana);

/**
 * UHUDWidgetController
 *
 * Extension of UGASWidgetController for HUD-related widgets.
 * Add HUD-specific logic and properties in this class.
 */
UCLASS(BlueprintType, Blueprintable)
class GAS_TOPDOWN_RPG_API UHUDWidgetController : public UGASWidgetController
{
	GENERATED_BODY()

public:

	/** Broadcasts the initial attribute values (health, mana, etc.) to the UI using delegates. */
	virtual void BroadcastInitialValues() override;

	/** Binds attribute change delegates from GAS (e.g. health/mana change) to handler functions. */
	virtual void BindCalbacksToDependencies() override;

	/** Delegate for listening to health value changes. */
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnHealthChangedSignature OnHealthChanged;

	/** Delegate for listening to max health changes. */
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnMaxHealthChangedSignature OnMaxHealthChanged;

	/** Delegate for listening to mana value changes. */
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnManaChangedSignature OnManaChanged;

	/** Delegate for listening to max mana changes. */
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnMaxManaChangedSignature OnMaxManaChanged;

protected:

	/** Handler for health attribute changes. Broadcasts new value to UI. */
	void HealthChanged(const FOnAttributeChangeData& Data) const;

	/** Handler for max health attribute changes. Broadcasts new value to UI. */
	void MaxHealthChanged(const FOnAttributeChangeData& Data) const;

	/** Handler for mana attribute changes. Broadcasts new value to UI. */
	void ManaChanged(const FOnAttributeChangeData& Data) const;

	/** Handler for max mana attribute changes. Broadcasts new value to UI. */
	void MaxManaChanged(const FOnAttributeChangeData& Data) const;
};
