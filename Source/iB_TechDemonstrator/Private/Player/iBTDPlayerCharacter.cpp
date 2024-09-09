// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/iBTDPlayerCharacter.h"
#include "Player/iBTDPlayerController.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AiBTDPlayerCharacter::AiBTDPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UiBTDCharacterMovementComponent>(AiBTDPlayerCharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCustomMovementComponent()->bOrientRotationToMovement = true;
	GetCustomMovementComponent()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCustomMovementComponent()->JumpZVelocity = 700.0f;
	GetCustomMovementComponent()->AirControl = 0.35f;
	GetCustomMovementComponent()->MaxWalkSpeed = 500.0f;
	GetCustomMovementComponent()->MinAnalogWalkSpeed = 20.0f;
	GetCustomMovementComponent()->BrakingDecelerationWalking = 2000.0f;
	GetCustomMovementComponent()->BrakingDecelerationFalling = 1500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetRelativeLocation(FVector(0.0f, 20.0f, 70.0f));
	CameraBoom->TargetArmLength = 150.0f;
	CameraBoom->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	DefaultMappingContext = nullptr;
	JumpAction = nullptr;
	MoveAction = nullptr;
	LookAction = nullptr;
	InteractAction = nullptr;
	ClimbLadderMontage = nullptr;
}

void AiBTDPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocallyControlled())
	{
		TScriptDelegate Delegate;
		Delegate.BindUFunction(this, FName("ChangeMovementMode"));
		MovementModeChangedDelegate.Add(Delegate);
	}
}

void AiBTDPlayerCharacter::Move(const FInputActionValue& Value)
{
	// Movement input is a 2D vector
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		// Forward vector
		const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// Right Direction Vector
		const FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardVector, MovementVector.Y);
		AddMovementInput(RightVector, MovementVector.X);
	}
}

void AiBTDPlayerCharacter::MoveOnLadder(const FInputActionValue& Value)
{
	FVector InputAxis = Value.Get<FVector>();

	if (GetCustomMovementComponent() != nullptr)
	{
		GetCustomMovementComponent()->SetLadderClimbDirection(InputAxis);
	}

}

void AiBTDPlayerCharacter::StopMovingOnLadder(const FInputActionValue& Value)
{
	if (GetCustomMovementComponent() != nullptr)
	{
		GetCustomMovementComponent()->SetLadderClimbDirection(FVector(0.0f, 0.0f, 0.0f));
	}
}

void AiBTDPlayerCharacter::ChangeMovementMode(ACharacter* Character, EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	if (AiBTDPlayerController* PC = Cast<AiBTDPlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (PrevMovementMode == EMovementMode::MOVE_Walking)
			{
				// Could be any land based movement mode. Change this to climbing ladders
				if (ClimbingMappingContext)
				{
					EnhancedInputLocalPlayerSubsystem->ClearAllMappings();
					EnhancedInputLocalPlayerSubsystem->AddMappingContext(ClimbingMappingContext, 0);
				}
			}
			else
			{
				// Change back to default
				EnhancedInputLocalPlayerSubsystem->ClearAllMappings();
				EnhancedInputLocalPlayerSubsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

void AiBTDPlayerCharacter::Look(const FInputActionValue& Value)
{
	// Input is a FVector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AiBTDPlayerCharacter::Interact_Implementation(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("INTERACT BUTTON PRESSED!"));
}

// Called to bind functionality to input
void AiBTDPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (AiBTDPlayerController* PC = Cast<AiBTDPlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				EnhancedInputLocalPlayerSubsystem->AddMappingContext(DefaultMappingContext, 0);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("No Default Input Mapping Context Found! Skipping."));
			}
		}
	}

	// Setup action bindings
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (JumpAction)
		{
			// Bind Jump
			EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &AiBTDPlayerCharacter::Jump);
			EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &AiBTDPlayerCharacter::StopJumping);
		}

		if (MoveAction)
		{
			// Bind Move
			EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AiBTDPlayerCharacter::Move);
		}

		if (LookAction)
		{
			// Bind Look
			EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AiBTDPlayerCharacter::Look);
		}

		if (InteractAction)
		{
			// Bind Interact
			EIC->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AiBTDPlayerCharacter::Interact);
		}

		if (MoveUpAction)
		{
			// Bind Climb
			EIC->BindAction(MoveUpAction, ETriggerEvent::Started, this, &AiBTDPlayerCharacter::MoveOnLadder);
			EIC->BindAction(MoveUpAction, ETriggerEvent::Completed, this, &AiBTDPlayerCharacter::StopMovingOnLadder);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

}

