// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WeaponDefault.h"
#include "BasePlayerState.h"
#include "Engine/DataTable.h"
#include "M_GameInstance.generated.h"

UCLASS()
class TDS_API UM_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponState")
	UDataTable* WeaponInfoTable = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponState")
	UDataTable* DropItemInfoTable = nullptr;
	UFUNCTION(BlueprintCallable)
	bool GetWeaponInfoByName(FName NameWeapon, FWeaponInfo& OutInfo);
	UFUNCTION(BlueprintCallable)
	bool GetDropItemInfoByName(FName NameItem, FDropItem& OutInfo);
};
