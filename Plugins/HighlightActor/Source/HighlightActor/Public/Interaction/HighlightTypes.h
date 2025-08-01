// © 2025 Heathrow (Derman). All rights reserved.This project is the intellectual property of Heathrow (Derman) and is protected by copyright law. Unauthorized reproduction, distribution, or use of this material is strictly prohibited.Unreal Engine and its associated trademarks are used under license from Epic Games.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EHighlightDetectionMode : uint8
{
	MouseUnderCursor    UMETA(DisplayName="Mouse Cursor"),
	CrosshairCenter     UMETA(DisplayName="Center-Screen Crosshair"),
	TouchInput          UMETA(DisplayName="Touch Input"),
	VRPointer           UMETA(DisplayName="VR Controller Pointer"),
	Custom              UMETA(DisplayName="Custom (BP Override)")
};


/**
 * 
 */
class HIGHLIGHTACTOR_API HighlightTypes
{
};
