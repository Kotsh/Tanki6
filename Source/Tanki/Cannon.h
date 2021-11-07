// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cannon.generated.h"
#include <GameStructs.generated.h>
#include <Tanki/GameStructs.h>

class UArrowComponent;

UCLASS()
class TANKI_API ACannon : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	class UForceFeedbackEffect* ShootForceEffect;

	UPROPERTY(EditAnywhere)
	class TSubclassOf<UCameraShake> ShootShake;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UParticleSystemComponent* ShootEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UAudioComponent* AudioEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	class TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
		float FireRate = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
		float FireRange = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
		float FireDamage = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
		ECannonType Type = ECannonType::FireProjectile;

private:
	FTimerHandle ReloadTimerHandle;
	bool bIsReadyToFire = false;

public:
	ACannon();
	void Fire();
	bool IsReadyToFire();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
	void Reload();
};
