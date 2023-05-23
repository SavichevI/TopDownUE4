// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "BasePlayerState.h"
#include "ProjectilesDefault.h"
#include "WeaponDefault.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponFire);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponReloadStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponReloadEnd, int32, AmmoTake);

UCLASS()
class TDS_API AWeaponDefault : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponDefault();

	FOnWeaponFire OnWeaponFire;
	FOnWeaponReloadEnd OnWeaponReloadEnd;
	FOnWeaponReloadStart OnWeaponReloadStart;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
	class USceneComponent* SceneComponent = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
	class USkeletalMeshComponent* SkeletalMeshWeapon = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
	class UStaticMeshComponent* StaticMeshWeapon = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
	class UArrowComponent* ShootLocation = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
	class UArrowComponent* ShellBulletsLocation = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
	class UArrowComponent* MagazineLocation = nullptr;

	UPROPERTY()
	FWeaponInfo WeaponSetting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponInfo")
	FAddicionalWeaponInfo WeaponInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireLogic")
	bool bWeaponFiring = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReloadLogic")
	bool bWeaponReloading = false;


	//Timer for shot.
	float FireTime = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReloadLogic")
	float ReloadTimer = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReloadLogicDrbug")
	float ReloadTime = 0.0f;
	UPROPERTY(BlueprintReadOnly)
	float ReloadTimeOfVidget;

	//Function.

	
	//Tick
	void ReloadTick(float DeltaTime);
	void DispersionTick(float DeltaTime);
	void FireTick(float DeltaTime);

	void WeaponInit();
	void Fire();
	void UpdateStateWeapon(EMovementState NewMovementState);
	void ChangeDispersionByShot();
	bool CheckWeaponCanFire();
	FProjectileInfo GetProjectile();//Blueprint.

	//////////
	UFUNCTION(BlueprintCallable) //<<<<Function of reload
	void DropMagazine();// My function.
	///////////////

	UFUNCTION(BlueprintCallable)
	void SetWeaponStateFire(bool bIsFire);

	float GetCurrentDispersion() const;
	FVector ApplyDispersionToShot(FVector DirectionShot) const;	//Shot or Shoot???
	int8 GetNumberProjectileByShot() const;
	FVector GetFireEndLocation() const;

	
	UFUNCTION(BlueprintCallable)
	int32 GetWeaponRound();//Blueprint.
	// Reload.
	void InitReload();
	void FinishReload();
	void CancelReload();
	
	//Inventory.
	UFUNCTION(BlueprintCallable)
	bool CheckCanWeaponReload();
	int8 GetAviableAmmoForReload();

	//Flags.
	bool bBlockFire = false;

	//Dispersion
	bool bShouldReduceDispersion = false;
	float CurrentDispersion = 0.0f;
	float CurrentDispersionMax = 1.0f;
	float CurrentDispersionMin = 0.1f;
	float CurrentDispersionRecoil = 0.1f;
	float CurrentDispersionReduction = 0.1;

	FVector ShootEndLocation = FVector(0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bShowDebug = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	float SizeVectorToChangeShootDirectionLogoc = 100.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
