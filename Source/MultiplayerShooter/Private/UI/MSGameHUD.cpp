// Test Multiplayer Shooter Game. All Rights Reserved.


#include "UI/MSGameHUD.h"
#include <Engine/Canvas.h>

void AMSGameHUD::DrawHUD()
{
    Super::DrawHUD();

    DrawCrossHair();
}

void AMSGameHUD::DrawCrossHair()
{
    int32 SizeX = Canvas->SizeX;
    int32 SizeY = Canvas->SizeY;

    const TInterval<float> Center(SizeX * 0.5f, SizeY * 0.5f);

    const float HalfLineSize = 10.0f;
    const float LineThickness = 2.0f;
    const FLinearColor LineColor = FLinearColor::Black;

    DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min - HalfLineSize * 0.5f, Center.Max, LineColor, LineThickness);
    DrawLine(Center.Min + HalfLineSize, Center.Max, Center.Min + HalfLineSize * 0.5f, Center.Max, LineColor, LineThickness);
    DrawLine(Center.Min, Center.Max + HalfLineSize, Center.Min, Center.Max + HalfLineSize * 0.5f, LineColor, LineThickness);
    DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max - HalfLineSize * 0.5f, LineColor, LineThickness);
}
