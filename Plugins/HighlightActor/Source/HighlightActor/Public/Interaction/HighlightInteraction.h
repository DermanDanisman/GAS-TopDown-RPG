// © 2025 Heathrow (Derman). All rights reserved.This project is the intellectual property of Heathrow (Derman) and is protected by copyright law. Unauthorized reproduction, distribution, or use of this material is strictly prohibited.Unreal Engine and its associated trademarks are used under license from Epic Games.

#pragma once

#include "CoreMinimal.h"
#include "HighlightTypes.h"
#include "Components/ActorComponent.h"
#include "HighlightInteraction.generated.h"


class UHighlightTraceStrategy;
class IHighlightInterface;
class IHighlightTraceStrategy;

/**
 * ActorComponent that drives the highlight system each tick.
 */

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HIGHLIGHTACTOR_API UHighlightInteraction : public UActorComponent
{
	GENERATED_BODY()

public:
	
	/**
	 * Matches Unreal’s expected constructor signature for components.
	 */
	UHighlightInteraction(const FObjectInitializer& ObjectInitializer);
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * Select which trace strategy to use.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Highlight")
	EHighlightDetectionMode HighlightMode = EHighlightDetectionMode::MouseUnderCursor;

	/**
	* Blueprint hook for custom tracing logic.
	*/
	UFUNCTION(BlueprintImplementableEvent, Category="Highlight")
	bool PerformCustomTrace(FHitResult& OutHit);

protected:

	virtual void BeginPlay() override;
	
	/** The active trace strategy instance (managed by GC). */
	UPROPERTY()
	UHighlightTraceStrategy* Strategy = nullptr;

	/** The currently highlighted actor (implements IHighlightInterface). */
	UPROPERTY()
	TScriptInterface<IHighlightInterface> HighlightedActor;

	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController;

private:
	/** Instantiates the correct strategy based on HighlightMode. */
	void InitializeStrategy();

	/** Executes the trace and invokes highlight/unhighlight callbacks. */
	void PerformHighlight();
	
};
