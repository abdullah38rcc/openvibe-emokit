#if defined TARGET_HAS_ThirdPartyEmokit

#include "ovasCDriverEmokit.h"
#include "ovasCConfigurationEmokit.h"

#include <toolkit/ovtk_all.h>
#include <iostream>

using namespace OpenViBEAcquisitionServer;
using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace std;

//___________________________________________________________________//
//                                                                   //

CDriverEmokit::CDriverEmokit(IDriverContext& rDriverContext) :IDriver(rDriverContext), m_pCallback(NULL), m_ui32SampleCountPerSentBlock(1), m_pSample(NULL), m_device(NULL)
{
    m_bUseGyroscope = m_bUseContactQuality = m_bUseBattery = false;
    m_ui32UserID = 1;
}

CDriverEmokit::~CDriverEmokit(void)
{
}

const char* CDriverEmokit::getName(void)
{
	return "Emokit";
}

//___________________________________________________________________//
//                                                                   //

boolean CDriverEmokit::initialize(const uint32 ui32SampleCountPerSentBlock,	IDriverCallback& rCallback)
{	
    int channelCount = 14, nextChannel = 14; // to begin with

	if(m_bUseGyroscope)
        channelCount += 2; // another 2 for the gyro

    if(m_bUseContactQuality)
        channelCount += 14; // 14 for contact quality

    if(m_bUseBattery)
        channelCount += 1; // and 1 for battery life

    m_oHeader.setChannelCount(channelCount);
	
    m_oHeader.setChannelName(0, "AF3");
    m_oHeader.setChannelName(1, "F7");
    m_oHeader.setChannelName(2, "F3");
    m_oHeader.setChannelName(3, "FC5");
    m_oHeader.setChannelName(4, "T7");
    m_oHeader.setChannelName(5, "P7");
    m_oHeader.setChannelName(6, "O1");
    m_oHeader.setChannelName(7, "O2");
    m_oHeader.setChannelName(8, "P8");
    m_oHeader.setChannelName(9, "T8");
	m_oHeader.setChannelName(10, "FC6");
	m_oHeader.setChannelName(11, "F4");
	m_oHeader.setChannelName(12, "F8");
	m_oHeader.setChannelName(13, "AF4");

	if(m_bUseGyroscope)
	{
        m_oHeader.setChannelName(nextChannel, "Gyro-X");
        m_oHeader.setChannelName(nextChannel + 1, "Gyro-Y");
        nextChannel += 2;
	}

    if(m_bUseContactQuality)
    {
        m_oHeader.setChannelName(nextChannel, "cqAF3");
        m_oHeader.setChannelName(nextChannel + 1, "cqF7");
        m_oHeader.setChannelName(nextChannel + 2, "cqF3");
        m_oHeader.setChannelName(nextChannel + 3, "cqFC5");
        m_oHeader.setChannelName(nextChannel + 4, "cqT7");
        m_oHeader.setChannelName(nextChannel + 5, "cqP7");
        m_oHeader.setChannelName(nextChannel + 6, "cqO1");
        m_oHeader.setChannelName(nextChannel + 7, "cqO2");
        m_oHeader.setChannelName(nextChannel + 8, "cqP8");
        m_oHeader.setChannelName(nextChannel + 9, "cqT8");
        m_oHeader.setChannelName(nextChannel + 10, "cqFC6");
        m_oHeader.setChannelName(nextChannel + 11, "cqF4");
        m_oHeader.setChannelName(nextChannel + 12, "cqF8");
        m_oHeader.setChannelName(nextChannel + 13, "cqAF4");
        nextChannel += 14;
    }

    if(m_bUseBattery)
    {
        m_oHeader.setChannelName(nextChannel, "battery");
        nextChannel += 1;
    }

    m_oHeader.setSamplingFrequency(128);

	m_rDriverContext.getLogManager() << LogLevel_Trace << "INIT called.\n";

	if(m_rDriverContext.isConnected()) 
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "[INIT] Emokit Driver: Driver already initialized.\n";
		return false;
	}

	if(!m_oHeader.isChannelCountSet() || !m_oHeader.isSamplingFrequencySet())
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "[INIT] Emokit Driver: Channel count or frequency not set.\n";
		return false;
	}
	
	//__________________________________
	// Builds up a buffer to store acquired samples. This buffer will be sent to the acquisition
	// server later...
    m_pSample = new float32[m_oHeader.getChannelCount() * ui32SampleCountPerSentBlock];

	if(!m_pSample)
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "[INIT] Emokit Driver: Samples allocation failed.\n";
        delete[] m_pSample;
        m_pSample = NULL;
		return false;
	}
	
	//__________________________________
	// Hardware initialization

	m_device = emokit_create();
	
	if(!m_device)
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "[INIT] Emokit: Failed to create emokit object\n";
		return false;
	}
	
	uint32_t nDevices = emokit_get_count( m_device, EMOKIT_VID, EMOKIT_PID );
	m_rDriverContext.getLogManager() << LogLevel_Info << "[INIT] Emokit: " << nDevices << " devices connected.\n";
	
	if(m_ui32UserID >= nDevices)
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "[INIT] Emokit: There is no headset with ID " << m_ui32UserID << "\n";
		emokit_delete( m_device );
		m_device = NULL;
		return false;
	}
	
    if(emokit_open(m_device, EMOKIT_VID, EMOKIT_PID, m_ui32UserID) != 0 )
	{
		m_rDriverContext.getLogManager() << LogLevel_Error << "[INIT] Emokit: Could not connect to headset " << m_ui32UserID << "\n";
		emokit_delete(m_device);
		m_device = NULL;
		return false;
	}
	
	//__________________________________
	// Saves parameters
    m_pCallback = &rCallback;
    m_ui32SampleCountPerSentBlock = ui32SampleCountPerSentBlock;

	return true;
}

boolean CDriverEmokit::start(void)
{
	m_rDriverContext.getLogManager() << LogLevel_Trace << "START called.\n";
	
	if(!m_device) return false;	
	if(!m_rDriverContext.isConnected()) return false;
	if(m_rDriverContext.isStarted()) return false;

	return true;
}

boolean CDriverEmokit::loop(void)
{
	if(!m_rDriverContext.isConnected()) return false;
	if(!m_rDriverContext.isStarted()) return true;

	OpenViBE::CStimulationSet l_oStimulationSet;

	// receive samples from hardware
    for(int i = 0; i < m_ui32SampleCountPerSentBlock; i++)
    {
        int read_result = emokit_read_data(m_device);

        if(read_result > 0)
        {
            emokit_frame c = emokit_get_next_frame(m_device);

            m_pSample[i] = float32(c.AF3);
            m_pSample[i + m_ui32SampleCountPerSentBlock] = float32(c.F7);
            m_pSample[i + 2 * m_ui32SampleCountPerSentBlock] = float32(c.F3);
            m_pSample[i + 3 * m_ui32SampleCountPerSentBlock] = float32(c.FC5);
            m_pSample[i + 4 * m_ui32SampleCountPerSentBlock] = float32(c.T7);
            m_pSample[i + 5 * m_ui32SampleCountPerSentBlock] = float32(c.P7);
            m_pSample[i + 6 * m_ui32SampleCountPerSentBlock] = float32(c.O1);
            m_pSample[i + 7 * m_ui32SampleCountPerSentBlock] = float32(c.O2);
            m_pSample[i + 8 * m_ui32SampleCountPerSentBlock] = float32(c.P8);
            m_pSample[i + 9 * m_ui32SampleCountPerSentBlock] = float32(c.T8);
            m_pSample[i + 10 * m_ui32SampleCountPerSentBlock] = float32(c.FC6);
            m_pSample[i + 11 * m_ui32SampleCountPerSentBlock] = float32(c.F4);
            m_pSample[i + 12 * m_ui32SampleCountPerSentBlock] = float32(c.F8);
            m_pSample[i + 13 * m_ui32SampleCountPerSentBlock] = float32(c.AF4);

            int nextChannel = 14;

            if(m_bUseGyroscope)
            {
                m_pSample[i + nextChannel * m_ui32SampleCountPerSentBlock] = float32(c.gyroX);
                m_pSample[i + (nextChannel + 1) * m_ui32SampleCountPerSentBlock] = float32(c.gyroY);
                nextChannel += 2;
            }

            if(m_bUseContactQuality)
            {
                m_pSample[i + nextChannel * m_ui32SampleCountPerSentBlock] = float32(c.cq.AF3);
                m_pSample[i + (nextChannel + 1) * m_ui32SampleCountPerSentBlock] = float32(c.cq.F7);
                m_pSample[i + (nextChannel + 2) * m_ui32SampleCountPerSentBlock] = float32(c.cq.F3);
                m_pSample[i + (nextChannel + 3) * m_ui32SampleCountPerSentBlock] = float32(c.cq.FC5);
                m_pSample[i + (nextChannel + 4) * m_ui32SampleCountPerSentBlock] = float32(c.cq.T7);
                m_pSample[i + (nextChannel + 5) * m_ui32SampleCountPerSentBlock] = float32(c.cq.P7);
                m_pSample[i + (nextChannel + 6) * m_ui32SampleCountPerSentBlock] = float32(c.cq.O1);
                m_pSample[i + (nextChannel + 7) * m_ui32SampleCountPerSentBlock] = float32(c.cq.O2);
                m_pSample[i + (nextChannel + 8) * m_ui32SampleCountPerSentBlock] = float32(c.cq.P8);
                m_pSample[i + (nextChannel + 9) * m_ui32SampleCountPerSentBlock] = float32(c.cq.T8);
                m_pSample[i + (nextChannel + 10) * m_ui32SampleCountPerSentBlock] = float32(c.cq.FC6);
                m_pSample[i + (nextChannel + 11) * m_ui32SampleCountPerSentBlock] = float32(c.cq.F4);
                m_pSample[i + (nextChannel + 12) * m_ui32SampleCountPerSentBlock] = float32(c.cq.F8);
                m_pSample[i + (nextChannel + 13) * m_ui32SampleCountPerSentBlock] = float32(c.cq.AF4);
                nextChannel += 14;
            }

            if(m_bUseBattery)
            {
                m_pSample[i + nextChannel * m_ui32SampleCountPerSentBlock] = float32(c.battery);
                nextChannel += 1;
            }
        }
        else
            m_rDriverContext.getLogManager() << LogLevel_Error << "[LOOP] emokit_read_data returned " << read_result << "\n";
    }

    m_pCallback->setSamples(m_pSample);

    // When your sample buffer is fully loaded,
    // it is advised to ask the acquisition server
    // to correct any drift in the acquisition automatically.
    m_rDriverContext.correctDriftSampleCount(m_rDriverContext.getSuggestedDriftCorrectionSampleCount());

    // ...
    // receive events from hardware
    // and put them the correct way in a CStimulationSet object
    //...
    m_pCallback->setStimulationSet(l_oStimulationSet);

    return true;
}

boolean CDriverEmokit::stop(void)
{
	if(!m_rDriverContext.isConnected()) return false;
	if(!m_rDriverContext.isStarted()) return false;

	return true;
}

boolean CDriverEmokit::uninitialize(void)
{
	if(!m_rDriverContext.isConnected()) return false;
	if(m_rDriverContext.isStarted()) return false;

	if(m_device)
	{
		emokit_close(m_device);
        emokit_delete(m_device);
	}

	m_device = NULL;

	delete [] m_pSample;
	m_pSample=NULL;
	m_pCallback=NULL;

	return true;
}

//___________________________________________________________________//
//                                                                   //
boolean CDriverEmokit::isConfigurable(void)
{
	return true; // change to false if your device is not configurable
}

boolean CDriverEmokit::configure(void)
{
    // Change this line if you need to specify some references to your driver attribute that need configuration, e.g. the connection ID.
    CConfigurationEmokit m_oConfiguration(m_rDriverContext, OpenViBE::Directories::getDataDir() + "/applications/acquisition-server/interface-Emokit.ui", m_bUseGyroscope, m_bUseContactQuality, m_bUseBattery, m_ui32UserID);

	if(!m_oConfiguration.configure(m_oHeader))
		return false;

	return true;
}

#endif //TARGET_HAS_ThirdPartyEmokit
