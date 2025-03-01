// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LyraBlueprintFunctions.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API ULyraBlueprintFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "Lyra Blueprint Functions")
	static void SpawnEnemy(UGameStateComponent* GameStateComponent, ALyraGameMode* GameMode, TSubclassOf<AAIController> BotControllerClass, int32 NumBots);

	// 봇 순차생성을 위해 SpawnEnemyInternal 함수를 UFUNCTION으로 선언 (05.31)
	UFUNCTION()
	static void SpawnEnemyInternal(UGameStateComponent* GameStateComponent, ALyraGameMode* GameMode, TSubclassOf<AAIController> BotControllerClass, AActor* SelectedStart);
};