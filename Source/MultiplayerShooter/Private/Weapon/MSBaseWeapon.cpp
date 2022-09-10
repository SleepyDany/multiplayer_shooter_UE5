// Test Multiplayer Shooter Game. All Rights Reserved.


#include "Weapon/MSBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include <Engine/World.h>
#include <DrawDebugHelpers.h>
#include <GameFramework/Character.h>
#include <GameFramework/Controller.h>
#include <Kismet/GameplayStatics.h>
#include <Net/UnrealNetwork.h>
#include "Player/MSBaseCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All)

AMSBaseWeapon::AMSBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);

	CurrentAmmo = HolderAmmo;

	bReplicates = true;
}


void AMSBaseWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMSBaseWeapon, CurrentAmmo);
	DOREPLIFETIME(AMSBaseWeapon, AllAmmo);
}


void AMSBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}


void AMSBaseWeapon::StartFire()
{
	if (!CanFire())
		return;

	MakeShot();
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AMSBaseWeapon::MakeShot, FireRate, true);
}


void AMSBaseWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
}


void AMSBaseWeapon::MakeShot()
{
	if (!GetWorld())
		return;

	if (!CanFire())
	{
		StopFire();
		return;
	}

	--CurrentAmmo;
	OnRep_AmmoChanged();

	FVector TraceStart, TraceEnd;

	if (!GetTraceData(TraceStart, TraceEnd))
		return;

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);
	DrawLineTrace(HitResult, TraceStart, TraceEnd);

	if (HitResult.bBlockingHit)
	{
		MakeDamage(HitResult);
	}
}


void AMSBaseWeapon::MakeHit(FHitResult& HitResult, FVector& TraceStart, FVector& TraceEnd) const
{
	if (!GetWorld())
		return;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);
}


void AMSBaseWeapon::MakeDamage(FHitResult& HitResult)
{
	AActor* DamagedActor = HitResult.GetActor();
	if (!DamagedActor || DamagedActor == GetOwner())
		return;

	DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
}


void AMSBaseWeapon::OnRep_AmmoChanged_Implementation()
{
	const auto ActorOwner = Cast<AMSBaseCharacter>(Owner);

	if (!ActorOwner)
		return;

	ActorOwner->OnRep_AmmoChanged();
}


void AMSBaseWeapon::Reload()
{
	if (FireTimerHandle.IsValid())
	{
		StopFire();
	}

	if (CurrentAmmo < HolderAmmo && AllAmmo > 0)
	{
		const int32 AvailableAmmo = AllAmmo >= HolderAmmo - CurrentAmmo ? HolderAmmo - CurrentAmmo : AllAmmo;
		CurrentAmmo += AvailableAmmo;

		AllAmmo -= AvailableAmmo;
	}
}


void AMSBaseWeapon::Aim()
{

}


APlayerController* AMSBaseWeapon::GetPlayerController() const
{
	const auto Player = Cast<ACharacter>(GetOwner());
	if (!Player)
		return nullptr;

	return Player->GetController<APlayerController>();
}


bool AMSBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	const APlayerController* Controller = GetPlayerController();
	if (!Controller)
		return false;

	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	return true;
}


FVector AMSBaseWeapon::GetMuzzleWorldLocation() const
{
	return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}


bool AMSBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
		return false;

	TraceStart = ViewLocation;
	const auto HalfRadSpread = FMath::DegreesToRadians(BullerSpread);
	const FVector TraceDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRadSpread);
	TraceEnd = TraceStart + TraceDirection * TraceMaxDistance;
	
	return true;
}

void AMSBaseWeapon::DrawLineTrace(const FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const
{
	if (HitResult.bBlockingHit)
	{
		DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Orange, false, 3.0f, 0, 2.0f);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.0f, 24, FColor::Red, false, 3.0f, 0, 2.0f);
	}
	else
	{
		DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Orange, false, 3.0f, 0, 2.0f);
	}
}
