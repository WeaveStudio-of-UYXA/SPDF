/*
* 此文件初始化包的元数据和包的实例中的静态单例字段。
*/

#include "../SPDFPackage.h"
#include "../SPOLStandardController.h"
VI_Singleton_StaticInit(SPDF::PackageMeta);
VI_Singleton_StaticInit(SPDF::Package);

namespace SPDF {
	def_init PackageMeta::PackageMeta() :VIPackageMeta() {
		VI_Singleton_Init;
		this->setPackageName("SPDF");
		this->setPackageVersionMajor(SPDF_VERSION_MAJOR);
		this->setPackageVersionMinor(SPDF_VERSION_MINOR);
		this->setPackageVersionPatch(SPDF_VERSION_PATCH);
		this->setAuthor({ "Tsing Yayin" });
		this->setDescription("SPDF is a universal story performance description framework that is suitable for performance expression in any scene.");
		this->setLicense("GPLv3.0");
		this->setURL("https://www.github.com/weavestudio-of-uyxa/SPDF");
		this->setOrganization("Weave Studio of UYXA");
		this->setOrganizationDomain("https://www.github.com/weavestudio-of-uyxa");
		this->addTranslationFileName(Visindigo::Language::zh_SC, "zh_SC.vil", true);
		this->addTranslationFileName(Visindigo::Language::en_US, "en_US.vil", true);
		this->setDefaultLanguage(Visindigo::Language::zh_SC);
	}
	def_init Package::Package() {
		VI_Singleton_Init;
		qRegisterMetaType<SPDF::SPOLExecutionMode>("SPDF::SPOLExecutionMode");
		setPackageMeta(new SPDF::PackageMeta);
		getPackageMeta()->addTranslatableObject(this);
		getPackageMeta()->initTranslation();
		Host = VI_NULL;
	}
	void Package::attachNewHostToTerminal(SPDFAbstractStage* terminal) {
		if (Host != VI_NULL) {
			Host->deleteLater();
		}
		Host = new SPDFHost(terminal, this);
		terminal->Host = Host;
		Host->installParser(new SPDF::EntranceController);
		Host->installParser(new SPDF::ExitController);
		Host->installParser(new SPDF::SpeakingController);
		Host->installParser(new SPDF::DialogueController);
		Host->installParser(new SPDF::CycloramaController);
		Host->installParser(new SPDF::MusicController);
		Host->installParser(new SPDF::SoundController);
		Host->installParser(new SPDF::ProsceniumCurtainController);
		Host->installParser(new SPDF::HologramTextController);
		Host->installParser(new SPDF::SpeechSpeedController);
		Host->installParser(new SPDF::ObjectEnableController);
		Host->installParser(new SPDF::ObjectDisableController);
		Host->installParser(new SPDF::ObjectFunctionController);
		Host->installParser(new SPDF::SpectatorChoiceController);
	}
}