#include "ovasCAcquisitionServerGUI.h"

#include <openvibe/ov_all.h>
#include <toolkit/ovtk_all.h>

#include <gtk/gtk.h>

#include <iostream>

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace std;

int main(int argc, char ** argv)
{
//___________________________________________________________________//
//                                                                   //


	CKernelLoader l_oKernelLoader;

	cout<<"[  INF  ] Created kernel loader, trying to load kernel module"<<endl;
	CString l_sError;
#if defined TARGET_OS_Windows
	CString l_sKernelFile = OpenViBE::Directories::getLibDir() + "/openvibe-kernel.dll";
#else
	CString l_sKernelFile = OpenViBE::Directories::getLibDir() + "/libopenvibe-kernel.so";
#endif
	if(!l_oKernelLoader.load(l_sKernelFile, &l_sError))
	{
		cout<<"[ FAILED ] Error loading kernel ("<<l_sError<<")" << " from [" << l_sKernelFile << "]\n";
	}
	else
	{
		cout<<"[  INF  ] Kernel module loaded, trying to get kernel descriptor"<<endl;
		IKernelDesc* l_pKernelDesc=NULL;
		IKernelContext* l_pKernelContext=NULL;
		l_oKernelLoader.initialize();
		l_oKernelLoader.getKernelDesc(l_pKernelDesc);
		if(!l_pKernelDesc)
		{
			cout<<"[ FAILED ] No kernel descriptor"<<endl;
		}
		else
		{
			cout<<"[  INF  ] Got kernel descriptor, trying to create kernel"<<endl;
			l_pKernelContext=l_pKernelDesc->createKernel("acquisition-server", OpenViBE::Directories::getDataDir() + "/kernel/openvibe.conf");
			if(!l_pKernelContext)
			{
				cout<<"[ FAILED ] No kernel created by kernel descriptor"<<endl;
			}
			else
			{
				OpenViBEToolkit::initialize(*l_pKernelContext);

				IConfigurationManager& l_rConfigurationManager=l_pKernelContext->getConfigurationManager();

				l_pKernelContext->getPluginManager().addPluginsFromFiles(l_rConfigurationManager.expand("${Kernel_Plugins}"));

				//initialise Gtk before 3D context
#if !GLIB_CHECK_VERSION(2,32,0)
				// although deprecated in newer GTKs (no more needed after (at least) 2.24.13, deprecated in 2.32), we need to use this on Windows with the older GTK (2.22.1), or acquisition server will crash on startup
				g_thread_init(NULL);
#endif
				gdk_threads_init();
				gtk_init(&argc, &argv);

				// gtk_rc_parse(OpenViBE::Directories::getDataDir() + "/applications/designer/interface.gtkrc");

#if 0 // This is not needed in the acquisition server
				if(l_rConfigurationManager.expandAsBoolean("${Kernel_3DVisualisationEnabled}"))
				{
					l_pKernelContext->getVisualisationManager().initialize3DContext();
				}
#endif

				{
					// If this is encapsulated by gdk_threads_enter() and gdk_threads_exit(), m_pThread->join() can hang when gtk_main() returns before destructor of app has been called.
					OpenViBEAcquisitionServer::CAcquisitionServerGUI app(*l_pKernelContext);

					try
					{
						gdk_threads_enter();	
						gtk_main();
						gdk_threads_leave();			
					}
					catch(...)
					{
						l_pKernelContext->getLogManager() << LogLevel_Fatal << "Catched top level exception\n";
					}
				}

				cout<<"[  INF  ] Application terminated, releasing allocated objects"<<endl;

				OpenViBEToolkit::uninitialize(*l_pKernelContext);

				l_pKernelDesc->releaseKernel(l_pKernelContext);
			}
		}
		l_oKernelLoader.uninitialize();
		l_oKernelLoader.unload();
	}

	return 0;
}
