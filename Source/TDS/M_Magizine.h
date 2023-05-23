// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "BasePlayerState.h"
#include "M_Magizine.generated.h"

UCLASS()
class TDS_API AM_Magizine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AM_Magizine();

	//FDropMagazine WeaponInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
	class UStaticMeshComponent* Magazine = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TamerLife")
	float TimeLife = 2.0f;
	UPROPERTY(BlueprintReadOnly)
	float TimerLife = TimeLife;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable)
	void TimerOfLife(float DeltaTime);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
