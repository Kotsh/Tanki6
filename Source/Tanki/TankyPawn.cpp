// Fill out your copyright notice in the Description page of Project Settings.


#include "TankyPawn.h"
#include <Components/StaticMeshComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include "Kismet/KismetMathLibrary.h"
#include <Camera/CameraComponent.h>
#include "Tanki.h"
#include "Components/ArrowComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Cannon.h"
#include "TankPlayerController.h"


// Sets default values
ATankyPawn::ATankyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank body"));
	RootComponent = BodyMesh;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank turret"));
	TurretMesh->SetupAttachment(BodyMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArm->SetupAttachment(BodyMesh);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch  = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	CannonSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
	CannonSpawnPoint->SetupAttachment(TurretMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	HealthComponent->OnDie.AddUObject(this, &ATankyPawn::Die);
	HealthComponent->OnDamaged.AddUObject(this, &ATankyPawn::DamageTaked);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(BodyMesh);

}



// Called when the game starts or when spawned
void ATankyPawn::BeginPlay()
{
	Super::BeginPlay();

	SetupCannon();
}

// Called every frame
void ATankyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentMoveFowardAxis = FMath::Lerp(CurrentMoveFowardAxis, TargetMoveForwardAxis, MovementSmoothness);

	FVector CurrentLocation = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector MovePosition = CurrentLocation+ForwardVector*MoveSpeed* CurrentMoveFowardAxis *DeltaTime;
	SetActorLocation(MovePosition, true);


	CurrentRotateRightAxis = FMath::Lerp(CurrentRotateRightAxis, TargetRotateRightAxis, RotationSmoothness);
	float Rotation = GetActorRotation().Yaw + CurrentRotateRightAxis * RotatiSpeed * DeltaTime;
	SetActorRotation(FRotator(0.f, Rotation, 0.f));

	UE_LOG(LogTanki, Verbose, TEXT("CurrentRotateRightAxis: %f"), CurrentRotateRightAxis);

	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TurretTargetPosition);
	FRotator CurrentRotation = TurretMesh->GetComponentRotation();
	TargetRotation.Roll = CurrentRotation.Roll;
	TargetRotation.Pitch = CurrentRotation.Pitch;
	TurretMesh->SetWorldRotation(FMath::Lerp(CurrentRotation, TargetRotation, TargetRotationSmootheness));
}

void ATankyPawn::MoveForward(float AxisValue)
{
	TargetMoveForwardAxis = AxisValue;
}

void ATankyPawn::RotateRight(float AxisValue)
{
	TargetRotateRightAxis = AxisValue;
}

void ATankyPawn::SetTurretTargetPosition(const FVector& TargetPosition)
{
	TurretTargetPosition = TargetPosition;
}

void ATankyPawn::Fire()
{
	if (Cannon)
	{
		Cannon->Fire();
	}
}

void ATankyPawn::SetupCannon()
{
	if (Cannon)
	{
		Cannon->Destroy();
	}

	FActorSpawnParameters params;
	params.Instigator = this;
	params.Owner = this;
	Cannon =GetWorld()->SpawnActor<ACannon>(DefaultCannonClass, params);
	Cannon->AttachToComponent(CannonSpawnPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void ATankyPawn::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

void ATankyPawn::Die()
{
	Destroy();
}

void ATankyPawn::DamageTaked(float DamageValue)
{
	UE_LOG(TankLog, Warning, TEXT("Tank %s taked damage:%f Health:%f"), *GetName(), DamageValue, HealthComponent->GetHealth());
}

FVector ATankyPawn::GetTurretForwardVector()
{
	return TurretMesh->GetForwardVector();
}

void ATankyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Tank movement
	FVector currentLocation = GetActorLocation();
	FVector forwardVector = GetActorForwardVector();
	FVector movePosition = currentLocation + forwardVector * MoveSpeed * TargetForwardAxisValue * DeltaTime;
	SetActorLocation(movePosition, true);

	// Tank rotation
	CurrentRightAxisValue = FMath::Lerp(CurrentRightAxisValue, TargetRightAxisValue, InterpolationKey);
	float yawRotation = RotationSpeed * CurrentRightAxisValue * DeltaTime;
	FRotator currentRotation = GetActorRotation();
	yawRotation = currentRotation.Yaw + yawRotation;
	FRotator newRotation = FRotator(0, yawRotation, 0);
	SetActorRotation(newRotation);


	// Turret rotation
	if (TankController)
	{
		FVector mousePos = TankController->GetMousePos();
		RotateTurretTo(mousePos);
	}
}

void ATankyPawn::RotateTurretTo(FVector TargetPosition)
{
	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetPosition);
	FRotator currRotation = TurretMesh->GetComponentRotation();
	targetRotation.Pitch = currRotation.Pitch;
	targetRotation.Roll = currRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::Lerp(currRotation, targetRotation, TurretRotationInterpolationKey));
}

FVector ATankyPawn::GetEyesPosition()
{
	return CannonSetupPoint->GetComponentLocation();
}




