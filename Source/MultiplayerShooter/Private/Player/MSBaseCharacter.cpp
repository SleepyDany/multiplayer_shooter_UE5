// Test Multiplayer Shooter Game. All Rights Reserved.


#include "Player/MSBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AMSBaseCharacter::AMSBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SocketOffset = { 0.0f, 0.0f, 70.0f };

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
}

// Called when the game starts or when spawned
void AMSBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMSBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMSBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMSBaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMSBaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AMSBaseCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnAround", this, &AMSBaseCharacter::AddControllerYawInput);
}

void AMSBaseCharacter::MoveForward(float Amount)
{
	AddMovementInput(GetActorForwardVector(), Amount);
}

void AMSBaseCharacter::MoveRight(float Amount)
{
    AddMovementInput(GetActorRightVector(), Amount);
}
