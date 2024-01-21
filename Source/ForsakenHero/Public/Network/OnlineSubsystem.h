// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "OnlineSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class FORSAKENHERO_API UOnlineSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	UOnlineSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
protected:
	UFUNCTION(BlueprintCallable)
	void CreateGameSession();

	UFUNCTION(BlueprintCallable)
	void JoinGameSession();

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful) const;
	void OnFindSessionsComplete(bool bWasSuccessful) const;
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) const;

protected:
	IOnlineSessionPtr OnlineSessionInterface;
	
private:
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;
};
