// © 2025 Heathrow (Derman). All rights reserved.This project is the intellectual property of Heathrow (Derman) and is protected by copyright law. Unauthorized reproduction, distribution, or use of this material is strictly prohibited.Unreal Engine and its associated trademarks are used under license from Epic Games.


#include "UI/Widget/GASUserWidget.h"

void UGASUserWidget::SetWidgetController(UObject* InWidgetController)
{
	// Store the new Widget Controller reference.
	WidgetController = InWidgetController;
	// Trigger the Blueprint event for UI initialization or data binding.
	OnWidgetControllerSet();
}
