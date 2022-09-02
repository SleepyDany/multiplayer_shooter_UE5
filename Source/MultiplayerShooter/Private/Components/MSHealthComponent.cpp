// Test Multiplayer Shooter Game. All Rights Reserved.


#include "Components/MSHealthComponent.h"
#include "GameFramework/Actor.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

UMSHealthComponent::UMSHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UMSHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UMSHealthComponent::OnTakeAnyDamage);
	}
}

void UMSHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogHealthComponent, Display, TEXT("Damage: %f"), Damage);
	Health -= Damage;
}
