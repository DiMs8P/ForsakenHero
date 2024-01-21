// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
#include "Actors/BaseConsumable.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryComponent::ConsumeItem(const FString& ItemId)
{
	if (CanConsumeItem(ItemId))
	{
		FConsumableItem& ConsumableItem = Inventory[ItemId];
		
		UBaseConsumable* Consumable = ConsumableItem.ItemClass->GetDefaultObject<UBaseConsumable>();
		Consumable->OnConsume(GetOwner());
		--ConsumableItem.NumOfItems;
	}
}

FORCEINLINE bool UInventoryComponent::CanConsumeItem(const FString& ItemId) const
{
	return GetItemsNum(ItemId) > 0;
}

int UInventoryComponent::GetItemsNum(const FString& ItemId) const
{
	if (IsItemValid(ItemId))
	{
		return Inventory[ItemId].NumOfItems;
	}

	UE_LOG(LogTemp, Fatal, TEXT("Item %s is not valid"), *ItemId)
	return -1;
}

FORCEINLINE bool UInventoryComponent::IsItemValid(const FString& ItemId) const
{
	return Inventory.Contains(ItemId) && Inventory[ItemId].ItemClass;
}
