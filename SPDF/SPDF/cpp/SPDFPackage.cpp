/*
* 此文件初始化包的元数据和包的实例中的静态单例字段。
*/

#include "../SPDFPackage.h"

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
		setPackageMeta(new SPDF::PackageMeta);
		getPackageMeta()->addTranslatableObject(this);
		getPackageMeta()->initTranslation();
		Host = VI_NULL;
	}
	void Package::attachNewHostToTerminal(SPDFAbstractTerminal* terminal) {
		if (Host != VI_NULL) {
			Host->deleteLater();
		}
		Host = new SPDFHost(terminal, this);
		terminal->Host = Host;
	}
}