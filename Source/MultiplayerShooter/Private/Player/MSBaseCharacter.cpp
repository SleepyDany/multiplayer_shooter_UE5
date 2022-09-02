// Test Multiplayer Shooter Game. All Rights Reserved.


#include "Player/MSBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/MSHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/MSCharacterMovementComponent.h"
#include "Components/MSWeaponComponent.h"
#include "Player/MSPlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All)

AMSBaseCharacter::AMSBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SocketOffset = { 50.0f, -60.0f, 90.0f };

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	HealthComponent = CreateDefaultSubobject<UMSHealthComponent>("HealtComponent");
	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
	HealthTextComponent->SetupAttachment(GetRootComponent());
	HealthTextComponent->SetOnlyOwnerSee(true);

	WeaponComponent = CreateDefaultSubobject<UMSWeaponComponent>("WeaponComponent");
}

void AMSBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	check(HealthComponent);
	check(HealthTextComponent);
	check(GetCharacterMovement());

	OnHealthChanged(HealthComponent->GetHealth());
	HealthComponent->OnDeath.AddUObject(this, &AMSBaseCharacter::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &AMSBaseCharacter::OnHealthChanged);
}

void AMSBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMSBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
	check(WeaponComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMSBaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMSBaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AMSBaseCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnAround", this, &AMSBaseCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMSBaseCharacter::Jump);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &UMSWeaponComponent::Fire);
}

void AMSBaseCharacter::MoveForward(float Amount)
{
	AddMovementInput(GetActorForwardVector(), Amount);
}

void AMSBaseCharacter::MoveRight(float Amount)
{
    AddMovementInput(GetActorRightVector(), Amount);
}

void AMSBaseCharacter::OnDeath()
{
	UE_LOG(LogBaseCharacter, Display, TEXT("Dead!"));

	GetCharacterMovement()->DisableMovement();
	
	FText text = FText::FromString(TEXT("DEAD"));

	HealthTextComponent->SetText(text);
	HealthTextComponent->SetTextRenderColor(FColor::Red);

	SetLifeSpan(2.5f);

	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
}

void AMSBaseCharacter::OnHealthChanged(float Health)
{
	const float color_param = Health / HealthComponent->GetMaxHealth();

	FLinearColor color = FLinearColor::Red * (1 - color_param) + FLinearColor::Green * color_param;

	HealthTextComponent->SetTextRenderColor(color.ToFColor(true));
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}
