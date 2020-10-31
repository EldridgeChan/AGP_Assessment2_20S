// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "PlayerCharacter.h"

APlayerHUD::APlayerHUD()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> PlayerHUDObject(TEXT("/Game/Widgets/PlayerHUDWidget"));

	PlayerHUDClass = PlayerHUDObject.Class;

	if (PlayerHUDClass)
	{
		CurrentPlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDClass);

		if (CurrentPlayerHUDWidget)
		{
			CurrentPlayerHUDWidget->AddToViewport();
			HealthProgressBar = Cast<UProgressBar>(CurrentPlayerHUDWidget->GetWidgetFromName(TEXT("ProgHealthBar")));
		}
	}
}

void APlayerHUD::SetPlayerHealthBarPercent(float Percent)
{
	if (HealthProgressBar) {
		HealthProgressBar->SetPercent(Percent);
	}
}

void APlayerHUD::SetHideWidgets(bool bIsHidden)
{
	if (bIsHidden)
	{
		if (HealthProgressBar)
			HealthProgressBar->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		if (HealthProgressBar)
		{
			HealthProgressBar->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Something went wrong with the hud."));
		}
		//Hack way. Having Gun C++ class instead of blueprints could fix this issue
		SetPlayerHealthBarPercent(1.0f);
	}
}