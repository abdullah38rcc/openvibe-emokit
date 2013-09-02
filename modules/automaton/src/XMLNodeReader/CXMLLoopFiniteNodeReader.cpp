#include "CXMLLoopFiniteNodeReader.hpp"

#include <sstream>

#include <iostream>
using namespace std;

using namespace Automaton;

CXMLLoopFiniteNodeReader::CXMLLoopFiniteNodeReader() :
	m_pNodeReaderFactory(NULL),
	m_pNodeFactory(NULL),
	m_pParameterParser(NULL),
	m_pNode(NULL),
	m_eStatus(ParsingStatus_Nothing)
{
	m_pNodeFactory = createNodeFactory();
	m_pNodeReaderFactory = createXMLNodeReaderFactory();
}

CXMLLoopFiniteNodeReader::~CXMLLoopFiniteNodeReader()
{
	releaseNodeFactory(m_pNodeFactory);
	releaseXMLNodeReaderFactory(m_pNodeReaderFactory);
}

EParsingStatus CXMLLoopFiniteNodeReader::openChild(const char* sName, const char** sAttributeName, const char** sAttributeValue, uint64 ui64AttributeCount, IAutomatonContext* pContext)
{
	//If we have to redirect to another reader
	if(m_oReaderStack.size() != 0)
	{
		if(m_oReaderStack.top()->openChild(sName, sAttributeName, sAttributeValue, ui64AttributeCount, pContext) == ParsingStatus_Error)
		{
			m_eStatus = ParsingStatus_Error;
		}
	}
	else
	{
		string l_oChildName(sName);
		m_oParsedXMLNodes.push(l_oChildName);

#ifdef DEBUG
		cout<<"LoopFiniteReader : Opening node : "<<l_oChildName<<endl;
#endif
		if(l_oChildName == "Node")
		{

			//If this is the first encountered "Node" Child, it is the one corresponding to the loop node
			if(m_eStatus == ParsingStatus_Nothing)
			{
				m_pNode = dynamic_cast<CNodeLoop*>(m_pNodeFactory->createNode(Automaton_Node_Identifier_Loop));
				m_eStatus = ParsingStatus_Loop;
			}
			//else it's a child of the loop
			else if(m_eStatus == ParsingStatus_Child)
			{
				m_eStatus = ParsingStatus_Node;

				const char * l_pValue = getAttributeValue(sAttributeName, sAttributeValue, ui64AttributeCount, "class");

				if(l_pValue)
				{
					string l_oClassAttribute = l_pValue;

					CIdentifier l_oChildIdentifier = pContext->getIdentifier(l_oClassAttribute);
					m_oReaderStack.push(m_pNodeReaderFactory->createNodeReader(l_oChildIdentifier));
					m_oReaderStack.top()->openChild(sName, sAttributeName, sAttributeValue, ui64AttributeCount, pContext);
				}
				else
				{
					cout<<"Missing attribute class in node"<<endl;
					m_eStatus = ParsingStatus_Error;
				}
			}
		}
		else if(l_oChildName == "Property" && m_eStatus == ParsingStatus_Loop)
		{
			string l_oValue = getAttributeValue(sAttributeName, sAttributeValue, ui64AttributeCount, "class");

			if(l_oValue == "Iteration")
			{
				m_eStatus = ParsingStatus_Property;
			}
			else
			{
				cout<<"Couldn't find class (=Iteration) attribute in property node"<<endl;
				m_eStatus = ParsingStatus_Error;
			}
		}
		else if(l_oChildName == "Parameter" && m_eStatus == ParsingStatus_Property)
		{
			m_pParameterParser = new CXMLParameterReader(m_oParameter);
			m_eStatus = m_pParameterParser->openChild(sName, sAttributeName, sAttributeValue, ui64AttributeCount, pContext);
		}
		else if(l_oChildName == "Child" && m_eStatus == ParsingStatus_Loop)
		{
			m_eStatus = ParsingStatus_Child;
		}
		else
		{
			cout<<"Unexpected node : "<<l_oChildName<<endl;
			m_eStatus = ParsingStatus_Error;
		}
	}

	return m_eStatus;
}

EParsingStatus CXMLLoopFiniteNodeReader::processChildData(const char* sData, IAutomatonContext* pContext)
{
	//If we have to redirect to another reader
	if(m_oReaderStack.size() != 0)
	{
		if(m_oReaderStack.top()->processChildData(sData, pContext) == ParsingStatus_Error)
		{
			m_eStatus = ParsingStatus_Error;
		}
	}
	else
	{

		string& l_oChildName = m_oParsedXMLNodes.top();

#ifdef DEBUG
		cout<<"LoopFiniteReader : Processing node : "<<l_oChildName<<"..."<<endl;
#endif
		if(l_oChildName == "Node" || l_oChildName == "Property" || l_oChildName == "Child")
		{

		}
		else if(l_oChildName == "Parameter")
		{
			m_eStatus = m_pParameterParser->processChildData(sData, pContext);
		}
		else
		{
			cout<<"Unexpected node : "<<l_oChildName<<endl;
			m_eStatus = ParsingStatus_Error;
		}
	}

	return m_eStatus;
}

EParsingStatus CXMLLoopFiniteNodeReader::closeChild(IAutomatonContext* pContext)
{
	//If we have to redirect to another reader
	if(m_oReaderStack.size() != 0)
	{
		EParsingStatus l_eChildStatus = m_oReaderStack.top()->closeChild(pContext);

		//the node has been read
		if(l_eChildStatus==ParsingStatus_Complete)
		{
			CIdentifier l_oLastNode = m_oReaderStack.top()->getNodeIdentifier();

			m_pNodeReaderFactory->releaseNodeReader(m_oReaderStack.top());
			m_oReaderStack.pop();

			m_oParsedXMLNodes.pop();

			m_oNodes.push_back(l_oLastNode);

			m_eStatus = ParsingStatus_Child;

			if(m_oNodes.size() > 1)
			{
#ifdef DEBUG
				cout<<"link between"<< (uint64)m_oNodes[m_oNodes.size()-2]<<" and "<<(uint64)l_oLastNode<<endl;
#endif
				pContext->addSuccessor(m_oNodes[m_oNodes.size()-2], l_oLastNode);
			}
		}
		else if(l_eChildStatus==ParsingStatus_Error)
		{
			m_eStatus = ParsingStatus_Error;
		}
	}
	else
	{

		string& l_oChildName = m_oParsedXMLNodes.top();

#ifdef DEBUG
		cout<<"LoopFiniteReader : Closing node : "<<l_oChildName<<endl;
#endif

		if(l_oChildName == "Node" && m_eStatus==ParsingStatus_Loop)
		{

			if(m_oNodes.size() != 0)
			{
				pContext->addSuccessor(m_oNodeIdentifier, m_oNodes.front());
				pContext->addSuccessor(m_oNodes.back(), m_oNodeIdentifier);
			}

			m_eStatus = ParsingStatus_Complete;
		}
		else if(l_oChildName == "Property" && m_eStatus==ParsingStatus_Property)
		{
			m_pNode->setLoopCondition("Finite", m_oParameter, pContext);

			m_oNodeIdentifier = pContext->addNode(m_pNode);

			m_eStatus = ParsingStatus_Loop;
		}
		else if(l_oChildName == "Parameter" && m_eStatus==ParsingStatus_Parameter)
		{
			m_eStatus = m_pParameterParser->closeChild(pContext);
			delete m_pParameterParser;
		}
		else if(l_oChildName == "Child" && m_eStatus==ParsingStatus_Child)
		{
			m_eStatus = ParsingStatus_Loop;
		}
		else
		{
			cout<<"Unexpected node : "<<l_oChildName<<endl;
			m_eStatus = ParsingStatus_Error;
		}

		m_oParsedXMLNodes.pop();

	}

	return m_eStatus;

}

void CXMLLoopFiniteNodeReader::release()
{
	delete this;
}

