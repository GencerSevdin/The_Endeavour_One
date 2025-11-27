#pragma once
UENUM(BlueprintType)
enum class ELocomotionState : uint8
{  
  Climbing UMETA(DisplayName = "Climbing"),
  Flapping UMETA(DisplayName = "Flapping"),
  Gliding UMETA(DisplayName = "Gliding"),
  Hover UMETA(DisplayName = "Hover"),
  
};
UENUM(BlueprintType)
enum class ELocomotionDirection : uint8
{
	None UMETA(DisplayName = "None"),
   TurningLeft UMETA(DisplayName = "TurningLeft"),
   TurningRight UMETA(DisplayName = "TurningRight"),
};
UENUM(BlueprintType)
enum class EGroundActions : uint8
{
   Unoccupied UMETA(DisplayName = "EAS_Unoccupied"),
   Eating UMETA(DisplayName = "Eating"),
   Jumping UMETA(DisplayName = "Jumping"),
   Attacking UMETA(DisplayName = "Attacking"),
   Walking UMETA(DisplayName = "Walking")
};
