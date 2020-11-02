// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerCharacter.h"
#include "MultiplayerPlayerState.h"
#include "GameFramework/PlayerState.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ADVGAMESPROGRAMMING_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;

	UPROPERTY(ReplicatedUsing = UpdateHealthBar, BlueprintReadWrite)
	float CurrentHealth;

	UFUNCTION(BlueprintCallable)
	void OnTakeDamage(float Damage, APlayerCharacter* HitFrom);

	UFUNCTION(BlueprintCallable)
	void OnDeath();

	UFUNCTION(BlueprintCallable)
	float HealthPercentageRemaining();

	UFUNCTION(NetMulticast, Reliable)
	void ClientSetScoreText(APlayerCharacter* HitFrom, AMultiplayerPlayerState* HitPlayerState);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION()
	void UpdateHealthBar();
};
