// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "EnemyCharacter.h"
#include "Engine/GameEngine.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerHUD.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	MaxHealth = 100.0;
	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	/*if (GEngine && GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Current Health: %f"), CurrentHealth));
	}*/
}

void UHealthComponent::OnTakeDamage(float Damage, APlayerCharacter* HitFrom)
{
	CurrentHealth -= Damage;
	if (CurrentHealth <= 0) 
	{
		CurrentHealth = 0;
		AEnemyCharacter* OwningEnemyCharacter = Cast<AEnemyCharacter>(GetOwner());
		if (HitFrom && OwningEnemyCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("Die with player hit"));
			AMultiplayerPlayerState* HitPlayerState = Cast<AMultiplayerPlayerState>(HitFrom->GetPlayerState());
			if (HitPlayerState)
			{
				UE_LOG(LogTemp, Warning, TEXT("Player State Found"));
				HitPlayerState->Score += 10;
				//UE_LOG(LogTemp, Warning, TEXT("%d"), HitPlayerState->PlayerScore);
				if (HitFrom->IsLocallyControlled())
				{
					APlayerHUD* HUD = Cast<APlayerHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
					if (HUD)
					{
						HUD->SetScoreText(HitPlayerState->Score);
					}
				}
			}
			ClientSetScoreText(HitFrom, HitPlayerState);
		}
		OnDeath();
	}

	if (GetOwner()->GetLocalRole() == ROLE_Authority)
	{
		UpdateHealthBar();
	}
}

void UHealthComponent::ClientSetScoreText_Implementation(APlayerCharacter* HitFrom, AMultiplayerPlayerState* HitPlayerState)
{
	if (HitFrom && HitPlayerState && HitFrom->IsLocallyControlled())
	{
		APlayerHUD* HUD = Cast<APlayerHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
		if (HUD)
		{
			HUD->SetScoreText(HitPlayerState->Score);
		}
	}
}

void UHealthComponent::OnDeath()
{
	APlayerCharacter* OwningPlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	AEnemyCharacter* OwningEnemyCharacter = Cast<AEnemyCharacter>(GetOwner());
	if (OwningPlayerCharacter)
	{
		OwningPlayerCharacter->OnDeath();
	}
	if (OwningEnemyCharacter)
	{
		OwningEnemyCharacter->OnDeath();
	}
}

float UHealthComponent::HealthPercentageRemaining()
{
	return CurrentHealth / MaxHealth;
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, CurrentHealth);
}

void UHealthComponent::UpdateHealthBar()
{
	if (APlayerCharacter* OwningCharacter = Cast<APlayerCharacter>(GetOwner()))
	{
		if (OwningCharacter->IsLocallyControlled())
		{
			APlayerHUD* HUD = Cast<APlayerHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
			if (HUD)
			{
				HUD->SetPlayerHealthBarPercent(HealthPercentageRemaining());
			}
		}
	}
}