// Test Multiplayer Shooter Game. All Rights Reserved.


#include "Components/MSHealthComponent.h"
#include "GameFramework/Actor.h"
#include <Engine/Engine.h>
#include <Net/UnrealNetwork.h>

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

UMSHealthComponent::UMSHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMSHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMSHealthComponent, Health);
}

void UMSHealthComponent::OnRep_Health()
{
	OnHealthChanged.Broadcast();
}

void UMSHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	OnHealthChanged.Broadcast();

	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UMSHealthComponent::OnTakeAnyDamage);
	}
}

void UMSHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || IsDead())
		return;
	
	SetHealth(Health - Damage);

	if (IsDead())
	{
		OnDeath.Broadcast();
	}
}

void UMSHealthComponent::SetHealth(float HealthValue)
{
	if (GetOwner()->GetLocalRole() == ROLE_Authority)
	{
		Health = FMath::Clamp(HealthValue, 0.0f, MaxHealth);
		OnHealthChanged.Broadcast();
	}
}
