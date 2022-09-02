// Test Multiplayer Shooter Game. All Rights Reserved.


#include "Weapon/MSBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AMSBaseWeapon::AMSBaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
}

// Called when the game starts or when spawned
void AMSBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}
