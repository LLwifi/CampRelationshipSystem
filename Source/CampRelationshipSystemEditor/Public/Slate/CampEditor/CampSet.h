// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <Widgets/SCompoundWidget.h>
#include <CampRelationshipSystem/Public/CR_Config.h>

DECLARE_DELEGATE_OneParam(FCampSetChangeEvent, FCR_CampInfo);

class SOneCampSet;
class SDefaultCampParameterOptions;

/*阵营设置面板
*/
class SCampSet : public SCompoundWidget
{

public:
	SLATE_BEGIN_ARGS(SCampSet)
	{}

	SLATE_EVENT(FCampSetChangeEvent, CampSetChangeEvent)
	SLATE_ATTRIBUTE(FCR_CampInfo, CampInfo)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
public:
	void SetAllRelationship(int32 CheckBoxIndex);

	//选择了阵营
	void SelectCamp(int32 Index, FString Option);

	//选择了预设
	void SelectPreset(int32 Index, FString Option);

	//设置可操作slate的激活状态
	void SetOptionsEnabled(bool IsEnabled);

	void CallCampSetChangeEvent();

	//改变某一行的阵营关系
	void ChangeOneCampRelationship(int32 CampID,int32 RelationshipID);

public:
	TSharedPtr<SVerticalBox> VerticalBox;
	TSharedPtr<SHorizontalBox> HorizontalBox_RelationshipName;
	TSharedPtr<SOneCampSet> SetAllSelectOneCampSet;
	TArray<TSharedPtr<SOneCampSet>> AllOneCampSet;
	TSharedPtr<SDefaultCampParameterOptions> SelectCampSlate;

	FCR_CampInfo CampInfo;

	//左侧阵营名称和右侧打勾区域的占比
	FVector2D FillSize = FVector2D(0.5f,1.0f);

	FCampSetChangeEvent CampSetChangeEvent;
};

