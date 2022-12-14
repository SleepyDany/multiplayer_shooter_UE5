// Test Multiplayer Shooter Game. All Rights Reserved.


#include "Player/MSBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/MSHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/MSCharacterMovementComponent.h"
#include "Components/MSWeaponComponent.h"
#include <Components/CapsuleComponent.h>
#include "Player/MSPlayerController.h"
#include "Weapon/MSBaseWeapon.h"

#include <Net/UnrealNetwork.h>
#include <Engine/Engine.h>

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All)

AMSBaseCharacter::AMSBaseCharacter(const FObjectInitializer& ObjInit) : 
	Super(ObjInit.SetDefaultSubobjectClass<UMSCharacterMovementComponent>(AMSBaseCharacter::CharacterMovementComponentName))
{
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 86.0f);

	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SocketOffset = { 50.0f, -60.0f, 90.0f };

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	HealthComponent = CreateDefaultSubobject<UMSHealthComponent>("HealtComponent");
	HealthComponent->SetIsReplicated(true);

	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
	HealthTextComponent->SetupAttachment(GetRootComponent());
	HealthTextComponent->SetIsReplicated(true);

	WeaponComponent = CreateDefaultSubobject<UMSWeaponComponent>("WeaponComponent");
	WeaponComponent->SetIsReplicated(true);

	WeaponTextComponent = CreateDefaultSubobject<UTextRenderComponent>("WeaponTextComponent");
	WeaponTextComponent->SetupAttachment(GetRootComponent());
	WeaponTextComponent->SetIsReplicated(true);

	bReplicates = true;
}


void AMSBaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMSBaseCharacter, HealthTextComponent);
	DOREPLIFETIME(AMSBaseCharacter, HealthComponent);
	DOREPLIFETIME(AMSBaseCharacter, WeaponComponent);
	DOREPLIFETIME(AMSBaseCharacter, WeaponTextComponent);
}


void AMSBaseCharacter::OnRep_HealthChanged()
{
	OnHealthChanged();
}


void AMSBaseCharacter::OnRep_AmmoChanged()
{
	OnAmmoChanged();
}


void AMSBaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(WeaponComponent);

	if (HasAuthority())
	{
		WeaponComponent->SpawnWeapon();
	}

	OnAmmoChanged();
	WeaponComponent->OnAmmoChanged.AddUObject(this, &AMSBaseCharacter::OnAmmoChanged);


	check(HealthComponent)

	OnHealthChanged();
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

	PlayerInputComponent->BindAction("Walk", IE_Pressed, this, &AMSBaseCharacter::StartWalk);
	PlayerInputComponent->BindAction("Walk", IE_Released, this, &AMSBaseCharacter::StopWalk);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &UMSWeaponComponent::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &UMSWeaponComponent::StopFire);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &UMSWeaponComponent::Reload);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, WeaponComponent, &UMSWeaponComponent::Aim);
	PlayerInputComponent->BindAction("Aim", IE_Released, WeaponComponent, &UMSWeaponComponent::Aim);
}


void AMSBaseCharacter::StartWalk()
{
	//auto Movement = GetCharacterMovement();
	//Movement->MaxWalkSpeed /= 2;

	IntendToWalk = true;
}


void AMSBaseCharacter::StopWalk()
{
	//auto Movement = GetCharacterMovement();
	//Movement->MaxWalkSpeed *= 2;

	IntendToWalk = false;
}


bool AMSBaseCharacter::IsWalking() const
{
	return IntendToWalk && !GetVelocity().IsZero();
}


float AMSBaseCharacter::GetMovementDirection() const
{
	if (GetVelocity().IsZero())
		return 0.0f;

	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto ForwardNormal = GetActorForwardVector();

	const auto Angle = FMath::Acos(FVector::DotProduct(VelocityNormal, ForwardNormal));
	const auto DirectionCoef = FMath::Sign(FVector::CrossProduct(ForwardNormal, VelocityNormal).Z);

	return FMath::RadiansToDegrees(Angle) * DirectionCoef;
}


void AMSBaseCharacter::MoveForward(float Amount)
{
	if (Amount == 0.0f)
		return;

	AddMovementInput(GetActorForwardVector(), Amount);
}


void AMSBaseCharacter::MoveRight(float Amount)
{
	if (Amount == 0.0f)
		return;

    AddMovementInput(GetActorRightVector(), Amount);
}


void AMSBaseCharacter::OnDeath()
{
	GetCharacterMovement()->DisableMovement();

	SetLifeSpan(1.5f);
	WeaponComponent->GetWeapon()->SetLifeSpan(1.5f);

	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}

	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	
}


void AMSBaseCharacter::OnHealthChanged()
{
	if (HealthComponent->GetHealth() > 0)
	{
		const float color_param = HealthComponent->GetHealth() / HealthComponent->GetMaxHealth();

		FLinearColor color = FLinearColor::Red * (1 - color_param) + FLinearColor::Green * color_param;

		HealthTextComponent->SetTextRenderColor(color.ToFColor(true));
		HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), HealthComponent->GetHealth())));
	}
	else
	{
		HealthTextComponent->SetText(FText::FromString(TEXT("DEAD")));
		HealthTextComponent->SetTextRenderColor(FColor::Red);
		HealthTextComponent->SetOnlyOwnerSee(false);

		OnDeath();
	}
}

void AMSBaseCharacter::OnAmmoChanged()
{
	const int32 CurrentAmmo = WeaponComponent->GetCurrentAmmo();
	const int32 AllAmmo = WeaponComponent->GetAllAmmo();

	WeaponTextComponent->SetTextRenderColor(FColor::Orange);
	WeaponTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentAmmo, AllAmmo)));
}
