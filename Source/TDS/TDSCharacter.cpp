// Copyright Epic Games, Inc. All Rights Reserved.

#include "TDSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "M_GameInstance.h"
#include "Engine/World.h"

ATDSCharacter::ATDSCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//Create a inventory...
	InventoryComponent = CreateDefaultSubobject<UM_InventaryComponent>(TEXT("InventoryComponent"));
	if (InventoryComponent)
	{
		InventoryComponent->OnSwitchWeapon.AddDynamic(this, &ATDSCharacter::InitWeapon);
	}

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ATDSCharacter::BeginPlay()
{
	Super::BeginPlay();

	//InitWeapon(InitWeaponName, );
	
	if (CursorMaterial)
	{
		CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector(0));
	}
}

void ATDSCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	if (CurrentCursor)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();

			CurrentCursor->SetWorldLocation(TraceHitResult.Location);
			CurrentCursor->SetWorldRotation(CursorR);
		}
	}
	MovementTick(DeltaSeconds);
}

void ATDSCharacter::SetupPlayerInputComponent(UInputComponent* MyInputComponent)
{
	Super::SetupPlayerInputComponent(MyInputComponent);

	MyInputComponent->BindAxis(TEXT("MoveForward"), this, &ATDSCharacter::InputAxisX);
	MyInputComponent->BindAxis(TEXT("MoveRight"), this, &ATDSCharacter::InputAxisY);

	MyInputComponent->BindAction(TEXT("MoveSprint"), EInputEvent::IE_Pressed, this, &ATDSCharacter::InputSprintPressed);
	MyInputComponent->BindAction(TEXT("MoveSprint"), EInputEvent::IE_Released, this, &ATDSCharacter::InputSprintReleased);
	MyInputComponent->BindAction(TEXT("AimEvent"), EInputEvent::IE_Pressed, this, &ATDSCharacter::InputAimPressed);
	MyInputComponent->BindAction(TEXT("AimEvent"), EInputEvent::IE_Released, this, &ATDSCharacter::InputAimRelesed);
	MyInputComponent->BindAction(TEXT("FireEvent"), EInputEvent::IE_Pressed, this, &ATDSCharacter::InputAttackPressed);
	MyInputComponent->BindAction(TEXT("FireEvent"), EInputEvent::IE_Released, this, &ATDSCharacter::InputAttackReleased);
	MyInputComponent->BindAction(TEXT("ReloadEvent"), EInputEvent::IE_Pressed, this, &ATDSCharacter::TryReloadWeapon);

	MyInputComponent->BindAction(TEXT("SwitchNextWeapon"), EInputEvent::IE_Pressed, this, &ATDSCharacter::TrySwitchNextWeapon);
	MyInputComponent->BindAction(TEXT("SwitchPreviousWeapon"), EInputEvent::IE_Pressed, this, &ATDSCharacter::TrySwitchPreviousWeapon);

}

void ATDSCharacter::RemoveCurrentWeapon()
{
	//////////////////
}

void ATDSCharacter::InputAxisX(float Value)
{
	AxisY = Value;
}

void ATDSCharacter::InputAxisY(float Value)
{
	AxisX = Value;
}

void ATDSCharacter::InputAttackPressed()
{	
	AttackCharEvent(true);	
}

void ATDSCharacter::InputAttackReleased()
{
	AttackCharEvent(false);
}

void ATDSCharacter::InputAimPressed()
{
	bAimEnabled = true;
	
	ChangeMovementState(EMovementState::Aim_State);
}

void ATDSCharacter::InputAimRelesed()
{
	bAimEnabled = false;

	ChangeMovementState(EMovementState::Run_State);
}

void ATDSCharacter::InputSprintPressed()
{
	bSprintEnabled = true;
	ChangeMovementState(EMovementState::Sprint_State);
}

void ATDSCharacter::InputSprintReleased()
{
	bSprintEnabled = false;
	ChangeMovementState(EMovementState::Run_State);
}

void ATDSCharacter::TrySwitchNextWeapon()
{
	if (InventoryComponent->WeaponSlot.Num() > 1)
	{
		int8 OldIndex = CurrentIndexWeapon;
		FAddicionalWeaponInfo OldInfo;
		if (CurrentWeapon)
		{
			OldInfo = CurrentWeapon->WeaponInfo;
			if (CurrentWeapon->bWeaponReloading)
			{
				CurrentWeapon->CancelReload();
			}
		}

		if (InventoryComponent)
		{			
 			if (InventoryComponent->SwitchWeaponToIndex(CurrentIndexWeapon + 1, OldIndex, OldInfo, true))
 			{	
				InventoryComponent->OnWeaponAmmoAviable.Broadcast(CurrentWeapon->WeaponSetting.WeaponType);
 			}
		}
	}
}

void ATDSCharacter::TrySwitchPreviousWeapon()
{
	if (InventoryComponent->WeaponSlot.Num() > 1)
	{
		int8 OldIndex = CurrentIndexWeapon;
		FAddicionalWeaponInfo OldInfo;
		if (CurrentWeapon)
		{
			OldInfo = CurrentWeapon->WeaponInfo;
			if (CurrentWeapon->bWeaponReloading)
			{
				CurrentWeapon->CancelReload();
			}
		}

		if (InventoryComponent)
		{
			if (InventoryComponent->SwitchWeaponToIndex(CurrentIndexWeapon - 1, OldIndex, OldInfo, false))
			{
				
			}
		}
	}
}

void ATDSCharacter::TryReloadWeapon()
{
	if (CurrentWeapon && !CurrentWeapon->bWeaponReloading)
	{
		if (CurrentWeapon->GetWeaponRound() < CurrentWeapon->WeaponSetting.MaxRound)
		{
			CurrentWeapon->InitReload();

			//PlayAnimReload();//Play animation.
		}
	}
}

void ATDSCharacter::AttackCharEvent(bool bIsFiring)
{
	AWeaponDefault* myWeapon = GetCurrentWeapon();
	
	if (myWeapon)
	{
		myWeapon->SetWeaponStateFire(bIsFiring);	
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ATDHCharacter::AttackCharEvent - CurrentWeapon - NULL"));
	}
}

void ATDSCharacter::ChangeMovementState(EMovementState NewMovementState)
{
	if (!bWalkEnabled && !bSprintEnabled && !bAimEnabled)
	{
		MovementState = EMovementState::Run_State;
	}
	else
	{
		if (bSprintEnabled)
		{
			bWalkEnabled = false;
			bAimEnabled = false;
			MovementState = EMovementState::Sprint_State;
		}
		else
		{
			if (bWalkEnabled && !bSprintEnabled && !bAimEnabled)
			{
				MovementState = EMovementState::Walk_State;
			}
			else
			{
				if (!bWalkEnabled && !bSprintEnabled && bAimEnabled)
				{
					MovementState = EMovementState::Aim_State;
				}
			}
		}
	}
	CharacterUpdate();

	AWeaponDefault* myWeapon = GetCurrentWeapon();
	if (myWeapon)
	{
		myWeapon->UpdateStateWeapon(MovementState);
	}
}

void ATDSCharacter::CharacterUpdate()
{
	float ResSpeed = 600.0f;
	switch (MovementState)
	{
	case EMovementState::Aim_State:ResSpeed = MovementInfo.AimSpeed;
		break;
	case EMovementState::Walk_State:ResSpeed = MovementInfo.WalkSpeed;
		break;
	case EMovementState::Run_State:ResSpeed = MovementInfo.RunSpeed;
		break;
	case EMovementState::Sprint_State:ResSpeed = MovementInfo.SprintSpeed;
		break;
	default:
		break;
	}

	GetCharacterMovement()->MaxWalkSpeed = ResSpeed;

}

void ATDSCharacter::InitWeapon(FName IdWeaponName, FAddicionalWeaponInfo WeaponAdditionInfo, int32 NewCurrentIndexWeapon)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}

	UM_GameInstance* myGI = Cast<UM_GameInstance>(GetGameInstance());
	FWeaponInfo myWeaponInfo;
	if (myGI)
	{		
		if (myGI->GetWeaponInfoByName(IdWeaponName, myWeaponInfo))
		{
			if (myWeaponInfo.WeaponClass)
			{
				FVector SpawnLocation = FVector(0);
				FRotator SpawnRotation = FRotator(0);
				/////////////////////////////////////////////////////////
				FActorSpawnParameters SpawnParameters;
				SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				SpawnParameters.Owner = this;
				SpawnParameters.Instigator = GetInstigator();

				AWeaponDefault* myWeapon = Cast<AWeaponDefault>(GetWorld()->SpawnActor(myWeaponInfo.WeaponClass, &SpawnLocation, &SpawnRotation, SpawnParameters));
				if (myWeapon)
				{
					FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, false);
					myWeapon->AttachToComponent(GetMesh(), Rule, FName("SocketOfWeapon_R"));
					CurrentWeapon = myWeapon;

					myWeapon->WeaponSetting = myWeaponInfo;
					//myWeapon->WeaponInfo.Round = myWeaponInfo.MaxRound;

					myWeapon->ReloadTime = myWeaponInfo.ReloadTime;
					myWeapon->UpdateStateWeapon(MovementState);

					myWeapon->WeaponInfo = WeaponAdditionInfo;

					CurrentIndexWeapon = NewCurrentIndexWeapon;

					myWeapon->OnWeaponFire.AddDynamic(this, &ATDSCharacter::PlayAnimFire);
					myWeapon->OnWeaponReloadStart.AddDynamic(this, &ATDSCharacter::PlayAnimReload);
					myWeapon->OnWeaponReloadEnd.AddDynamic(this, &ATDSCharacter::StopAnimReload);

					if (CurrentWeapon->WeaponInfo.Round <= 0 && CurrentWeapon->CheckCanWeaponReload())
					{
						CurrentWeapon->InitReload();
					}

					if (InventoryComponent)
					{
						InventoryComponent->OnWeaponAmmoAviable.Broadcast(myWeapon->WeaponSetting.WeaponType);
					}
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ATDSCharacter::InitWeapon - Weapon not found in table - NULL"));
		}
	}
}

AWeaponDefault* ATDSCharacter::GetCurrentWeapon()//Blueprint.
{
	return CurrentWeapon;
}

void ATDSCharacter::MovementTick(float DeltaTime)
{
	AddMovementInput(FVector(0.0f, 1.0f, 0.0f), AxisX);
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), AxisY);

	if (MovementState == EMovementState::Sprint_State)
	{
		FVector myRotationVector = FVector(AxisY, AxisX, 0.0f);
		FRotator myRotator = myRotationVector.ToOrientationRotator();
		SetActorRotation((FQuat(myRotator)));
	}
	else
	{
		APlayerController* myController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (myController)
		{
			FHitResult ResultHit;
			myController->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);
			float FindRotaterResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location).Yaw;
			SetActorRotation(FQuat(FRotator(0.0f, FindRotaterResultYaw, 0.0f)));

			if (CurrentWeapon)
			{
				FVector Displacement = FVector(0);
				switch (MovementState)
				{
				case EMovementState::Aim_State:
					Displacement = FVector(0.0f, 0.0f, 160.0f);
					CurrentWeapon->bShouldReduceDispersion = true;
					break;
				case EMovementState::Walk_State:
					CurrentWeapon->bShouldReduceDispersion = false;
					Displacement = FVector(0.0f, 0.0f, 120.0f);
					break;
				case EMovementState::Run_State:
					Displacement = FVector(0.0f, 0.0f, 120.0f);
					CurrentWeapon->bShouldReduceDispersion = false;
					break;
				case EMovementState::Sprint_State:
					break;
				default:
					break;
				}

				CurrentWeapon->ShootEndLocation = ResultHit.Location + Displacement;
			}
		}
	}
}

UDecalComponent* ATDSCharacter::GetCursorToWorld()
{
	return CurrentCursor;
}

void ATDSCharacter::PlayAnimFire()
{
	if (InventoryComponent && CurrentWeapon)
	{
		InventoryComponent->SetAdditionalInfoWeapon(CurrentIndexWeapon, CurrentWeapon->WeaponInfo);
	}

	if (bAimEnabled)
	{
		if (CurrentWeapon->WeaponSetting.AnimCharFireIro)
		{
			PlayAnimMontage(CurrentWeapon->WeaponSetting.AnimCharFireIro, 1, NAME_None);
		}		
	}
	else
	{
		if (CurrentWeapon->WeaponSetting.AnimCharFire)
		{
			PlayAnimMontage(CurrentWeapon->WeaponSetting.AnimCharFire, 1, NAME_None);
		}
	}
}

 void ATDSCharacter::PlayAnimReload()
 {
	 if (bAimEnabled)
	 {
		 if (CurrentWeapon->WeaponSetting.AnimCharReloadIro)
		 {
			 PlayAnimMontage(CurrentWeapon->WeaponSetting.AnimCharReloadIro, 1, NAME_None);
		 }
	 }
	 else
	 {
		 if (CurrentWeapon->WeaponSetting.AnimCharReload)
		 {
			 PlayAnimMontage(CurrentWeapon->WeaponSetting.AnimCharReload, 1, NAME_None);
		 }
	 }
 }

 void ATDSCharacter::StopAnimReload(int32 AmmoTake)
 {
	 if (InventoryComponent && CurrentWeapon)
	 {
		 InventoryComponent->AmmoSlotChangeValue(CurrentWeapon->WeaponSetting.WeaponType, AmmoTake);
		 InventoryComponent->SetAdditionalInfoWeapon(CurrentIndexWeapon, CurrentWeapon->WeaponInfo);
	 }

	 if (bAimEnabled)
	 {
		 if (CurrentWeapon->WeaponSetting.AnimCharReloadIro)
		 {
			 StopAnimMontage(CurrentWeapon->WeaponSetting.AnimCharReloadIro);
		 }
	 } 
	 else
	 {
		 if (CurrentWeapon->WeaponSetting.AnimCharReload)
		 {
			 StopAnimMontage(CurrentWeapon->WeaponSetting.AnimCharReload);
		 }
	 }
 }