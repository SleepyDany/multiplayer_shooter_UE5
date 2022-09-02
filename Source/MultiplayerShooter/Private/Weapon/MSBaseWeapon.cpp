// Test Multiplayer Shooter Game. All Rights Reserved.


#include "Weapon/MSBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include <Engine/World.h>
#include <DrawDebugHelpers.h>

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

void AMSBaseWeapon::Fire()
{
	MakeShot();
}

void AMSBaseWeapon::MakeShot()
{
	if (!GetWorld())
		return;

	const FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);
	const FVector TraceStart = SocketTransform.GetLocation();
	const FVector TraceDirection = SocketTransform.GetRotation().GetForwardVector();
	const FVector TraceEnd = TraceStart + TraceDirection * TraceMaxDistance;

	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Orange, false, 3.0f, 0, 2.0f);

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);

	if (HitResult.bBlockingHit)
	{
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.0f, 24, FColor::Red, false, 3.0f, 0, 2.0f);
	}
}
