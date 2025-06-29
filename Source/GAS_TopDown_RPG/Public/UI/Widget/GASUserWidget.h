// © 2025 Heathrow (Derman). All rights reserved.This project is the intellectual property of Heathrow (Derman) and is protected by copyright law. Unauthorized reproduction, distribution, or use of this material is strictly prohibited.Unreal Engine and its associated trademarks are used under license from Epic Games.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GASUserWidget.generated.h"

/**
 * UGASUserWidget
 *
 * Base class for all GAS TopDown RPG user widgets that interact with a Widget Controller.
 * 
 * Purpose:
 * - Provides a standard interface for associating a Widget Controller object with UI widgets.
 * - Enables easy communication between Blueprint widgets and C++/Blueprint Widget Controller logic.
 * - Allows widgets to be dynamically updated in response to gameplay data or events handled by the controller.
 *
 * Usage:
 * - Call SetWidgetController from code or Blueprints to assign the controller.
 * - Implement OnWidgetControllerSet in your Blueprint to initialize bindings or update the UI when the controller is set.
 */
UCLASS()
class GAS_TOPDOWN_RPG_API UGASUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/** Returns the associated Widget Controller object (may be nullptr if not set). */
	UFUNCTION(BlueprintPure, Category = "GAS User Widget | Widget Controller")
	UObject* GetWidgetController() { return WidgetController; }

	/** Sets the Widget Controller object and triggers the Blueprint event for initialization. */
	UFUNCTION(BlueprintCallable, Category = "GAS User Widget | Widget Controller")
	void SetWidgetController(UObject* InWidgetController);

protected:

	/** Blueprint event called after the Widget Controller is set.
	  * Implement this in BP to bind delegates, update UI, etc.
	  */
	UFUNCTION(BlueprintImplementableEvent, Category = "GAS User Widget | Widget Controller", meta = (DisplayName = "On Widget Controller Set"))
	void OnWidgetControllerSet();
	
private:
	
	/** The object that serves as the Widget Controller for this widget.
	  * Typically a custom UWidgetController-derived object.
	  */
	UPROPERTY()
	TObjectPtr<UObject> WidgetController;
};
