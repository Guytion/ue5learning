// Copyright Guytion's Studio


#include "UI/Widget/TDMUserWidget.h"

void UTDMUserWidget::SetWidgetController(UObject* InWidgetController)
{
    WidgetController = InWidgetController;
    WidgetControllerSet();
}