#ifndef __OpenViBEPlugins_BoxAlgorithm_ChannelSelector_H__
#define __OpenViBEPlugins_BoxAlgorithm_ChannelSelector_H__

#include "../../ovp_defines.h"
#include <openvibe/ov_all.h>
#include <toolkit/ovtk_all.h>

#include <string>

#define OVP_ClassId_BoxAlgorithm_ChannelSelector     OpenViBE::CIdentifier(0x361722E8, 0x311574E8)
#define OVP_ClassId_BoxAlgorithm_ChannelSelectorDesc OpenViBE::CIdentifier(0x67633C1C, 0x0D610CD8)

namespace OpenViBEPlugins
{
	namespace SignalProcessing
	{
		class CBoxAlgorithmChannelSelector : public OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >
		{
		public:

			virtual void release(void) { delete this; }

			virtual OpenViBE::boolean initialize(void);
			virtual OpenViBE::boolean uninitialize(void);
			virtual OpenViBE::boolean processInput(OpenViBE::uint32 ui32InputIndex);
			virtual OpenViBE::boolean process(void);

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxAlgorithm < OpenViBE::Plugins::IBoxAlgorithm >, OVP_ClassId_BoxAlgorithm_ChannelSelector);

		protected:

			OpenViBEToolkit::TDecoder < CBoxAlgorithmChannelSelector >* m_pDecoder;
			OpenViBEToolkit::TEncoder < CBoxAlgorithmChannelSelector >* m_pEncoder;

			OpenViBE::IMatrix* m_pInputMatrix;
			OpenViBE::IMatrix* m_pOutputMatrix;

			std::vector < OpenViBE::uint32 > m_vLookup;
		};

		class CBoxAlgorithmChannelSelectorListener : public OpenViBEToolkit::TBoxListener < OpenViBE::Plugins::IBoxListener >
		{
		public:

			virtual OpenViBE::boolean onOutputTypeChanged(OpenViBE::Kernel::IBox& rBox, const OpenViBE::uint32 ui32Index)
			{
				OpenViBE::CIdentifier l_oTypeIdentifier;
				rBox.getOutputType(0, l_oTypeIdentifier);
				if(l_oTypeIdentifier==OV_TypeId_Signal || l_oTypeIdentifier==OV_TypeId_Spectrum)
				{
					rBox.setInputType(0, l_oTypeIdentifier);
				}
				else
				{
					rBox.getInputType(0, l_oTypeIdentifier);
					rBox.setOutputType(0, l_oTypeIdentifier);
				}
				return true;
			}

			virtual OpenViBE::boolean onInputTypeChanged(OpenViBE::Kernel::IBox& rBox, const OpenViBE::uint32 ui32Index)
			{
				OpenViBE::CIdentifier l_oTypeIdentifier;
				rBox.getInputType(0, l_oTypeIdentifier);
				if(l_oTypeIdentifier==OV_TypeId_Signal || l_oTypeIdentifier==OV_TypeId_Spectrum)
				{
					rBox.setOutputType(0, l_oTypeIdentifier);
				}
				else
				{
					rBox.getOutputType(0, l_oTypeIdentifier);
					rBox.setInputType(0, l_oTypeIdentifier);
				}
				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBEToolkit::TBoxListener < OpenViBE::Plugins::IBoxListener >, OV_UndefinedIdentifier);
		};

		class CBoxAlgorithmChannelSelectorDesc : public OpenViBE::Plugins::IBoxAlgorithmDesc
		{
		public:

			virtual void release(void) { }

			virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Channel Selector"); }
			virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("Yann Renard"); }
			virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INRIA"); }
			virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString("Select a subset of signal channels"); }
			virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString("Selection can be based on channel name (case-sensitive) or index starting from 0"); }
			virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Signal processing/Basic"); }
			virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.0"); }
			virtual OpenViBE::CString getStockItemName(void) const       { return OpenViBE::CString("gtk-missing-image"); }

			virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_BoxAlgorithm_ChannelSelector; }
			virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::SignalProcessing::CBoxAlgorithmChannelSelector; }
			virtual OpenViBE::Plugins::IBoxListener* createBoxListener(void) const               { return new CBoxAlgorithmChannelSelectorListener; }
			virtual void releaseBoxListener(OpenViBE::Plugins::IBoxListener* pBoxListener) const { delete pBoxListener; }

			virtual OpenViBE::boolean getBoxPrototype(
				OpenViBE::Kernel::IBoxProto& rBoxAlgorithmPrototype) const
			{
				rBoxAlgorithmPrototype.addInput  ("Input signal",             OV_TypeId_Signal);
				rBoxAlgorithmPrototype.addOutput ("Output signal",            OV_TypeId_Signal);
				rBoxAlgorithmPrototype.addSetting("Channel List",             OV_TypeId_String, ":");
				rBoxAlgorithmPrototype.addSetting("Action",                   OVP_TypeId_SelectionMethod, OVP_TypeId_SelectionMethod_Select.toString());
				rBoxAlgorithmPrototype.addSetting("Channel Matching Method",  OVP_TypeId_MatchMethod,     OVP_TypeId_MatchMethod_Smart.toString());
				rBoxAlgorithmPrototype.addFlag   (OpenViBE::Kernel::BoxFlag_CanModifyInput);
				rBoxAlgorithmPrototype.addFlag   (OpenViBE::Kernel::BoxFlag_CanModifyOutput);
				return true;
			}

			_IsDerivedFromClass_Final_(OpenViBE::Plugins::IBoxAlgorithmDesc, OVP_ClassId_BoxAlgorithm_ChannelSelectorDesc);
		};
	};
};

#endif // __OpenViBEPlugins_BoxAlgorithm_ChannelSelector_H__
