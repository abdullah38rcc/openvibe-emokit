#if defined TARGET_HAS_ThirdPartyEmokit

#include "ovasCConfigurationEmokit.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBEAcquisitionServer;
using namespace std;

/*_________________________________________________

Insert callback to specific widget here
Example with a button that launch a calibration of the device:

//Callback connected to a dedicated gtk button:
static void button_calibrate_pressed_cb(::GtkButton* pButton, void* pUserData)
{
	CConfigurationEmokit* l_pConfig=static_cast<CConfigurationEmokit*>(pUserData);
	l_pConfig->buttonCalibratePressedCB();
}

//Callback actually called:
void CConfigurationGTecGUSBamp::buttonCalibratePressedCB(void)
{
	// Connect to the hardware, ask for calibration, verify the return code, etc.
}
_________________________________________________*/

CConfigurationEmokit::CConfigurationEmokit(IDriverContext& rDriverContext, const char* sGtkBuilderFileName, boolean& rUseGyroscope, OpenViBE::boolean& rUseContactQuality, OpenViBE::boolean& rUseBattery, OpenViBE::uint32& rUserID)
	:CConfigurationBuilder(sGtkBuilderFileName)
	,m_rDriverContext(rDriverContext)
    ,m_rUseGyroscope(rUseGyroscope)
    ,m_rUseContactQuality(rUseContactQuality)
    ,m_rUseBattery(rUseBattery)
	,m_rUserID(rUserID)
{
}

boolean CConfigurationEmokit::preConfigure(void)
{
    if(!CConfigurationBuilder::preConfigure())
		return false;

	::GtkToggleButton* l_pCheckbuttonGyro = GTK_TOGGLE_BUTTON(gtk_builder_get_object(m_pBuilderConfigureInterface, "checkbutton_gyro"));
	gtk_toggle_button_set_active(l_pCheckbuttonGyro,m_rUseGyroscope);

    ::GtkToggleButton* l_pCheckbuttonContact = GTK_TOGGLE_BUTTON(gtk_builder_get_object(m_pBuilderConfigureInterface, "checkbutton_contact"));
    gtk_toggle_button_set_active(l_pCheckbuttonContact, m_rUseContactQuality);

    ::GtkToggleButton* l_pCheckbuttonBattery = GTK_TOGGLE_BUTTON(gtk_builder_get_object(m_pBuilderConfigureInterface, "checkbutton_battery"));
    gtk_toggle_button_set_active(l_pCheckbuttonBattery, m_rUseBattery);

	::GtkSpinButton* l_pSpinButtonUserID = GTK_SPIN_BUTTON(gtk_builder_get_object(m_pBuilderConfigureInterface, "spinbutton_userid"));
	gtk_spin_button_set_value(l_pSpinButtonUserID, m_rUserID);

	return true;
}

boolean CConfigurationEmokit::postConfigure(void)
{
	if(m_bApplyConfiguration)
	{
		::GtkToggleButton* l_pCheckbuttonGyro = GTK_TOGGLE_BUTTON(gtk_builder_get_object(m_pBuilderConfigureInterface, "checkbutton_gyro"));
		m_rUseGyroscope = ::gtk_toggle_button_get_active(l_pCheckbuttonGyro);

        ::GtkToggleButton* l_pCheckbuttonContact = GTK_TOGGLE_BUTTON(gtk_builder_get_object(m_pBuilderConfigureInterface, "checkbutton_contact"));
        m_rUseContactQuality = ::gtk_toggle_button_get_active(l_pCheckbuttonContact);

        ::GtkToggleButton* l_pCheckbuttonBattery = GTK_TOGGLE_BUTTON(gtk_builder_get_object(m_pBuilderConfigureInterface, "checkbutton_battery"));
        m_rUseBattery = ::gtk_toggle_button_get_active(l_pCheckbuttonBattery);

		::GtkSpinButton* l_pSpinButtonUserID = GTK_SPIN_BUTTON(gtk_builder_get_object(m_pBuilderConfigureInterface, "spinbutton_userid"));
		m_rUserID = gtk_spin_button_get_value(l_pSpinButtonUserID);

	}

    if(!CConfigurationBuilder::postConfigure()) // normal header is filled, ressources are realesed
		return false;

	return true;
}

#endif // TARGET_HAS_ThirdPartyEmokit
