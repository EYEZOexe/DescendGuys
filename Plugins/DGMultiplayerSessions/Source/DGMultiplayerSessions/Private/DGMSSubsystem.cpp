// Fill out your copyright notice in the Description page of Project Settings.


#include "DGMSSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"


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
		bHostSessionOnDestroy = true;
		LastMaxNumPlayers = MaxNumPlayers;
		LastMatchType = MatchType;

		DestroySession();
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
	LastSessionSettings->bUseLobbiesIfAvailable = true;
	LastSessionSettings->BuildUniqueId = 1;

	if (const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController(); !SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings))
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(OnHostSessionCompleteDelegateHandle);

		//broadcast our own custom delegate
		DGMOnHostSessionComplete.Broadcast(false);
	}
}

void UDGMSSubsystem::FindSessions(int32 MaxSearchResults)
{
	if (!SessionInterface.IsValid()) return;

	OnFindSessionsCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

	LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
	LastSessionSearch->MaxSearchResults = MaxSearchResults;
	LastSessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	if (const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController(); !SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef()))
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

		DGMOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
	}
}

void UDGMSSubsystem::JoinSession(const FOnlineSessionSearchResult& SearchResult)
{
	if (!SessionInterface.IsValid())
	{
		DGMOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}

	OnJoinSessionCompleteDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

	if (const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController(); !SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SearchResult))
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

		DGMOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}
}

void UDGMSSubsystem::DestroySession()
{
	if (!SessionInterface.IsValid())
	{
		DGMOnDestroySessionComplete.Broadcast(false);
		return;
	}
	
	// destroy the existing session
	OnDestroySessionCompleteDelegateHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);

	if (!SessionInterface->DestroySession(NAME_GameSession))
	{
		//clear delegate
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
		DGMOnDestroySessionComplete.Broadcast(false);
	}
}

void UDGMSSubsystem::StartSession()
{
	
}

void UDGMSSubsystem::OnHostSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (!SessionInterface.IsValid()) return;

	SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(OnHostSessionCompleteDelegateHandle);

	DGMOnHostSessionComplete.Broadcast(bWasSuccessful);
}

void UDGMSSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (!SessionInterface.IsValid()) return;

	SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

	if (LastSessionSearch->SearchResults.Num() <= 0)
	{
		DGMOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}
	
	DGMOnFindSessionsComplete.Broadcast(LastSessionSearch->SearchResults, bWasSuccessful);
}

void UDGMSSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid()) return;

	SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

	DGMOnJoinSessionComplete.Broadcast(Result);
}

void UDGMSSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (!SessionInterface.IsValid()) return;

	SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);

	if (bWasSuccessful && bHostSessionOnDestroy)
	{
		bHostSessionOnDestroy = false;
		HostSession(LastMaxNumPlayers, LastMatchType);
	}

	DGMOnDestroySessionComplete.Broadcast(bWasSuccessful);
	
}

void UDGMSSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
	
}
