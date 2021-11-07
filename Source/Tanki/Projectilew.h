// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectilew.generated.h"

UCLASS()
class TANKI_API AProjectilew : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    class UStaticMeshComponent* Mesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
        float MoveSpeed = 100;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
        float MoveRate = 0.005f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
        float Damage = 1;

    FTimerHandle MovementTimerHandle;

public:
    AProjectilew();

    void Start();

protected:
    UFUNCTION()
        void OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
        void Move();
};

 