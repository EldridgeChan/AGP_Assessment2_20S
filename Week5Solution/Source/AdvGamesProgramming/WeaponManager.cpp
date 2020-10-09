// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponManager.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "WeaponPickup.h"
#include "TimerManager.h"
#include "Engine/GameEngine.h"

// Sets default values
AWeaponManager::AWeaponManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeaponManager::BeginPlay()
{
	Super::BeginPlay();

	//Populate a random nodes with a random weapon from legendary to common
	PopulateNodes();
	CreateWeapon();
	GetWorldTimerManager().SetTimer(WeaponSpawnTimer, this, &AWeaponManager::CreateWeapon, FrequencyOfWeaponPickupSpawns, true, 0.0f);
}

// Called every frame
void AWeaponManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//spawn the weapon on a random navnode and then make them respawn on another navnode
void AWeaponManager::Init(const TArray<ANavigationNode*>& SpawnLocations, TSubclassOf<APickup> WeaponPickup, float FrequencyOfSpawn)
{
	AllNodes = SpawnLocations;
	WeaponToSpawn = WeaponPickup;
	FrequencyOfWeaponPickupSpawns = FrequencyOfSpawn;
}

//populate the node with a weapon
void AWeaponManager::PopulateNodes()
{
	for (TActorIterator<ANavigationNode> It(GetWorld()); It; ++It)
	{
		AllNodes.Add(*It);
	}
}

//create a weapon on a random navnode
void AWeaponManager::CreateWeapon()
{
	
		int32 RandIndex = FMath::RandRange(0, AllNodes.Num() - 1);
		if(AWeaponPickup* Weapon = GetWorld()->SpawnActor<AWeaponPickup>(WeaponToSpawn, AllNodes[RandIndex]->GetActorLocation(), FRotator(0.f, 0.f, 0.f)))
		{ 
			Weapon->Manager = this;
			Weapon->CurrentNode = AllNodes[RandIndex];
			WeaponList.Add(Weapon);
			//weapon stays for 20 seconds then disapear
			Weapon->SetLifeSpan(20.0f);
				if (GEngine)
					{
					//print on screen a message when the weapon is spawned
						GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Pickup Spawned")));
					}

				else
					{
						UE_LOG(LogTemp, Warning, TEXT("Unable to spawn weapon pickup."));
					}
		}
}




