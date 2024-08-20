// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DGMenu.generated.h"

/**
 * 
 */
UCLASS()
class DGMULTIPLAYERSESSIONS_API UDGMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void MenuSetup(int32 MaxNumberPlayers = 4, FString TypeOfMatch = FString(TEXT("FreeForAll")));

protected:
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;
	
private:

	// This macro binds the widget to the variable make sure the variable name is the same as the widget name
	UPROPERTY(meta = (BindWidget)) 
	class UButton* B_HostButton;

	UPROPERTY(meta = (BindWidget))
	UButton* B_JoinButton;

	UFUNCTION()
	void HostButtonClicked();
	
	UFUNCTION()
	void JoinButtonClicked();

	void MenuTearDown();

	// subsystem designed to handle online multiplayer functionality
	class UDGMSSubsystem* DGMSSubsystem;

	int32 MaxNumPlayers{4};
	FString MatchType{TEXT("FreeForAll")};
	
};
