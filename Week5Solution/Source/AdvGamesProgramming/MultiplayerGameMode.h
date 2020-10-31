// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ProcedurallyGeneratedRooms.h"
#include "PickupManager.h"
#include "MultiplayerGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ADVGAMESPROGRAMMING_API AMultiplayerGameMode : public AGameMode
{
	GENERATED_BODY()
	
private:
	AProcedurallyGeneratedRooms* ProceduralRooms;
	APickupManager* PickupManager;
	
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APickup> WeaponPickupClass;

	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessages) override;

	void Respawn(AController* Controller);

	UFUNCTION()
	void TriggerRespawn(AController* Controller);
};
