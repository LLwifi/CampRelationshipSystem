#include <Slate/CampEditor/CampSet.h>
#include "Slate/CampEditor/DefaultCampParameterOptions.h"
#include "Slate/CampEditor/OneCampSet.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include <CampRelationshipSystem/Public/CR_Config.h>


#define LOCTEXT_NAMESPACE "CampSet"

void SCampSet::Construct(const FArguments& InArgs)
{
	CampSetChangeEvent = InArgs._CampSetChangeEvent;
	CampInfo = InArgs._CampInfo.Get();

	UCR_Config* CDS = UCR_Config::GetInstance();
	int32 PresetCampInfoIndex = 0;//0是自定义
	TArray<FString> PresetCampInfoKeys;
	CDS->PresetCampInfo.GenerateKeyArray(PresetCampInfoKeys);
	//判断自己属不属于预设的信息
	for (int32 i = 0; i < PresetCampInfoKeys.Num(); i++)
	{
		if (CampInfo == CDS->PresetCampInfo[PresetCampInfoKeys[i]])
		{
			PresetCampInfoIndex = i + 1;//获取正确的预设下标
			break;
		}
	}

	ChildSlot
	[
		SNew(SConstraintCanvas)
		+ SConstraintCanvas::Slot()
		.Anchors(FAnchors(0.5f,0.5f,0.5f,0.5f))
		.Alignment(FVector2D(0.5f,0.5f))
		.AutoSize(true)
		[
		SNew(SBox)
		.WidthOverride(FOptionalSize(300.0f))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Center)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.FillWidth(FillSize.X)
				.AutoWidth()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("PresetName", "预设选择"))
				]
				+ SHorizontalBox::Slot()
				.FillWidth(FillSize.Y)
				.AutoWidth()
				[
					SNew(SDefaultCampParameterOptions)
					.ReferenceProperty("PresetCampInfoName")
					.ParameterChangeEvent(this, &SCampSet::SelectPreset)
					.DefaultDisplayIndex(PresetCampInfoIndex)
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Center)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot().FillWidth(FillSize.X).AutoWidth()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("Camp", "所属阵营"))
				]
				+ SHorizontalBox::Slot()
				.FillWidth(FillSize.Y)
				.AutoWidth()
				[
					SAssignNew(SelectCampSlate, SDefaultCampParameterOptions)
					.ReferenceProperty("AllCamp")
					.ParameterChangeEvent(this,&SCampSet::SelectCamp)
					.DefaultDisplayIndex(CampInfo.MyClam - 1)////ID是从1开始的，Slate下标是从0开始的，所以这里 - 1
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			//.HAlign(EHorizontalAlignment::HAlign_Fill)
			[
				SAssignNew(VerticalBox, SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.FillWidth(FillSize.X)
					[
						SNew(SSpacer)
					]
					+ SHorizontalBox::Slot()
					.FillWidth(FillSize.Y)
					[
						SAssignNew(HorizontalBox_RelationshipName, SHorizontalBox)
					]
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SAssignNew(SetAllSelectOneCampSet, SOneCampSet)
					.FillSize(FillSize)
					.CampName("")
					.CheckBoxChangeEvent(this, &SCampSet::SetAllRelationship)
				]
			
			]
		]
		]
	];

	//全部预设的阵营
	TArray<FString> AllCampName;
	TArray<int32> AllCampID;
	CDS->AllCamp.GenerateValueArray(AllCampName);
	CDS->AllCamp.GenerateKeyArray(AllCampID);
	TSharedPtr<SOneCampSet> OneCampSet;
	int32 InitCheckIndex;
	for (int32 i = 0; i < AllCampID.Num(); i++)
	{
		//拿到和这个阵营的关系下标
		InitCheckIndex = CampInfo.RelationshipKey.Find(AllCampID[i]);
		//关系中是否有这个下标（每一对阵营和关系的下标一一对应）？返回这个下标的关系：-1
		InitCheckIndex = CampInfo.RelationshipValue.IsValidIndex(InitCheckIndex) ? CampInfo.RelationshipValue[InitCheckIndex] - 1 : -1;

		VerticalBox->AddSlot()
			[
				SAssignNew(OneCampSet, SOneCampSet)
				.FillSize(FillSize)
				.CampName(AllCampName[i])
				.InitCheckIndex(InitCheckIndex)
				.CheckBoxChangeEvent_Lambda([&](int32 CheckIndex)
					{
						ChangeOneCampRelationship(i + 1, CheckIndex + 1);
					}
				)
			];
			AllOneCampSet.Add(OneCampSet);
	}

	//阵营关系
	CDS->AllRelationship.GenerateValueArray(AllCampName);
	for (int32 i = 0; i < AllCampName.Num(); i++)
	{
		HorizontalBox_RelationshipName->AddSlot()
			.FillWidth(1.0f)
			[
				SNew(STextBlock)
				.Text(FText::FromString(*AllCampName[i]))
			];
	}

	//如果初始不是自定义则需要关闭全部可操作slate
	SetOptionsEnabled(PresetCampInfoIndex == 0);
}

void SCampSet::SetAllRelationship(int32 CheckBoxIndex)
{
	for (TSharedPtr<SOneCampSet> OneCampSet : AllOneCampSet)
	{
		OneCampSet->CheckBoxChange(CheckBoxIndex);
	}
}

void SCampSet::SelectCamp(int32 Index, FString Option)
{
	CampInfo.MyClam = Index;
	CallCampSetChangeEvent();
}

void SCampSet::SelectPreset(int32 Index, FString Option)
{
	if (Index == 1)//自定义
	{
		SetOptionsEnabled(true);

	}
	else
	{
		//ID本来就是从1开始，这里Index的0被自定义占用了，所以可以直接使用
		SetOptionsEnabled(false);

		UCR_Config* CDS = UCR_Config::GetInstance();
		if (CDS->PresetCampInfo.Contains(Option))
		{
			FCR_CampInfo PresetCampInfo = CDS->PresetCampInfo[Option];

			//改变所属阵营提示
			SelectCampSlate->TextBlock->SetText(FText::FromString(*CDS->AllCamp[PresetCampInfo.MyClam]));
			SelectCamp(PresetCampInfo.MyClam, CDS->AllCamp[PresetCampInfo.MyClam]);

			//改变勾选组
			int32 SelectIndex;
			TArray<int32> AllCampID;
			CDS->AllCamp.GenerateKeyArray(AllCampID);
			for (int32 i = 0; i < CDS->AllCamp.Num(); i++)//使用CDS->AllCamp的Num是为了避免删除了数据后遍历多了
			{
				//拿到和这个阵营的关系下标
				SelectIndex = PresetCampInfo.RelationshipKey.Find(AllCampID[i]);
				//关系中是否有这个下标（每一对阵营和关系的下标一一对应）？返回这个下标的关系：-1
				SelectIndex = PresetCampInfo.RelationshipValue.IsValidIndex(SelectIndex) ? PresetCampInfo.RelationshipValue[SelectIndex] - 1 : -1;

				AllOneCampSet[i]->CheckBoxChange(SelectIndex);
			}
		}
	}
}

void SCampSet::SetOptionsEnabled(bool IsEnabled)
{
	SelectCampSlate->ComboBoxSlate->SetEnabled(IsEnabled);
	SetAllSelectOneCampSet->SetEnabled(IsEnabled);
	for (TSharedPtr<SOneCampSet> OneCampSet : AllOneCampSet)
	{
		OneCampSet->SetEnabled(IsEnabled);
	}
}

void SCampSet::CallCampSetChangeEvent()
{
	CampSetChangeEvent.ExecuteIfBound(CampInfo);
}

void SCampSet::ChangeOneCampRelationship(int32 AllCampID, int32 RelationshipID)
{
	/*CampInfo.Relationship.Add(AllCampID, RelationshipID);*/
	if (CampInfo.RelationshipKey.Contains(AllCampID))
	{
		CampInfo.RelationshipValue[CampInfo.RelationshipKey.Find(AllCampID)] = RelationshipID;
	}
	else
	{
		CampInfo.RelationshipKey.Add(AllCampID);
		CampInfo.RelationshipValue.Add(RelationshipID);
	}
	CallCampSetChangeEvent();
}

#undef LOCTEXT_NAMESPACE
