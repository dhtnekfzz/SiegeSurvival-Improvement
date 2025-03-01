// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ModularAIController.h"
#include "Teams/LyraTeamAgentInterface.h"

#include "LyraPlayerBotController.generated.h"

namespace ETeamAttitude { enum Type : int; }
struct FGenericTeamId;

class APlayerState;
class UAIPerceptionComponent;
class UObject;
struct FFrame;

// 폰 데이터 구분위해 로직 추가 (06.08)
UENUM(BlueprintType)
enum class EBotType : uint8
{
    Shooter,
    Melee,
    Other
};

/**
 * ALyraPlayerBotController
 *
 *	The controller class used by player bots in this project.
 */
UCLASS(Blueprintable)
class ALyraPlayerBotController : public AModularAIController, public ILyraTeamAgentInterface
{
    GENERATED_BODY()

public:
    ALyraPlayerBotController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    //~ILyraTeamAgentInterface interface
    virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
    virtual FGenericTeamId GetGenericTeamId() const override;
    virtual FOnLyraTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
    ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
    //~End of ILyraTeamAgentInterface interface

    // Attempts to restart this controller (e.g., to respawn it)
    void ServerRestartController();

    //Update Team Attitude for the AI
    UFUNCTION(BlueprintCallable, Category = "Lyra AI Player Controller")
    void UpdateTeamAttitude(UAIPerceptionComponent* AIPerception);

    virtual void OnUnPossess() override;

    // Bot Type 설정 함수
    UFUNCTION(BlueprintCallable, Category = "Lyra AI Player Controller")
    void SetBotType(EBotType NewBotType);

    // Bot Type 반환 함수
    UFUNCTION(BlueprintCallable, Category = "Lyra AI Player Controller")
    EBotType GetBotType() const;

protected:
    // Called when the player state is set or cleared
    virtual void OnPlayerStateChanged();

private:
    // Bot Type 변수
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lyra AI Player Controller", meta = (AllowPrivateAccess = "true"))
    EBotType BotType;

protected:
    //~AController interface
    virtual void InitPlayerState() override;
    virtual void CleanupPlayerState() override;
    virtual void OnRep_PlayerState() override;
    //~End of AController interface

private:
    UFUNCTION()
    void OnPlayerStateChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);

    void BroadcastOnPlayerStateChanged();

    UPROPERTY()
    FOnLyraTeamIndexChangedDelegate OnTeamChangedDelegate;

    UPROPERTY()
    TObjectPtr<APlayerState> LastSeenPlayerState;
};


// 기존 코드
//UCLASS(Blueprintable)
//class ALyraPlayerBotController : public AModularAIController, public ILyraTeamAgentInterface
//{
//	GENERATED_BODY()
//
//public:
//	ALyraPlayerBotController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
//
//	//~ILyraTeamAgentInterface interface
//	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
//	virtual FGenericTeamId GetGenericTeamId() const override;
//	virtual FOnLyraTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
//	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
//	//~End of ILyraTeamAgentInterface interface
//
//	// Attempts to restart this controller (e.g., to respawn it)
//	void ServerRestartController();
//
//	//Update Team Attitude for the AI
//	UFUNCTION(BlueprintCallable, Category = "Lyra AI Player Controller")
//	void UpdateTeamAttitude(UAIPerceptionComponent* AIPerception);
//
//	virtual void OnUnPossess() override;
//
//
//private:
//	UFUNCTION()
//	void OnPlayerStateChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);
//
//protected:
//	// Called when the player state is set or cleared
//	virtual void OnPlayerStateChanged();
//
//private:
//	void BroadcastOnPlayerStateChanged();
//
//protected:	
//	//~AController interface
//	virtual void InitPlayerState() override;
//	virtual void CleanupPlayerState() override;
//	virtual void OnRep_PlayerState() override;
//	//~End of AController interface
//
//private:
//	UPROPERTY()
//	FOnLyraTeamIndexChangedDelegate OnTeamChangedDelegate;
//
//	UPROPERTY()
//	TObjectPtr<APlayerState> LastSeenPlayerState;
//};