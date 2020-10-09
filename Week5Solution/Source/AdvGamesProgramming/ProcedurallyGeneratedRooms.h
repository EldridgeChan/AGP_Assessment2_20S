// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NavigationNode.h"
#include "ProcedurallyGeneratedRooms.generated.h"

UCLASS()
class ADVGAMESPROGRAMMING_API AProcedurallyGeneratedRooms : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProcedurallyGeneratedRooms();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MapXNum;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MapYNum;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<int> RoomType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<int> RoomDirection;
	UPROPERTY(VisibleAnywhere)
	TArray<ANavigationNode*> NavNodes;
	UPROPERTY(VisibleAnywhere)
	TArray<bool> bIsConnected;
	UPROPERTY(EditAnywhere)
	bool bRegenerateMap;

	void GenerateMapIndex();
	void ConnectNodes(ANavigationNode* FromNode, ANavigationNode* ToNode);
	void ChangeConnectivity(int startIndex);
	void RoomInfomation();
	void CalDirection(int Index);
	void ClearMap();

	virtual bool ShouldTickIfViewportsOnly() const override;

	UFUNCTION(BlueprintImplementableEvent)
	void GenerateMap();
	UFUNCTION(BlueprintImplementableEvent)
	void ClearCubes();
};
