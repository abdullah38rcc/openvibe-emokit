#ifndef __OpenViBE_AcquisitionServer_CConfigurationEmokit_H__
#define __OpenViBE_AcquisitionServer_CConfigurationEmokit_H__

#if defined TARGET_HAS_ThirdPartyEmokit

#include "../ovasCConfigurationBuilder.h"
#include "ovasIDriver.h"
#include "../ovasCHeader.h"

#include <gtk/gtk.h>

namespace OpenViBEAcquisitionServer
{
	/**
	 * \class CConfigurationEmokit
	 * \author Tom Stewart (None)
	 * \date Mon Aug 26 13:05:55 2013
	 * \erief The CConfigurationEmokit handles the configuration dialog specific to the Emokit device.
	 *
	 * \sa CDriverEmokit
	 */
	class CConfigurationEmokit : public OpenViBEAcquisitionServer::CConfigurationBuilder
	{
	public:
        CConfigurationEmokit(OpenViBEAcquisitionServer::IDriverContext& rDriverContext, const char* sGtkBuilderFileName, OpenViBE::boolean& rUseGyroscope, OpenViBE::boolean& rUseContactQuality, OpenViBE::boolean& rUseBattery, OpenViBE::uint32& rUserID);

		virtual OpenViBE::boolean preConfigure(void);
		virtual OpenViBE::boolean postConfigure(void);

	protected:

        OpenViBEAcquisitionServer::IDriverContext &m_rDriverContext;
        OpenViBE::boolean &m_rUseGyroscope, &m_rUseContactQuality, &m_rUseBattery;
        OpenViBE::uint32 &m_rUserID;
	};
};

#endif // TARGET_HAS_ThirdPartyEmokit

#endif // __OpenViBE_AcquisitionServer_CConfigurationEmokit_H__
