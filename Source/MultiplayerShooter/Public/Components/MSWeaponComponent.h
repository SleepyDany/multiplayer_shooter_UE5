// Test Multiplayer Shooter Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <Components/ActorComponent.h>
#include "MSWeaponComponent.generated.h"

class AMSBaseWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTIPLAYERSHOOTER_API UMSWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UMSWeaponComponent();

	void Fire();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AMSBaseWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponAttachPointName = "WeaponSocket";

private:

	UPROPERTY()
	AMSBaseWeapon* CurrentWeapon = nullptr;

	void SpawnWeapon();
};
