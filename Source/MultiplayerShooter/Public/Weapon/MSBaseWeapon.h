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

protected:
	
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* WeaponMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FName MuzzleSocketName = "MuzzleSocket";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float TraceMaxDistance = 4000.0f;

	void MakeShot();
};
