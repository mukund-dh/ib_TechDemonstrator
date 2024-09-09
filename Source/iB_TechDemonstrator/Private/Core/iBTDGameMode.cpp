// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/iBTDGameMode.h"
#include "Core/iBTDGameState.h"
#include "Player/iBTDPlayerController.h"
#include "Player/iBTDPlayerState.h"
#include "Player/iBTDPlayerCharacter.h"

AiBTDGameMode::AiBTDGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Initialize defaults for the game mode
	GameStateClass = AiBTDGameState::StaticClass();
	PlayerControllerClass = AiBTDPlayerController::StaticClass();
	PlayerStateClass = AiBTDPlayerState::StaticClass();
	DefaultPawnClass = AiBTDPlayerCharacter::StaticClass();
}
