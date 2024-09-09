// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Perception/AIPerceptionTypes.h"
#include "iBTDPlayerController.generated.h"

class UAIPerceptionComponent;

/**
 * Base class for the Player Controller
 */
UCLASS()
class IB_TECHDEMONSTRATOR_API AiBTDPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	AiBTDPlayerController(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Perception")
	void PerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

private:

	/** Perception Component to sense other actors around us */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Perception", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> PerceptionComponent;
};
