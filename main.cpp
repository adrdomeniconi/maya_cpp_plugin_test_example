#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include "CenterPointNode.h"

MStatus initializePlugin(MObject pluginObj)
{
	const char* vendor = "Adriano Domeniconi";
	const char* version = "1.0.0";
	const char* requiredApiVersion = "Any";

	MStatus status;

	MFnPlugin pluginFn(pluginObj, vendor, version, requiredApiVersion, &status);

	if(!status)
	{
		MGlobal::displayError("Failed to initialize plugin: " + status.errorString());
		return(status);
	}

	status = pluginFn.registerNode(CenterPointNode::GetTypeName(),
								CenterPointNode::GetTypeId(),
								CenterPointNode::Creator,
								CenterPointNode::Initialize,
								CenterPointNode::kDependNode);
	if (!status)
	{
		MGlobal::displayError("Failed to register centerPointNode: " + status.errorString());
		return(status);
	}

	return status;
}

MStatus uninitializePlugin(MObject pluginObj)
{
	MStatus status;
	MFnPlugin pluginFn(pluginObj);

	status = pluginFn.deregisterNode(CenterPointNode::GetTypeId());
	if (!status)
	{
		MGlobal::displayError("Failed to de-register centerPointNode: " + status.errorString());
		return(status);
	}

	return status;
}