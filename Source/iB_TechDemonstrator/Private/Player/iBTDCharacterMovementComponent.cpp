// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/iBTDCharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"


void UiBTDCharacterMovementComponent::SetClimbingMode(bool bClimbing)
{
	ClimbingLadder = bClimbing;
}

bool UiBTDCharacterMovementComponent::BeginClimbing(bool bClimbing)
{
	if (ClimbingLadder)
	{
		WantsToClimbLadder = 1;
		SetMovementMode(EMovementMode::MOVE_Custom, ECustomMovementMode::CMOVE_ClimbLadder);
		return true;
	}

	return false;
}

void UiBTDCharacterMovementComponent::EndClimbing()
{
	WantsToClimbLadder = false;
	ClimbingLadder = false;
	SetLadderClimbDirection(FVector::ZeroVector);
	SetMovementMode(EMovementMode::MOVE_Walking);
}

bool UiBTDCharacterMovementComponent::SetLadderClimbDirection_Validate(const FVector& NewDirection)
{
	return true;
}

void UiBTDCharacterMovementComponent::SetLadderClimbDirection_Implementation(const FVector& NewDirection)
{
	LadderClimbDirection = NewDirection;
	if (NewDirection.Equals(FVector(0.0f, 0.0f, -1.0f)))
	{
		LadderMovementType = ELadderMovementType::ClimbingDown;
	}
	else if (NewDirection.Equals(FVector(0.0f, 0.0f, 1.0f)))
	{
		LadderMovementType = ELadderMovementType::ClimbingUp;
	}
	else
	{
		LadderMovementType = ELadderMovementType::StayingStill;
	}
}

bool UiBTDCharacterMovementComponent::IsCustomMovementMode(uint8 custom_movement_mode) const
{
	return (MovementMode == EMovementMode::MOVE_Custom && CustomMovementMode == custom_movement_mode);
}

void UiBTDCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UiBTDCharacterMovementComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);

}

void UiBTDCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UiBTDCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	/*  There are 4 custom move flags for us to use. Below is what each is currently being used for:
		FLAG_Custom_0		= 0x10, // ClimbingLadder
		FLAG_Custom_1		= 0x20, // Unused
		FLAG_Custom_2		= 0x40, // Unused
		FLAG_Custom_3		= 0x80, // Unused
	*/

	WantsToClimbLadder = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}

void UiBTDCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	if (MovementMode == EMovementMode::MOVE_Custom)
	{
		switch (CustomMovementMode)
		{
			case ECustomMovementMode::CMOVE_ClimbLadder:
			{
				UE_LOG(LogTemp, Log, TEXT("Climbing Ladder."));
			}
			break;
		}
	}

	if (PreviousMovementMode == EMovementMode::MOVE_Custom)
	{
		switch (PreviousMovementMode)
		{
			case ECustomMovementMode::CMOVE_ClimbLadder:
			{
				UE_LOG(LogTemp, Log, TEXT("Exiting Climbing Ladder."));
			}
			break;
		}
	}

	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
}

void UiBTDCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	// DO NOT RUN ON SIMULATED PROXY
	if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Local Role: %d"), GetOwner()->GetLocalRole());

	Super::PhysCustom(deltaTime, Iterations);

	switch (CustomMovementMode)
	{
		case ECustomMovementMode::CMOVE_ClimbLadder:
		{
			PhysClimbLadder(deltaTime, Iterations);
			UE_LOG(LogTemp, Log, TEXT("TYRING TO CLIMB THE LADDER!!!!"));
			break;
		}
	}
}

void UiBTDCharacterMovementComponent::PhysClimbLadder(float deltaTime, int32 Iterations)
{
	if (MovementMode != EMovementMode::MOVE_Custom)
	{
		EndClimbing();
		return;
	}

	// Set the owning player's new velocity
	Velocity = LadderClimbDirection * ClimbUpSpeed;
	const FVector Adjusted = Velocity * deltaTime;
	FHitResult Hit(1.f);
	SafeMoveUpdatedComponent(Adjusted, UpdatedComponent->GetComponentQuat(), true, Hit);
	UE_LOG(LogTemp, Log, TEXT("%f %f %f"), Adjusted.X, Adjusted.Y, Adjusted.Z);
}

void UiBTDCharacterMovementComponent::ProcessLanded(const FHitResult& Hit, float remainingTime, int32 Iterations)
{
	Super::ProcessLanded(Hit, remainingTime, Iterations);

	if (IsCustomMovementMode(ECustomMovementMode::CMOVE_ClimbLadder))
	{
		EndClimbing();
	}
}

FNetworkPredictionData_Client* UiBTDCharacterMovementComponent::GetPredictionData_Client() const
{
	if (ClientPredictionData == nullptr)
	{
		// Return our custom client prediction class
		UiBTDCharacterMovementComponent* MutableThis = const_cast<UiBTDCharacterMovementComponent*>(this);
		MutableThis->ClientPredictionData = new FCustomNetworkPredictionData_Client(*this);
	}

	return ClientPredictionData;
}

void FCustomSavedMove::Clear()
{
	Super::Clear();

	SavedClimbUp = false;
	SavedWantsToClimbUp = false;
}

uint8 FCustomSavedMove::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	/* There are 4 custom move flags for us to use. Below is what each is currently being used for:
	FLAG_Custom_0		= 0x10, // Sprinting
	FLAG_Custom_1		= 0x20, // Unused
	FLAG_Custom_2		= 0x40, // Unused
	FLAG_Custom_3		= 0x80, // Unused
	*/

	// Write to the compressed flags 
	if (SavedWantsToClimbUp)
		Result |= FLAG_Custom_0;

	return Result;
}

bool FCustomSavedMove::CanCombineWith(const FSavedMovePtr& NewMovePtr, ACharacter* Character, float MaxDelta) const
{
	const FCustomSavedMove* NewMove = static_cast<const FCustomSavedMove*>(NewMovePtr.Get());

	if (SavedWantsToClimbUp != NewMove->SavedWantsToClimbUp)
	{
		return false;
	}

	return Super::CanCombineWith(NewMovePtr, Character, MaxDelta);
}

void FCustomSavedMove::SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	UiBTDCharacterMovementComponent* CharMove = Cast<UiBTDCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharMove)
	{
		SavedWantsToClimbUp = CharMove->WantsToClimbLadder;
	}
}

void FCustomSavedMove::PrepMoveFor(ACharacter* Character)
{
	Super::PrepMoveFor(Character);

	UiBTDCharacterMovementComponent* CharMove = Cast<UiBTDCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharMove)
	{
		CharMove->WantsToClimbLadder = SavedWantsToClimbUp;
	}
}

FCustomNetworkPredictionData_Client::FCustomNetworkPredictionData_Client(const UCharacterMovementComponent& ClientMovement)
	: Super(ClientMovement)
{
}

FSavedMovePtr FCustomNetworkPredictionData_Client::AllocateNewMove()
{
	return FSavedMovePtr(new FCustomSavedMove());
}
