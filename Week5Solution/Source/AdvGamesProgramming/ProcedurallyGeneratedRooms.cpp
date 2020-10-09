// Fill out your copyright notice in the Description page of Project Settings.


#include "ProcedurallyGeneratedRooms.h"
#include "EngineUtils.h"


// Sets default values
AProcedurallyGeneratedRooms::AProcedurallyGeneratedRooms()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bRegenerateMap = false;
}

// Called when the game starts or when spawned
void AProcedurallyGeneratedRooms::BeginPlay()
{
	Super::BeginPlay();

	//ClearMap();
	//GenerateMapIndex();
	GenerateMap();
}

// Called every frame
void AProcedurallyGeneratedRooms::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bRegenerateMap) {
		bRegenerateMap = false;
		ClearMap();
		GenerateMapIndex();
		//GenerateMap();
	}
}

void AProcedurallyGeneratedRooms::GenerateMapIndex()
{
	for (int Col = 0; Col < MapYNum; Col++) {
		for (int Row = 0; Row < MapXNum; Row++) {
			NavNodes.Add(GetWorld()->SpawnActor<ANavigationNode>(FVector(Row * 1000.0f, Col * 1000.0f, 0), FRotator::ZeroRotator));
			bIsConnected.Add(false);
			RoomType.Add(4);
			RoomDirection.Add(0);
		}
	}

	for (int Col = 0; Col < MapYNum; Col++) {
		for (int Row = 0; Row < MapXNum; Row++) {
			int DoorNum = FMath::RandRange(1, 3);

			if (DoorNum > 1 && Row < MapXNum - 1) {
				ConnectNodes(NavNodes[Row + Col * MapYNum], NavNodes[Row + Col * MapYNum + 1]);
			}
			if (DoorNum % 2 == 1 && Col < MapYNum - 1) {
				ConnectNodes(NavNodes[Row + Col * MapYNum], NavNodes[Row + (Col + 1) * MapYNum]);
			}
		}
	}
	ChangeConnectivity(0);
	for (int Col = 0; Col < MapYNum; Col++) {
		for (int Row = 0; Row < MapXNum; Row++) {
			if (!bIsConnected[Row + Col * MapXNum]) {
				if (Row > 0 && bIsConnected[Row + Col * MapXNum - 1]) {
					ConnectNodes(NavNodes[Row + Col * MapXNum], NavNodes[Row + Col * MapXNum - 1]);
					ChangeConnectivity(Row + Col * MapXNum);
				}
				else if (Col > 0 && bIsConnected[Row + (Col - 1) * MapXNum]) {
					ConnectNodes(NavNodes[Row + Col * MapXNum], NavNodes[Row + (Col - 1) * MapXNum]);
					ChangeConnectivity(Row + Col * MapXNum);

				}
				else if (Row < MapXNum - 1 && bIsConnected[Row + Col * MapXNum + 1]) {
					ConnectNodes(NavNodes[Row + Col * MapXNum], NavNodes[Row + Col * MapXNum + 1]);
					ChangeConnectivity(Row + Col * MapXNum);

				}
				else if (Col < MapYNum - 1 && bIsConnected[Row + (Col + 1) * MapXNum]) {
					ConnectNodes(NavNodes[Row + Col * MapXNum], NavNodes[Row + (Col + 1) * MapXNum]);
					ChangeConnectivity(Row + Col * MapXNum);

				}
			}
		}
	}
	RoomInfomation();
}

void AProcedurallyGeneratedRooms::ConnectNodes(ANavigationNode* FromNode, ANavigationNode* ToNode)
{
	FromNode->ConnectedNodes.Add(ToNode);
	ToNode->ConnectedNodes.Add(FromNode);
}

void AProcedurallyGeneratedRooms::ChangeConnectivity(int startIndex)
{
	TArray<ANavigationNode*> TempSet;
	TempSet.Push(NavNodes[startIndex]);

	while (TempSet.Num() > 0) {
		int TempIndex = NavNodes.Find(TempSet[TempSet.Num() - 1]);
		TempSet.Pop();
		if (!bIsConnected[TempIndex]) {
			bIsConnected[TempIndex] = true;
			for (auto It = NavNodes[TempIndex]->ConnectedNodes.CreateConstIterator(); It; ++It) {
				TempSet.Push(*It);
			}
		}
	}
}

void AProcedurallyGeneratedRooms::RoomInfomation()
{
	for (int Col = 0; Col < MapYNum; Col++) {
		for (int Row = 0; Row < MapXNum; Row++) {
			int DoorNum = NavNodes[Row + Col * MapXNum]->ConnectedNodes.Num();
			if (DoorNum != 2) {
				RoomType[Row + Col * MapXNum] = DoorNum - 1;
				if (DoorNum != 4) {
				}
			}
			else {
				if ((NavNodes.Find(NavNodes[Row + Col * MapXNum]->ConnectedNodes[0]) + NavNodes.Find(NavNodes[Row + Col * MapXNum]->ConnectedNodes[1])) / 2 == Row + Col * MapXNum) {
					RoomType[Row + Col * MapXNum] = 4;
				}
				else {
					RoomType[Row + Col * MapXNum] = 1;
				}
			}
			CalDirection(Row + Col * MapXNum);
		}
	}
}

void AProcedurallyGeneratedRooms::CalDirection(int Index)
{
	TArray<bool> DoorDir = { false, false, false, false };
	for (int i = 0; i < NavNodes[Index]->ConnectedNodes.Num(); i++) {
		int ConnectedIndex = NavNodes.Find(NavNodes[Index]->ConnectedNodes[i]);

		if (ConnectedIndex - Index > 0) {
			if (ConnectedIndex % MapXNum == Index % MapXNum) {
				DoorDir[2] = true;
			}
			else {
				DoorDir[1] = true;
			}
		}
		else {
			if (ConnectedIndex % MapXNum == Index % MapXNum) {
				DoorDir[0] = true;
			}
			else {
				DoorDir[3] = true;
			}
		}
	}

	if (RoomType[Index] == 4) {
		if (!DoorDir[0]) {
			RoomDirection[Index] = 1;
		}
	}
	else if (RoomType[Index] == 0) {
		int trueDoor;
		for (int i = 0; i < DoorDir.Num(); i++) {
			if (DoorDir[i]) {
				trueDoor = i;
			}
		}
		RoomDirection[Index] = trueDoor;
	}
	else if (RoomType[Index] == 2) {
		int falseDoor;
		for (int i = 0; i < DoorDir.Num(); i++) {
			if (!DoorDir[i]) {
				falseDoor = i;
			}
		}
		RoomDirection[Index] = (falseDoor + 1) % 4;
	}
	else if (RoomType[Index] == 1) {
		if (DoorDir[1]) {
			if (DoorDir[2]) {
				RoomDirection[Index] = 1;
			}
			else {
				RoomDirection[Index] = 0;
			}
		}
		else {
			if (DoorDir[2]) {
				RoomDirection[Index] = 2;
			}
			else {
				RoomDirection[Index] = 3;
			}
		}
	}
}

bool AProcedurallyGeneratedRooms::ShouldTickIfViewportsOnly() const
{
	return true;
}

void AProcedurallyGeneratedRooms::ClearMap()
{
	ClearCubes();
	RoomType.Empty();
	RoomDirection.Empty();
	NavNodes.Empty();
	bIsConnected.Empty();
	for (TActorIterator<ANavigationNode> It(GetWorld()); It; ++It) {
		(*It)->Destroy();
	}
	
}