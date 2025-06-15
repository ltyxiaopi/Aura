// Xiaopi


#include "UI/WidgetController/AuraWidgetController.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams) {
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponnet = WCParams.AbilitySystemComponnet;
	AttributeSet = WCParams.AttributeSet;
}
