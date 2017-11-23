#include "vdb_deformer.h"

#include <GU/GU_Detail.h>
#include <GU/GU_PrimVolume.h>
#include <GEO/GEO_PrimPoly.h>
#include <OP/OP_Operator.h>
#include <OP/OP_OperatorTable.h>
#include <OP/OP_AutoLockInputs.h>
#include <PRM/PRM_Include.h>
#include <CH/CH_LocalVariable.h>
#include <UT/UT_DSOVersion.h>
#include <UT/UT_Interrupt.h>
#include <SYS/SYS_Math.h>
#include <OP/OP_Director.h>
#include <limits.h>
#include <UT/UT_Assert.h>

#include <GU/GU_PrimVDB.h>

#include <openvdb/openvdb.h>
#include <openvdb/tools/Interpolation.h>
#include <openvdb/tools/Dense.h>
#include <iostream>


void
newSopOperator(OP_OperatorTable *table)
{
    table->addOperator(new OP_Operator(
        "vdb_deformer",                 // Internal name
        "VDB_Deformer",                     // UI name
        VDB_Deformer::myConstructor,    // How to build the SOP
        VDB_Deformer::myTemplateList,   // My parameters
        1,                          // Min # of sources
        1));                           // Max # of sources
}

static PRM_Name     negativeName("nradius", "Negative Radius");
static PRM_Name     helloName("debug_view", "debug_view");

//                   ^^^^^^^^    ^^^^^^^^^^^^^^^
//                   internal    descriptive version

static PRM_Default  fiveDefault(5);     // Default to 5 divisions
static PRM_Default  radiiDefaults[] = {
               PRM_Default(1),      // Outside radius
               PRM_Default(0.3)     // Inside radius
            };
int
VDB_Deformer::testUI(void *op, int, fpreal, const PRM_Template *)
{
    UT_OStrStream   os;
    UT_String       result_str;
    int             result_int;
    const char *    button;
    UT_String       command;


    // construct and run the command
    command = "message "
              "-b \"Left,Middle,Right\" "
              "-d 0 "
              "Please press any button.";
    OPgetDirector()->getCommandManager()->execute( command, false, &os );
    // retrieve the output stream string and convert it into an integer
    printf("dialog was opened\n");
    return 1;
}

PRM_Template
VDB_Deformer::myTemplateList[] = {
    PRM_Template(PRM_CALLBACK,1,&helloName,0, 0, 0, &VDB_Deformer::testUI),
    PRM_Template()
};



OP_Node *
VDB_Deformer::myConstructor(OP_Network *net, const char *name, OP_Operator *op)
{
    return new VDB_Deformer(net, name, op);
}

VDB_Deformer::VDB_Deformer(OP_Network *net, const char *name, OP_Operator *op)
    : SOP_Node(net, name, op)
{
}

VDB_Deformer::~VDB_Deformer() {}

OP_ERROR
VDB_Deformer::cookMySop(OP_Context &context)
{

    OP_AutoLockInputs inputs(this);
    if (inputs.lock(context) >= UT_ERROR_ABORT)
        return error();

    gdp->clearAndDestroy();
    duplicateSource(0, context);//const GU_Detail *gdp = inputGeo(0);//duplicateSource(0, context);


    UT_AutoInterrupt boss("Building Deformer");
    if (boss.wasInterrupted())
    {
        return error();
    }

    GU_PrimVDB   *volumePtr = (GU_PrimVDB *)(gdp->getGEOPrimitiveByIndex(0));


    //openvdb::FloatGrid::Ptr grid = openvdb::FloatGrid::create(volumePtr->getGrid());

    //openvdb::FloatGrid::Accessor accessor = grid->getAccessor();
    //GU_PrimVDB::buildFromGrid((GU_Detail&)*gdp, grid, NULL, "density1");

    //int x,y,z;
    //volumePtr->getRes(x,y,z);
    volumePtr->makeGridUnique();
    openvdb::GridBase::Ptr gg=volumePtr->getGridPtr();
    openvdb::FloatGrid::Ptr gridf = openvdb::gridPtrCast<openvdb::FloatGrid>(gg);
    openvdb::FloatGrid::Accessor accessor2 = gridf->getAccessor();

    /////
    std::vector<openvdb::math::Coord> mas;
    for (openvdb::FloatGrid::ValueOnIter iter = gridf->beginValueOn(); iter; ++iter) {
        float dist = iter.getValue();
        mas.push_back(iter.getCoord());
    }

    for (int ix=0;ix<mas.size();ix++){
        openvdb::math::Coord val=mas[ix];
        //accessor2.setValue(val,0);
        val.setZ(val[2]+25);
        accessor2.setValue(val,1);
    }


    return error();
}
