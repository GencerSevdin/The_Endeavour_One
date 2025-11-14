#pragma once
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
   Flapping    UMETA(DisplayName = "Flapping"),
   Gliding     UMETA(DisplayName = "Gliding"),
   Hover       UMETA(DisplayName = "Hover"),

};