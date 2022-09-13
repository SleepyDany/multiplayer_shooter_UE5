// Test Multiplayer Shooter Game. All Rights Reserved.


#include "Components/MSCharacterMovementComponent.h"
#include "Player/MSBaseCharacter.h"

float UMSCharacterMovementComponent::GetMaxSpeed() const
{
    const float MaxSpeed = Super::GetMaxSpeed();
    const AMSBaseCharacter* Player = Cast<AMSBaseCharacter>(GetPawnOwner());
    
    return Player && Player->IsWalking() ? MaxSpeed * WalkModifier : MaxSpeed;
}
