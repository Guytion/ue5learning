// Copyright Guytion's Studio

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Input/InputConfig.h"
#include "TDMInputComponent.generated.h"

struct FTDMInputAction;

/**
 * 专门用于ThreeD Moba游戏技能系统的增强型输入组件
 * 继承自UEnhancedInputComponent，处理技能相关输入绑定
 */
UCLASS()
class THREEDMOBA_API UTDMInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:

	/**
	 * 绑定技能输入动作到对应的回调函数
	 * @tparam UserClass 拥有回调函数的对象类型
	 * @tparam PressedFuncType 按下回调函数类型（自动推导）
	 * @tparam ReleasedFuncType 释放回调函数类型（自动推导）
	 * @tparam HeldFuncType 长按回调函数类型（自动推导）
	 * @param InputConfig 包含技能输入动作配置的输入配置对象
	 * @param Object 拥有回调函数的用户对象
	 * @param PressedFunc 按下事件回调函数（ETriggerEvent::Started）
	 * @param ReleasedFunc 释放事件回调函数（ETriggerEvent::Completed）
	 * @param HeldFunc 持续触发事件回调函数（ETriggerEvent::Triggered）
	 */
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(
		const UInputConfig* InputConfig,
		UserClass* Object,
		PressedFuncType PressedFunc,
		ReleasedFuncType ReleasedFunc,
		HeldFuncType HeldFunc
	);
};

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UTDMInputComponent::BindAbilityActions(
	const UInputConfig* InputConfig,
	UserClass* Object,
	PressedFuncType PressedFunc,
	ReleasedFuncType ReleasedFunc,
	HeldFuncType HeldFunc
) {
	check(InputConfig);

	// 遍历配置中的所有技能输入动作
	for (const FTDMInputAction& Action : InputConfig->AbilityInputActions)
	{
		// 确保输入动作和输入标签有效
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			// 绑定按下事件回调
			if (PressedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}

			// 绑定释放事件回调
			if (ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}

			// 绑定持续触发事件回调
			if (HeldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}
		}
	}
}