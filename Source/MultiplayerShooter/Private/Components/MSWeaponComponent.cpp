// Test Multiplayer Shooter Game. All Rights Reserved.


#include "Components/MSWeaponComponent.h"
#include "Weapon/MSBaseWeapon.h"
#include <GameFramework/Character.h>
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


void UMSWeaponComponent::Fire_Implementation()
{
	if (!CurrentWeapon)
		return;

	CurrentWeapon->Fire();
}


void UMSWeaponComponent::SpawnWeapon()
{
	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	if (!Owner)
		return;

	if (!GetWorld())
		return;

	CurrentWeapon = GetWorld()->SpawnActor<AMSBaseWeapon>(WeaponClass);
	if (!CurrentWeapon)
		return;

	FString spawnMessage = FString::Printf(TEXT("Spawned weapon %s for char %s. Role: %d"), *CurrentWeapon->GetName(), *Owner->GetName(), Owner->GetLocalRole());
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, spawnMessage);

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	CurrentWeapon->AttachToComponent(Owner->GetMesh(), AttachmentRules, WeaponAttachPointName);
	CurrentWeapon->SetOwner(Owner);
}
