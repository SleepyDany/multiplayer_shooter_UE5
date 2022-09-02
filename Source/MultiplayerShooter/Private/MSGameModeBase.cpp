// Test Multiplayer Shooter Game. All Rights Reserved.

#include "MSGameModeBase.h"
#include "Player/MSBaseCharacter.h"
#include "Player/MSPlayerController.h"
#include "UI/MSGameHUD.h"

AMSGameModeBase::AMSGameModeBase()
{
    DefaultPawnClass = AMSBaseCharacter::StaticClass();
    PlayerControllerClass = AMSPlayerController::StaticClass();
    HUDClass = AMSGameHUD::StaticClass();
}
