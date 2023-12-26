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
#include "NNERuntimeCPU.h"

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

    if (PreLoadedModelData)
	{
		// Example for model creation
		TWeakInterfacePtr<INNERuntimeCPU> Runtime = UE::NNE::GetRuntime<INNERuntimeCPU>(FString("NNERuntimeORTCpu"));
		if (Runtime.IsValid())
		{
			ModelHelper = MakeShared<FMyModelHelper>();
			
			TUniquePtr<UE::NNE::IModelCPU> Model = Runtime->CreateModel(PreLoadedModelData);
			if (Model.IsValid())
			{
				ModelHelper->ModelInstance = Model->CreateModelInstance();
				if (ModelHelper->ModelInstance.IsValid())
				{
					ModelHelper->bIsRunning = false;
 
					// Example for querying and testing in- and outputs
					TConstArrayView<UE::NNE::FTensorDesc> InputTensorDescs = ModelHelper->ModelInstance->GetInputTensorDescs();
					checkf(InputTensorDescs.Num() == 1, TEXT("The current example supports only models with a single input tensor"));
					UE::NNE::FSymbolicTensorShape SymbolicInputTensorShape = InputTensorDescs[0].GetShape();
					checkf(SymbolicInputTensorShape.IsConcrete(), TEXT("The current example supports only models without variable input tensor dimensions"));
					TArray<UE::NNE::FTensorShape> InputTensorShapes = { UE::NNE::FTensorShape::MakeFromSymbolic(SymbolicInputTensorShape) };
 
					ModelHelper->ModelInstance->SetInputTensorShapes(InputTensorShapes);
 
					TConstArrayView<UE::NNE::FTensorDesc> OutputTensorDescs = ModelHelper->ModelInstance->GetOutputTensorDescs();
					checkf(OutputTensorDescs.Num() == 1, TEXT("The current example supports only models with a single output tensor"));
					UE::NNE::FSymbolicTensorShape SymbolicOutputTensorShape = OutputTensorDescs[0].GetShape();
					checkf(SymbolicOutputTensorShape.IsConcrete(), TEXT("The current example supports only models without variable output tensor dimensions"));
					TArray<UE::NNE::FTensorShape> OutputTensorShapes = { UE::NNE::FTensorShape::MakeFromSymbolic(SymbolicOutputTensorShape) };
 
					// Example for creating in- and outputs
					ModelHelper->InputData.SetNumZeroed(InputTensorShapes[0].Volume());
					ModelHelper->InputBindings.SetNumZeroed(1);
					ModelHelper->InputBindings[0].Data = ModelHelper->InputData.GetData();
					ModelHelper->InputBindings[0].SizeInBytes = ModelHelper->InputData.Num() * sizeof(float);
 
					ModelHelper->OutputData.SetNumZeroed(OutputTensorShapes[0].Volume());
					ModelHelper->OutputBindings.SetNumZeroed(1);
					ModelHelper->OutputBindings[0].Data = ModelHelper->OutputData.GetData();
					ModelHelper->OutputBindings[0].SizeInBytes = ModelHelper->OutputData.Num() * sizeof(float);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to create the model instance"));
					ModelHelper.Reset();
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to create the model"));
				ModelHelper.Reset();
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Cannot find runtime NNERuntimeORTCpu, please enable the corresponding plugin"));
		}
	}
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
PRAGMA_DISABLE_OPTIMIZATION
void AFHBaseCharacter::RunModel()
{
	if (ModelHelper.IsValid())
	{
		// Example for async inference
		if (!ModelHelper->bIsRunning)
		{
			// Process ModelHelper->OutputData from the previous run here
			// Fill in new data into ModelHelper->InputData here
 
			ModelHelper->bIsRunning = true;
			TSharedPtr<FMyModelHelper> ModelHelperPtr = ModelHelper;
			AsyncTask(ENamedThreads::AnyNormalThreadNormalTask, [ModelHelperPtr]()
			{
				if (ModelHelperPtr->ModelInstance->RunSync(ModelHelperPtr->InputBindings, ModelHelperPtr->OutputBindings) != 0)
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to run the model"));
				}
				AsyncTask(ENamedThreads::GameThread, [ModelHelperPtr]()
				{
					ModelHelperPtr->bIsActualData = true;
					ModelHelperPtr->bIsRunning = false;
				});
			});
			
		}
	}
}
PRAGMA_ENABLE_OPTIMIZATION
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
PRAGMA_DISABLE_OPTIMIZATION
void AFHBaseCharacter::UpdateInputData(float Distance, float PlayerHp, float EnemyHp, FVector PlayerLocation, FVector EnemyLocation)
{
	ModelHelper->InputData[0] = Distance;
	ModelHelper->InputData[1] = PlayerHp;
	ModelHelper->InputData[2] = EnemyHp;
	ModelHelper->InputData[3] = EnemyLocation[0];
	ModelHelper->InputData[4] = EnemyLocation[1];
	ModelHelper->InputData[5] = EnemyLocation[2];
	ModelHelper->InputData[6] = PlayerLocation[0];
	ModelHelper->InputData[7] = PlayerLocation[1];
	ModelHelper->InputData[8] = PlayerLocation[2];
}

int AFHBaseCharacter::GetAction() const
{
	int returnValue = 2;
	if(ModelHelper->OutputData[0] >= ModelHelper->OutputData[1]) {
		if(ModelHelper->OutputData[0] >= ModelHelper->OutputData[2])
		{
			returnValue = 0;
		}
		else
		{
			returnValue = 2;
		}
	}
	else {
		if(ModelHelper->OutputData[1] >= ModelHelper->OutputData[2])
		{
			returnValue = 1;
		}
		else
		{
			returnValue = 2;
		}
	}

	return returnValue;
}
PRAGMA_ENABLE_OPTIMIZATION

bool AFHBaseCharacter::GetIsModelRunning() const
{
	return ModelHelper->bIsRunning;
}

bool AFHBaseCharacter::GetIsModelActualData() const
{
	return ModelHelper->bIsActualData;
}

void AFHBaseCharacter::SetIsModelActualData(bool bActualData)
{
	ModelHelper->bIsActualData = bActualData;
}


