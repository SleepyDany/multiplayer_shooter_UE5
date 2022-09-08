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

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	virtual void StartFire();

	virtual void StopFire();

	virtual void Reload();

	virtual void Aim();

	FORCEINLINE bool CanFire() { return CurrentAmmo > 0 && !bIsOnReload; }

	FORCEINLINE bool IsOnAim() { return bIsOnAim; }

	FORCEINLINE bool IsOnReload() { return bIsOnReload; }

	FORCEINLINE int32 GetCurrentAmmo() const { return CurrentAmmo; }

	FORCEINLINE int32 GetAllAmmo() const { return AllAmmo; }

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float FireRate = 0.1f;

	UPROPERTY(VisibleAnywhere)
	float BullerSpread = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HolderAmmo = 30;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_AmmoChanged)
	int32 CurrentAmmo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_AmmoChanged)
	int32 AllAmmo = 120;

	UFUNCTION(NetMulticast, Reliable)
	void OnRep_AmmoChanged();

	UPROPERTY(VisibleAnywhere)
	bool bIsOnAim = false;

	UPROPERTY(VisibleAnywhere)
	bool bIsOnReload = false;

protected:

	FTimerHandle FireTimerHandle;

	void MakeShot();

	APlayerController* GetPlayerController() const;

	bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;

	FVector GetMuzzleWorldLocation() const;

	bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

	void MakeHit(FHitResult& HitResult, FVector& TraceStart, FVector& TraceEnd) const;

	void MakeDamage(FHitResult& HitResult);

	void DrawLineTrace(const FHitResult& FHitResult, const FVector& TraceStart, const FVector& TraceEnd) const;
};
