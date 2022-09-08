// Test Multiplayer Shooter Game. All Rights Reserved.


#include "Components/MSWeaponComponent.h"
#include "Weapon/MSBaseWeapon.h"
#include <GameFramework/Character.h>
#include "Player/MSBaseCharacter.h"
#include <Engine/Engine.h>
#include <Net/UnrealNetwork.h>

UMSWeaponComponent::UMSWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UMSWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMSWeaponComponent, CurrentWeapon);
}


void UMSWeaponComponent::StartFire_Implementation()
{
	if (!CurrentWeapon)
		return;

	CurrentWeapon->StartFire();
	OnAmmoChanged.Broadcast();
}


void UMSWeaponComponent::StopFire_Implementation()
{ 
	if (!CurrentWeapon) 
		return; 
	
	CurrentWeapon->StopFire();
	OnAmmoChanged.Broadcast();
}


void UMSWeaponComponent::Reload_Implementation()
{
	if (!CurrentWeapon)
		return;

	CurrentWeapon->Reload();
	OnAmmoChanged.Broadcast();
}


void UMSWeaponComponent::Aim()
{
	if (!CurrentWeapon)
		return;

	CurrentWeapon->Aim();
}


void UMSWeaponComponent::SpawnWeapon()
{
	auto Owner = Cast<AMSBaseCharacter>(GetOwner());
	if (!Owner)
		return;

	if (!GetWorld())
		return;

	CurrentWeapon = GetWorld()->SpawnActor<AMSBaseWeapon>(WeaponClass);
	if (!CurrentWeapon)
		return;

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	CurrentWeapon->AttachToComponent(Owner->GetMesh(), AttachmentRules, WeaponAttachPointName);
	CurrentWeapon->SetOwner(Owner);

	OnAmmoChanged.Broadcast();
}

int32 UMSWeaponComponent::GetCurrentAmmo() const
{
	return !CurrentWeapon ? 0 : CurrentWeapon->GetCurrentAmmo();
}

int32 UMSWeaponComponent::GetAllAmmo() const
{ 
	return !CurrentWeapon ? 0 : CurrentWeapon->GetAllAmmo();
}
