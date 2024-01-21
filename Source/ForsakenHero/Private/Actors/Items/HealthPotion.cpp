// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Items/HealthPotion.h"

#include "Components/HealthComponent.h"

void UHealthPotion::OnConsume(UObject* WorldContextObject)
{
	const AActor* Target = Cast<AActor>(WorldContextObject);
	if(!Target)
	{
		UE_LOG(LogTemp, Fatal, TEXT("Instigator must be aactor"))
	}

	UHealthComponent* HealthComponent = Target->GetComponentByClass<UHealthComponent>();
	if (!HealthComponent)
	{
		UE_LOG(LogTemp, Fatal, TEXT("Instigator must have health component"))
	}

	HealthComponent->AddHealth(AdditionalHealth);
}
