﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Taker.generated.h"

class UC_WSPickUp;
// This class does not need to be modified.
UINTERFACE()
class U_WSTaker : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WUNTHSHIN_API I_WSTaker
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// 물체가 주어짐을 당했을 때 줍는 대상의 callback
	virtual bool Take(UC_WSPickUp* InTakenComponent) = 0;

	// 물체를 떨어트리려는 대상의 callback
	virtual bool Drop(UC_WSPickUp* InTakenComponent) = 0;
	
};
