#include "CenterPointNode.h"

static const MTypeId TYPE_ID = MTypeId(0x0007F7F3);
static const MString TYPE_NAME = "centerPoint";

MObject CenterPointNode::inputObjectsAttr;
MObject CenterPointNode::outputPosAttr;


CenterPointNode::CenterPointNode() 
{
}

MStatus CenterPointNode::compute(const MPlug& plug, MDataBlock& data)
{
	if (!isDirty(plug))
	{
		return MS::kSuccess;
	}

	std::vector<MVector> positions;
	MStatus result;

	MArrayDataHandle arrayDataHandle = data.inputArrayValue(inputObjectsAttr, &result);
	if (!result)
	{
		MGlobal::displayError("Error getting the input value!");
		return MS::kFailure;
	}

	result = arrayDataHandle.jumpToArrayElement(0);
	if (!result)
	{
		MGlobal::displayError("Error getting the first element of the inputs!");
		return MS::kFailure;
	}
	
	do
	{
		MDataHandle inputDataHandle = arrayDataHandle.inputValue(&result);
		if (!result)
		{
			MGlobal::displayError("Error getting value of the input!");
			return MS::kFailure;
		}

		MMatrix worldMatrix = inputDataHandle.asMatrix();
		MTransformationMatrix trasnformationMatrix(worldMatrix);
		MVector translation = trasnformationMatrix.getTranslation(MSpace::kWorld, &result);
		if (!result)
		{
			MGlobal::displayError("Error getting the translation value of the input!");
			return MS::kFailure;
		}

		positions.push_back(translation);

	} while (arrayDataHandle.next() == MS::kSuccess);

	MVector centerPoint = findCenterPoint(positions);

	MDataHandle outputDataHandle = data.outputValue(outputPosAttr, &result);
	if (!result)
	{
		MGlobal::displayError("Error getting the output data handle!");
		return MS::kFailure;
	}

	outputDataHandle.set3Double(centerPoint.x, centerPoint.y, centerPoint.z);

	data.setClean(plug);

	return MS::kSuccess;
}

void* CenterPointNode::Creator()
{
	return new CenterPointNode();
}

MStatus CenterPointNode::Initialize()
{
	defineAttributes();
	return MS::kSuccess;
}

MTypeId CenterPointNode::GetTypeId()
{
	return TYPE_ID;
}

MString CenterPointNode::GetTypeName()
{
	return TYPE_NAME;
}

bool CenterPointNode::isDirty(const MPlug& plug)
{
	return plug == outputPosAttr;
}

MVector CenterPointNode::findCenterPoint(std::vector<MVector> positions)
{
	MVector sum_vector(0.0f, 0.0f, 0.0f);

	for (MVector pos : positions)
	{
		sum_vector += pos;
	}

	MVector center_point = sum_vector / positions.size();

	return center_point;
}

void CenterPointNode::defineAttributes()
{
	MFnMatrixAttribute matrixAttrFn;
	inputObjectsAttr = matrixAttrFn.create("input", "in", MFnMatrixAttribute::kDouble);
	matrixAttrFn.setArray(true);
	matrixAttrFn.setStorable(true);
	matrixAttrFn.setKeyable(true);
	matrixAttrFn.setReadable(false);
	matrixAttrFn.setWritable(true);

	addAttribute(inputObjectsAttr);

	MFnNumericAttribute numericAttrFn;
	outputPosAttr = numericAttrFn.create("outputPosition", "op", MFnNumericData::k3Double);
	numericAttrFn.setStorable(false);
	numericAttrFn.setReadable(true);
	numericAttrFn.setWritable(false);

	addAttribute(outputPosAttr);

	attributeAffects(inputObjectsAttr, outputPosAttr);
}

CenterPointNode::~CenterPointNode()
{
}
