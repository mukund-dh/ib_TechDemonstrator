// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/iBTDGameState.h"
#include "Player/iBTDPlayerController.h"
#include "Net/UnrealNetwork.h"

AiBTDGameState::AiBTDGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TArray<AiBTDPlayerController*> AiBTDGameState::GetConnectedPlayers() const
{
	TArray<AiBTDPlayerController*> ConnectedPlayers;

	for (FPlayerLadderCount PLC : PlayerLadderCount)
	{
		ConnectedPlayers.Add(PLC.PlayerController);
	}

	return ConnectedPlayers;
}

int32 AiBTDGameState::GetNumLaddersClimbedByPlayer(AiBTDPlayerController* InController)
{
	for (FPlayerLadderCount PLC : PlayerLadderCount)
	{
		if (PLC.PlayerController == InController)
		{
			return PLC.LaddersClimbed;
		}
	}

	return 0;
}

bool AiBTDGameState::IncrementLaddersClimbedByPlayer_Validate(AiBTDPlayerController* InController)
{
	// Detect cheats here. For now, always returning true
	return true;
}

void AiBTDGameState::IncrementLaddersClimbedByPlayer_Implementation(AiBTDPlayerController* InController)
{
	if (HasAuthority())
	{
		for (FPlayerLadderCount PLC : PlayerLadderCount)
		{
			if (PLC.PlayerController == InController)
			{
				PLC.LaddersClimbed += 1;
			}
		}
	}
}

void AiBTDGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AiBTDGameState, PlayerLadderCount);
}
