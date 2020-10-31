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

	//Max X Value; Length of the Map
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MapXNum;
	//Map Y Value; Width of the Map
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MapYNum;
	//An Array store the RoomType i.e. Level stream index
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<int> RoomType;
	//An Array store the Yaw Rotation of each Room
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<int> RoomDirection;
	//An Array store All the Navigation Nodes
	UPROPERTY(VisibleAnywhere)
	TArray<ANavigationNode*> NavNodes;
	//An Array store if the Room is Reachable from the starting room
	UPROPERTY(VisibleAnywhere)
	TArray<bool> bIsConnected;
	UPROPERTY(VisibleAnywhere)
	TArray<FVector> Vertices;
	//An boolean value serve as a button to regenerate the Navigation Node and their Connection
	UPROPERTY(EditAnywhere)
	bool bRegenerateMap;

	//Generate All Array members
	void GenerateMapIndex();
	//Set up a two way connection between Nodes
	void ConnectNodes(ANavigationNode* FromNode, ANavigationNode* ToNode);
	//Check and change the bIsConnected value from the startIndex Node if reachable
	void ChangeConnectivity(int startIndex);
	//Calculate the Room Type and Room Direction from the Connectivity of the Navigation Nodes
	void RoomInfomation();
	//Calculate the Rotation of the Room by Index, must use after initialize of Room Type
	void CalDirection(int Index);
	//Clear all the Navigation Nodes and Static Mesh, Empty all the Array above.
	void ClearMap();

	virtual bool ShouldTickIfViewportsOnly() const override;

	//Implemented in blueprint. Generate the Static mesh for the Rooms
	UFUNCTION(BlueprintImplementableEvent)
	void GenerateMap();
	//Destroy all the static mesh
	UFUNCTION(BlueprintImplementableEvent)
	void ClearCubes();
};
