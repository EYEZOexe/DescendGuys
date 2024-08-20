// Fill out your copyright notice in the Description page of Project Settings.


#include "DGMenu.h"
#include "Components/Button.h"
#include "DGMSSubsystem.h"
#include "OnlineSessionSettings.h"


void UDGMenu::MenuSetup(int32 MaxNumberPlayers, FString TypeOfMatch, FString LobbyPath)
{
	PathToLobby = FString::Printf(TEXT("%s?listen"), *LobbyPath);
	MaxNumPlayers = MaxNumberPlayers;
	MatchType = TypeOfMatch;
	
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	SetIsFocusable(true);
	

	const UWorld* World = GetWorld(); // Get the world
	if (!World) return;

	APlayerController* PlayerController = World->GetFirstPlayerController(); // Get the player controller
	if (!PlayerController) return;

	FInputModeUIOnly InputModeData; // Create an input mode data object
	InputModeData.SetWidgetToFocus(TakeWidget()); // Set the widget to focus
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // Set the mouse lock behavior
	PlayerController->SetInputMode(InputModeData); // Set the input mode
	PlayerController->SetShowMouseCursor(true); // Show the mouse cursor

	const UGameInstance* GameInstance = GetGameInstance(); // Get the game instance
	if (!GameInstance) return;

	DGMSSubsystem = GameInstance->GetSubsystem<UDGMSSubsystem>(); // Get the subsystem

	if (!DGMSSubsystem) return;

	DGMSSubsystem->DGMOnHostSessionComplete.AddDynamic(this, &ThisClass::OnHostSession);
	DGMSSubsystem->DGMOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessions); //we use AddUObject for non-dynamic functions
	DGMSSubsystem->DGMOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
	DGMSSubsystem->DGMOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
	DGMSSubsystem->DGMOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSession);
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

void UDGMenu::OnHostSession(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Host Session Successful"));
		}

		UWorld* World = GetWorld(); // Get the world
		if (!World) return;

		World->ServerTravel(PathToLobby); // Server travel to the lobby
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Host Session Failed"));
		}
		B_HostButton->SetIsEnabled(true);
	}
}

void UDGMenu::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
	if (DGMSSubsystem == nullptr) return;
	
	for (auto Result : SessionResults)
	{
		FString SettingsValue;
		Result.Session.SessionSettings.Get(FName("MatchType"), SettingsValue);
		if (SettingsValue == MatchType)
		{
			DGMSSubsystem->JoinSession(Result);
			return;
		}
	}
	
	if (!bWasSuccessful || SessionResults.Num() == 0)
	{
		B_JoinButton->SetIsEnabled(true);
	}
}

void UDGMenu::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	if (const IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	{
		if (const IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface(); SessionInterface.IsValid())
		{
			FString Address;
			SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);

			if (APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController())
			{
				PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
		}
	}

	if (Result != EOnJoinSessionCompleteResult::Success)
	{
		B_JoinButton->SetIsEnabled(true);
	}
}

void UDGMenu::OnDestroySession(bool bWasSuccessful)
{
	
}

void UDGMenu::OnStartSession(bool bWasSuccessful)
{
	
}

void UDGMenu::HostButtonClicked()
{
	B_HostButton->SetIsEnabled(false);
	if (!DGMSSubsystem) return;

	DGMSSubsystem->HostSession(MaxNumPlayers, MatchType);
}

void UDGMenu::JoinButtonClicked()
{
	B_JoinButton->SetIsEnabled(false);
	if (!DGMSSubsystem) return;

	DGMSSubsystem->FindSessions(10000);
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
