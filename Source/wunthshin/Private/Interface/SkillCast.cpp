// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/SkillCast.h"

#include "Data/Skill/O_WSBaseSkill.h"
#include "Data/Skill/SkillTableRow.h"
#include "Subsystem/SkillSubsystem.h"


// Add default functionality here for any ISkillCast functions that are not pure virtual.
bool ISkillCast::CastSkill(const FSkillRowHandle& InSkill, const FVector& InTargetLocation, AActor* InTargetActor)
{
	ICommonPawn* CommonPawn = Cast<ICommonPawn>(this);

	if (!CommonPawn)
	{
		return false;
	}

	const AActor* Self = Cast<AActor>(this);
	// SkillCast가 AActor 상속이 아닌 객체에 적용된 경우
	check(Self);
	return Self->GetWorld()->GetGameInstance()->GetSubsystem<USkillSubsystem>()->CastSkill
	(
		InSkill,
		CommonPawn,
		InTargetLocation,
		InTargetActor
	);
}
