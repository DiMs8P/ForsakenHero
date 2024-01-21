// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CollisionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORSAKENHERO_API UCollisionComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnHitDelegate, const FHitResult&);

public:
	UCollisionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetAlreadyHitActors() const;

	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetIgnoreActors() const;

	UFUNCTION(BlueprintCallable)
	bool GetIsCollisionEnabled() const;

	UFUNCTION(BlueprintCallable)
	FHitResult GetHitResult() const;

	UFUNCTION(BlueprintCallable)
	UPrimitiveComponent* GetCollisionMeshComponent() const;

	void SetCollisionMesh(UPrimitiveComponent* PrimitiveComponent);

	UFUNCTION(BlueprintCallable)
	void AddActorToIgnore(AActor* ActorToIgnore);

	UFUNCTION(BlueprintCallable)
	void RemoveIgnoreActor(AActor* ActorToRemove);

	UFUNCTION(BlueprintCallable)
	void CollisionTrace();

	void EnableCollision();
	void DisableCollision();

protected:
	virtual void BeginPlay() override;

public:
	FOnHitDelegate OnHitDelegate;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UPrimitiveComponent* CollisionComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<AActor*> AlreadyHitActors;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName StartSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName EndSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Radius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<TEnumAsByte<EObjectTypeQuery>> CollisionObjectTypes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FHitResult LastHit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	AActor* HitActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bIsCollisionEnabled;
};
