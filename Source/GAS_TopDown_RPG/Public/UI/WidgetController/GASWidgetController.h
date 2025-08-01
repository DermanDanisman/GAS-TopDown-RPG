// © 2025 Heathrow (Derman). All rights reserved.This project is the intellectual property of Heathrow (Derman) and is protected by copyright law. Unauthorized reproduction, distribution, or use of this material is strictly prohibited.Unreal Engine and its associated trademarks are used under license from Epic Games.

#pragma once

#include "CoreMinimal.h"
#include "GASWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

/**
 * FWidgetControllerParams
 *
 * Struct for passing references to all systems a GAS widget controller needs.
 * - Marked as BlueprintType for usage in Blueprints, allowing designers to initialize widget controllers.
 * - Contains references (TObjectPtr) to the player's controller, state, ability system, and attribute set.
 */
USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
    GENERATED_BODY()

	/** Default constructor.
     *  All member pointers initialize to nullptr, ensuring a safe state.
     */
    FWidgetControllerParams() = default;

    /**
     * Parameterized constructor for initializing all system references at once.
     * @param InPlayerController         Pointer to the APlayerController owning the widget.
     * @param InPlayerState              Pointer to the APlayerState for persistent data.
     * @param InAbilitySystemComponent   Pointer to the UAbilitySystemComponent for GAS.
     * @param InAttributeSet             Pointer to the UAttributeSet for character attributes.
     */
    explicit FWidgetControllerParams(APlayerController* InPlayerController,
                                    APlayerState* InPlayerState,
                                    UAbilitySystemComponent* InAbilitySystemComponent,
                                    UAttributeSet* InAttributeSet)
        : PlayerController(InPlayerController)
        , PlayerState(InPlayerState)
        , AbilitySystemComponent(InAbilitySystemComponent)
        , AttributeSet(InAttributeSet)
    {}

    /** Pointer to the player's controller. Used for input, HUD, and actor communication.
      * Can be nullptr if not yet set.
      * Marked as EditAnywhere and BlueprintReadWrite for maximum designer flexibility.
      */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetController")
    TObjectPtr<APlayerController> PlayerController = nullptr;

    /** Pointer to the player's state. Holds persistent and replicated player info.
      * Editable and readable in Blueprints.
      */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetController")
    TObjectPtr<APlayerState> PlayerState = nullptr;

    /** Pointer to the Gameplay Ability System component.
      * Provides access to abilities, effects, and GAS interfaces.
      */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetController")
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

    /** Pointer to the Attribute Set for the player/character.
      * Holds all gameplay-modifiable stats (health, mana, etc.).
      */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetController")
    TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};

/**
 * UGASWidgetController
 *
 * Base UObject class for all GAS widget controllers.
 * - Designed to be extended for specific widget types.
 * - Manages references to all core gameplay systems for the UI.
 * - Exposes initialization via a single struct parameter.
 */
UCLASS()
class GAS_TOPDOWN_RPG_API UGASWidgetController : public UObject
{
	GENERATED_BODY()

public:

	/**
	 * Initializes all references at once from a single struct.
	 * Call this after construction and before using the widget controller.
	 * @param InWidgetControllerParams   Struct with all required system references.
	 */
	UFUNCTION(BlueprintCallable, Category = "WidgetController")
	void SetWidgetControllerParams(const FWidgetControllerParams& InWidgetControllerParams);

	/** Called to broadcast the initial values of attributes to the UI. Should be overridden in child classes. */
	virtual void BroadcastInitialValues();

	/** Called to bind attribute change callbacks/delegates to the ability system. Should be overridden in child classes. */
	virtual void BindCalbacksToDependencies();

protected:

	/** Pointer to the player's controller (HUD/UI, input, etc.).
	  * Only accessible to child classes, not externally modifiable in Blueprints.
	  */
	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<APlayerController> PlayerController;

	/** Pointer to the player's state (score, identity, etc.). */
	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<APlayerState> PlayerState;

	/** Pointer to the Ability System Component (GAS). */
	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	/** Pointer to the Attribute Set with all gameplay stats. */
	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<UAttributeSet> AttributeSet;
	
};
