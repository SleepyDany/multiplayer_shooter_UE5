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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AMSBaseWeapon* GetWeapon() const { return CurrentWeapon; }

	void SpawnWeapon();

	UFUNCTION(Server, Reliable)
	void Fire();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AMSBaseWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponAttachPointName = "WeaponSocket";

private:

	UPROPERTY(Replicated)
	AMSBaseWeapon* CurrentWeapon = nullptr;
};
