// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/ResetAttackAnimNotify.h"
#include "Components/CombatComponent.h"

void UResetAttackAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                    const FAnimNotifyEventReference& EventReference)
{
	const AActor* OwnerCharacter = MeshComp->GetOwner();
	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Fatal, TEXT("Can't get owner character"));
	}

	UCombatComponent* CombatComponent = OwnerCharacter->GetComponentByClass<UCombatComponent>();
	if (!CombatComponent)
	{
		UE_LOG(LogTemp, Fatal, TEXT("Can't get combat component"));
	}
	
	CombatComponent->ResetAttack();
	Super::Notify(MeshComp, Animation, EventReference);
}
