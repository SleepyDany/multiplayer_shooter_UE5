// Test Multiplayer Shooter Game. All Rights Reserved.


#include "Weapon/MSBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include <Engine/World.h>
#include <DrawDebugHelpers.h>
#include <GameFramework/Character.h>
#include <GameFramework/Controller.h>
#include <Kismet/GameplayStatics.h>
#include <Net/UnrealNetwork.h>

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All)

AMSBaseWeapon::AMSBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);

	bReplicates = true;
	//bNetUseOwnerRelevancy = true;

	CurrentAmmo = HolderAmmo;
	FireCooldown = 0.05f;
	LastFireTime = TNumericLimits<float>::Lowest();
}

//
//void AMSBaseWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	DOREPLIFETIME(AMSBaseWeapon, LastHitResult);
//}
//
//
//void AMSBaseWeapon::OnRep_MakeDamage()
//{
//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("OnRep_MakeDamage")));
//}


void AMSBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}


void AMSBaseWeapon::Fire()
{
	if (CurrentAmmo > 0)
	{
		MakeShot();
	}
	else
	{
		// sound|visuals effects
	}
}


void AMSBaseWeapon::MakeShot()
{
	if (!GetWorld())
		return;
	
	FVector TraceStart, TraceEnd;

	if (!GetTraceData(TraceStart, TraceEnd))
		return;
	
	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	if (HitResult.bBlockingHit)
	{
		DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Orange, false, 3.0f, 0, 2.0f);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.0f, 24, FColor::Red, false, 3.0f, 0, 2.0f);

		MakeDamage(HitResult);
	}
	else
	{
		DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Orange, false, 3.0f, 0, 2.0f);
	}
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
	const FVector TraceDirection = ViewRotation.Vector();
	TraceEnd = TraceStart + TraceDirection * TraceMaxDistance;
	
	return true;
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

	//if (HasAuthority())
	//{
		FString damageMessage = FString::Printf(TEXT("MakeDamage. DamagedChar:%s, Weapon: %s, This: %s, NetOwner: %s"), *(DamagedActor->GetName()), *GetName(), *GetOwner()->GetName(), *GetNetOwner()->GetName());
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, damageMessage);
	//}
	//else
	//{
		//FString damageMessage = FString::Printf(TEXT("Char %s get %f damage from local char %s with %s weapon. Role: %d"), *(DamagedActor->GetName()), DamageAmount, *(GetOwner()->GetName()), *GetName(), GetLocalRole());
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, damageMessage);
	//}

	//if (GetLocalRole() == ROLE_Authority)
	//{
	//	FString damageMessage = FString::Printf(TEXT("Char %s get %f damage from char %s with %s weapon. Role: %d"), *(DamagedActor->GetName()), DamageAmount, *(GetOwner()->GetName()), *GetName(), GetLocalRole());
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, damageMessage);
	//}

	DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
	//}
}


void AMSBaseWeapon::Reload()
{
	
}


void AMSBaseWeapon::Aim()
{

}
