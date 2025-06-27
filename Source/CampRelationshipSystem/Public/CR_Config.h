// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CR_Config.generated.h"

//阵营信息的编辑器操作UI
USTRUCT(BlueprintType)
struct FCR_CampInfoOperation
{
	GENERATED_BODY()
public:
	//该变量在构造中赋值 之后在细节面板中使用该变量调用函数
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		UObject* TargetObj;
};

/*
* 阵营信息
*/
USTRUCT(BlueprintType)
struct FCR_CampInfo
{
	GENERATED_BODY()
public:
	bool operator==(FCR_CampInfo RightValue) 
	{ 
		if (this->MyClam == RightValue.MyClam)
		{
			/*for (int32 i = 0; i < this->Relationship.Num(); i++)
			{
				if (!RightValue.Relationship.Contains(i) || this->Relationship[i] != RightValue.Relationship[i])
				{
					return false;
				}
			}*/
			for (int32 i = 0; i < this->RelationshipKey.Num(); i++)
			{
				if (!RightValue.RelationshipKey.IsValidIndex(i) || this->RelationshipKey[i] != RightValue.RelationshipKey[i])
				{
					return false;
				}
			}
			for (int32 i = 0; i < this->RelationshipValue.Num(); i++)
			{
				if (!RightValue.RelationshipValue.IsValidIndex(i) || this->RelationshipValue[i] != RightValue.RelationshipValue[i])
				{
					return false;
				}
			}
		}
		else
		{
			return false;
		}

		return true;
	};

public:
	//自己所属的阵营
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
		int32 MyClam = 1;

	//与哪个阵营
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<int32> RelationshipKey;

	//处于什么关系
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<int32> RelationshipValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FCR_CampInfoOperation Operation;
};

/**
 * 阵营相关的开发人员配置项
 */
UCLASS(config = Camp, defaultconfig)
class CAMPRELATIONSHIPSYSTEM_API UCR_Config : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	virtual FName GetCategoryName() const final override
	{
		return FName("GameEditorConfig");
	}
	static UCR_Config* GetInstance() { return GetMutableDefault<UCR_Config>(); }
	UFUNCTION(BlueprintPure, BlueprintCallable)
		static UCR_Config* GetCR_Config() { return GetInstance(); }

public:
	//阵营之间的全部关系<关系ID,显示名称>	ID从1开始，0用于Map的创建
	UPROPERTY(Config, BlueprintReadOnly,EditAnywhere)
		TMap<int32, FString> AllRelationship;

	//全部阵营<阵营ID,显示名称>	ID从1开始，0用于Map的创建
	UPROPERTY(Config, BlueprintReadWrite, EditAnywhere)
		TMap<int32, FString> AllCamp;

	//全部预设的阵营信息<预设名称，阵营信息>
	UPROPERTY(Config, BlueprintReadOnly,EditAnywhere)
		TMap<FString, FCR_CampInfo> PresetCampInfo;

	//测试用
	//UPROPERTY(Config, BlueprintReadOnly, EditAnywhere)
		//FCampInfo CampInfo;
};
