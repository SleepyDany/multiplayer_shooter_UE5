// Test Multiplayer Shooter Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MSBaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UMSHealthComponent;
class UTextRenderComponent;
class UMSWeaponComponent;
class UMSCharacterMovementComponent;

UCLASS()
class MULTIPLAYERSHOOTER_API AMSBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AMSBaseCharacter(const FObjectInitializer& ObjInit);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_HealthChanged();

	UFUNCTION()
	void OnRep_AmmoChanged();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* CameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	USpringArmComponent* SpringArmComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Components", ReplicatedUsing = OnRep_HealthChanged)
	UMSHealthComponent* HealthComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", ReplicatedUsing = OnRep_HealthChanged)
	UTextRenderComponent* HealthTextComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", Replicated)
	UMSWeaponComponent* WeaponComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", Replicated = OnRep_AmmoChanged)
	UTextRenderComponent* WeaponTextComponent;

public:	
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsWalking() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetMovementDirection() const;

private:

	void MoveForward(float Amount);
	void MoveRight(float Amount);

	bool IntendToWalk = false;
	void StartWalk();
	void StopWalk();

	void OnDeath();
	void OnHealthChanged();

	void OnAmmoChanged();
};
