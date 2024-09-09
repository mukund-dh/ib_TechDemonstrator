// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "iBTDCharacterMovementComponent.h"
#include "iBTDPlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;
struct FInputActionValue;

UCLASS()
class IB_TECHDEMONSTRATOR_API AiBTDPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	/** The Camera Boom */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "iBTD|Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** The Player Camera Component */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "iBTD|Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> Camera;

	/** The default input mapping context */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "iBTD|Character|Inputs", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/** The climbing input mapping context */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "iBTD|Character|Inputs", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> ClimbingMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "iBTD|Character|Inputs", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	/** Move Input Action */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "iBTD|Character|Inputs", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	/** Move Input Action */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "iBTD|Character|Inputs", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveUpAction;

	/** Look Around Input Action */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "iBTD|Character|Inputs", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	/** Interact Input Action */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "iBTD|Character|Inputs", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InteractAction;

	/** Climb Up Animation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "iBTD|Character|Animations", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> ClimbLadderMontage;

public:
	// Sets default values for this character's properties
	AiBTDPlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintNativeEvent, Category = "iBTD|Character|Movement")
	void Interact(const FInputActionValue& Value);

	virtual void BeginPlay() override;

protected:

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void MoveOnLadder(const FInputActionValue& Value);
	void StopMovingOnLadder(const FInputActionValue& Value);

	UFUNCTION()
	void ChangeMovementMode (class ACharacter* Character, EMovementMode PrevMovementMode, uint8 PreviousCustomMode);

public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure, Category = "iBTD|Movement")
	UiBTDCharacterMovementComponent* GetCustomMovementComponent() const { return Cast<UiBTDCharacterMovementComponent>(GetCharacterMovement()); }
};
