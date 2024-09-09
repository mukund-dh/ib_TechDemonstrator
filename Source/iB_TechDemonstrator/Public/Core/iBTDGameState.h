// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "iBTDGameState.generated.h"

class AiBTDPlayerController;

USTRUCT(BlueprintType)
struct FPlayerLadderCount
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerStats")
	TObjectPtr<AiBTDPlayerController> PlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerStats")
	int32 LaddersClimbed;
};

/**
 * Stores the current state of the game. Ideally used to keep track of: 
 * Team Scores
 * Players in the match
 * Win/Lose Condition Checks (based on win/lose conditions defined in Game Mode
 * 
 */
UCLASS()
class IB_TECHDEMONSTRATOR_API AiBTDGameState : public AGameState
{
	GENERATED_BODY()

public:

	AiBTDGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintPure, Category = "GameState|Connections")
	TArray<AiBTDPlayerController*> GetConnectedPlayers() const;

	UFUNCTION(BlueprintCallable, Category = "GameState|Score")
	int32 GetNumLaddersClimbedByPlayer(AiBTDPlayerController* InController);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "GameState|Score")
	void IncrementLaddersClimbedByPlayer(AiBTDPlayerController* InController);


private:
	
	UPROPERTY(Replicated)
	TArray<FPlayerLadderCount> PlayerLadderCount;
};
