// Fill out your copyright notice in the Description page of Project Settings.

#include "Network/OnlineSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"

// TODO Mb move to plugin
UOnlineSubsystem::UOnlineSubsystem():
    CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &UOnlineSubsystem::OnCreateSessionComplete)),
    FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &UOnlineSubsystem::OnFindSessionsComplete)),
    JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &UOnlineSubsystem::OnJoinSessionComplete))
{
}

void UOnlineSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    if (const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Blue,
				FString::Printf(TEXT("Found subsystem %s"), *OnlineSubsystem->GetSubsystemName().ToString()));
		}
	}
	
	Super::Initialize(Collection);
}

void UOnlineSubsystem::CreateGameSession()
{
    if (!OnlineSessionInterface.IsValid())
    {
        return;
    }

    const auto ExistingSession = OnlineSessionInterface->GetNamedSession(NAME_GameSession);
    if (ExistingSession != nullptr)
    {
        OnlineSessionInterface->DestroySession(NAME_GameSession);
    }
    OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

    const TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());
    SessionSettings->bIsLANMatch = false;
    SessionSettings->NumPublicConnections = 4;
    SessionSettings->bAllowJoinInProgress = true;
    SessionSettings->bAllowJoinViaPresence = true;
    SessionSettings->bShouldAdvertise = true;
    SessionSettings->bUsesPresence = true;
    SessionSettings->bUseLobbiesIfAvailable = true;
    SessionSettings->Set(FName("MatchType"), FString("FreeForAll"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
    
    const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
    OnlineSessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *SessionSettings);
}

void UOnlineSubsystem::JoinGameSession()
{
    if (!OnlineSessionInterface.IsValid())
    {
        return;
    }
    OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);
    
    SessionSearch = MakeShareable(new FOnlineSessionSearch());
    SessionSearch->MaxSearchResults = 10000;
    SessionSearch->bIsLanQuery = false;
    SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

    const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
    OnlineSessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
}

void UOnlineSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful) const
{
    if (bWasSuccessful)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1,
                15.f,
                FColor::Green,
                FString::Printf(TEXT("Created session: %s"), *SessionName.ToString()));
        }

        if (UWorld* World = GetWorld()){
            World->ServerTravel(FString("/Game/ThirdPerson/Maps/TrainingMap?listen"));
        }
    }
    else
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1,
                15.f,
                FColor::Red,
                FString(TEXT("Failed to create session!")));
        }
    }
}

void UOnlineSubsystem::OnFindSessionsComplete(bool bWasSuccessful) const
{
    if (!OnlineSessionInterface.IsValid())
    {
        return;
    }
    
    for (FOnlineSessionSearchResult SearchResult : SessionSearch->SearchResults)
    {
        FString Id = SearchResult.GetSessionIdStr();
        FString User = SearchResult.Session.OwningUserName;
        FString MatchType;
        SearchResult.Session.SessionSettings.Get(FName("MatchType"), MatchType);

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1,
                15.f,
                FColor::Cyan,
                FString::Printf(TEXT("Id: %s, User: %s"), *Id, *User));
        }

        if (MatchType == FString("FreeForAll"))
        {
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(
                    -1,
                    15.f,
                    FColor::Cyan,
                    FString::Printf(TEXT("Joining Match Type %s"), *MatchType));
            }

            OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

            const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
            OnlineSessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SearchResult);
        }
    }
    
}

void UOnlineSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) const
{
    if (!OnlineSessionInterface.IsValid())
    {
        return;
    }

    FString Adress;
    if (OnlineSessionInterface->GetResolvedConnectString(NAME_GameSession, Adress))
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1,
                15.f,
                FColor::Yellow,
                FString::Printf(TEXT("Connect string %s"), *Adress));
        }
    }

    if (APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController())
    {
        PlayerController->ClientTravel(Adress, ETravelType::TRAVEL_Absolute);
    }
}
