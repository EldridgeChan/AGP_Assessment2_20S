// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ThisGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "HealthComponent.h"
#include "MultiplayerGameMode.h"
#include "PlayerHUD.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//AutoPossessPlayer = EAutoReceiveInput::Player0;
	bUseControllerRotationPitch = true;

	LookSensitivity = 75.0f;
	MoveSpeed = 150.0f;
	SprintMultiplier = 1.5f;

	SprintMovementSpeed = GetCharacterMovement()->MaxWalkSpeed * SprintMultiplier;
	NormalMovementSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	HealthComponent = FindComponentByClass<UHealthComponent>();
	if (HealthComponent)
	{
		HealthComponent->SetIsReplicated(true);
	}
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("Strafe"), this, &APlayerCharacter::Strafe);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APlayerCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APlayerCharacter::Turn);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &APlayerCharacter::SprintStart);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &APlayerCharacter::SprintEnd);
}

void APlayerCharacter::MoveForward(float Value)
{
	FRotator ForwardRotation = GetControlRotation();
	ForwardRotation.Roll = 0.0f;
	ForwardRotation.Pitch = 0.0f;
	AddMovementInput(ForwardRotation.Vector(), Value * GetWorld()->GetDeltaSeconds() * MoveSpeed);
}

void APlayerCharacter::Strafe(float Value)
{
	AddMovementInput(GetActorRightVector(), Value * GetWorld()->GetDeltaSeconds() * MoveSpeed);
}

void APlayerCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value * GetWorld()->GetDeltaSeconds() * LookSensitivity);
}

void APlayerCharacter::Turn(float Value)
{
	AddControllerYawInput(Value * GetWorld()->GetDeltaSeconds() * LookSensitivity);
}

void APlayerCharacter::SprintStart()
{
	GetCharacterMovement()->MaxWalkSpeed *= SprintMultiplier;
	ServerSprintStart();
}

void APlayerCharacter::SprintEnd()
{
	GetCharacterMovement()->MaxWalkSpeed /= SprintMultiplier;
	ServerSprintEnd();
}

void APlayerCharacter::OnDeath()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		AMultiplayerGameMode* GameMode = Cast<AMultiplayerGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->Respawn(GetController());
		}
	}

}

void APlayerCharacter::HidePlayerHUD_Implementation(bool bSetHUDVisibility)
{
	if (GetLocalRole() == ROLE_AutonomousProxy || (GetLocalRole() == ROLE_Authority && IsLocallyControlled()))
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
		{
			if (APlayerHUD* HUD = Cast<APlayerHUD>(PlayerController->GetHUD()))
			{
				HUD->SetHideWidgets(bSetHUDVisibility);
			}
		}
	}
}

void APlayerCharacter::ServerSprintStart_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintMovementSpeed;
}

void APlayerCharacter::ServerSprintEnd_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = NormalMovementSpeed;
}
