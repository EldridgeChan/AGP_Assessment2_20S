// Fill out your copyright notice in the Description page of Project Settings.


#include "ThisGameMode.h"
#include "EngineUtils.h"
#include "WeaponManager.h"
#include "Engine/GameEngine.h"
#include "GameFramework/HUD.h"
#include "TimerManager.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"

//make the enemy respawns once he dies
void AThisGameMode::Respawn(AController* Controller)
{
	if (Controller)
	{
		if (GEngine)
		{
			//print "respawn" when the enemy's life goes down to 0
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("Respawning")));
		}
	
	}
}

