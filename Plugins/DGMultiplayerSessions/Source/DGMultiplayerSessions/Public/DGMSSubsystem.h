// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "DGMSSubsystem.generated.h"

/**
 * Declaring our own custom delegates for the menu class to bind ballbacks to
 */

//Dynamic is for blueprint as its a UFUNCTION
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDGMOnHostSessionComplete, bool, bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FDGMOnFindSessionsComplete, const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_OneParam(FDGMOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDGMOnDestroySessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDGMOnStartSessionComplete, bool, bWasSuccessful);

UCLASS()
class DGMULTIPLAYERSESSIONS_API UDGMSSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UDGMSSubsystem();

	// To handle multiplayer functionality, the menu class will call these functions
	void HostSession(int32 MaxNumPlayers, FString MatchType);
	void FindSessions(int32 MaxSearchResults);
	void JoinSession(const FOnlineSessionSearchResult& SearchResult);
	void DestroySession();
	void StartSession();

	// Custom delegates for the menu class to bind callbacks to
	FDGMOnHostSessionComplete DGMOnHostSessionComplete;
	FDGMOnFindSessionsComplete DGMOnFindSessionsComplete;
	FDGMOnJoinSessionComplete DGMOnJoinSessionComplete;
	FDGMOnDestroySessionComplete DGMOnDestroySessionComplete;
	FDGMOnStartSessionComplete DGMOnStartSessionComplete;
	
protected:

	//Internal callbacks for the delegates that we will add to the online session interface delegate list
	//These don't need to be called outside this class
	void OnHostSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);

private:
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;

	// To add to the Online Session Interface delegate list
	// We bind our MultiplayerSessionSubsystem internal callbacks to these delegates
	FOnCreateSessionCompleteDelegate OnHostSessionCompleteDelegate;
	FDelegateHandle OnHostSessionCompleteDelegateHandle;
	
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;
	
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
	
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;
	
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;

	bool bHostSessionOnDestroy{false};
	int32 LastMaxNumPlayers;
	FString LastMatchType;
};


