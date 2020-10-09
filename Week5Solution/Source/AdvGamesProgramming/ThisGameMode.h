// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ThisGameMode.generated.h"


/**
 *
 */
UCLASS()
class ADVGAMESPROGRAMMING_API AThisGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	void Respawn(AController* Controller);

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class APickup> WeaponPickupClass;

};
