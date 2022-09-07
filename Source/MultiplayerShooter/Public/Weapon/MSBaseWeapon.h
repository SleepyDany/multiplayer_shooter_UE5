// Test Multiplayer Shooter Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MSBaseWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class MULTIPLAYERSHOOTER_API AMSBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AMSBaseWeapon();

	virtual void Fire();

	virtual void Reload();

	virtual void Aim();

	FORCEINLINE bool CanFire() { return CurrentAmmo > 0 && (GetWorld()->GetTimeSeconds() - LastFireTime) >= FireCooldown; }

protected:
	
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* WeaponMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FName MuzzleSocketName = "MuzzleSocket";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float TraceMaxDistance = 4000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageAmount = 8.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireCooldown = 0.5f;

	UPROPERTY(VisibleAnywhere)
	float LastFireTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HolderAmmo = 30;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentAmmo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AllAmmo = 120;

protected:

	void MakeShot();

	APlayerController* GetPlayerController() const;

	bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;

	FVector GetMuzzleWorldLocation() const;

	bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

	void MakeHit(FHitResult& HitResult, FVector& TraceStart, FVector& TraceEnd) const;

	void MakeDamage(FHitResult& HitResult);
};
