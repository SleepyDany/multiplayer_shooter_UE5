// Test Multiplayer Shooter Game. All Rights Reserved.


#include "Player/MSBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/MSHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "Weapon/MSBaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All)

AMSBaseCharacter::AMSBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SocketOffset = { 0.0f, 0.0f, 70.0f };

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	HealthComponent = CreateDefaultSubobject<UMSHealthComponent>("HealtComponent");
	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
	HealthTextComponent->SetupAttachment(GetRootComponent());
}

void AMSBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	check(HealthComponent);
	check(HealthTextComponent);

	SpawnWeapon();
}

void AMSBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const auto Health = HealthComponent->GetHealth();
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

void AMSBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMSBaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMSBaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AMSBaseCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnAround", this, &AMSBaseCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMSBaseCharacter::Jump);
}

void AMSBaseCharacter::MoveForward(float Amount)
{
	AddMovementInput(GetActorForwardVector(), Amount);
}

void AMSBaseCharacter::MoveRight(float Amount)
{
    AddMovementInput(GetActorRightVector(), Amount);
}

void AMSBaseCharacter::SpawnWeapon()
{
	if (!GetWorld())
		return;

	const auto Weapon = GetWorld()->SpawnActor<AMSBaseWeapon>(WeaponClass);
	if (Weapon)
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
		Weapon->AttachToComponent(GetMesh(), AttachmentRules, "WeaponSocket");
	}
}
