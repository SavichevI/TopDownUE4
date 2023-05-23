
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameFramework/PlayerState.h"
#include "Engine/DataTable.h"
//#include "Containers/Map.h"
#include "M_Magizine.h"
#include "BasePlayerState.generated.h"

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Aim_State UMETA(DisplayName = "Aim State"),
	Walk_State UMETA(DisplayName = "Walk State"),
	Run_State UMETA(DisplayName = "Run State"),
	Sprint_State UMETA(DisplayName = "Sprint State"),
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	AR4Type UMETA(DisplayName = "AR-4"),
	ShotGunType UMETA(DisplayName = "ShotGun"),
	ColtType UMETA(DisplayName = "Colt"),
	GrenateLauncherType UMETA(DisplayName = "GrenateLauncher")
};

USTRUCT(BlueprintType)
struct FCharacterSpeed
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movevent")
	float AimSpeed = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed = 300;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RunSpeed = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed = 800.0f;
};

USTRUCT(BlueprintType)
struct FDropMagazine
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magazine")
	TSubclassOf<class AM_Magizine> Magazine = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magazine")
	float LifeTimeMagazine = 2.0f;
	
};

USTRUCT(BlueprintType)
struct FProjectileInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSetting")
	TSubclassOf<class AProjectilesDefault> Projectile = nullptr;

	UPROPERTY(EditAnywhere, Category = "Demo") ///Delete.
	UStaticMesh* BulletMesh = nullptr;		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demo") ///Delete.
	UParticleSystem* BulletFX = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSetting")
	float ProjectileInitSpeed = 9999.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSetting")
	float PtojectileDamage = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSetting")
	float ProjectileLifeTime = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")   
	TMap<TEnumAsByte<EPhysicalSurface>, UMaterialInstance*> HitDecals;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")   
	TMap<TEnumAsByte<EPhysicalSurface>, UParticleSystem*> HitFXs;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	USoundBase* HitSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ForExplosion")
	UParticleSystem* ExplosionFX = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ForExplosion")
	USoundBase* ExplosionSound = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ForExplosion")
	float ProjectileMaxRadiusDamage = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ForExplosion")
	float ProjectileMinRadiusDamage = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ForExplosion")
	float ExplosionMaxDamage = 40.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ForExplosion")
	float ExplosionFalloffCoef = 1.0f;
	//Timer
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSetting")
// 	float IntensityShots = 3.0f;
};

USTRUCT(BlueprintType)
struct FShellBullets
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShellBullet")
	TSubclassOf<class AProjectilesDefault> ShellBullet = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShellBullet")
	float LifeTimeShellBullet = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShellBullet")
	float SpeedShellBullet = 500.0f;
};

USTRUCT(BlueprintType)
struct FWeaponDispersion
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion")
	float Aim_StateDispersionAimMax = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion")
	float Aim_StateDispersionAimMin = 0.3f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion")
	float Aim_StateDispersionAimRecoil = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion")
	float Aim_StateDispersionReduntion = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion")
	float Walk_StateDispersionAimMax = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion")
	float Walk_StateDispersionAimMin = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion")
	float Walk_StateDispersionAimRecoil = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion")
	float Walk_StateDispersionReduntion = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion")
	float Run_StateDispersionAimMax = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion")
	float Run_StateDispersionAimMin = 4.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion")
	float Run_StateDispersionAimRecoil = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion")
	float Run_StateDispersionReduntion = 0.1f;

};

USTRUCT(BlueprintType)
struct FWeaponInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
	TSubclassOf<class AWeaponDefault> WeaponClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class")
	UTexture2D* WeaponIcon = nullptr; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	EWeaponType WeaponType = EWeaponType::ColtType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	float SwichTimeToWeapon = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	FProjectileInfo ProjectileSetting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magazine")
	FDropMagazine Magazine;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShellBullet")
	FShellBullets ShellBullets;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float RateOfFire = 0.5f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "State")
	int32 MaxRound = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float ReloadTime = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	int32 NumberProjectileByShot = 1;

	//Dispersion
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dispersion")
	FWeaponDispersion DispersionWeapon;

	//FXs
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* SoundFireWeapon = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* SoundReloadWeapon = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	UParticleSystem* EffectFireWeapon = nullptr;
		
	//I don't know.
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitEffect")
// 	UDecalComponent* DecalOnHit = nullptr;

	//Animation.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	UAnimMontage* AnimCharFire = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	UAnimMontage* AnimCharFireIro = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	UAnimMontage* AnimCharReload = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	UAnimMontage* AnimCharReloadIro = nullptr;

	//Trace.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
	float WeaponDamage = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
	float DistacneTrace = 2000.0f;
};

USTRUCT(BlueprintType)
struct FAddicionalWeaponInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponStates")
	int32 Round = 10;
};

//Inventory.
USTRUCT(BlueprintType)
struct FWeaponSlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponSlot")
	FName NameItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponSlot")
	FAddicionalWeaponInfo AdditionalInfo;
};

USTRUCT(BlueprintType)
struct FAmmoSlot 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AmmoSlot")
	EWeaponType WeaponType = EWeaponType::ColtType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AmmoSlot")
	int32 Cout = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AmmoSlot")
	int32 MaxCout = 100;
};

USTRUCT(BlueprintType)
struct FDropItem : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DropWeapon")
	USkeletalMesh* WeaponSkeletMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DropWeapon")
	UStaticMesh* WeaponStaticMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DropWeapon")
	FWeaponSlot WeaponInfo;
};

UCLASS()
class TDS_API UBasePlayerState : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

};
