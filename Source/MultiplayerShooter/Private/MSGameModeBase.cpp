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


void AMSGameModeBase::StartPlay()
{
    Super::StartPlay();

    CurrentRound = 1;
    StartRound();
}


void AMSGameModeBase::StartRound()
{
    RoundCountDown = GameData.RoundTime;
    GetWorldTimerManager().SetTimer(RoundTimerHandle, this, &AMSGameModeBase::RoundTimerUpdate, 1.0f, true);
}


void AMSGameModeBase::RoundTimerUpdate()
{
    if (--RoundCountDown == 0)
    {
        GetWorldTimerManager().ClearTimer(RoundTimerHandle);

        if (CurrentRound + 1 <= GameData.RoundsNum)
        {
            ++CurrentRound;
            RespawnPlayers();
            StartRound();
        }
    }
}


void AMSGameModeBase::RespawnPlayers()
{
    if (!GetWorld())
        return;

    for (auto Controller = GetWorld()->GetControllerIterator(); Controller; ++Controller)
    {
        RespawnOnePlayer(Controller->Get());
    }
}


void AMSGameModeBase::RespawnOnePlayer(AController* Controller)
{
    if (Controller && Controller->GetPawn())
    {
        Controller->GetPawn()->Reset();
    }

    RestartPlayer(Controller);
}

void AMSGameModeBase::Killed(AController* KillerController, AController* VictimController)
{
    if (!GetWorld())
        return;

    FTimerHandle RespawnTimerHandle;

    GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AMSGameModeBase::RespawnPlayers, 1.5f, false);
}
