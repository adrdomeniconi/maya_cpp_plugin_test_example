#pragma once

#include <vector>
#include <sstream>

#include <maya/MPxNode.h>
#include <maya/MGlobal.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnNumericData.h>
#include <maya/MVector.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MMatrix.h>
#include <maya/MTransformationMatrix.h>


class CenterPointNode : public MPxNode
{

public:
	CenterPointNode();

	virtual MStatus compute(const MPlug& plug, MDataBlock& data) override;

	static void* Creator();
	static MStatus Initialize();

	static MTypeId GetTypeId();
	static MString GetTypeName();

	virtual ~CenterPointNode() override;

private:
	static void defineAttributes();

	bool isDirty(const MPlug& plug);
	MVector findCenterPoint(std::vector<MVector> positions);

	static MObject inputObjectsAttr;
	static MObject outputPosAttr;
};