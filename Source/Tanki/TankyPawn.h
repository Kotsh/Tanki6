// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Cannon.h"
#include "CoreMinimal.h"
#include "DamageTaker.h"
#include "HealthComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Pawn.h"
#include "TankyPawn.generated.h"

UCLASS()
class TANKI_API ATankyPawn : public APawn, public IDamageTaker
{
   GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params|Patrol points", Meta = (MakeEditWidget = true))
	    TArray<FVector> PatrollingPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params|Accurency")
		float MovementAccurency = 50;

public:
	// Sets default values for this pawn's properties
	ATankyPawn();

	UFUNCTION()
	FVector GetEyesPosition();

	UFUNCTION()
		FVector GetTurretForwardVector();

	UFUNCTION()
		void RotateTurretTo(FVector TargetPosition);


	UFUNCTION()
		TArray<FVector> GetPatrollingPoints() { return PatrollingPoints; };
	UFUNCTION()
		float GetMovementAccurency() { return MovementAccurency; };


	UFUNCTION()
	void MoveForward(float AxisValue);

	UFUNCTION()
		void RotateRight(float AxisValue);

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UHealthComponent* HealthComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UBoxComponent* HitCollider;


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UArrowComponent* CannonSpawnPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float MoveSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float RotatiSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
		float MovementSmoothness=0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
		float RotationSmoothness=0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
		float TargetRotationSmootheness = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
		TSubclassOf<class ACannon> DefaultCannonClass;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
protected:
	UFUNCTION()
		void Die();

	UFUNCTION()
		void DamageTaked(float DamageValue);

public:
	UFUNCTION()
		void TakeDamage(FDamageData DamageData);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="Movement")
		void SetTurretTargetPosition(const FVector& TargetPosition);

	UFUNCTION(BlueprintCallable, Category = "Turret")
		void Fire();

private:
	void SetupCannon();

	UPROPERTY()
	class ACannon* Cannon = nullptr;

	float CurrentMoveFowardAxis = 0.f;
	float TargetMoveForwardAxis = 0.f;
	float CurrentRotateRightAxis = 0.f;
	float TargetRotateRightAxis = 0.f;

	FVector TurretTargetPosition;
};
