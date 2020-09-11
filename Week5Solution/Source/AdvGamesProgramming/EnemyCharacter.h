// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavigationNode.h"
#include "GameFramework/Character.h"
#include "AIManager.h"
#include "Perception/AIPerceptionComponent.h"
#include "HealthComponent.h"
#include "EnemyCharacter.generated.h"

UENUM()
enum class AgentState : uint8
{
	PATROL,
	ENGAGE,
	EVADE,

};

UCLASS()
class ADVGAMESPROGRAMMING_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<ANavigationNode*> Path;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ANavigationNode* CurrentNode;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AAIManager* Manager;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UAIPerceptionComponent* PerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AgentState CurrentAgentState;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AActor* DetectedActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bCanSeeActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bCanHearActor;
	UPROPERTY(BlueprintReadWrite)
	UHealthComponent* HealthComponent;

	UFUNCTION(BlueprintCallable)
	void AgentPatrol();
	UFUNCTION(BlueprintCallable)
	void AgentEngage();
	UFUNCTION(BlueprintCallable)
	void AgentEvade();
	UFUNCTION()
	void SensePlayer(AActor* ActorSensed, FAIStimulus Stimulus);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Fire(FVector FireDirection);
	UFUNCTION(BlueprintCallable)
	void MoveAlongPath();
	UFUNCTION(BlueprintCallable)
	bool PathExist();
	UFUNCTION(BlueprintCallable)
	void EmptyPath();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
