#pragma once
#include "SPOLAbstractParser.h"


namespace SPDF
{
	class SPDFPublicAPI EntranceController :public SPOLStandardControllerParser { Q_OBJECT; VI_OBJECT; _Public EntranceController(); };
	class SPDFPublicAPI ExitController :public SPOLStandardControllerParser { Q_OBJECT; VI_OBJECT; _Public ExitController(); };
	class SPDFPublicAPI SpeakingController :public SPOLStandardControllerParser { Q_OBJECT; VI_OBJECT; _Public SpeakingController(); };
	class SPDFPublicAPI DialogueController :public SPOLStandardControllerParser { Q_OBJECT; VI_OBJECT; _Public DialogueController(); };
	class SPDFPublicAPI CycloramaController :public SPOLStandardControllerParser { Q_OBJECT; VI_OBJECT; _Public CycloramaController(); };
	class SPDFPublicAPI MusicController :public SPOLStandardControllerParser { Q_OBJECT; VI_OBJECT; _Public MusicController(); };
	class SPDFPublicAPI SoundController :public SPOLStandardControllerParser { Q_OBJECT; VI_OBJECT; _Public SoundController(); };
	class SPDFPublicAPI ProsceniumCurtainController :public SPOLStandardControllerParser { Q_OBJECT; VI_OBJECT; _Public ProsceniumCurtainController(); };
	class SPDFPublicAPI HologramTextController :public SPOLStandardControllerParser { Q_OBJECT; VI_OBJECT; _Public HologramTextController(); };
	class SPDFPublicAPI SpeechSpeedController :public SPOLStandardControllerParser { Q_OBJECT; VI_OBJECT; _Public SpeechSpeedController(); };
	class SPDFPublicAPI ObjectEnableController :public SPOLStandardControllerParser { Q_OBJECT; VI_OBJECT; _Public ObjectEnableController(); };
	class SPDFPublicAPI ObjectDisableController :public SPOLStandardControllerParser { Q_OBJECT; VI_OBJECT; _Public ObjectDisableController(); };
	class SPDFPublicAPI ObjectFunctionController :public SPOLStandardControllerParser { Q_OBJECT; VI_OBJECT; _Public ObjectFunctionController(); };
	class SPDFPublicAPI SpectatorChoiceController :public SPOLStandardControllerParser { Q_OBJECT; VI_OBJECT; _Public SpectatorChoiceController(); };
}