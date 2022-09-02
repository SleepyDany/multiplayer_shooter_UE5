// Test Multiplayer Shooter Game. All Rights Reserved.


#include "Weapon/MSBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"

AMSBaseWeapon::AMSBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
}

void AMSBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}
