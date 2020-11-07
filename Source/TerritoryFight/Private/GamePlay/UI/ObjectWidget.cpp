// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectWidget.h"
#include "Components/ProgressBar.h"

void UObjectWidget::UpdateHealth(float InPercent)
{
    this->HealthBar->SetPercent(InPercent);
}