// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "iBTDGameMode.generated.h"

/**
 * Game rules go here. As this is a fairly simple demostrator, we don't really need to override/define
 * anything here.
 * 
 * However, if we had to, we should override the following:
 * Player Login/Logout
 * Init Player in the game
 * Handle MatchState Start/Stop
 * Game Timer(s) if any
 * Choosing Player Start Points (based on player teams)
 */
UCLASS()
class IB_TECHDEMONSTRATOR_API AiBTDGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	
	AiBTDGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


};
