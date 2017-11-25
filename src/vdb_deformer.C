/*
 * Copyright (c) 2017
 *  Side Effects Software Inc.  All rights reserved.
 *
 * Redistribution and use of Houdini Development Kit samples in source and
 * binary forms, with or without modification, are permitted provided that the
 * following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. The name of Side Effects Software may not be used to endorse or
 *    promote products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY SIDE EFFECTS SOFTWARE `AS IS' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
 * NO EVENT SHALL SIDE EFFECTS SOFTWARE BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *----------------------------------------------------------------------------
 * The Star SOP
 */

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



//
// Help is stored in a "wiki" style text file.  This text file should be copied
// to $HOUDINI_PATH/help/nodes/sop/star.txt
//
// See the sample_install.sh file for an example.
//


///
/// newSopOperator is the hook that Houdini grabs from this dll
/// and invokes to register the SOP.  In this case we add ourselves
/// to the specified operator table.
///
void
newSopOperator(OP_OperatorTable *table)
{
    table->addOperator(new OP_Operator(
        "vdb_deformer",                 // Internal name
        "VDB_Deformer",                     // UI name
        VDB_Deformer::myConstructor,    // How to build the SOP
        VDB_Deformer::myTemplateList,   // My parameters
        3,                          // Min # of sources
        3));                           // Max # of sources
}

const char *
VDB_Deformer::inputLabel(unsigned idx) const
{
    switch (idx){
        case 0: return "Original VDBs";
        case 1: return "Rest Lattice";
        case 2: return "Lattice";
        default: return "default";
    }
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
    //mySopFlags.setManagesDataIDs(true);


    //myCurrPoint = -1; // To prevent garbage values from being returned
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
    const GU_Detail *gpd_rest_deform_points = inputGeo(1);
    const GU_Detail *gpd_deform_points = inputGeo(2);
    std::cout<<"debug "<<gpd_rest_deform_points->getNumPoints()<<std::endl;
    for (int i=0;i<gpd_rest_deform_points->getNumPoints();i++){

        UT_Vector3 p = gpd_rest_deform_points->getPos3(i);
        std::cout<<"debugP "<<p<<std::endl;
    }

    UT_AutoInterrupt boss("Building Deformer");
    if (boss.wasInterrupted())
    {
        return error();
    }

    GU_PrimVDB   *vdbPrim = reinterpret_cast<GU_PrimVDB *> (gdp->getGEOPrimitiveByIndex(0));  //(GU_PrimVDB *)(gdp->getGEOPrimitiveByIndex(0));

    if(!vdbPrim)
        {
            addError(SOP_MESSAGE, "Input geometry must contain a VDB");
            return error();
        }


    openvdb::FloatGrid::Ptr grid_new = openvdb::FloatGrid::create(vdbPrim->getGrid());

    openvdb::FloatGrid::Accessor accessor_new = grid_new->getAccessor();
    GU_PrimVDB::buildFromGrid((GU_Detail&)*gdp, grid_new, NULL, "deformed");

    int x,y,z;
    vdbPrim->getRes(x,y,z);
    //vdbPrim->makeGridUnique();
    openvdb::GridBase::Ptr gridbase=vdbPrim->getGridPtr();
    openvdb::FloatGrid::Ptr grid = openvdb::gridPtrCast<openvdb::FloatGrid>(gridbase);
    //openvdb::FloatGrid::Accessor accessor = grid->getAccessor();


    std::vector<openvdb::math::Coord> mas;
    for (openvdb::FloatGrid::ValueOnIter iter = grid->beginValueOn(); iter; ++iter) {
        float dist = iter.getValue();
        mas.push_back(iter.getCoord());
    }

    for (int ix=0;ix<mas.size();ix++){
        openvdb::math::Coord val=mas[ix];
        //accessor2.setValue(val,0);
        val.setZ(val[2]+25);
        accessor_new.setValue(val,1);
    }


    return error();
}
