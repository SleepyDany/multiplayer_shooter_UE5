// Test Multiplayer Shooter Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MSBaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class MULTIPLAYERSHOOTER_API AMSBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMSBaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* CameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	USpringArmComponent* SpringArmComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	void MoveForward(float Amount);
	void MoveRight(float Amount);
};
