// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "iBTDCharacterMovementComponent.generated.h"

UENUM(BlueprintType)
enum ELadderMovementType
{
	StayingStill,
	ClimbingUp,
	ClimbingDown
};

/**
 * Custom Movement Mode for climbing up a ladder 
 */
UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_ClimbLadder	UMETA(DisplayName = "Climb Ladder"),
	CMOVE_MAX			UMETA(Hidden)
};

/**
 * Custom Character Movement Mode to enabe climbing ladders
 */
UCLASS()
class IB_TECHDEMONSTRATOR_API UiBTDCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	friend class FCustomSavedMove;
	
	/** The Climb Speed */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "iBTD|Movement|Climbing", meta = (AllowPrivateAccess = "true"))
	float ClimbUpSpeed = 250.0f;

public:

	UFUNCTION(BlueprintCallable, Category = "iBTD|Movement|Climbing")
	void SetClimbingMode(bool bClimbing);

	UFUNCTION(BlueprintCallable, Category = "iBTD|Movement|Climbing")
	bool BeginClimbing(bool bClimbing);

	UFUNCTION(BlueprintCallable, Category = "iBTD|Movement|Climbing")
	void EndClimbing();

	UFUNCTION(BlueprintPure, Category = "iBTD|Movement|Ladder")
	ELadderMovementType GetLadderMovementType() const { return LadderMovementType; }

	UFUNCTION(Server, WithValidation, Reliable)
	void SetLadderClimbDirection(const FVector& NewDirection);

	// Returns true if the movement mode is custom and matches the provided custom movement mode
	bool IsCustomMovementMode(uint8 custom_movement_mode) const;


protected:

	virtual void BeginPlay() override;

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

public:

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	void PhysClimbLadder(float deltaTime, int32 Iterations);
	virtual void ProcessLanded(const FHitResult& Hit, float remainingTime, int32 Iterations) override;
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;

private:
	uint8 WantsToClimbLadder : 1;
	uint8 ClimbingLadder : 1;

	ELadderMovementType LadderMovementType;
	FVector LadderClimbDirection;
};

class FCustomSavedMove : public FSavedMove_Character
{
	typedef FSavedMove_Character Super;

	// Resets all saved variables.
	virtual void Clear() override;

	// Store input commands in the compressed flags.
	virtual uint8 GetCompressedFlags() const override;

	// This is used to check whether or not two moves can be combined into one.
	// Basically you just check to make sure that the saved variables are the same.
	virtual bool CanCombineWith(const FSavedMovePtr& NewMovePtr, ACharacter* Character, float MaxDelta) const override;

	// Sets up the move before sending it to the server. 
	virtual void SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData) override;
	
	// Sets variables on character movement component before making a predictive correction.
	virtual void PrepMoveFor(class ACharacter* Character) override;

private:
	uint8 SavedWantsToClimbUp : 1;
	uint8 SavedClimbUp : 1;
};

class FCustomNetworkPredictionData_Client : public FNetworkPredictionData_Client_Character
{
public:
	typedef FNetworkPredictionData_Client_Character Super;

	// Constructor
	FCustomNetworkPredictionData_Client(const UCharacterMovementComponent& ClientMovement);

	// Allocates a new copy of our custom saved move
	virtual FSavedMovePtr AllocateNewMove() override;
};