#pragma once

// Check if we are building the DLL, or using it
#ifdef CENTER_POINT_CPP_EXPORTS 
#define CENTER_POINT_API __declspec(dllexport)
#else
#define CENTER_POINT_API __declspec(dllimport)
#endif

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

class CENTER_POINT_API CenterPointNode : public MPxNode
{
public:
    CenterPointNode();
    virtual ~CenterPointNode() override;

    virtual MStatus compute(const MPlug& plug, MDataBlock& data) override;

    static void* Creator();
    static MStatus Initialize();

    static MTypeId GetTypeId();
    static MString GetTypeName();

    static MObject inputObjectsAttr;
    static MObject outputPosAttr;

    MVector findCenterPoint(std::vector<MVector> positions);

private:
    static void defineAttributes();

    bool isDirty(const MPlug& plug);
};
