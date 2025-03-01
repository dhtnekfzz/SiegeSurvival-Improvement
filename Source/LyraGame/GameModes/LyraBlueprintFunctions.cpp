// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/LyraBlueprintFunctions.h"

#include "GameModes/LyraGameMode.h"
#include "Components/GameStateComponent.h"
#include "Components/ActorComponent.h"
#include "Engine/World.h"
#include "GameModes/LyraExperienceDefinition.h"
#include "Character/LyraPawnExtensionComponent.h"
#include "GameModes/LyraExperienceManagerComponent.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "AIController.h"
#include "Character/LyraPawnExtensionComponent.h"
#include "Player/LyraPlayerStart.h" // ������� �߰� (05.25)
#include "EngineUtils.h" // for TActorIterator (05.25)
#include "TimerManager.h" // Ÿ�̸� �Ŵ��� �߰� (05.31)



// ���� ���������� �����ϴ� ���� �߰� (05.31)
void ULyraBlueprintFunctions::SpawnEnemyInternal(UGameStateComponent* GameStateComponent, ALyraGameMode* GameMode, TSubclassOf<AAIController> BotControllerClass, AActor* SelectedStart)
{
    if (!GameMode || !GameStateComponent || !SelectedStart) return;
    UWorld* World= GameStateComponent->GetWorld();
    if (!World || World->bIsTearingDown) return;
    if (!World)
    {
        return;
    }

    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnInfo.OverrideLevel = GameStateComponent->GetComponentLevel();
    SpawnInfo.ObjectFlags |= RF_Transient;

    FVector SpawnLocation = SelectedStart->GetActorLocation();
    FRotator SpawnRotation = SelectedStart->GetActorRotation();

    AAIController* NewController = World->SpawnActor<AAIController>(BotControllerClass, SpawnLocation, SpawnRotation, SpawnInfo);

    if (NewController != nullptr)
    {
        check(GameMode);

        GameMode->GenericPlayerInitialization(NewController);
        GameMode->RestartPlayerAtPlayerStart(NewController, SelectedStart);

        if (NewController->GetPawn() != nullptr)
        {
            if (ULyraPawnExtensionComponent* PawnExtComponent = NewController->GetPawn()->FindComponentByClass<ULyraPawnExtensionComponent>())
            {
                PawnExtComponent->CheckDefaultInitialization();
            }
        }
    }
}

void ULyraBlueprintFunctions::SpawnEnemy(UGameStateComponent* GameStateComponent, ALyraGameMode* GameMode, TSubclassOf<AAIController> BotControllerClass, int32 NumBots)
{
    // LyraPlayerStart �ν��Ͻ� �迭 ��������
    TArray<AActor*> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(GameStateComponent->GetWorld(), ALyraPlayerStart::StaticClass(), PlayerStarts);

    // Melee �±װ� �ִ� �ν��Ͻ��� ���͸�
    TArray<AActor*> MeleeTaggedStarts;
    for (AActor* Start : PlayerStarts)
    {
        ALyraPlayerStart* LyraStart = Cast<ALyraPlayerStart>(Start);
        if (LyraStart && LyraStart->PlayerStartTag == FName("Melee"))
        {
            MeleeTaggedStarts.Add(Start);
        }
    }

    if (MeleeTaggedStarts.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No Melee-tagged PlayerStarts found in the level."));
        return;
    }

    const float SpawnDelay = 3.0f; // �� ���� ������ (����: ��)
    int32 NumSpawnsPerCycle = MeleeTaggedStarts.Num();
    int32 NumCycles = FMath::CeilToInt(static_cast<float>(NumBots) / NumSpawnsPerCycle);

    for (int32 Cycle = 0; Cycle < NumCycles; ++Cycle)
    {
        for (int32 i = 0; i < NumSpawnsPerCycle; ++i)
        {
            int32 BotIndex = Cycle * NumSpawnsPerCycle + i;
            if (BotIndex >= NumBots)
            {
                return;
            }

            AActor* SelectedStart = MeleeTaggedStarts[i];
            FTimerHandle TimerHandle;
            FTimerDelegate TimerDel;
            TimerDel.BindStatic(&ULyraBlueprintFunctions::SpawnEnemyInternal, GameStateComponent, GameMode, BotControllerClass, SelectedStart);

            // ���� ���������� �����ϱ� ���� Ÿ�̸� ����
            if(!GameMode) return;
            GameStateComponent->GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, SpawnDelay * (Cycle + 1), false);
        }
    }
}