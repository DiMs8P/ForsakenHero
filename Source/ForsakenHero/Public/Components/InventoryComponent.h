// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

USTRUCT(Blueprintable)
struct FConsumableItem
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UClass* ItemClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int NumOfItems;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORSAKENHERO_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

	void ConsumeItem(const FString& ItemId);

	int GetItemsNum(const FString& ItemId) const;
	bool CanConsumeItem(const FString& ItemId) const;

protected:
	virtual void BeginPlay() override;

private:
	bool IsItemValid(const FString& ItemId) const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<FString, FConsumableItem> Inventory;
};
