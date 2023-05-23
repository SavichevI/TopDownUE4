// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponDefault.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Math/Color.h"
#include "M_InventaryComponent.h"

// Sets default values
AWeaponDefault::AWeaponDefault()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneComponent;

	SkeletalMeshWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletelMesh"));
	SkeletalMeshWeapon->SetGenerateOverlapEvents(false);
	SkeletalMeshWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	SkeletalMeshWeapon->SetupAttachment(RootComponent);

	StaticMeshWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshWeapon->SetGenerateOverlapEvents(false);
	StaticMeshWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	StaticMeshWeapon->SetupAttachment(RootComponent);

	ShootLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("ShootLocotion"));
	ShootLocation->SetupAttachment(RootComponent);

	ShellBulletsLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("CasingLocation"));
	ShellBulletsLocation->SetupAttachment(RootComponent);

	MagazineLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("MagazineLocation"));
	MagazineLocation->SetupAttachment(RootComponent);

	ReloadTimeOfVidget = WeaponSetting.ReloadTime;
	
}



void AWeaponDefault::DispersionTick(float DeltaTime)
{
	if (!bWeaponReloading)
	{
		if (!bWeaponFiring)
		{
			if (bShouldReduceDispersion)
			{
				CurrentDispersion = CurrentDispersion - CurrentDispersionReduction;
			} 
			else
			{
				CurrentDispersion = CurrentDispersion + CurrentDispersionReduction;
			}
		}
		if (CurrentDispersion < CurrentDispersionMin)
		{
			CurrentDispersion = CurrentDispersionMin;
		}
		else
		{
			if (CurrentDispersion > CurrentDispersionMax)
			{
				CurrentDispersion = CurrentDispersionMax;
			}
		}
	}
	if (bShowDebug)
	{
		UE_LOG(LogTemp, Warning, TEXT("Dispersion: MAX = %f. Min = %f. Current = %f."), CurrentDispersionMax, CurrentDispersionMin, CurrentDispersion)
	}
}

void AWeaponDefault::WeaponInit()
{
	if (SkeletalMeshWeapon && !SkeletalMeshWeapon->SkeletalMesh)
	{
		SkeletalMeshWeapon->DestroyComponent(true);
	}

	if (StaticMeshWeapon && !StaticMeshWeapon->GetStaticMesh())
	{
		StaticMeshWeapon->DestroyComponent(true);
	}
}

void AWeaponDefault::UpdateStateWeapon(EMovementState NewMovementState)
{
	bBlockFire = false;

	switch (NewMovementState)
	{
	case EMovementState::Aim_State:
		CurrentDispersionMax = WeaponSetting.DispersionWeapon.Aim_StateDispersionAimMax;
		CurrentDispersionMin = WeaponSetting.DispersionWeapon.Aim_StateDispersionAimMin;
		CurrentDispersionRecoil = WeaponSetting.DispersionWeapon.Aim_StateDispersionAimRecoil;
		CurrentDispersionReduction = WeaponSetting.DispersionWeapon.Aim_StateDispersionReduntion;

		break;
	case EMovementState::Walk_State:
		CurrentDispersionMax = WeaponSetting.DispersionWeapon.Walk_StateDispersionAimMax;
		CurrentDispersionMin = WeaponSetting.DispersionWeapon.Walk_StateDispersionAimMin;
		CurrentDispersionRecoil = WeaponSetting.DispersionWeapon.Walk_StateDispersionAimRecoil;
		CurrentDispersionReduction = WeaponSetting.DispersionWeapon.Walk_StateDispersionReduntion;

		break;
	case EMovementState::Run_State:
		CurrentDispersionMax = WeaponSetting.DispersionWeapon.Run_StateDispersionAimMax;
		CurrentDispersionMin = WeaponSetting.DispersionWeapon.Run_StateDispersionAimMin;
		CurrentDispersionRecoil = WeaponSetting.DispersionWeapon.Run_StateDispersionAimRecoil;
		CurrentDispersionReduction = WeaponSetting.DispersionWeapon.Run_StateDispersionReduntion;

		break;
	case EMovementState::Sprint_State:
		bBlockFire = false;
		SetWeaponStateFire(false);

		break;
	default:
		break;
	}
}

bool AWeaponDefault::CheckWeaponCanFire()
{
	return !bBlockFire;
}

bool AWeaponDefault::CheckCanWeaponReload()
{
	bool Result = true;
	if (GetOwner())
	{
		UM_InventaryComponent* myInventory = Cast<UM_InventaryComponent>(GetOwner()->GetComponentByClass(UM_InventaryComponent::StaticClass()));
		if (myInventory)
		{
			int8 AviableAmmoForWeapon = WeaponSetting.MaxRound;
			if (!myInventory->CheckAmmoForWeapon(WeaponSetting.WeaponType, AviableAmmoForWeapon))
			{
				Result = false;
			}
		}
	}
	return Result;
}

FProjectileInfo AWeaponDefault::GetProjectile()//Blueprint.
{
	return WeaponSetting.ProjectileSetting;
}

void AWeaponDefault::SetWeaponStateFire(bool bIsFire)
{
	if (CheckWeaponCanFire())
	{
		bWeaponFiring = bIsFire;
	}
	else
	{
		bWeaponFiring = false;
		FireTime = 0.01f;		/////////////
	}
}

void AWeaponDefault::DropMagazine()
{
	if (WeaponSetting.Magazine.Magazine)
	{
		FVector SpawnLocation = MagazineLocation->GetComponentLocation();
		FRotator SpawnRotator = MagazineLocation->GetComponentRotation();
		FActorSpawnParameters SpawnParameter;
		SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParameter.Owner = nullptr;
		AM_Magizine* Magazine = GetWorld()->SpawnActor<AM_Magizine>(WeaponSetting.Magazine.Magazine, SpawnLocation, SpawnRotator, SpawnParameter);
	}
}

float AWeaponDefault::GetCurrentDispersion() const
{
	float Result = CurrentDispersion;
	return Result;
}

FVector AWeaponDefault::ApplyDispersionToShot(FVector DispersionShot) const
{
	return FMath::VRandCone(DispersionShot, GetCurrentDispersion() * PI / 180.0f);
}

int8 AWeaponDefault::GetNumberProjectileByShot() const
{
	return WeaponSetting.NumberProjectileByShot;
}

int32 AWeaponDefault::GetWeaponRound()//Blueprint.
{
	return WeaponInfo.Round;
}

void AWeaponDefault::InitReload()
{
	bWeaponReloading = true;

	ReloadTime = WeaponSetting.ReloadTime;
		
	OnWeaponReloadStart.Broadcast();//play animation.
}

void AWeaponDefault::FinishReload()
{
	bWeaponReloading = false;

	int8 AviableAmmoFromInventory = GetAviableAmmoForReload();
	int8 AmmoNeedTakeFromInventory;
	int8 NeedToReload = WeaponSetting.MaxRound - WeaponInfo.Round;

	if (NeedToReload > AviableAmmoFromInventory)
	{
		WeaponInfo.Round = AviableAmmoFromInventory;
		AmmoNeedTakeFromInventory = AviableAmmoFromInventory;
	}
	else
	{
		WeaponInfo.Round += NeedToReload;
		AmmoNeedTakeFromInventory = NeedToReload;
	}
	OnWeaponReloadEnd.Broadcast(-AmmoNeedTakeFromInventory);//Play animation.
}

void AWeaponDefault::CancelReload()
{
	bWeaponReloading = false;
	if (SkeletalMeshWeapon && SkeletalMeshWeapon->GetAnimInstance())
	{
		SkeletalMeshWeapon->GetAnimInstance()->StopAllMontages(0.15f);
	}

	OnWeaponReloadEnd.Broadcast(0);//Play animation.
	
}



int8 AWeaponDefault::GetAviableAmmoForReload()
{
	int8 AviableAmmoForWeapon = WeaponSetting.MaxRound;
	if (GetOwner())
	{
		UM_InventaryComponent* myInventory = Cast<UM_InventaryComponent>(GetOwner()->GetComponentByClass(UM_InventaryComponent::StaticClass()));
		if (myInventory)
		{
			if (myInventory->CheckAmmoForWeapon(WeaponSetting.WeaponType, AviableAmmoForWeapon))
			{
				AviableAmmoForWeapon = AviableAmmoForWeapon;
			}
		}
	}
	return AviableAmmoForWeapon;
}

FVector AWeaponDefault::GetFireEndLocation() const
{
	//bool bShootDirection = false;
	FVector EndLocation = FVector(0.0f);
	
	FVector tmpV = (ShootLocation->GetComponentLocation() - ShootEndLocation);
	//UE_LOG(LogTemp, Warning, TEXT("Vector: X = %f. Y = %f. Size = %f"), tmpV.X, tmpV.Y, tmpV.Size());

	if(tmpV.Size() > SizeVectorToChangeShootDirectionLogoc)
	{
		EndLocation = ShootLocation->GetComponentLocation() + ApplyDispersionToShot((ShootLocation->GetComponentLocation() - ShootEndLocation).GetSafeNormal()) * -20000.0f;
		if(bShowDebug)
			DrawDebugCone(GetWorld(), ShootLocation->GetComponentLocation(), -(ShootLocation->GetComponentLocation() - ShootEndLocation), WeaponSetting.DistacneTrace, GetCurrentDispersion()* PI / 180.f, GetCurrentDispersion()* PI / 180.f, 32, FColor::Emerald, false, .1f, (uint8)'\000', 1.0f);
	}	
	else
	{
		EndLocation = ShootLocation->GetComponentLocation() + ApplyDispersionToShot(ShootLocation->GetForwardVector()) * 20000.0f;
		if (bShowDebug)
			DrawDebugCone(GetWorld(), ShootLocation->GetComponentLocation(), ShootLocation->GetForwardVector(), WeaponSetting.DistacneTrace, GetCurrentDispersion()* PI / 180.f, GetCurrentDispersion()* PI / 180.f, 32, FColor::Emerald, false, .1f, (uint8)'\000', 1.0f);
	}
		

	if (bShowDebug)
	{
		
		DrawDebugLine(GetWorld(), ShootLocation->GetComponentLocation(), ShootLocation->GetComponentLocation() + ShootLocation->GetForwardVector() * 500.0f, FColor::Cyan, false, 5.f, (uint8)'\000', 0.5f);
		////////////////////////
		DrawDebugLine(GetWorld(), ShootLocation->GetComponentLocation(), ShootEndLocation, FColor::Red, false, 5.f, (uint8)'\000', 0.5f);
		/////////////////////////
		DrawDebugLine(GetWorld(), ShootLocation->GetComponentLocation(), EndLocation, FColor::Black, false, 5.f, (uint8)'\000', 0.5f);

		////DrawDebugSphere(GetWorld(), ShootLocation->GetComponentLocation() + ShootLocation->GetForwardVector()*SizeVectorToChangeShootDirectionLogic, 10.f, 8, FColor::Red, false, 4.0f);
	}		

	return EndLocation;

}

void AWeaponDefault::ChangeDispersionByShot()
{
	CurrentDispersion = CurrentDispersion + CurrentDispersionRecoil;
}

void AWeaponDefault::Fire()
{
	FireTime = WeaponSetting.RateOfFire;
	WeaponInfo.Round = WeaponInfo.Round - 1;
	ChangeDispersionByShot();

	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), WeaponSetting.SoundFireWeapon, ShootLocation->GetComponentLocation());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponSetting.EffectFireWeapon, ShootLocation->GetComponentTransform());

	int8 NumberProjectile = GetNumberProjectileByShot();
	
	if (ShootLocation)
	{
		FVector SpawnLocation = ShootLocation->GetComponentLocation();
		FRotator SpawnRotation = ShootLocation->GetComponentRotation();
		FProjectileInfo ProjectaileInfo = GetProjectile();
		FVector EndLocation;

		FCollisionQueryParams Param;
		FHitResult HitResuit;

		for (int8 i = 0; i < NumberProjectile; i++)
		{
			EndLocation = GetFireEndLocation();

			FVector Dir = EndLocation - SpawnLocation;
			Dir.Normalize();

			FMatrix myMatrix(Dir, FVector(0, 1, 0), FVector(0, 0, 1), FVector::ZeroVector);
			SpawnRotation = myMatrix.Rotator();

			if (ProjectaileInfo.Projectile)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				SpawnParams.Owner = GetOwner();
				SpawnParams.Instigator = GetInstigator();

				AProjectilesDefault* myProjectile = Cast<AProjectilesDefault>(GetWorld()->SpawnActor(ProjectaileInfo.Projectile, &SpawnLocation, &SpawnRotation, SpawnParams));
				if (myProjectile)
				{
					myProjectile->InitProjectile(WeaponSetting.ProjectileSetting);
					
					OnWeaponFire.Broadcast();//Play animation.

					FShellBullets SpawnShell = WeaponSetting.ShellBullets;
					if (SpawnShell.ShellBullet)
					{
						FVector SpawnShellBulletLocation = ShellBulletsLocation->GetComponentLocation();
						FRotator SpawnShellBulletRotator = ShellBulletsLocation->GetComponentRotation();
												
						AProjectilesDefault* IsShellBullet = Cast<AProjectilesDefault>(GetWorld()->SpawnActor(SpawnShell.ShellBullet, &SpawnShellBulletLocation, &SpawnShellBulletRotator, SpawnParams));
						IsShellBullet->InitShellBullet(WeaponSetting.ShellBullets);
					}
				}
			}
			else
			{
				GetWorld()->LineTraceSingleByChannel(HitResuit, SpawnLocation, EndLocation, ECollisionChannel::ECC_Visibility, Param, FCollisionResponseParams());
			}
		}
	}

	if (GetWeaponRound() <= 0 && !bWeaponReloading)
	{
		if (CheckCanWeaponReload())
		{
			InitReload();
		}
	}
}

void AWeaponDefault::ReloadTick(float DeltaTime)
{
	if (bWeaponReloading)
	{
		if (ReloadTime < 0.0f)
		{
			FinishReload();
		} 
		else
		{
			ReloadTime -= DeltaTime;
		}
	}
}

void AWeaponDefault::FireTick(float DeltaTime)
{
	
	if (bWeaponFiring && GetWeaponRound() && !bWeaponReloading)
	{
		if (FireTime < 0)
		{
			Fire();		
		} 
		else
		{
			FireTime -= DeltaTime;
		}
	}

}

// Called when the game starts or when spawned
void AWeaponDefault::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AWeaponDefault::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FireTick(DeltaTime);
	ReloadTick(DeltaTime);
	DispersionTick(DeltaTime);
	//m_PlayingAnimMontage(DeltaTime);
}

