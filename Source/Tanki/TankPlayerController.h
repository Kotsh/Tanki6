// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

class ATankyPawn;

/**
 * 
 */
UCLASS()
class TANKI_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY()
    ATankyPawn* TankyPawn;

public:
	ATankPlayerController();
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;
	void MoveForward(float AxisValue);
	void RotateRight(float AxisValue);
	void Fire();

	

	
	

};
