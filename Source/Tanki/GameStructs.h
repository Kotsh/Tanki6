#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "GameStructs.generated.h"

UENUM(BlueprintType)
enum class ECannonType : uint8
{
	FireProjectile = 0 UMETA(Display = "Use projectile"),
	FireTrace = 1 UMETA(Display = "Use trace")
};

struct FDamageData
{
    GENERATED_BODY()

        UPROPERTY()
        float DamageValue;
    UPROPERTY()
        AActor* Instigator;
    UPROPERTY()
        AActor* DamageMaker;
};