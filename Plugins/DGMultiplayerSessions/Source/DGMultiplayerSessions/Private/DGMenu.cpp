// Fill out your copyright notice in the Description page of Project Settings.


#include "DGMenu.h"
#include "Components/Button.h"
#include "DGMSSubsystem.h"

void UDGMenu::MenuSetup(int32 MaxNumberPlayers, FString TypeOfMatch)
{
	MaxNumPlayers = MaxNumberPlayers;
	MatchType = TypeOfMatch;
	
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UWorld* World = GetWorld(); // Get the world
	if (!World) return;

	APlayerController* PlayerController = World->GetFirstPlayerController(); // Get the player controller
	if (!PlayerController) return;

	FInputModeUIOnly InputModeData; // Create an input mode data object
	InputModeData.SetWidgetToFocus(TakeWidget()); // Set the widget to focus
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // Set the mouse lock behavior
	PlayerController->SetInputMode(InputModeData); // Set the input mode
	PlayerController->SetShowMouseCursor(true); // Show the mouse cursor

	UGameInstance* GameInstance = GetGameInstance(); // Get the game instance
	if (!GameInstance) return;

	DGMSSubsystem = GameInstance->GetSubsystem<UDGMSSubsystem>(); // Get the subsystem
}

bool UDGMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (!B_HostButton || !B_JoinButton) // If the buttons are not valid
	{
		return false;
	}

	// Bind the buttons to the functions
	B_HostButton->OnClicked.AddDynamic(this, &ThisClass::HostButtonClicked);
	B_JoinButton->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked);
	
	return true;
}

void UDGMenu::NativeDestruct()
{
	MenuTearDown();
	
	Super::NativeDestruct();
}

void UDGMenu::HostButtonClicked()
{
	if (!DGMSSubsystem) return;

	DGMSSubsystem->HostSession(MaxNumPlayers, MatchType);

	UWorld* World = GetWorld(); // Get the world
	if (!World) return;

	World->ServerTravel("/Game/ThirdPerson/Maps/Lobby?listen");
	
}

void UDGMenu::JoinButtonClicked()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Join Button Clicked"));
	}
}

void UDGMenu::MenuTearDown()
{
	RemoveFromParent();
	UWorld* World = GetWorld(); // Get the world
	if (!World) return;

	APlayerController* PlayerController = World->GetFirstPlayerController(); // Get the player controller
	if (!PlayerController) return;

	const FInputModeGameOnly InputModeData; // Create an input mode data object
	PlayerController->SetInputMode(InputModeData); // Set the input mode
	PlayerController->SetShowMouseCursor(false); // Hide the mouse cursor
}
