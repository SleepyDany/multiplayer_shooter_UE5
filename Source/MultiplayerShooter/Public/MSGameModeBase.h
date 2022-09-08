// Test Multiplayer Shooter Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MSGameModeBase.generated.h"

class AMSBaseCharacter;

USTRUCT(BlueprintType)
struct FGameData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", ClampMax = "4"))
    int32 PlayersNum = 2;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", ClampMax = "15"))
    int32 RoundsNum = 5;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", ClampMax = "600"))
    int32 RoundTime = 120; // sec
};

UCLASS()
class MULTIPLAYERSHOOTER_API AMSGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:

    AMSGameModeBase();

    virtual void StartPlay() override;

    void Killed(AController* KillerController, AController* VictimController);

protected:

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FGameData GameData;

private:

    int32 CurrentRound = 1;
    int32 RoundCountDown = 0;
    FTimerHandle RoundTimerHandle;

    void RespawnPlayers();
    void RespawnOnePlayer(AController* Controller);

    void StartRound();
    void RoundTimerUpdate();
};
