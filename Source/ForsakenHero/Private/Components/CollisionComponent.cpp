// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CollisionComponent.h"

UCollisionComponent::UCollisionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsCollisionEnabled)
	{
		CollisionTrace();
	}
}

void UCollisionComponent::BeginPlay()
{
	Super::BeginPlay();
}

FORCEINLINE TArray<AActor*> UCollisionComponent::GetAlreadyHitActors() const
{
	return AlreadyHitActors;
}

FORCEINLINE TArray<AActor*> UCollisionComponent::GetIgnoreActors() const
{
	return ActorsToIgnore;
}

FORCEINLINE bool UCollisionComponent::GetIsCollisionEnabled() const
{
	return bIsCollisionEnabled;
}

FORCEINLINE FHitResult UCollisionComponent::GetHitResult() const
{
	return LastHit;
}

FORCEINLINE UPrimitiveComponent* UCollisionComponent::GetCollisionMeshComponent() const
{
	return CollisionComponent;
}

void UCollisionComponent::SetCollisionMesh(UPrimitiveComponent* PrimitiveComponent)
{
	CollisionComponent = PrimitiveComponent;
}

void UCollisionComponent::AddActorToIgnore(AActor* ActorToIgnore)
{
	ActorsToIgnore.AddUnique(ActorToIgnore);
}

void UCollisionComponent::RemoveIgnoreActor(AActor* ActorToRemove)
{
	ActorsToIgnore.Remove(ActorToRemove);
}

void UCollisionComponent::CollisionTrace()
{
	const FVector StartLocation = CollisionComponent->GetSocketLocation(StartSocketName);
	const FVector EndLocation = CollisionComponent->GetSocketLocation(EndSocketName);

	TArray<FHitResult> OutHits;
	UKismetSystemLibrary::SphereTraceMultiForObjects(this, StartLocation, EndLocation, Radius, CollisionObjectTypes, false, ActorsToIgnore, DrawDebugType, OutHits, true);

	for (const FHitResult& OutHit : OutHits)
	{
		if(!AlreadyHitActors.Contains(OutHit.GetActor()))
		{
			AlreadyHitActors.Add(OutHit.GetActor());
			HitActor = OutHit.GetActor();

			OnHitDelegate.Broadcast(OutHit);
		}
	}
}

void UCollisionComponent::EnableCollision()
{
	AlreadyHitActors.Empty();
	bIsCollisionEnabled = true;
}

void UCollisionComponent::DisableCollision()
{
	bIsCollisionEnabled = false;
}
