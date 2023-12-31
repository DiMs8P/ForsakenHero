// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NNERuntimeCPU.h"
#include "GameFramework/Character.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "FHBaseCharacter.generated.h"

class FMyModelHelper
{
public:
	TUniquePtr<UE::NNE::IModelInstanceCPU> ModelInstance;
	TArray<float> InputData = {10, 100, 100, 100, 100, 100};
	TArray<float> OutputData = {0, 0, 0};
	TArray<UE::NNE::FTensorBindingCPU> InputBindings;
	TArray<UE::NNE::FTensorBindingCPU> OutputBindings;
	bool bIsRunning = false;
	bool bIsActualData = false;
};

UCLASS()
class FORSAKENHERO_API AFHBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFHBaseCharacter();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void OnDeath();

	UFUNCTION(BlueprintCallable)
	virtual void RunModel();

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
	class UHealthComponent* HealthComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = "true"))
	TArray<class UAnimMontage*> DeathMontages;

	UPROPERTY(EditDefaultsOnly, Category = Damage)
	float LifeSpanOnDeath = 5.0f;

	
#pragma region Multiplayer
public:
	IOnlineSessionPtr OnlineSessionInterface;

protected:
	UFUNCTION(BlueprintCallable)
	void CreateGameSession();

	UFUNCTION(BlueprintCallable)
	void JoinGameSession();

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

private:
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;
#pragma endregion

#pragma region MachineLearning
public:
	UFUNCTION(BlueprintCallable)
	void UpdateInputData(float Distance, float PlayerHp, float EnemyHp);
	
	UFUNCTION(BlueprintCallable)
	int GetAction() const;

	UFUNCTION(BlueprintCallable)
	bool GetIsModelRunning() const;

	UFUNCTION(BlueprintCallable)
	bool GetIsModelActualData() const;

	UFUNCTION(BlueprintCallable)
	void SetIsModelActualData(bool bActualData);
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UNNEModelData> PreLoadedModelData;

private:
	TSharedPtr<FMyModelHelper> ModelHelper;
#pragma endregion
};
