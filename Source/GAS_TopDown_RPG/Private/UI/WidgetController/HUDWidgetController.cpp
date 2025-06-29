// © 2025 Heathrow (Derman). All rights reserved.This project is the intellectual property of Heathrow (Derman) and is protected by copyright law. Unauthorized reproduction, distribution, or use of this material is strictly prohibited.Unreal Engine and its associated trademarks are used under license from Epic Games.


#include "UI/WidgetController/HUDWidgetController.h"

#include "Core/AbilitySystem/GASAttributeSet.h"

// No additional implementation yet. Extend as needed for HUD logic.
void UHUDWidgetController::BroadcastInitialValues()
{
	const UGASAttributeSet* GASAttributeSet = CastChecked<UGASAttributeSet>(AttributeSet);
	
	// Broadcast the current values of health, max health, mana, and max mana to the UI.
	OnHealthChanged.Broadcast(GASAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(GASAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(GASAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(GASAttributeSet->GetMaxMana());

}

void UHUDWidgetController::BindCalbacksToDependencies()
{
	const UGASAttributeSet* GASAttributeSet = CastChecked<UGASAttributeSet>(AttributeSet);

	// Bind the GAS attribute change delegates to this controller's methods for real-time updates.
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GASAttributeSet->GetHealthAttribute()).AddUObject(this, &ThisClass::HealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GASAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &ThisClass::MaxHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GASAttributeSet->GetManaAttribute()).AddUObject(this, &ThisClass::ManaChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GASAttributeSet->GetMaxManaAttribute()).AddUObject(this, &ThisClass::MaxManaChanged);
}

void UHUDWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	// When health changes, broadcast the new value to the UI.
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UHUDWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	// When max health changes, broadcast the new value to the UI.
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UHUDWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	// When mana changes, broadcast the new value to the UI.
	OnManaChanged.Broadcast(Data.NewValue);
}

void UHUDWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	// When max mana changes, broadcast the new value to the UI.
	OnMaxManaChanged.Broadcast(Data.NewValue);
}
