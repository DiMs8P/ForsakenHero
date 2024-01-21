// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/FHMLCharacter.h"

#include "NNE.h"
#include "NNEModelData.h"
#include "NNERuntimeCPU.h"

// Sets default values
AFHMLCharacter::AFHMLCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AFHMLCharacter::BeginPlay()
{
    if (PreLoadedModelData)
	{
		// Example for model creation
		TWeakInterfacePtr<INNERuntimeCPU> Runtime = UE::NNE::GetRuntime<INNERuntimeCPU>(FString("NNERuntimeORTCpu"));
		if (Runtime.IsValid())
		{
			ModelHelper = MakeShared<FMyModelHelper>();
			
			TUniquePtr<UE::NNE::IModelCPU> Model = Runtime->CreateModel(PreLoadedModelData);
			if (Model.IsValid())
			{
				ModelHelper->ModelInstance = Model->CreateModelInstance();
				if (ModelHelper->ModelInstance.IsValid())
				{
					ModelHelper->bIsRunning = false;
 
					// Example for querying and testing in- and outputs
					TConstArrayView<UE::NNE::FTensorDesc> InputTensorDescs = ModelHelper->ModelInstance->GetInputTensorDescs();
					checkf(InputTensorDescs.Num() == 1, TEXT("The current example supports only models with a single input tensor"));
					UE::NNE::FSymbolicTensorShape SymbolicInputTensorShape = InputTensorDescs[0].GetShape();
					checkf(SymbolicInputTensorShape.IsConcrete(), TEXT("The current example supports only models without variable input tensor dimensions"));
					TArray<UE::NNE::FTensorShape> InputTensorShapes = { UE::NNE::FTensorShape::MakeFromSymbolic(SymbolicInputTensorShape) };
 
					ModelHelper->ModelInstance->SetInputTensorShapes(InputTensorShapes);
 
					TConstArrayView<UE::NNE::FTensorDesc> OutputTensorDescs = ModelHelper->ModelInstance->GetOutputTensorDescs();
					checkf(OutputTensorDescs.Num() == 1, TEXT("The current example supports only models with a single output tensor"));
					UE::NNE::FSymbolicTensorShape SymbolicOutputTensorShape = OutputTensorDescs[0].GetShape();
					checkf(SymbolicOutputTensorShape.IsConcrete(), TEXT("The current example supports only models without variable output tensor dimensions"));
					TArray<UE::NNE::FTensorShape> OutputTensorShapes = { UE::NNE::FTensorShape::MakeFromSymbolic(SymbolicOutputTensorShape) };
 
					// Example for creating in- and outputs
					ModelHelper->InputData.SetNumZeroed(InputTensorShapes[0].Volume());
					ModelHelper->InputBindings.SetNumZeroed(1);
					ModelHelper->InputBindings[0].Data = ModelHelper->InputData.GetData();
					ModelHelper->InputBindings[0].SizeInBytes = ModelHelper->InputData.Num() * sizeof(float);
 
					ModelHelper->OutputData.SetNumZeroed(OutputTensorShapes[0].Volume());
					ModelHelper->OutputBindings.SetNumZeroed(1);
					ModelHelper->OutputBindings[0].Data = ModelHelper->OutputData.GetData();
					ModelHelper->OutputBindings[0].SizeInBytes = ModelHelper->OutputData.Num() * sizeof(float);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to create the model instance"));
					ModelHelper.Reset();
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to create the model"));
				ModelHelper.Reset();
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Cannot find runtime NNERuntimeORTCpu, please enable the corresponding plugin"));
		}
    	
	}
	
	Super::BeginPlay();
}

bool AFHMLCharacter::GetIsModelRunning() const
{
	return ModelHelper->bIsRunning;
}

bool AFHMLCharacter::GetIsModelActualData() const
{
	return ModelHelper->bIsActualData;
}

void AFHMLCharacter::SetIsModelActualData(bool bActualData)
{
	ModelHelper->bIsActualData = bActualData;
}

void AFHMLCharacter::UpdateInputData(float Distance, float PlayerHp, float EnemyHp)
{
	ModelHelper->InputData[0] = Distance;
	ModelHelper->InputData[1] = PlayerHp;
	ModelHelper->InputData[2] = EnemyHp;
}

int AFHMLCharacter::GetAction() const
{
	int returnValue = 0;
	for (int i = 1; i < ModelHelper->OutputData.Num(); ++i) {
		
		if (ModelHelper->OutputData[returnValue] < ModelHelper->OutputData[i]) {
			returnValue = i;
		}
	}
	
	return returnValue;
}

void AFHMLCharacter::RunModel()
{
	if (ModelHelper.IsValid())
	{
		// Example for async inference
		if (!ModelHelper->bIsRunning)
		{
			// Process ModelHelper->OutputData from the previous run here
			// Fill in new data into ModelHelper->InputData here
 
			ModelHelper->bIsRunning = true;
			TSharedPtr<FMyModelHelper> ModelHelperPtr = ModelHelper;
			AsyncTask(ENamedThreads::AnyNormalThreadNormalTask, [ModelHelperPtr]()
			{
				if (ModelHelperPtr->ModelInstance->RunSync(ModelHelperPtr->InputBindings, ModelHelperPtr->OutputBindings) != 0)
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to run the model"));
				}
				AsyncTask(ENamedThreads::GameThread, [ModelHelperPtr]()
				{
					ModelHelperPtr->bIsActualData = true;
					ModelHelperPtr->bIsRunning = false;
				});
			});
		}
	}
}

FORCEINLINE FText AFHMLCharacter::GetEnemyLabel() const
{
	return FText::FromString("ML");
}