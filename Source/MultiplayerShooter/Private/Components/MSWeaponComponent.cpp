// Test Multiplayer Shooter Game. All Rights Reserved.


#include "Components/MSWeaponComponent.h"
#include "Weapon/MSBaseWeapon.h"
#include <GameFramework/Character.h>

UMSWeaponComponent::UMSWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UMSWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapon();
}


void UMSWeaponComponent::Fire()
{
	if (!CurrentWeapon)
		return;

	CurrentWeapon->Fire();
}

void UMSWeaponComponent::SpawnWeapon()
{
	if (!GetWorld())
		return;

	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	if (!Owner)
		return;

	CurrentWeapon = GetWorld()->SpawnActor<AMSBaseWeapon>(WeaponClass);
	if (!CurrentWeapon)
		return;

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	CurrentWeapon->AttachToComponent(Owner->GetMesh(), AttachmentRules, WeaponAttachPointName);
}
