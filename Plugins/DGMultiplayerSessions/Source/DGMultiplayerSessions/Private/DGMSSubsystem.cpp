// Fill out your copyright notice in the Description page of Project Settings.


#include "DGMSSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

UDGMSSubsystem::UDGMSSubsystem():
	OnHostSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnHostSessionComplete)),
	OnFindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsComplete)),
	OnJoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete)),
	OnDestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionComplete)),
	OnStartSessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionComplete))
{
	if (const IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	{
		SessionInterface = Subsystem->GetSessionInterface();
	}
}

void UDGMSSubsystem::HostSession(int32 MaxNumPlayers, FString MatchType)
{
	if (!SessionInterface.IsValid()) return;

	// get the existing session
	if (const auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession); ExistingSession != nullptr)
	{
		// destroy the existing session
		SessionInterface->DestroySession(NAME_GameSession);
	}

	//store the delegate in a FDelegateHandle, so we can later remove it from the delegate list
	OnHostSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(OnHostSessionCompleteDelegate);

	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	LastSessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	LastSessionSettings->NumPublicConnections = MaxNumPlayers;
	LastSessionSettings->bAllowJoinInProgress = true;
	LastSessionSettings->bAllowJoinViaPresence = true;
	LastSessionSettings->bShouldAdvertise = true;
	LastSessionSettings->bUsesPresence = true;
	LastSessionSettings->Set(FName("MatchType"), MatchType, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	if (const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController(); !SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings))
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(OnHostSessionCompleteDelegateHandle);
	}
}

void UDGMSSubsystem::FindSessions(int32 MaxSearchResults)
{
	
}

void UDGMSSubsystem::JoinSession(const FOnlineSessionSearchResult& SearchResult)
{
	
}

void UDGMSSubsystem::DestroySession()
{
	
}

void UDGMSSubsystem::StartSession()
{
	
}
