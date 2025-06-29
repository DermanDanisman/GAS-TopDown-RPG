// © 2025 Heathrow (Derman). All rights reserved.This project is the intellectual property of Heathrow (Derman) and is protected by copyright law. Unauthorized reproduction, distribution, or use of this material is strictly prohibited.Unreal Engine and its associated trademarks are used under license from Epic Games.


#include "Interaction/HighlightInteraction.h"

#include "Interaction/HighlightInterface.h"
#include "Interaction/Strategies/CrosshairTraceStrategy.h"
#include "Interaction/Strategies/CursorTraceStrategy.h"
#include "Interaction/Strategies/TouchTraceStrategy.h"
#include "Interaction/Strategies/VRControllerTraceStrategy.h"
#include "Kismet/GameplayStatics.h"

UHighlightInteraction::UHighlightInteraction(const FObjectInitializer& ObjInit)
    : Super(ObjInit)
{
    // Enable ticking every frame
    PrimaryComponentTick.bCanEverTick = true;
}

void UHighlightInteraction::BeginPlay()
{
    Super::BeginPlay();
    
    PlayerController = Cast<APlayerController>(GetOwner());   // or GetWorld()->GetFirstPlayerController()
    if (!PlayerController)
    {
        PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    }
    // Set up the initial strategy instance
    InitializeStrategy();
}

void UHighlightInteraction::InitializeStrategy()
{
    // Clean up any existing strategy
    if (Strategy)
    {
        Strategy->ConditionalBeginDestroy();
        Strategy = nullptr;
    }

    // Choose and instantiate the new strategy
    switch (HighlightMode)
    {
    case EHighlightDetectionMode::MouseUnderCursor:
        Strategy = NewObject<UCursorTraceStrategy>(this);
        break;
    case EHighlightDetectionMode::CrosshairCenter:
        Strategy = NewObject<UCrosshairTraceStrategy>(this);
        break;
    case EHighlightDetectionMode::TouchInput:
        Strategy = NewObject<UTouchTraceStrategy>(this);
        break;
    case EHighlightDetectionMode::VRPointer:
        Strategy = NewObject<UVRControllerTraceStrategy>(this);
        break;
    case EHighlightDetectionMode::Custom:
    default:
        Strategy = nullptr;
        break;
    }
}

void UHighlightInteraction::TickComponent(
    float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    // Run the highlight logic each frame
    PerformHighlight();
}

void UHighlightInteraction::PerformHighlight()
{
    FHitResult Hit;
    bool bHit = false;

    // Use strategy or custom Blueprint logic
    if (Strategy)
    {
        if (PlayerController)
        {
            bHit = Strategy->PerformTrace(PlayerController, Hit);
        }
    }
    else if (HighlightMode == EHighlightDetectionMode::Custom)
    {
        bHit = PerformCustomTrace(Hit);
    }

    // Determine new actor under highlight
    AActor* NewActor = bHit ? Hit.GetActor() : nullptr;
    if (NewActor != HighlightedActor)
    {
        // Unhighlight previous
        if (HighlightedActor)
        {
            HighlightedActor->UnHighlightActor();
        }
        HighlightedActor = nullptr;

        // Highlight new
        if (NewActor && NewActor->GetClass()->ImplementsInterface(UHighlightInterface::StaticClass()))
        {
            HighlightedActor = NewActor;
            HighlightedActor->HighlightActor();
        }
    }
}
