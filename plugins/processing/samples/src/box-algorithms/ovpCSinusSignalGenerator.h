#ifndef __SamplePlugin_CSinusSignalGenerator_H__
#define __SamplePlugin_CSinusSignalGenerator_H__

#include "../ovp_defines.h"

#include <toolkit/ovtk_all.h>

#include <ebml/TWriterCallbackProxy.h>

namespace OpenViBEPlugins
{
	namespace Samples
	{
		class CSinusSignalGenerator : public OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>
		{
		public:

			CSinusSignalGenerator(void);

			virtual void release(void);

			virtual OpenViBE::uint64 getClockFrequency(void);
			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);

			virtual OpenViBE::boolean processClock(OpenViBE::Kernel::IMessageClock& rMessageClock);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm<OpenViBE::Plugins::IBoxAlgorithm>, OVP_ClassId_SinusSignalGenerator)

		public:

			virtual void writeSignalOutput(const void* pBuffer, const EBML::uint64 ui64BufferSize);

		protected:

			EBML::TWriterCallbackProxy1<OpenViBEPlugins::Samples::CSinusSignalGenerator> m_oSignalOutputWriterCallbackProxy;
			OpenViBEToolkit::IBoxAlgorithmSignalOutputWriter* m_pSignalOutputWriterHelper;
			EBML::IWriter* m_pSignalOutputWriter;

			OpenViBE::boolean m_bHeaderSent;
			OpenViBE::uint32 m_ui32ChannelCount;
			OpenViBE::uint32 m_ui32SamplingFrequency;
			OpenViBE::uint32 m_ui32GeneratedEpochSampleCount;
			OpenViBE::float64* m_pSampleBuffer;
			OpenViBE::uint32 m_ui32SentSampleCount;
		};

		class CSinusSignalGeneratorDesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }
			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Sinus oscillator"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Yann Renard"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA/IRISA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("Simple sinus signal generator"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Samples"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_SinusSignalGenerator; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::Samples::CSinusSignalGenerator(); }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString("gtk-execute"); }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rPrototype) const
			{
				rPrototype.addOutput("Generated signal",              OV_TypeId_Signal);

				rPrototype.addSetting("Channel count",                OV_TypeId_Integer, "4");
				rPrototype.addSetting("Sampling frequency",           OV_TypeId_Integer, "512");
				rPrototype.addSetting("Generated epoch sample count", OV_TypeId_Integer, "32");

				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_SinusSignalGeneratorDesc)
		};
	};
};

#endif // __SamplePlugin_CSinusSignalGenerator_H__
