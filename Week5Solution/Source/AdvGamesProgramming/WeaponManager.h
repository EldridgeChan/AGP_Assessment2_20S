// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NavigationNode.h"
#include "WeaponManager.generated.h"

class AWeaponPickup;

UCLASS()
class ADVGAMESPROGRAMMING_API AWeaponManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "AI Properties", BlueprintReadOnly)
		int32 NumAI;
	//An array of navigation nodes
	UPROPERTY(VisibleAnywhere, Category = "Navigation Nodes", BlueprintReadOnly)
		TArray<ANavigationNode*> AllNodes;
	//an array of weapons from legendary to common
	UPROPERTY(VisibleAnywhere, Category = "Weapons")
		TArray<AWeaponPickup*> WeaponList;
	UPROPERTY(EditAnywhere, Category = "Weapons")
		TSubclassOf<AWeaponPickup> WeaponToSpawn;

	void PopulateNodes();
	
	
	void Init(const TArray<ANavigationNode*>& SpawnLocations, TSubclassOf<class APickup> WeaponPickup, float FrequencyOfSpawn);

	
private: 
	TSubclassOf<class APickup> WeaponPickupClass;
	float FrequencyOfWeaponPickupSpawns;
	FTimerHandle WeaponSpawnTimer;
	void CreateWeapon();
};
