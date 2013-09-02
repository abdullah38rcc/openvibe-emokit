#ifndef __OpenViBEPlugins_SignalProcessingGpl_Algorithms_Classif_CNaiveBayesApplyFunction_H__
#define __OpenViBEPlugins_SignalProcessingGpl_Algorithms_Classif_CNaiveBayesApplyFunction_H__

#if defined TARGET_HAS_ThirdPartyITPP

#include "../ovp_defines.h"

#include <openvibe/ov_all.h>

#include <toolkit/ovtk_all.h>

#include <itpp/itstat.h>
#include <itpp/itsignal.h>

namespace OpenViBEPlugins
{
	namespace SignalProcessingGpl
	{
		class CNaiveBayesApplyFunction : virtual public OpenViBEToolkit::TAlgorithm < OpenViBE::Plugins::IAlgorithm >
		{
			public:

				virtual void release(void) { delete this; }

				virtual OpenViBE::boolean initialize(void);
				virtual OpenViBE::boolean uninitialize(void);

				virtual OpenViBE::boolean process(void);

				_IsDerivedFromClass_Final_(OpenViBEToolkit::TAlgorithm < OpenViBE::Plugins::IAlgorithm >, OVP_ClassId_Algorithm_NaiveBayesApplyFunction);

			public:

				OpenViBE::Kernel::TParameterHandler < OpenViBE::CMatrix* > ip_pMatrixSignal;
				OpenViBE::Kernel::TParameterHandler < OpenViBE::CMatrix* > ip_pMatrixMean;
				OpenViBE::Kernel::TParameterHandler < OpenViBE::CMatrix* > ip_pMatrixVariance;
				OpenViBE::Kernel::TParameterHandler < OpenViBE::CMatrix* > ip_pMatrixLogTerm;
				OpenViBE::Kernel::TParameterHandler < OpenViBE::CMatrix* > op_pMatrixFunctionalNB;
		};

		class CNaiveBayesApplyFunctionDesc : virtual public OpenViBE::Plugins::IAlgorithmDesc
		{
			public:

				virtual void release(void) { }

				virtual OpenViBE::CString getName(void) const                { return OpenViBE::CString("Apply Linear Naive Bayes function"); }
				virtual OpenViBE::CString getAuthorName(void) const          { return OpenViBE::CString("E. Maby - P.E. Aguera"); }
				virtual OpenViBE::CString getAuthorCompanyName(void) const   { return OpenViBE::CString("INSERM/U821"); }
				virtual OpenViBE::CString getShortDescription(void) const    { return OpenViBE::CString(""); }
				virtual OpenViBE::CString getDetailedDescription(void) const { return OpenViBE::CString(""); }
				virtual OpenViBE::CString getCategory(void) const            { return OpenViBE::CString("Algorithm/Classification"); }
				virtual OpenViBE::CString getVersion(void) const             { return OpenViBE::CString("1.00"); }

				virtual OpenViBE::CIdentifier getCreatedClass(void) const    { return OVP_ClassId_Algorithm_NaiveBayesApplyFunction; }
				virtual OpenViBE::Plugins::IPluginObject* create(void)       { return new OpenViBEPlugins::SignalProcessingGpl::CNaiveBayesApplyFunction(); }

				virtual OpenViBE::boolean getAlgorithmPrototype(
						OpenViBE::Kernel::IAlgorithmProto& rAlgorithmProto) const
						{
							rAlgorithmProto.addInputParameter (OVP_Algorithm_NaiveBayesApplyFunction_InputParameterId_MatrixSignal,            "Matrix signal",             OpenViBE::Kernel::ParameterType_Matrix);
							
												
							rAlgorithmProto.addInputParameter (OVP_Algorithm_NaiveBayesApplyFunction_InputParameterId_MatrixMean,        "Matrix mean",        OpenViBE::Kernel::ParameterType_Matrix);
							
							rAlgorithmProto.addInputParameter (OVP_Algorithm_NaiveBayesApplyFunction_InputParameterId_MatrixVariance,  "Matrix variance",  OpenViBE::Kernel::ParameterType_Matrix);
							rAlgorithmProto.addInputParameter (OVP_Algorithm_NaiveBayesApplyFunction_InputParameterId_LogTerm, "Matrix log term", OpenViBE::Kernel::ParameterType_Matrix);
							rAlgorithmProto.addOutputParameter(OVP_Algorithm_NaiveBayesApplyFunction_OutputParameterId_MatrixClassFunctional,      "Matrix class functional",       OpenViBE::Kernel::ParameterType_Matrix);
							rAlgorithmProto.addInputTrigger   (OVP_Algorithm_NaiveBayesApplyFunction_InputTriggerId_Initialize,                "Initialize");
							rAlgorithmProto.addInputTrigger   (OVP_Algorithm_NaiveBayesApplyFunction_InputTriggerId_Apply,             "Apply function");
							
							return true;
						}

						_IsDerivedFromClass_Final_(OpenViBE::Plugins::IAlgorithmDesc, OVP_ClassId_Algorithm_NaiveBayesApplyDesc);
		};
	};
};

#endif // TARGET_HAS_ThirdPartyITPP

#endif // __OpenViBEPlugins_SignalProcessingGpl_Algorithms_Classif_CNaiveBayesApplyFunction_H__
