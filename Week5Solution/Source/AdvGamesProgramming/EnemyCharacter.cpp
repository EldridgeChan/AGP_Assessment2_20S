// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EngineUtils.h"
#include "ThisGameMode.h"
#include "Engine/Engine.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CurrentAgentState = AgentState::PATROL;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	PerceptionComponent = FindComponentByClass<UAIPerceptionComponent>();
	if (!PerceptionComponent) { UE_LOG(LogTemp, Error, TEXT("NO PERCEPTION COMPONENT FOUND")) }
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyCharacter::SensePlayer);
	
	HealthComponent = FindComponentByClass<UHealthComponent>();
	DetectedActor = nullptr;
	bCanSeeActor = false;
	bCanHearActor = false;
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetActorLocation().Z < -20) {
		UE_LOG(LogTemp, Warning, TEXT("Character drop out of world"));
		SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, 0.0f), false, nullptr, ETeleportType::None);
	}

	/*
	//Enemy in patrol state
	if (CurrentAgentState == AgentState::PATROL)
	{
		AgentPatrol();
		if (bCanSeeActor && HealthComponent->HealthPercentageRemaining() >= 0.4f)
		{
			CurrentAgentState = AgentState::ENGAGE;
			Path.Empty();
		}
		else if (bCanSeeActor && HealthComponent->HealthPercentageRemaining() < 0.4f)
		{
			CurrentAgentState = AgentState::EVADE;
			Path.Empty();
		}
	}
	//Enemy in engage state
	else if (CurrentAgentState == AgentState::ENGAGE)
	{
		AgentEngage();
		if (!bCanSeeActor)
		{
			CurrentAgentState = AgentState::PATROL;
		}
		else if (bCanSeeActor && HealthComponent->HealthPercentageRemaining() < 0.4f)
		{
			CurrentAgentState = AgentState::EVADE;
			Path.Empty();
		}
	}
	//Enemy in evade state
	else if (CurrentAgentState == AgentState::EVADE)
	{
		AgentEvade();
		if (!bCanSeeActor)
		{
			CurrentAgentState = AgentState::PATROL;
		}
		else if (bCanSeeActor && HealthComponent->HealthPercentageRemaining() >= 0.4f)
		{
			CurrentAgentState = AgentState::ENGAGE;
			Path.Empty();
		}
	}
	MoveAlongPath();
	*/
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//Enemy patrol if no player character in sight
void AEnemyCharacter::AgentPatrol()
{
	if (Path.Num() == 0 && Manager != NULL)
	{
		Path = Manager->GeneratePath(CurrentNode, Manager->AllNodes[FMath::RandRange(0, Manager->AllNodes.Num() - 1)]);
	}
}

//Enemy engage the player if he sees or hears him
void AEnemyCharacter::AgentEngage()
{
	if (DetectedActor && (bCanSeeActor || bCanHearActor))
	{
		FVector DirectionToTarget = DetectedActor->GetActorLocation() - GetActorLocation();
		if (bCanSeeActor) 
		{
			Fire(DirectionToTarget);
		}
		if (Path.Num() == 0)
		{
			Path = Manager->GeneratePath(CurrentNode, Manager->FindNearestNode(DetectedActor->GetActorLocation()));
		}
	}
	
}

//Enemy evade the player
void AEnemyCharacter::AgentEvade()
{
	
	if (bCanSeeActor && DetectedActor)
	{
		FVector DirectionToTarget = DetectedActor->GetActorLocation() - GetActorLocation();
		//Fire(DirectionToTarget);
		if (Path.Num() == 0)
		{
			Path = Manager->GeneratePath(CurrentNode, Manager->FindFurthestNode(DetectedActor->GetActorLocation()));
		}
	}
}

//if player is sensed (seen/heard), player is detected
void AEnemyCharacter::SensePlayer(AActor* SensedActor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		UE_LOG(LogTemp, Warning, TEXT("Player currently detected at %s"), *SensedActor->GetActorLocation().ToString()) // + player location
		DetectedActor = SensedActor;
		bCanSeeActor = true;
		bCanHearActor = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player last detected at %s"), *SensedActor->GetActorLocation().ToString())
		bCanSeeActor = false;
		bCanHearActor = false;
	}
}

void AEnemyCharacter::MoveAlongPath()
{
	if (Path.Num() > 0 && Manager != NULL)
	{
		//UE_LOG(LogTemp, Display, TEXT("Current Node: %s"), *CurrentNode->GetName())
		if ((GetActorLocation() - CurrentNode->GetActorLocation()).IsNearlyZero(100.0f))
		{
			UE_LOG(LogTemp, Display, TEXT("At Node %s"), *CurrentNode->GetName())
			CurrentNode = Path.Pop();
		}
		else
		{
			FVector WorldDirection = CurrentNode->GetActorLocation() - GetActorLocation();
			WorldDirection.Normalize();
			AddMovementInput(WorldDirection, 1.0f);

			//Get the AI to face in the direction of travel.
			FRotator FaceDirection = WorldDirection.ToOrientationRotator();
			FaceDirection.Roll = 0.f;
			FaceDirection.Pitch = 0.f;
			SetActorRotation(FaceDirection);
		}
	}
}

bool AEnemyCharacter::PathExist()
{
	return Path.Num() > 0;
}

void AEnemyCharacter::EmptyPath()
{
	Path.Empty();
}

void AEnemyCharacter::OnDeath()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		AThisGameMode* GameMode = Cast<AThisGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->Respawn(GetController());
		}
	}

}
