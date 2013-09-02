#ifndef __OpenViBE_AcquisitionServer_IAcquisitionServerPlugin_H__
#define __OpenViBE_AcquisitionServer_IAcquisitionServerPlugin_H__

#include "ovas_base.h"

#include "boost/variant.hpp"
#include <map>

/**
  * \brief Interface for acquisition server plugins
  *
  * Contains an interface to the acquisition server plugins. Any plugin must inherit from this class in order to be able to register with the acquisition server.
  */

namespace OpenViBEAcquisitionServer
{
	class CAcquisitionServer;

	/// Structure holding a single user-available setting for the plugin
	struct PluginSetting
	{
			OpenViBE::CIdentifier type;

			// The setting value can be either a boolean, int64 or a CString
			// The boost::variant will hold one of these types
			boost::variant<OpenViBE::boolean, OpenViBE::int64, OpenViBE::CString> value;

			/// This getter hides the boost::variant from external applications
			template<typename T> const T getValue() const
			{
				return boost::get<T>(value);
			}
	};

	class IAcquisitionServerPlugin
	{
		public:
			// Interface of the plugin. To develop a new plugin override any of the Hook functions in your implementation.

			/// Hook called at the end of the AcquisitionServer constructor
			virtual void createHook() {}

			/// Hook called at the end of the start() function of AcquisitionServer
			virtual void startHook() {}

			/// Hook called at the end of the stop() function of AcquisitionServer
			virtual void stopHook() {}


			/** \brief Hook called in the loop() function of AcquisitionServer
			  *
			  * This hook is called after injecting the signal matrix and before sending the stimulations.
			  * It gets a reference to the current stimulation set with its start and end dates.
			  */
			virtual void loopHook(OpenViBE::CStimulationSet&, OpenViBE::uint64, OpenViBE::uint64) {}

			/// Hook called at the end of the acceptNewConnection() function of AcquisitionServer
			virtual void acceptNewConnectionHook() {}

		public:

			struct PluginProperties
			{
					OpenViBE::CString name;
					std::map<OpenViBE::CString, PluginSetting> settings;
			};

			IAcquisitionServerPlugin(const OpenViBE::Kernel::IKernelContext& rKernelContext) :
				m_rKernelContext(rKernelContext)
			{}

			virtual ~IAcquisitionServerPlugin() {}


			/// Adds a setting to the plugin. This method should be called from the constructor
			template<typename T> void addSetting(OpenViBE::CString name, T value)
			{
				const OpenViBE::CIdentifier l_TypeIDs[3] = {OVTK_TypeId_Boolean, OVTK_TypeId_Integer, OVTK_TypeId_String};
				PluginSetting ps;

				ps.value = value;
				// sets the type member to the identifier as described in the OpenViBE Toolkit
				ps.type =  l_TypeIDs[ps.value.which()]; // .which() method returns the index of the type in the table of types as defined in the boost::variant

				m_oProperties.settings.insert(std::pair<OpenViBE::CString, PluginSetting>(name, ps));
			}

			/// Gets the value of the settings, return type is defined by the template
			template<typename T> T getSetting(OpenViBE::CString name)
			{
				return boost::get<T>(m_oProperties.settings[name].value);
			}


			const PluginProperties& getProperties() const { return m_oProperties; }
			PluginProperties& getProperties() { return m_oProperties; }


		protected:
			struct PluginProperties m_oProperties;

			const OpenViBE::Kernel::IKernelContext& m_rKernelContext;
	};
}

#endif // __OpenViBE_AcquisitionServer_IAcquisitionServerPlugin_H__
