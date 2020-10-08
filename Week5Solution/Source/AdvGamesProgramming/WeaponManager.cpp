// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponManager.h"
#include "EngineUtils.h"
#include "EnemyCharacter.h"
#include "Engine/World.h"
#include "WeaponPickup.h"
#include "Math/UnrealMathUtility.h"

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

	PopulateNodes();
	CreateWeapon();
}

// Called every frame
void AWeaponManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AWeaponManager::PopulateNodes()
{
	for (TActorIterator<ANavigationNode> It(GetWorld()); It; ++It)
	{
		AllNodes.Add(*It);
	}
}

void AWeaponManager::CreateWeapon()
{
	for (int32 i = 0; i < NumAI; i++)
	{
		int32 RandIndex = FMath::RandRange(0, AllNodes.Num() - 1);
		AWeaponPickup* Weapon = GetWorld()->SpawnActor<AWeaponPickup>(WeaponToSpawn, AllNodes[RandIndex]->GetActorLocation(), FRotator(0.f, 0.f, 0.f));
		Weapon->Manager = this;
		Weapon->CurrentNode = AllNodes[RandIndex];
		WeaponList.Add(Weapon);
	}
}


