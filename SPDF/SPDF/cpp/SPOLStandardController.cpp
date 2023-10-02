#include "../SPOLStandardController.h"

namespace SPDF {
	EntranceController::EntranceController() {
		setControllerJSON(PackageMeta::getInstance()->getPackageInternalPath() + "/resource/StandardController/Entrance.json");
	}
	ExitController::ExitController() {
		setControllerJSON(PackageMeta::getInstance()->getPackageInternalPath() + "/resource/StandardController/Exit.json");
	}
	SpeakingController::SpeakingController() {
		setControllerJSON(PackageMeta::getInstance()->getPackageInternalPath() + "/resource/StandardController/Speaking.json");
	}
	DialogueController::DialogueController() {
		setControllerJSON(PackageMeta::getInstance()->getPackageInternalPath() + "/resource/StandardController/Dialogue.json");
	}
	CycloramaController::CycloramaController() {
		setControllerJSON(PackageMeta::getInstance()->getPackageInternalPath() + "/resource/StandardController/Cyclorama.json");
	}
	MusicController::MusicController() {
		setControllerJSON(PackageMeta::getInstance()->getPackageInternalPath() + "/resource/StandardController/Music.json");
	}
	SoundController::SoundController() {
		setControllerJSON(PackageMeta::getInstance()->getPackageInternalPath() + "/resource/StandardController/Sound.json");
	}
	ProsceniumCurtainController::ProsceniumCurtainController() {
		setControllerJSON(PackageMeta::getInstance()->getPackageInternalPath() + "/resource/StandardController/ProsceniumCurtain.json");
	}
	HologramTextController::HologramTextController() {
		setControllerJSON(PackageMeta::getInstance()->getPackageInternalPath() + "/resource/StandardController/HologramText.json");
	}
	SpeechSpeedController::SpeechSpeedController() {
		setControllerJSON(PackageMeta::getInstance()->getPackageInternalPath() + "/resource/StandardController/SpeechSpeed.json");
	}
	ObjectEnableController::ObjectEnableController() {
		setControllerJSON(PackageMeta::getInstance()->getPackageInternalPath() + "/resource/StandardController/ObjectEnable.json");
	}
	ObjectDisableController::ObjectDisableController() {
		setControllerJSON(PackageMeta::getInstance()->getPackageInternalPath() + "/resource/StandardController/ObjectDisable.json");
	}
	ObjectFunctionController::ObjectFunctionController() {
		setControllerJSON(PackageMeta::getInstance()->getPackageInternalPath() + "/resource/StandardController/ObjectFunction.json");
	}
	SpectatorChoiceController::SpectatorChoiceController() {
		setControllerJSON(PackageMeta::getInstance()->getPackageInternalPath() + "/resource/StandardController/SpectatorChoice.json");
	}
}