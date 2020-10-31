// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameMode.h"
#include "EngineUtils.h"
#include "Engine/GameEngine.h"
#include "GameFramework/HUD.h"
#include "TimerManager.h"
#include "PlayerHUD.h"
#include "PlayerCharacter.h"

void AMultiplayerGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessages)
{
	Super::InitGame(MapName, Options, ErrorMessages);

	for (TActorIterator<AProcedurallyGeneratedRooms> It(GetWorld()); It; ++It)
	{
		ProceduralRooms = *It;
	}

	PickupManager = GetWorld()->SpawnActor<APickupManager>();

	if (PickupManager && ProceduralRooms)
	{
		PickupManager->Init(ProceduralRooms->Vertices, WeaponPickupClass, 10.0f);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to find the procedural map or unable to spawn the pickup manager"));
	}
}

void AMultiplayerGameMode::Respawn(AController* Controller)
{
	if (Controller)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("Respawning")));
		}

		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Controller->GetPawn()))
		{
			PlayerCharacter->HidePlayerHUD(true);
		}

		Controller->GetPawn()->SetLifeSpan(0.1f);

		FTimerHandle RespawnTimer;
		FTimerDelegate RespawnDelegate;
		RespawnDelegate.BindUFunction(this, TEXT("TriggerRespawn"), Controller);

		GetWorldTimerManager().SetTimer(RespawnTimer, RespawnDelegate, 5.0f, false);
	}
}

void AMultiplayerGameMode::TriggerRespawn(AController* Controller)
{
	if (Controller)
	{
		if (ProceduralRooms) {
			AActor* SpawnPoint = ProceduralRooms->NavNodes[FMath::RandRange(0, ProceduralRooms->NavNodes.Num() - 1)];
			if (SpawnPoint)
			{
				APawn* NewPlayerPawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, SpawnPoint->GetActorLocation(), SpawnPoint->GetActorRotation());
				if (NewPlayerPawn)
				{
					Controller->Possess(NewPlayerPawn);
					if (APlayerCharacter* Character = Cast<APlayerCharacter>(NewPlayerPawn))
					{
						UE_LOG(LogTemp, Display, TEXT("Showing the HUD"));
						Character->HidePlayerHUD(false);
					}
				}
			}
		}
	}
}