#include "ovpiCPluginObjectDescEnumAlgorithmGlobalDefinesGenerator.h"

using namespace OpenViBE;
using namespace OpenViBE::Kernel;
using namespace OpenViBE::Plugins;

#include <iostream>

CPluginObjectDescEnumAlgorithmGlobalDefinesGenerator::CPluginObjectDescEnumAlgorithmGlobalDefinesGenerator(const IKernelContext& rKernelContext, const CString& sFilename)
	:CPluginObjectDescEnum(rKernelContext)
{
	m_oUsedIdentifiers.clear();

	OpenViBE::CString l_sFullFilename = sFilename + CString("/ovp_global_defines.h");

	m_rKernelContext.getLogManager() << LogLevel_Info << "Writing global defines to '" << l_sFullFilename << "\n";

	m_oFile.open(l_sFullFilename.toASCIIString());

	if(!m_oFile.good()) {
		m_rKernelContext.getLogManager() << LogLevel_Error << "Error opening '" << l_sFullFilename << "for writing\n";
		return;
	}

	m_oFile << "#ifndef __OpenViBEPlugins_Global_Defines_H__\n";
	m_oFile << "#define __OpenViBEPlugins_Global_Defines_H__\n";
	m_oFile << "\n";
	m_oFile << "// This file was automatically generated by Plugin Inspector\n";
	m_oFile << "\n";
}

CPluginObjectDescEnumAlgorithmGlobalDefinesGenerator::~CPluginObjectDescEnumAlgorithmGlobalDefinesGenerator(void)
{
	if(!m_oFile.good()) return;

	m_oFile << "#endif // __OpenViBEPlugins_Global_Defines_H__\n";
	m_oFile.close();
}

void CPluginObjectDescEnumAlgorithmGlobalDefinesGenerator::addIdentifier(const OpenViBE::CString &objectName, const OpenViBE::CIdentifier candidate, const OpenViBE::CString &sSpaces)
{
	if(m_oUsedIdentifiers.find(candidate) == m_oUsedIdentifiers.end() ) {
		m_oUsedIdentifiers[candidate]=objectName;
		m_oFile << "#define " << objectName << sSpaces << "OpenViBE::CIdentifier" << candidate.toString().toASCIIString() << "\n";
	} 
	else
	{
		m_rKernelContext.getLogManager() << LogLevel_Warning << "    Object '" << objectName << "' has duplicate id " << candidate.toString().toASCIIString() << " with " << m_oUsedIdentifiers[candidate] << "\n";
		m_oFile << "#define " << objectName << sSpaces << "OpenViBE::CIdentifier" << candidate.toString().toASCIIString() << " // Duplicate of " << m_oUsedIdentifiers[candidate] << "\n";
	}
}

boolean CPluginObjectDescEnumAlgorithmGlobalDefinesGenerator::callback(const IPluginObjectDesc& rPluginObjectDesc)
{
	if(!m_oFile.good()) return false;

	int l_iWidth=120;

	m_rKernelContext.getLogManager() << LogLevel_Info << "  Dumping [" << rPluginObjectDesc.getName() << "]\n";

	IAlgorithmManager& l_rAlgorithmManager=m_rKernelContext.getAlgorithmManager();
	IAlgorithmProxy& l_rAlgorithmProxy=l_rAlgorithmManager.getAlgorithm(l_rAlgorithmManager.createAlgorithm(rPluginObjectDesc.getCreatedClass()));
	CIdentifier l_oIdentifier;

	m_oFile << "// -----------------------------------------------------\n";
	m_oFile << "// " << rPluginObjectDesc.getName().toASCIIString() << "\n";
	m_oFile << "// -----------------------------------------------------\n";
	m_oFile << "\n";

	 l_rAlgorithmProxy.initialize();

	{
		std::string l_sDefName;
		l_sDefName+="OVP_GD_ClassId_Algorithm_";
		l_sDefName+=transform(rPluginObjectDesc.getName().toASCIIString(), true);
		l_sDefName+="";

		std::string l_sSpaces;
		l_sSpaces.resize(l_iWidth-l_sDefName.length(), ' ');
		l_oIdentifier = rPluginObjectDesc.getCreatedClassIdentifier();

		addIdentifier(l_sDefName.c_str(), l_oIdentifier, l_sSpaces.c_str());
	}

	{
		std::string l_sDefName;
		l_sDefName+="OVP_GD_ClassId_Algorithm_";
		l_sDefName+=transform(rPluginObjectDesc.getName().toASCIIString(), true);
		l_sDefName+="Desc";

		std::string l_sSpaces;
		l_sSpaces.resize(l_iWidth-l_sDefName.length(), ' ');
		l_oIdentifier = rPluginObjectDesc.getClassIdentifier();

		addIdentifier(l_sDefName.c_str(), l_oIdentifier, l_sSpaces.c_str());
	}

	l_oIdentifier = OV_UndefinedIdentifier;
	while((l_oIdentifier=l_rAlgorithmProxy.getNextInputParameterIdentifier(l_oIdentifier))!=OV_UndefinedIdentifier)
	{
		std::string l_sDefName;
		l_sDefName+="OVP_GD_Algorithm_";
		l_sDefName+=transform(rPluginObjectDesc.getName().toASCIIString(), true);
		l_sDefName+="_InputParameterId_";
		l_sDefName+=transform(l_rAlgorithmProxy.getInputParameterName(l_oIdentifier).toASCIIString(), true);

		std::string l_sSpaces;
		l_sSpaces.resize(l_iWidth-l_sDefName.length(), ' ');

		addIdentifier(l_sDefName.c_str(), l_oIdentifier, l_sSpaces.c_str());
	}

	l_oIdentifier = OV_UndefinedIdentifier;
	while((l_oIdentifier=l_rAlgorithmProxy.getNextOutputParameterIdentifier(l_oIdentifier))!=OV_UndefinedIdentifier)
	{
		std::string l_sDefName;
		l_sDefName+="OVP_GD_Algorithm_";
		l_sDefName+=transform(rPluginObjectDesc.getName().toASCIIString(), true);
		l_sDefName+="_OutputParameterId_";
		l_sDefName+=transform(l_rAlgorithmProxy.getOutputParameterName(l_oIdentifier).toASCIIString(), true);

		std::string l_sSpaces;
		l_sSpaces.resize(l_iWidth-l_sDefName.length(), ' ');

		addIdentifier(l_sDefName.c_str(), l_oIdentifier, l_sSpaces.c_str());
	}

	l_oIdentifier = OV_UndefinedIdentifier;
	while((l_oIdentifier=l_rAlgorithmProxy.getNextInputTriggerIdentifier(l_oIdentifier))!=OV_UndefinedIdentifier)
	{
		std::string l_sDefName;
		l_sDefName+="OVP_GD_Algorithm_";
		l_sDefName+=transform(rPluginObjectDesc.getName().toASCIIString(), true);
		l_sDefName+="_InputTriggerId_";
		l_sDefName+=transform(l_rAlgorithmProxy.getInputTriggerName(l_oIdentifier).toASCIIString(), true);

		std::string l_sSpaces;
		l_sSpaces.resize(l_iWidth-l_sDefName.length(), ' ');

		addIdentifier(l_sDefName.c_str(), l_oIdentifier, l_sSpaces.c_str());
	}

	l_oIdentifier = OV_UndefinedIdentifier;
	while((l_oIdentifier=l_rAlgorithmProxy.getNextOutputTriggerIdentifier(l_oIdentifier))!=OV_UndefinedIdentifier)
	{
		std::string l_sDefName;
		l_sDefName+="OVP_GD_Algorithm_";
		l_sDefName+=transform(rPluginObjectDesc.getName().toASCIIString(), true);
		l_sDefName+="_OutputTriggerId_";
		l_sDefName+=transform(l_rAlgorithmProxy.getOutputTriggerName(l_oIdentifier).toASCIIString(), true);

		std::string l_sSpaces;
		l_sSpaces.resize(l_iWidth-l_sDefName.length(), ' ');

		addIdentifier(l_sDefName.c_str(), l_oIdentifier, l_sSpaces.c_str());
	}

	l_rAlgorithmProxy.uninitialize();

	m_oFile << "\n";

	l_rAlgorithmManager.releaseAlgorithm(l_rAlgorithmProxy);

	return true;
}
