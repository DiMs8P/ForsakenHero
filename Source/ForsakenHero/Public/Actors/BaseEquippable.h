// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseEquippable.generated.h"

UCLASS()
class FORSAKENHERO_API ABaseEquippable : public AActor
{
	GENERATED_BODY()

public:
	ABaseEquippable();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetIsEquipped();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UPrimitiveComponent* GetItemMesh();

	UFUNCTION(BlueprintCallable)
	void AttachActor(const FName& SocketName);

	UFUNCTION(BlueprintCallable)
	virtual void OnEquipped();

	UFUNCTION(BlueprintCallable)
	void OnUnequipped();

	UFUNCTION(BlueprintCallable)
	void SetIsEquipped(bool IsEquipped);

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName AttachSocketName;

private:
	bool bIsEquipped = false;
};
