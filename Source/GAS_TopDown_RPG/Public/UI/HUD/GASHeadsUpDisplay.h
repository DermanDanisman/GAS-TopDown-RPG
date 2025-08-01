// © 2025 Heathrow (Derman). All rights reserved.This project is the intellectual property of Heathrow (Derman) and is protected by copyright law. Unauthorized reproduction, distribution, or use of this material is strictly prohibited.Unreal Engine and its associated trademarks are used under license from Epic Games.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameFramework/HUD.h"
#include "UI/WidgetController/GASWidgetController.h"
#include "GASHeadsUpDisplay.generated.h"

// Forward declare widget/controller classes to minimize dependencies.
class UHUDWidgetController;
class UGASUserWidget;

/**
 * AGASHeadsUpDisplay
 *
 * Custom HUD class for GAS-driven games.
 * Responsible for widget/controller creation and initialization.
 */
UCLASS()
class GAS_TOPDOWN_RPG_API AGASHeadsUpDisplay : public AHUD
{
    GENERATED_BODY()

public:

    /** Pointer to the on-screen HUD widget instance (your custom user widget). */
    UPROPERTY()
    TObjectPtr<UGASUserWidget> HUDWidget;

    /**
     * Returns a pointer to the HUD Widget Controller, creating and initializing it if none exists.
     * @param InWidgetControllerParams   The struct of references required by the widget controller.
     * @return                          A valid pointer to the widget controller for HUD widgets.
     */
    UHUDWidgetController* GetHUDWidgetController(const FWidgetControllerParams& InWidgetControllerParams);

    /**
     * Initializes the HUD widget and its controller, and adds the widget to the viewport.
     * @param InPlayerController         Pointer to the player's controller.
     * @param InPlayerState              Pointer to the player's state.
     * @param InAbilitySystemComponent   Pointer to the player's GAS component.
     * @param InAttributeSet             Pointer to the player's attribute set.
     */
    void InitializeHUD(APlayerController* InPlayerController,
        APlayerState* InPlayerState,
        UAbilitySystemComponent* InAbilitySystemComponent,
        UAttributeSet* InAttributeSet);

private:

    /** The class used to spawn the HUD widget at runtime.
      * Settable in the editor or Blueprint.
      */
    UPROPERTY(EditAnywhere)
    TSubclassOf<UUserWidget> HUDWidgetClass;

    /** The instance of the HUD widget controller.
      * Created at runtime, owned by this HUD.
      */
    UPROPERTY()
    TObjectPtr<UHUDWidgetController> HUDWidgetController;

    /** The class used to instantiate the HUD widget controller.
      * Settable in the editor or Blueprint.
      */
    UPROPERTY(EditAnywhere)
    TSubclassOf<UHUDWidgetController> HUDWidgetControllerClass;
};