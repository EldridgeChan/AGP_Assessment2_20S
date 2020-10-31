// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "PlayerCharacter.h"
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

void UHealthComponent::OnTakeDamage(float Damage)
{
	CurrentHealth -= Damage;
	if (CurrentHealth <= 0) 
	{
		CurrentHealth = 0;
		OnDeath();
	}

	if (GetOwner()->GetLocalRole() == ROLE_Authority)
	{
		UpdateHealthBar();
	}
}

void UHealthComponent::OnDeath()
{
	APlayerCharacter* OwningPlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	if (OwningPlayerCharacter)
	{
		OwningPlayerCharacter->OnDeath();
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