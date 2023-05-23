// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BasePlayerState.h"
#include "WeaponDefault.h"
#include "M_InventaryComponent.h"
#include "Animation/AnimMontage.h"
#include "TDSCharacter.generated.h"

UCLASS(Blueprintable)
class ATDSCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	ATDSCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* MyInputComponent) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	//FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

	// My Work.
	// Cursor.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	UMaterial* CursorMaterial = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	FVector CursorSize = FVector(20.0f, 40.0f, 40.0f);

	UDecalComponent* CurrentCursor = nullptr;

	// Movement character.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	EMovementState MovementState = EMovementState::Run_State;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FCharacterSpeed MovementInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bAimEnabled = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bWalkEnabled = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bSprintEnabled = false;

	float AxisX = 0.0f;
	float AxisY = 0.0f;


	//A Weapon.
	AWeaponDefault* CurrentWeapon = nullptr;

	//Inventory.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UM_InventaryComponent* InventoryComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 CurrentIndexWeapon = 0;

	//The Demo.
 
	//Functions.
	UFUNCTION(BlueprintCallable)
	void AttackCharEvent(bool bIsFiring);
	UFUNCTION(BlueprintCallable)
	void ChangeMovementState(EMovementState NewMovementState);
	UFUNCTION(BlueprintCallable)
	void CharacterUpdate();
	
	//Functions for Weapon
	UFUNCTION(BlueprintCallable)
	void InitWeapon(FName IdWeaponName, FAddicionalWeaponInfo WeaponAdditionalInfo, int32 NewCurrrentIndexWeapon);
	UFUNCTION(BlueprintCallable)
	AWeaponDefault* GetCurrentWeapon();
	UFUNCTION(BlueprintCallable)
	void TryReloadWeapon();
//	UFUNCTION()
// 	void WeaponReloadStart(UAnimMontage* AnimReload, UAnimMontage* AnimReloadAim);
// 	UFUNCTION()
// 	void WeaponReloadEnd(bool bIsSuccess, int32 AmmoSafe);
// 	UFUNCTION(BlueprintNativeEvent)
// 	void WeaponReloadStart_BP(UAnimMontage* Anim);
// 	UFUNCTION(BlueprintNativeEvent)
// 	void WeaponReloadEnd_BP();
	UFUNCTION(BlueprintCallable)
	void RemoveCurrentWeapon();

	//Inputs.
	UFUNCTION()
	void InputAxisX(float Value);
	UFUNCTION()
	void InputAxisY(float Value);
	UFUNCTION()
	void InputAttackPressed();
	UFUNCTION()
	void InputAttackReleased();
	UFUNCTION()
	void InputAimPressed();
	UFUNCTION()
	void InputAimRelesed();
	UFUNCTION()
	void InputSprintPressed();
	UFUNCTION()
	void InputSprintReleased();
	
	void TrySwitchNextWeapon();
	void TrySwitchPreviousWeapon();

	// Tick function.
	UFUNCTION()
	void MovementTick(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	UDecalComponent* GetCursorToWorld();

	//Function of animation.
	UFUNCTION(BlueprintCallable)
	void PlayAnimFire();
 	UFUNCTION(BlueprintCallable)
 	void PlayAnimReload();
	UFUNCTION(BlueprintCallable)
	void StopAnimReload(int32 AmmoTake);

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;*/
};

