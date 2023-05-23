// Fill out your copyright notice in the Description page of Project Settings.


#include "M_GameInstance.h"

bool UM_GameInstance::GetWeaponInfoByName(FName NameWeapon, FWeaponInfo& OutInfo)
{
	FWeaponInfo* WeaponInfoRow;
	bool bIsFind = false;

	if (WeaponInfoTable)
	{
		WeaponInfoRow = WeaponInfoTable->FindRow<FWeaponInfo>(NameWeapon, "", false);
			if (WeaponInfoRow)
			{
				bIsFind = true;
				OutInfo = *WeaponInfoRow;
			}
	} 
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UM_GameInstance::GetWeaponByName - Weapon table - NULL"));
	}

	return bIsFind;
}

bool UM_GameInstance::GetDropItemInfoByName(FName NameItem, FDropItem& OutInfo)
{
	bool bIsFind = false;
	if (DropItemInfoTable)
	{
		FDropItem* DropItemInfoRow;
		TArray<FName> RowNames = DropItemInfoTable->GetRowNames();

		int8 i = 0;
		while (i < RowNames.Num() && !bIsFind)
		{
			DropItemInfoRow = DropItemInfoTable->FindRow<FDropItem>(RowNames[i], "");
			if (DropItemInfoRow->WeaponInfo.NameItem == NameItem)
			{
				OutInfo = (*DropItemInfoRow);
				bIsFind = true;
			}
			i++;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UM_GameInstance::GetDropItemInfoByName - DropItemInfoTable - NULL"));
	}
	return bIsFind;
}
