#include "ovtkIBoxAlgorithmExperimentInformationInputReaderCallback.h"
#include "ovtkTBoxAlgorithmExperimentInformationInputReaderCallback.inl"

namespace OpenViBEToolkit
{

	typedef TBoxAlgorithmExperimentInformationInputReaderCallback<IBoxAlgorithmExperimentInformationInputReaderCallback> CBoxAlgorithmExperimentInformationInputReaderCallback;

	IBoxAlgorithmExperimentInformationInputReaderCallback* createBoxAlgorithmExperimentInformationInputReaderCallback(IBoxAlgorithmExperimentInformationInputReaderCallback::ICallback& rCallback)
	{
		CBoxAlgorithmExperimentInformationInputReaderCallback* l_pResult=new CBoxAlgorithmExperimentInformationInputReaderCallback();
		l_pResult->setExperimentInformationCallback(&rCallback);
		return l_pResult;
	}

	void releaseBoxAlgorithmExperimentInformationInputReaderCallback(IBoxAlgorithmExperimentInformationInputReaderCallback* pBoxAlgorithmExperimentInformationInputReaderCallback)
	{
		delete pBoxAlgorithmExperimentInformationInputReaderCallback;
	}

};
