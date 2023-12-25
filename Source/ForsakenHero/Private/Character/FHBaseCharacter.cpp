// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FHBaseCharacter.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Components/CapsuleComponent.h"
#include "Components/HealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/WeakInterfacePtr.h"
#include "NNE.h"
#include "NNEModelData.h"

// Sets default values
AFHBaseCharacter::AFHBaseCharacter() :
    CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &AFHBaseCharacter::OnCreateSessionComplete)),
    FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &AFHBaseCharacter::OnFindSessionsComplete)),
    JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &AFHBaseCharacter::OnJoinSessionComplete))
{
    bReplicates = true;
    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
}

// Called when the game starts or when spawned
void AFHBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

    check(HealthComponent);
    HealthComponent->OnDeath.AddUObject(this, &AFHBaseCharacter::OnDeath);
    
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
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

    checkf(DeathMontages.Num() > 0, TEXT("Death animations forgotten to set"));
}

// Called to bind functionality to input
void AFHBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AFHBaseCharacter::OnDeath()
{
    PlayAnimMontage(DeathMontages[FMath::RandRange(0, DeathMontages.Num() - 1)]);
    
    GetCharacterMovement()->DisableMovement();

    SetLifeSpan(LifeSpanOnDeath);
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void AFHBaseCharacter::CreateGameSession()
{
    if (!OnlineSessionInterface.IsValid())
    {
        return;
    }

    auto ExistingSession = OnlineSessionInterface->GetNamedSession(NAME_GameSession);
    if (ExistingSession != nullptr)
    {
        OnlineSessionInterface->DestroySession(NAME_GameSession);
    }
    OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);
    
    TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());
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

void AFHBaseCharacter::JoinGameSession()
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

void AFHBaseCharacter::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
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

        UWorld* World = GetWorld();
        if (World){
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

void AFHBaseCharacter::OnFindSessionsComplete(bool bWasSuccessful)
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

void AFHBaseCharacter::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
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

    APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
    if (PlayerController)
    {
        PlayerController->ClientTravel(Adress, ETravelType::TRAVEL_Absolute);
    }
}

