// Fill out your copyright notice in the Description page of Project Settings.


#include "C_WSWeapon.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "../../Actors/AA_WSCharacter.h"
#include "wunthshin/Components/PickUp/C_WSPickUp.h"
#include "wunthshin/Actors/Item/Weapon/A_WSWeapon.h"


// Sets default values for this component's properties
UC_WSWeapon::UC_WSWeapon()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> Asset
	{ TEXT("/Script/EnhancedInput.InputMappingContext'/Game/ThirdPerson/Input/IMC_SwordAtteck.IMC_SwordAtteck'") };
	check(Asset.Object);
	IMC_Weapon = Asset.Object;
	

	// ...
}


// Called when the game starts
void UC_WSWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (UC_WSPickUp* PickUpComponent = GetOwner()->GetComponentByClass<UC_WSPickUp>()) 
	{
		PickUpComponent->OnPickUpSuccess.AddUniqueDynamic(this, &UC_WSWeapon::UpdateCache);
	}

	if (AA_WSWeapon* WeaponCasting = Cast<AA_WSWeapon>(GetOwner())) 
	{
		AttackMontages = WeaponCasting->GetAttackMontages();
	}

	// ...
	
}

void UC_WSWeapon::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	

}

void UC_WSWeapon::AttackDefault()
{	
	int32 Index = ContinuousAttackCount % AttackMontages.Num();

	if (AttackMontages[Index]) 
	{
		if (!BasicAnimInstance->Montage_IsPlaying(nullptr)) 
		{
			BasicAnimInstance->Montage_Play(AttackMontages[Index]);
			ContinuousAttackCount++;
		}
	}

	OwningPawn->GetWorldTimerManager().ClearTimer(ResetCounterTimerHandle);
	OwningPawn->GetWorldTimerManager().SetTimer(ResetCounterTimerHandle, this, &UC_WSWeapon::ResetCounter, ResetTime, false);
}

void UC_WSWeapon::UpdateCache(TScriptInterface<I_WSTaker> InTaker)
{
	OwningPawn = Cast<APawn>(GetOwner()->GetAttachParentActor());
	check(OwningPawn);
	check(InTaker == OwningPawn);

	USkeletalMeshComponent* MeshComponent = OwningPawn->FindComponentByClass<USkeletalMeshComponent>();
	if (!MeshComponent)
	{
		check(false);
		return;
	}

	BasicAnimInstance = Cast<UBaseAnimInstance>(MeshComponent->GetAnimInstance());
	check(BasicAnimInstance);

	SetupInputComponent();
}

void UC_WSWeapon::SetupInputComponent()
{
	if (APlayerController* PC = Cast<APlayerController>(OwningPawn->GetController()))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());

		if (!Subsystem->HasMappingContext(IMC_Weapon))
		{
			Subsystem->AddMappingContext(IMC_Weapon, 0);
		}

		AActor* hi = Cast<AActor>(GetOwner());

		if (!hi->InputComponent)
		{
			hi->EnableInput(PC);

			UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(hi->InputComponent);
			ensure(EnhancedInputComponent);

			const UInputAction* InputAction = nullptr;
			const TArray<FEnhancedActionKeyMapping>& Mappings = IMC_Weapon->GetMappings();
			for (const FEnhancedActionKeyMapping& It : Mappings)
			{
				if (It.Action->GetFName() == TEXT("IA_Attack"))
				{
					InputAction = It.Action.Get();
					AttackActionBinding = &EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &ThisClass::AttackDefault);
					break;
				}
			}
		}
	}
}

void UC_WSWeapon::ResetCounter()
{
	ContinuousAttackCount = 0;
	UE_LOG(LogTemp, Warning, TEXT("Attack Counter Reset"));

}

void UC_WSWeapon::BeginDestroy()
{
	Super::BeginDestroy();

	if (OwningPawn) 
	{
		if (APlayerController* PC = Cast<APlayerController>(OwningPawn->GetController()))
		{
			UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());

			// todo: 무기가 여러개인 경우?
			if (Subsystem->HasMappingContext(IMC_Weapon))
			{
				Subsystem->RemoveMappingContext(IMC_Weapon);
			}

			AActor* hi = Cast<AActor>(GetOwner());

			if (hi->InputComponent)
			{
				UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(hi->InputComponent);
				ensure(EnhancedInputComponent);

				EnhancedInputComponent->RemoveBinding(*AttackActionBinding);
				hi->DisableInput(PC);
			}
		}
	}
}

void UC_WSWeapon::AttackSecondary()
{
	// todo: 몽타주 재생
}




