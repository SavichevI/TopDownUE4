// Fill out your copyright notice in the Description page of Project Settings.

#include "M_Magizine.h"

// Sets default values
AM_Magizine::AM_Magizine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Magazine = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Magazine"));
	Magazine->SetCollisionResponseToAllChannels(ECR_Block);
	Magazine->SetSimulatePhysics(true);
	RootComponent = Magazine;
}

// Called when the game starts or when spawned
void AM_Magizine::BeginPlay()
{
	Super::BeginPlay();	
}

void AM_Magizine::TimerOfLife(float DeltaTime)
{
	if (TimeLife < 0)
	{
		Destroy(true);
	}
	else
	{
		TimeLife -= DeltaTime;
	}
}

// Called every frame
void AM_Magizine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimerOfLife(DeltaTime);
}

