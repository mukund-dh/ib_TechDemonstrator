// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "iBTDInteractiveActorBase.generated.h"

class UAIPerceptionStimuliSourceComponent;

UCLASS()
class IB_TECHDEMONSTRATOR_API AiBTDInteractiveActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AiBTDInteractiveActorBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Functionality to be overridden in Blueprints on a per derived actor basis */
	UFUNCTION(BlueprintImplementableEvent, Category = "Interact")
	void OnActorInteract(APawn* CallingPawn);

	UFUNCTION(Server, WithValidation, Reliable)
	void CallInteractOnServer(APawn* CallingPawn);

	UFUNCTION(BlueprintCallable, Category = "Interact")
	void Interact(APawn* CallingPawn);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Perception", meta = (AllowPrivateAccess = "true"))
	UAIPerceptionStimuliSourceComponent* PerceptionStimuliSource;

};
