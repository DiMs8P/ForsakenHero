// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NNERuntimeCPU.h"
#include "FHBaseAICharacter.h"
#include "FHMLCharacter.generated.h"

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
class FORSAKENHERO_API AFHMLCharacter : public AFHBaseAICharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFHMLCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual FText GetEnemyLabel() const override;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateInputData(float Distance, float PlayerHp, float EnemyHp);

	UFUNCTION(BlueprintCallable)
	virtual void RunModel();
	
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
};
