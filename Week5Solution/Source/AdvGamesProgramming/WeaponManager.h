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
	UPROPERTY(VisibleAnywhere, Category = "Navigation Nodes", BlueprintReadOnly)
		TArray<ANavigationNode*> AllNodes;
	UPROPERTY(VisibleAnywhere, Category = "Weapons")
		TArray<AWeaponPickup*> WeaponList;
	UPROPERTY(EditAnywhere, Category = "Weapons")
		TSubclassOf<AWeaponPickup> WeaponToSpawn;

	void PopulateNodes();
	void CreateWeapon();

};
