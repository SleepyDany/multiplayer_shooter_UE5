// Test Multiplayer Shooter Game. All Rights Reserved.

#include "MSGameModeBase.h"
#include "Player/MSBaseCharacter.h"
#include "Player/MSPlayerController.h"

AMSGameModeBase::AMSGameModeBase()
{
    DefaultPawnClass = AMSBaseCharacter::StaticClass();
    PlayerControllerClass = AMSPlayerController::StaticClass();
}
