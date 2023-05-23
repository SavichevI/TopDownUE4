// Fill out your copyright notice in the Description page of Project Settings.

#include "M_InventaryComponent.h"
#include "M_GameInstance.h"

// Sets default values for this component's properties
UM_InventaryComponent::UM_InventaryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UM_InventaryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	for (int8 i = 0; i < WeaponSlot.Num(); i++)
	{
		UM_GameInstance* myGI = Cast<UM_GameInstance>(GetWorld()->GetGameInstance());
		if (myGI)
		{
			if (!WeaponSlot[i].NameItem.IsNone())
			{
				FWeaponInfo Info;
				if (myGI->GetWeaponInfoByName(WeaponSlot[i].NameItem, Info))
				{
					WeaponSlot[i].AdditionalInfo.Round = Info.MaxRound;
				} 
				else
				{
					//WeaponSlot.RemoveAt(i);
					//i--;
				}
			}
		}
	}

	MaxSlotWeapon = WeaponSlot.Num();
	if (WeaponSlot.IsValidIndex(0))
	{
		if (!WeaponSlot[0].NameItem.IsNone())
		{
			OnSwitchWeapon.Broadcast(WeaponSlot[0].NameItem, WeaponSlot[0].AdditionalInfo, 0);
		}
	}
}

// Called every frame
void UM_InventaryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UM_InventaryComponent::SwitchWeaponToIndex(int32 ChangeToIndex, int32 OldIndex, FAddicionalWeaponInfo OldInfo, bool bIsForward)
{
	bool bIsSuccess = false;
	int8 CorrectIndex = ChangeToIndex;
	if (ChangeToIndex > WeaponSlot.Num()-1)
	{
		CorrectIndex = 0;
	}
	else
	{
		if (ChangeToIndex < 0)
		{
			CorrectIndex = WeaponSlot.Num()-1;
		}
	}
	FName NewIdWeapon;
	FAddicionalWeaponInfo NewAdditionalWeaponInfo;
	int32 NewCurrentIndex = 0;

	if (WeaponSlot.IsValidIndex(CorrectIndex))
	{
		if (!WeaponSlot.IsValidIndex(CorrectIndex))
		{
			if (WeaponSlot[CorrectIndex].NameItem.IsNone())
			{
				bIsSuccess = true;
			} 
			else
			{
				UM_GameInstance* myGI = Cast<UM_GameInstance>(GetWorld()->GetGameInstance());
				if (myGI)
				{
					FWeaponInfo myInfo;
					myGI->GetWeaponInfoByName(WeaponSlot[CorrectIndex].NameItem, myInfo);

					bool bIsFine = false;
					int8 i = 0;
					while (i < AmmoSlot.Num() && !bIsFine)
					{
						if (AmmoSlot[i].WeaponType == myInfo.WeaponType && AmmoSlot[i].Cout > 0)
						{
							bIsFine = true;
							bIsSuccess = true;
						}

						i++;
					}
				}
			}
			if (bIsSuccess)
			{
				NewCurrentIndex = CorrectIndex;
				NewIdWeapon = WeaponSlot[CorrectIndex].NameItem;
				NewAdditionalWeaponInfo = WeaponSlot[CorrectIndex].AdditionalInfo;
			}
		}
	}

	if (!bIsSuccess)
	{
		if (bIsForward)
		{
			int8 Iteration = 0;
			int8 Seconditeration = 0;
			while (Iteration < WeaponSlot.Num() && !bIsSuccess)
			{
				Iteration++;
				int8 tmpIndex = ChangeToIndex + Iteration;
				if (WeaponSlot.IsValidIndex(tmpIndex))
				{
					if (!WeaponSlot[tmpIndex].NameItem.IsNone())
					{
						if (WeaponSlot[tmpIndex].AdditionalInfo.Round > 0)
						{
							bIsSuccess = true;
							NewIdWeapon = WeaponSlot[tmpIndex].NameItem;
							NewAdditionalWeaponInfo = WeaponSlot[tmpIndex].AdditionalInfo;
							NewCurrentIndex = tmpIndex;
						}
						else
						{
							FWeaponInfo myInfo;
							UM_GameInstance* myGI = Cast<UM_GameInstance>(GetWorld()->GetGameInstance());

							myGI->GetWeaponInfoByName(WeaponSlot[tmpIndex].NameItem, myInfo);

							bool bIsFine = false;
							int8 i = 0;
							while (i < WeaponSlot.Num() && !bIsFine)
							{
								if (AmmoSlot[i].WeaponType == myInfo.WeaponType && AmmoSlot[i].Cout > 0)
								{
									bIsSuccess = true;
									bIsFine = true;
									NewAdditionalWeaponInfo = WeaponSlot[tmpIndex].AdditionalInfo;
									NewIdWeapon = WeaponSlot[tmpIndex].NameItem;
									NewCurrentIndex = tmpIndex;

								}
								i++;
							}
						}
					}
				}
				else
				{
					if (OldIndex != Seconditeration)
					{
						if (WeaponSlot.IsValidIndex(Seconditeration))
						{
							if (!WeaponSlot[Seconditeration].NameItem.IsNone())
							{
								if (WeaponSlot[Seconditeration].AdditionalInfo.Round > 0)
								{
									bIsSuccess = true;
									NewAdditionalWeaponInfo = WeaponSlot[Seconditeration].AdditionalInfo;
									NewIdWeapon = WeaponSlot[Seconditeration].NameItem;
									NewCurrentIndex = Seconditeration;
								}
								else
								{
									FWeaponInfo myInfo;
									UM_GameInstance* myGI = Cast<UM_GameInstance>(GetWorld()->GetGameInstance());

									myGI->GetWeaponInfoByName(WeaponSlot[Seconditeration].NameItem, myInfo);

									bool bIsFind = false;
									int8 i = 0;
									while (i < AmmoSlot.Num() && !bIsFind)
									{
										if (AmmoSlot[i].WeaponType == myInfo.WeaponType && AmmoSlot[i].Cout > 0)
										{
											bIsSuccess = true;
											NewAdditionalWeaponInfo = WeaponSlot[Seconditeration].AdditionalInfo;
											NewIdWeapon = WeaponSlot[i].NameItem;
											NewCurrentIndex = Seconditeration;
											bIsFind = true;
										}
										i++;
									}
								}
							}
						}
					}
					else
					{
						if (WeaponSlot.IsValidIndex(Seconditeration))
						{
							if (WeaponSlot[Seconditeration].NameItem.IsNone())
							{
								if (WeaponSlot[Seconditeration].AdditionalInfo.Round > 0)
								{
									//WeaponGood, it same weapon do nothing
								}
								else
								{
									FWeaponInfo myInfo;
									UM_GameInstance* myGI = Cast<UM_GameInstance>(GetWorld()->GetGameInstance());

									myGI->GetWeaponInfoByName(WeaponSlot[Seconditeration].NameItem, myInfo);

									bool bIsFind = false;
									int8 i = 0;
									while (i < AmmoSlot.Num() && !bIsFind)
									{
										if (AmmoSlot[i].WeaponType == myInfo.WeaponType)
										{
											if (AmmoSlot[i].Cout > 0)
											{
												//WeaponGood, it same weapon do nothing.
											}
											else
											{
												UE_LOG(LogTemp, Warning, TEXT("UM_InventoryVmponent::SwitchWeaponToIndex - Itit PISTOL - Need"));
											}
										}
										i++;
									}
								}
							}
						}
					}
					Seconditeration++;
				}
			}
		}
		else
		{
			int8 Iteration = 0;
			int8 Seconditeration = WeaponSlot.Num() - 1;
			while (Iteration < WeaponSlot.Num() && !bIsSuccess)
			{
				Iteration++;
				int8 tmpIndex = ChangeToIndex - Iteration;
				if (WeaponSlot.IsValidIndex(tmpIndex))
				{
					if (!WeaponSlot[tmpIndex].NameItem.IsNone())
					{
						if (WeaponSlot[tmpIndex].AdditionalInfo.Round > 0)		
						{
							bIsSuccess = true;
							NewAdditionalWeaponInfo = WeaponSlot[tmpIndex].AdditionalInfo;
							NewIdWeapon = WeaponSlot[tmpIndex].NameItem;
							NewCurrentIndex = tmpIndex;
						} 
						else
						{
							FWeaponInfo myInfo;
							UM_GameInstance* myGI = Cast<UM_GameInstance>(GetWorld()->GetGameInstance());
							myGI->GetWeaponInfoByName(WeaponSlot[tmpIndex].NameItem, myInfo);

							bool bIsFind = false;
							int8 i = 0;
							while (i < WeaponSlot.Num() && !bIsFind)
							{
								if (AmmoSlot[i].WeaponType == myInfo.WeaponType && AmmoSlot[i].Cout > 0)
								{
									bIsSuccess = true;
									NewIdWeapon = WeaponSlot[tmpIndex].NameItem;
									NewAdditionalWeaponInfo = WeaponSlot[tmpIndex].AdditionalInfo;
									NewCurrentIndex = tmpIndex;
									bIsFind = true;
								}
								i++;
							}
						}
					}
				}
				else
				{
					if (OldIndex != Seconditeration)
					{
						if (WeaponSlot.IsValidIndex(Seconditeration))
						{
							if (!WeaponSlot[Seconditeration].NameItem.IsNone())
							{
								if (WeaponSlot[Seconditeration].AdditionalInfo.Round > 0)
								{
									bIsSuccess = true;
									NewIdWeapon = WeaponSlot[Seconditeration].NameItem;
									NewAdditionalWeaponInfo = WeaponSlot[Seconditeration].AdditionalInfo;
									NewCurrentIndex = Seconditeration;

								} 
								else
								{
									FWeaponInfo myInfo;
									UM_GameInstance* MyGI = Cast<UM_GameInstance>(GetWorld()->GetGameInstance());
									MyGI->GetWeaponInfoByName(WeaponSlot[Seconditeration].NameItem, myInfo);

									bool bIsFind = false;
									int8 i = 0;
									while (i < AmmoSlot.Num() && !bIsFind)
									{
										if (AmmoSlot[i].WeaponType == myInfo.WeaponType && AmmoSlot[i].Cout > 0)
										{
											bIsSuccess = true;
											NewAdditionalWeaponInfo = WeaponSlot[Seconditeration].AdditionalInfo;
											NewIdWeapon = WeaponSlot[Seconditeration].NameItem;
											NewCurrentIndex = Seconditeration;
											bIsFind = true;
										}
										i++;
									}
								}
							}
						}
					}
					else
					{
						if (WeaponSlot.IsValidIndex(Seconditeration))
						{
							if (!WeaponSlot[Seconditeration].NameItem.IsNone())
							{
								if (WeaponSlot[Seconditeration].AdditionalInfo.Round > 0)
								{
									///////
								} 
								else
								{
									FWeaponInfo MyInfo;
									UM_GameInstance* MyGI = Cast<UM_GameInstance>(GetWorld()->GetGameInstance());
									MyGI->GetWeaponInfoByName(WeaponSlot[Seconditeration].NameItem, MyInfo);

									bool bIsFind = false;
									int8 i = 0;
									while (i < AmmoSlot.Num() && !bIsFind)
									{
										if (AmmoSlot[i].WeaponType == MyInfo.WeaponType)
										{
											if (AmmoSlot[i].Cout > 0)
											{
												/////
											}
											else
											{
												UE_LOG(LogTemp, Warning, TEXT("UTPSInventoryComponent::SwitchWeaponToIndex - Init PISTOL - NEED"));
											}
										}
										i++;
									}
								}
							}
						}
					}
					Seconditeration--;
				}
			}
		}
	}

	if (bIsSuccess)
	{
		SetAdditionalInfoWeapon(OldIndex, OldInfo);
		OnSwitchWeapon.Broadcast(NewIdWeapon, NewAdditionalWeaponInfo, NewCurrentIndex);
		//OnWeaponAmmoAviable.Broadcast();
	}
	return bIsSuccess;
}

FAddicionalWeaponInfo UM_InventaryComponent::GetAdditionalInfoWeapon(int32 IndexWeapon)
{
	FAddicionalWeaponInfo Result;
	if (WeaponSlot.IsValidIndex(IndexWeapon))
	{
		bool bIsFind = false;
		int8 i = 0;
		while (i < WeaponSlot.Num() && !bIsFind)
		{
			if (i == IndexWeapon)
			{
				Result = WeaponSlot[i].AdditionalInfo;
				bIsFind = true;
			}

			i++;
		}

		if (!bIsFind)
		{
			UE_LOG(LogTemp, Warning, TEXT("M_InvebtloryComponrnt::GetAdditionalInfoWeapon - No found weapon with index - %d"), IndexWeapon);
		}
	} 
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("M_InvebtloryComponrnt::GetAdditionalInfoWeapon - No found weapon with index - %d"), IndexWeapon);
	}
	return Result;
}

int32 UM_InventaryComponent::GetWeaponIndexSlotByName(FName IdWeaponName)
{
	int32 Result = -1;
	int8 i = 0;
	bool bIsFind = false;

	while (i < WeaponSlot.Num() && !bIsFind)
	{
		if (WeaponSlot[i].NameItem == IdWeaponName)
		{
			bIsFind = true;
			Result = i;
		}

		i++;
	}

	return Result;
}

FName UM_InventaryComponent::GetWeaponNameBySlotIndex(int32 IndexSlot)
{
	FName Result;
	if (WeaponSlot.IsValidIndex(IndexSlot))
	{
		Result = WeaponSlot[IndexSlot].NameItem;
	}

	return Result;
}

void UM_InventaryComponent::SetAdditionalInfoWeapon(int32 IndexWeapon, FAddicionalWeaponInfo NewInfo)
{
	if (WeaponSlot.IsValidIndex(IndexWeapon))
	{
		bool bIsFind = false;
		int8 i = 0;
		while (i < WeaponSlot.Num() && !bIsFind)
		{
			if (i == IndexWeapon)
			{
				WeaponSlot[i].AdditionalInfo = NewInfo;
				bIsFind = true;

				OnWeaponAdditionalInfoChange.Broadcast(IndexWeapon, NewInfo);
			}

			i++;
		}
		if (!bIsFind)
		{
			UE_LOG(LogTemp, Warning, TEXT("M_InventaryComponent::SetAdditionalInfoWeapon - No found weapon with index - %d"), IndexWeapon);
		}
	} 
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("M_InventaryComponent::SetAdditionalInfoWeapon - No found weapon with index - %d"), IndexWeapon);
	}
}

void UM_InventaryComponent::AmmoSlotChangeValue(EWeaponType TypeWeapon, int32 CoutChangeAmmo)
{
	bool bIsFind = false;
	int8 i = 0;
	while (i < AmmoSlot.Num() && !bIsFind)
	{
		if (AmmoSlot[i].WeaponType == TypeWeapon)
		{
			AmmoSlot[i].Cout += CoutChangeAmmo;
			if (AmmoSlot[i].Cout > AmmoSlot[i].MaxCout)
			{
				AmmoSlot[i].Cout = AmmoSlot[i].MaxCout;
			}

			OnAmmoChange.Broadcast(AmmoSlot[i].WeaponType, AmmoSlot[i].Cout);
			bIsFind = true;
		}

		i++;
	}
}

bool UM_InventaryComponent::CheckAmmoForWeapon(EWeaponType TypeWeapon, int8 &AvailableAmmoForWeapon)
{
	AvailableAmmoForWeapon = 0;
	bool bIsFind = false;
	int8 i = 0;
	while (i < AmmoSlot.Num() && !bIsFind)
	{
		if (AmmoSlot[i].WeaponType == TypeWeapon)
		{
			bIsFind = true;
			AvailableAmmoForWeapon = AmmoSlot[i].Cout;
			if (AmmoSlot[i].Cout > 0)
			{
				return true;
			}
		}

		i++;
	}

	OnWeaponAmmoEmpty.Broadcast(TypeWeapon); /////

	return false;
}

bool UM_InventaryComponent::CheckCanTakeAmmo(EWeaponType AmmoType)
{
	bool Result = false;
	int8 i = 0;
	while (i < AmmoSlot.Num() && !Result)
	{
		if (AmmoSlot[i].WeaponType == AmmoType && AmmoSlot[i].Cout < AmmoSlot[i].MaxCout)
		{
			Result = true;
		}

		i++;
	}
	return Result;
}

bool UM_InventaryComponent::CheckCantakeWeapon(int32& FreeSlot)
{
	bool bIsFreeSlot = false;
	int8 i = 0;
	while (i < WeaponSlot.Num() && !bIsFreeSlot)
	{
		if (WeaponSlot[i].NameItem.IsNone())
		{
			bIsFreeSlot = true;
			FreeSlot = i;
		}

		i++;
	}
	return bIsFreeSlot;
}

bool UM_InventaryComponent::SwitchWeaponToInventory(FWeaponSlot NewWeapon, int32 IndexSlot, int32 CurrentIndexWeaponChar, FDropItem &DropItemInfo)
{
	bool Result = false;
	if (WeaponSlot.IsValidIndex(IndexSlot) && GetDropItemInfoFromInventory(IndexSlot, DropItemInfo))
	{
		WeaponSlot[IndexSlot] = NewWeapon;

		SwitchWeaponToIndex(CurrentIndexWeaponChar, -1, NewWeapon.AdditionalInfo, true);

		OnUpdateWeaponSlots.Broadcast(IndexSlot, NewWeapon);
		Result = true;
	}
	return Result;
}

bool UM_InventaryComponent::GetDropItemInfoFromInventory(int32 IndexSlot, FDropItem& DropItemInfo)
{
	bool Result = false;

	FName DropItemName = GetWeaponNameBySlotIndex(IndexSlot);
	UM_GameInstance* myGI = Cast<UM_GameInstance>(GetWorld()->GetGameInstance());

	if (myGI)
	{
		Result = myGI->GetDropItemInfoByName(DropItemName, DropItemInfo);
		if (WeaponSlot.IsValidIndex(IndexSlot))
		{
			DropItemInfo.WeaponInfo.AdditionalInfo = WeaponSlot[IndexSlot].AdditionalInfo;
		}
	}
	return Result;
}

bool UM_InventaryComponent::TeyGetWeaponToInventory(FWeaponSlot NewWeapon)
{
	int32  IndexSlot = -1;
	if (CheckCantakeWeapon(IndexSlot))
	{
		if (WeaponSlot.IsValidIndex(IndexSlot))
		{
			WeaponSlot[IndexSlot] = NewWeapon;
			OnUpdateWeaponSlots.Broadcast(IndexSlot, NewWeapon);

			return true;
		}
	}
	return false;
}

void UM_InventaryComponent::WeaponReloaded(EWeaponType TypeWeapon, int32 AmmoTaken)
{
	bool bIsFind = false;
	int8 i = 0;
	while (i < AmmoSlot.Num() && !bIsFind)
	{
		if (AmmoSlot[i].WeaponType == TypeWeapon)
		{
			AmmoSlot[i].Cout += AmmoTaken;
		}

		i++;
	}
}

#pragma optimize ("", on)
