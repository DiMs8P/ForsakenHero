// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FHBaseCharacter.h"

#include "Actors/Items/HealthPotion.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/WeakInterfacePtr.h"
#include "Components/CombatComponent.h"
#include "Actors/Weapons/BaseWeapon.h"
#include "Components/InputComponent.h"
#include "Components/HealthComponent.h"
#include "Components/InventoryComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AFHBaseCharacter::AFHBaseCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	
    bReplicates = true;
    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
}

// Called when the game starts or when spawned
void AFHBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

    check(HealthComponent);
    HealthComponent->OnDeath.AddUObject(this, &AFHBaseCharacter::OnDeath);

	check(CombatComponent);
	check(InventoryComponent);

    checkf(DeathMontages.Num() > 0, TEXT("Death animations forgotten to set"));
	checkf(DodgeMontages.Num() > 0, TEXT("Dodge animations forgotten to set"));
	checkf(HealingMontages.Num() > 0, TEXT("Healing animations forgotten to set"));
    checkf(HitMontages.Num() > 0, TEXT("Hit animations forgotten to set"));

	OnTakeAnyDamage.AddDynamic(this, &AFHBaseCharacter::OnTakeAnyDamageHandle);
}

void AFHBaseCharacter::Dodge()
{
	if(CanPerformAction())
	{
		Dodge_Internal(0);
	}
}

void AFHBaseCharacter::OnDeath()
{
    PlayAnimMontage(DeathMontages[FMath::RandRange(0, DeathMontages.Num() - 1)]);
    
    GetCharacterMovement()->DisableMovement();

    SetLifeSpan(LifeSpanOnDeath);
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void AFHBaseCharacter::OnTakeAnyDamageHandle(AActor* DamageActor, float Damage, const UDamageType* DamageType,
	AController* Instigatebly, AActor* DamageCauser)
{
	if (CanReceiveDamage())
	{
		UAnimMontage* SelectedHitAnimMontage = HitMontages[FMath::RandRange(0, HitMontages.Num() - 1)];
		PlayAnimMontage(SelectedHitAnimMontage);
	
		IsDisabled = true;
	}
}

void AFHBaseCharacter::ResetCombat()
{
	CombatComponent->ResetAttack();
	
	bIsDodging = false;
	IsDisabled = false;
	bIsHealing = false;
}

void AFHBaseCharacter::Heal()
{
	if(CanPerformHeal())
	{
		Heal_Internal(0);
	}
}

void AFHBaseCharacter::Dodge_Internal(int DodgeIndex, bool IsIndexRandom)
{
	const FRotator DesiredRotator = GetDesiredRotation();
	SetActorRotation(DesiredRotator);
	
	UAnimMontage* SelectedDodgeAnimMontage = IsIndexRandom ? DodgeMontages[FMath::RandRange(0, DodgeMontages.Num() - 1)] : DodgeMontages[DodgeIndex];
	PlayAnimMontage(SelectedDodgeAnimMontage);

	bIsDodging = true;
}

void AFHBaseCharacter::Heal_Internal(int HealIndex, bool IsIndexRandom)
{
	bIsHealing = true;

	UAnimMontage* SelectedHealingAnimMontage = IsIndexRandom ? HealingMontages[FMath::RandRange(0, HealingMontages.Num() - 1)] : HealingMontages[HealIndex];
	PlayAnimMontage(SelectedHealingAnimMontage);
}

bool AFHBaseCharacter::CanPerformAction() const
{
	return !CombatComponent->GetIsAttacking() && !bIsDodging && !IsDisabled && !bIsHealing && !HealthComponent->IsDead();
}

bool AFHBaseCharacter::CanPerformHeal() const
{
	return CanPerformAction() && InventoryComponent->CanConsumeItem("HealthPotion");
}

bool AFHBaseCharacter::CanReceiveDamage() const
{
	return !HealthComponent->IsDead();
}

FRotator AFHBaseCharacter::GetDesiredRotation() const
{
	if (GetCharacterMovement()->GetLastInputVector().Length() != UE_KINDA_SMALL_NUMBER)
	{
		return UKismetMathLibrary::MakeRotFromX(GetLastMovementInputVector());
	}

	return GetActorRotation();
}
