// Fill out your copyright notice in the Description page of Project Settings.


#include "AJSonChar.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkyLightComponent.h"

// Sets default values
AAJSonChar::AAJSonChar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    Sky = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("Sky"));
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Sky;
}

// Called when the game starts or when spawned
void AAJSonChar::BeginPlay()
{
	Super::BeginPlay();
    ReadTemp(); 
}

// Called every frame
void AAJSonChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAJSonChar::ReadTemp()
{
    // Getting info from the json file requires finding the file first
    const FString JsonFilePath = FPaths::ProjectContentDir() + "/JsonFiles/hourly.json";
    FString JsonString;
    FFileHelper::LoadFileToString(JsonString, *JsonFilePath);

    // Need code to actually read in and store the json info
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
    TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

    // Check if can read in and actually found the file, if so, read file info
    if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
    {

        // Find the properties object, in it lies the periods array w/ info
        TSharedPtr<FJsonObject> PropertiesObject = JsonObject->GetObjectField("properties");

        //get periods object
        TArray<TSharedPtr<FJsonValue>> objArray = PropertiesObject->GetArrayField("periods");

        // Get the forecast property from a period
        TSharedPtr<FJsonObject> currentObject = objArray[0]->AsObject();
        FString forecast = currentObject->GetStringField("shortForecast");
        FString debug = "this is the forecast: " + forecast;
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, debug);
        // Now find something to do with this information?
        FColor color = ReturnSkyColor(forecast);
        Sky->SetLightColor(color);
    }
}

FColor AAJSonChar::ReturnSkyColor(FString forecast)
{
    FColor retval = FColor::Magenta; //this default magenta is to tell if we missed a type of forecast
    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue,forecast);
    if (forecast.Equals("Clear"))
    {
        retval = FColor(103, 131, 142);
    }
    else if (forecast.Contains("Sunny")) //we did contains because there are a lot of partly/mostly cloudy or chance of in the strings
    {
        retval = FColor(253, 238, 115);
    }
    else if (forecast.Contains("Rain"))
    {
        retval = FColor(60, 50, 80);
    }
    else if (forecast.Contains("Cloud"))
    {
        retval = FColor(100, 100, 105);
    }
    return retval;
}

