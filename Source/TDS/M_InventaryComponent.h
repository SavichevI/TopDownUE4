// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BasePlayerState.h"
#include "M_InventaryComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSwitchWeapon, FName, WeaponIdName, FAddicionalWeaponInfo, WeaponAdditionalInfo, int32, NewCurrentIndexWeapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChange, EWeaponType, TypeAmmo, int32, Cout);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponAdditionalInfoChange, int32, IndexSlot, FAddicionalWeaponInfo, AdditionalInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponAmmoEmpty, EWeaponType, WeaponType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponAmmoAviable, EWeaponType, WeaponType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateWeaponSlots, int32, IndexSlotChange, FWeaponSlot, NewInfo);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TDS_API UM_InventaryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UM_InventaryComponent();

	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FOnSwitchWeapon OnSwitchWeapon;
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FOnAmmoChange OnAmmoChange;
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FOnWeaponAdditionalInfoChange OnWeaponAdditionalInfoChange;
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FOnWeaponAmmoEmpty OnWeaponAmmoEmpty;
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FOnWeaponAmmoAviable OnWeaponAmmoAviable;
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FOnUpdateWeaponSlots OnUpdateWeaponSlots;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	TArray<FWeaponSlot> WeaponSlot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	TArray<FAmmoSlot> AmmoSlot;

	int32 MaxSlotWeapon = 4;

	//Function.
	bool SwitchWeaponToIndex(int32  ChangeToIndex, int32 OldIndex, FAddicionalWeaponInfo OldInfo, bool bIsForward);

	FAddicionalWeaponInfo GetAdditionalInfoWeapon(int32 IndexWeapon);
	int32 GetWeaponIndexSlotByName(FName IdWeaponName);
	FName GetWeaponNameBySlotIndex(int32 IndexSlot);
	void SetAdditionalInfoWeapon(int32 IndexWeapon, FAddicionalWeaponInfo NewInfo);

	UFUNCTION(BlueprintCallable)
	void AmmoSlotChangeValue(EWeaponType TypeWeapon, int32 CoutChangeAmmo);

	bool CheckAmmoForWeapon(EWeaponType TypeWeapon, int8 &AvailableAmmoForWeapon);

	//Functions for interface.
	UFUNCTION(BlueprintCallable, Category = "Interface")
	bool CheckCanTakeAmmo(EWeaponType AmmoType);
	UFUNCTION(BlueprintCallable, Category = "Interface")
	bool CheckCantakeWeapon(int32 &FreeSlot);
	UFUNCTION(BlueprintCallable, Category = "Interface")
	bool SwitchWeaponToInventory(FWeaponSlot NewWeapon, int32 IndexSlot, int32 CurrentIndexWeaponCher, FDropItem &DropItemInfo);
	UFUNCTION(BlueprintCallable, Category = "Interface")
	bool GetDropItemInfoFromInventory(int32 IndexSlot, FDropItem &DropItemInfo);
	UFUNCTION(BlueprintCallable, Category = "Interface")
	bool TeyGetWeaponToInventory(FWeaponSlot NewWeapon);
	UFUNCTION(BlueprintCallable, Category = "Interface")
	void WeaponReloaded(EWeaponType TypeWeapon, int32 AmmoTaken);

};
