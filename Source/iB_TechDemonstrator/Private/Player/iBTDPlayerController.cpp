// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/iBTDPlayerController.h"
#include "Perception/AIPerceptionComponent.h"

AiBTDPlayerController::AiBTDPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("Perception Component");
}

void AiBTDPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AiBTDPlayerController::PerceptionUpdated);
}
