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

UCLASS()
class MULTIPLAYERSHOOTER_API AMSBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AMSBaseCharacter();

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* CameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	USpringArmComponent* SpringArmComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Components")
	UMSHealthComponent* HealthComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UTextRenderComponent* HealthTextComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UMSWeaponComponent* WeaponComponent;

public:	
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	void MoveForward(float Amount);
	void MoveRight(float Amount);

	void OnDeath();
	void OnHealthChanged(float Health);
};
