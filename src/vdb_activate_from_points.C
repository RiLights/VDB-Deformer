#include "vdb_activate_from_points.h"

#include <limits.h>
#include <SYS/SYS_Math.h>

#include <UT/UT_DSOVersion.h>
#include <UT/UT_Interrupt.h>

#include <OP/OP_Operator.h>
#include <OP/OP_OperatorTable.h>

#include <GU/GU_Detail.h>
#include <GEO/GEO_PrimPoly.h>

#include <PRM/PRM_Include.h>
#include <CH/CH_LocalVariable.h>

#include <OP/OP_AutoLockInputs.h>

#include <GU/GU_PrimVDB.h>

#include <openvdb/openvdb.h>


using namespace VdbActivateFromPoints;

// register the operator
void newSopOperator(OP_OperatorTable *table)
{
	table->addOperator(new OP_Operator(
		"vdbActivateFromPoints",
		"VDB Activate from Points",
		SOP_VdbActivateFromPoints::myConstructor,
		SOP_VdbActivateFromPoints::myTemplateList,
		2,
		2));
}

// label node inputs
const char *
SOP_VdbActivateFromPoints::inputLabel(unsigned idx) const
{
    switch (idx){
        case 0: return "VDB";
        case 1: return "Points where active voxels should be";
        default: return "default";
    }
}

// set empty parameter interface
PRM_Template SOP_VdbActivateFromPoints::myTemplateList[] = {
    PRM_Template()
};

OP_Node * SOP_VdbActivateFromPoints::myConstructor(OP_Network *net, const char *name, OP_Operator *op)
{
    return new SOP_VdbActivateFromPoints(net, name, op);
}

SOP_VdbActivateFromPoints::SOP_VdbActivateFromPoints(OP_Network *net, const char *name, OP_Operator *op)
    : SOP_Node(net, name, op)
{
    //mySopFlags.setManagesDataIDs(true);
}

SOP_VdbActivateFromPoints::~SOP_VdbActivateFromPoints() {}

OP_ERROR
SOP_VdbActivateFromPoints::cookMySop(OP_Context &context)
{
    // lock inputs
    OP_AutoLockInputs inputs(this);
    if (inputs.lock(context) >= UT_ERROR_ABORT)
        return error();

    // clear gdp and duplicate src
    gdp->clearAndDestroy();
    duplicateSource(0, context);

    // get pointer to points from second input
    const GU_Detail *points = inputGeo(1);

    std::cout << "number of active points: " << points->getNumPoints() << std::endl;

    // check for escape
    UT_AutoInterrupt progress("Activating voxels");
    if (progress.wasInterrupted())
        return error();

    // get pointer to GU_PrimVDB primitive
    GU_PrimVDB *vdbPrim = reinterpret_cast<GU_PrimVDB *> (gdp->getGEOPrimitiveByIndex(0));

    // make deep copy
    vdbPrim->makeGridUnique();
    
    // get grid base pointer and cast to float grid pointer
    openvdb::GridBase::Ptr vdbPtrBase = vdbPrim->getGridPtr();
    openvdb::FloatGrid::Ptr vdbPtr = openvdb::gridPtrCast<openvdb::FloatGrid>(vdbPtrBase);

    // get accessor to the float grid
    openvdb::FloatGrid::Accessor vdb_acess = vdbPtr->getAccessor();

    // get transformation of the grid
    const openvdb::math::Transform &vdbGridXform = vdbPtr->transform();

    // loop over all the points by handle
    GA_ROHandleV3 Phandle(points->findAttribute(GA_ATTRIB_POINT, "P"));
    GA_Offset ptoff;
    GA_FOR_ALL_PTOFF(points, ptoff)
    {
        UT_Vector3 Pvalue = Phandle.get(ptoff);
        std::cout << ptoff << ". point world space position: " << Pvalue << std::endl;

        // create openvdb vector with values from houdini's vector, transform it from world space to vdb's index space (based on vdb's transformation)
        openvdb::Vec3R p_( Pvalue[0], Pvalue[1], Pvalue[2] );
        openvdb::Coord p_xformed( vdbGridXform.worldToIndexCellCentered(p_) );
        std::cout << " volmue index space position: " << p_xformed << std::endl;
        vdb_acess.setValueOn( p_xformed );
    }
    /*
    // loop over all the points and activate voxels at points' positions
    for (int i=0; i < points->getNumPoints(); i++){
        // get current point position
        UT_Vector3 p = points->getPos3(i);
        std::cout << i << ". point world space position: " << p << std::endl;

        // create openvdb vector with values from houdini's vector, transform it from world space to vdb's index space (based on vdb's transformation)
        openvdb::Vec3R p_( p[0], p[1], p[2] );
        openvdb::Coord p_xformed( vdbGridXform.worldToIndexCellCentered(p_) );
        std::cout << " volmue index space position: " << p_xformed << std::endl;
        vdb_acess.setValueOn( p_xformed );
    }*/

    return error();
}
