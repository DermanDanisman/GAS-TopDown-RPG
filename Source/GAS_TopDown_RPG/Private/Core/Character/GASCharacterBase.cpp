// © 2025 Heathrow (Derman). All rights reserved.This project is the intellectual property of Heathrow (Derman) and is protected by copyright law. Unauthorized reproduction, distribution, or use of this material is strictly prohibited.Unreal Engine and its associated trademarks are used under license from Epic Games.


#include "Core/Character/GASCharacterBase.h"

AGASCharacterBase::AGASCharacterBase()
{
	// Disable tick by default for performance. Subclasses can enable as needed.
	PrimaryActorTick.bCanEverTick = false;

	// Create the skeletal mesh component for the weapon.
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	// Attach the weapon mesh to the main character mesh at the "WeaponHandSocket" socket.
	WeaponMesh->SetupAttachment(GetMesh(), "WeaponHandSocket");
	// Disable collision on the weapon mesh (can be enabled in subclasses if needed).
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGASCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGASCharacterBase::InitializeAbilityActorInfo()
{
	// Child classes will fill this function
}

UAbilitySystemComponent* AGASCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AGASCharacterBase::GetAttributeSet() const
{
	return AttributeSet;
}

