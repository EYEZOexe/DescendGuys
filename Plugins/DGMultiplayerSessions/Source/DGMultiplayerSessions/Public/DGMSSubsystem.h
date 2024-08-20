// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "DGMSSubsystem.generated.h"

/**
 * 
 */
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
};

inline void UDGMSSubsystem::OnHostSessionComplete(FName SessionName, bool bWasSuccessful)
{
	
}

inline void UDGMSSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
	
}

inline void UDGMSSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	
}

inline void UDGMSSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	
}

inline void UDGMSSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
	
}
