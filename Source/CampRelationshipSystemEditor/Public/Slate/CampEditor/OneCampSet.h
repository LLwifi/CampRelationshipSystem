// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <Widgets/SCompoundWidget.h>


//DECLARE_MULTICAST_DELEGATE_OneParam(FOnCheckBoxChangeEvent, int32);

DECLARE_DELEGATE_OneParam(FOnCheckBoxChangeEvent, int32);


/*与某一个阵营关系的设置面板
*/
class SOneCampSet : public SCompoundWidget
{

public:
	SLATE_BEGIN_ARGS(SOneCampSet)
	{}

	SLATE_ATTRIBUTE(FString, CampName)
	SLATE_ATTRIBUTE(FVector2D, FillSize)
	SLATE_ATTRIBUTE(int32, InitCheckIndex)
	SLATE_EVENT(FOnCheckBoxChangeEvent, CheckBoxChangeEvent)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
public:
	void CheckBoxChange(int32 CheckBoxIndex);

	void SetEnabled(bool IsEnabled);

public:
	//左侧阵营名称
	FString CampName;

	//左侧阵营名称和右侧打勾区域的占比
	FVector2D FillSize;

	TSharedPtr<SHorizontalBox> HorizontalBox;

	TArray<TSharedPtr<SCheckBox>> AllCheckBox;

	FOnCheckBoxChangeEvent CheckBoxChangeEvent;

	//初始选择下标
	int32 InitCheckIndex;
};

