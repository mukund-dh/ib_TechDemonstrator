// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactibles/iBTDInteractiveActorBase.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

// Sets default values
AiBTDInteractiveActorBase::AiBTDInteractiveActorBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to not call Tick() every frame.  
	// We don't need this, unless one of our subclasses explicitly needs it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	PerceptionStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Perception Stimuli Source Component"));
}

bool AiBTDInteractiveActorBase::CallInteractOnServer_Validate(APawn* CallingPawn)
{
	return true;
}

void AiBTDInteractiveActorBase::CallInteractOnServer_Implementation(APawn* CallingPawn)
{
	Interact(CallingPawn);
}

void AiBTDInteractiveActorBase::Interact(APawn* CallingPawn)
{
	if (!CallingPawn->HasAuthority())
	{
		// If the calling pawn isn't authoratative, call the function 
		// on the server from the authoratative pawn
		CallInteractOnServer(CallingPawn);
		return;
	}

	OnActorInteract(CallingPawn);
}

// Called when the game starts or when spawned
void AiBTDInteractiveActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AiBTDInteractiveActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

