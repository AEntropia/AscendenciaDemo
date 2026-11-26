// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/AscUserWidget.h"

void UAscUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
