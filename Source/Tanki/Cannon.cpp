// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include <Tanki/GameStructs.h>
#include "TimerManager.h"

ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceeneCpm;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
	ProjectileSpawnPoint->SetupAttachment(Mesh);
}

void ACannon::Fire()
{
    if (!bIsReadyToFire)
    {
        return;
    }
    bIsReadyToFire = false;

    if (Type == ECannonType::FireProjectile)
    {
        GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, "Fire - projectile");
        FTransform projectileTransform(ProjectileSpawnPoint->GetComponentRotation(), ProjectileSpawnPoint->GetComponentLocation(), FVector(1));

        AProjectilew* projectilew = GetWorld()->SpawnActor<AProjectilew>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
        if (projectilew)
        {
            projectilew->Start();
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, "Fire - trace");
        FHitResult hitResult;
        FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
        traceParams.bTraceComplex = true;
        traceParams.bReturnPhysicalMaterial = false;

        FVector start = ProjectileSpawnPoint->GetComponentLocation();
        FVector end = ProjectileSpawnPoint->GetForwardVector() * FireRange + start;
        if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility, traceParams))
        {
            DrawDebugLine(GetWorld(), start, hitResult.Location, FColor::Red, false, 0.5f, 0, 5);
            if (hitResult.Actor.Get())
            {
                hitResult.Actor.Get()->Destroy();
            }
        }
        else
        {
            DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 0.5f, 0, 5);
        }
    }

    GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1 / FireRate, false);
}

bool ACannon::IsReadyToFire()
{
	return bIsReadyToFire;
}

void ACannon::BeginPlay()
{
	Super::BeginPlay();
	bIsReadyToFire = true;
}

void ACannon::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
}

void ACannon::Reload()
{
	bIsReadyToFire = true;
}

void ACannon::Fire()
{
    if (!ReadyToFire)
    {
        return;
    }
    ReadyToFire = false;

    ShootEffect->ActivateSystem();
    AudioEffect->Play();
}

void ACannon::Fire()
{
    if (!ReadyToFire)
    {
        return;
    }
    ReadyToFire = false;

    ShootEffect->ActivateSystem();
    AudioEffect->Play();

    if (GetOwner() && GetOwner() == GetWorld()->GetFirstPlayerController()->GetPawn())
    {
        if (ShootForceEffect)
        {
            FForceFeedbackParameters shootForceEffectParams;
            shootForceEffectParams.bLooping = false;
            shootForceEffectParams.Tag = "shootForceEffectParams";
            GetWorld()->GetFirstPlayerController()->ClientPlayForceFeedback(ShootForceEffect, shootForceEffectParams);
        }

        if (ShootShake)
        {
            GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(ShootShake);
        }
    }

}


