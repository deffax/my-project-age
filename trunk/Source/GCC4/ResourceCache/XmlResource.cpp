#include "GameCodeStd.h"
#include "XmlResource.h"

void XmlResourceExtraData::ParseXml(char* pRawBuffer)
{
	m_xmlDocument.Parse(pRawBuffer);
}

bool XmlResourceLoader::VLoadResource(char* rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle)
{
	if(rawSize < 0)
		return false;

	shared_ptr<XmlResourceExtraData> pExtraData = 
		shared_ptr<XmlResourceExtraData> (GCC_NEW XmlResourceExtraData());
	pExtraData->ParseXml(rawBuffer);
	handle->SetExtra(shared_ptr<XmlResourceExtraData> (pExtraData));
	return true;
}


TiXmlElement* XmlResourceLoader::LoadAndReturnRootXmlElement(const char* resourceString)
{
	Resource resource(resourceString);
	shared_ptr<ResHandle> pResourceHandle = g_pApp->m_resCache->GetHandle(&resource);
	shared_ptr<XmlResourceExtraData> pExtraData = 
		static_pointer_cast<XmlResourceExtraData>(pResourceHandle->GetExtra());
	return pExtraData->GetRoot();
}

shared_ptr<IResourceLoader> CreateXmlResourceLoader()
{
	return shared_ptr<IResourceLoader> (GCC_NEW XmlResourceLoader());
}

