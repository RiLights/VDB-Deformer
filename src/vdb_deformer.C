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
    mySopFlags.setManagesDataIDs(true);


    myCurrPoint = -1; // To prevent garbage values from being returned
}

VDB_Deformer::~VDB_Deformer() {}

OP_ERROR
VDB_Deformer::cookMySop(OP_Context &context)
{

    fpreal t = context.getTime();

    OP_AutoLockInputs inputs(this);
    if (inputs.lock(context) >= UT_ERROR_ABORT)
        return error();

    duplicateSource(0, context);

    //gdp->clearAndDestroy();

    UT_AutoInterrupt boss("Building Star");
    if (boss.wasInterrupted())
    {
        myCurrPoint = -1;
        return error();
    }
    //openvdb::initialize();
    //GU_PrimVDB           *vol;
    //vol = (GU_PrimVDB *)GU_PrimVDB::build((GU_Detail *)gdp);
    //vol->build(gdp);
    //openvdb::createGrid<openvdb::FloatGrid>();
    //openvdb::FloatGrid::Ptr grid = openvdb::FloatGrid::create();
    //vol->buildFromGrid(gdp,grid);

    //GA_Primitive *vo=gdp->getPrimitiveByIndex(0);
    GEO_Primitive *vo=gdp->getGEOPrimitiveByIndex(0);
    GU_PrimVDB   *volumePtr = (GU_PrimVDB *)(gdp->getGEOPrimitiveByIndex(0));



    //context.getData();

    openvdb::FloatGrid::Ptr grid = openvdb::FloatGrid::create();

    openvdb::FloatGrid::Accessor accessor = grid->getAccessor();
    openvdb::Coord xyz(5,5,5);
    accessor.setValue(xyz, 1.0);
    //xyz.reset(0,0,0);
    accessor.setValue(xyz, 1.0);
    //GU_PrimVDB::buildFromGrid((GU_Detail&)*gdp, grid, NULL, "density1");

    int x,y,z;
    volumePtr->getRes(x,y,z);
    openvdb::GridBase::Ptr gg=volumePtr->getGridPtr();
    openvdb::FloatGrid::Ptr gridf = openvdb::gridPtrCast<openvdb::FloatGrid>(gg);
    openvdb::FloatGrid::Accessor accessor2 = gridf->getAccessor();

    std::vector<openvdb::math::Coord> mas;
    //accessor2.setValue(xyz, 100.0);
    for (openvdb::FloatGrid::ValueOnIter iter = gridf->beginValueOn(); iter; ++iter) {
        float dist = iter.getValue();
        //std::cout << iter.getCoord() << std::endl;
        mas.push_back(iter.getCoord());
        openvdb::math::Coord val=iter.getCoord();
        val.setZ(val[2]+1);
        //accessor2.setValue(val,1);
        //iter.setValueOff();
        //std::cout<<"hi "<<dist<<"\n";
        //iter.setValue((outside - dist) / width);
    }
    //for (std::vector::iterator ix=mas.begin();mas.end();ix++){
//
  //  }
/*
    for (int ix=-x/2;ix<=x/2;ix++){
        for (int iy=-y/2;iy<=y/2;iy++){
            for (int iz=-z/2;iz<=z/2;iz++){

                openvdb::Coord xyz2(ix,iy,iz);
                openvdb::Coord xyzs(ix,iy,iz+5);
                //accessor2.setActiveState(xyzs,true);
                //accessor2.setValue(xyzs, accessor2.getValue(xyz2));
                accessor2.setValue(xyzs, 1);
                //accessor2.setActiveState(xyz2,false);
            }
        }
    }*/

    /*
    for (openvdb::FloatGrid::ValueOnCIter iter = gridf->cbeginValueOn(); iter.test(); ++iter) {
        //const openvdb::Vec3f& value = *iter;
        // Print the coordinates of all voxels whose vector value has
        // a length greater than 10, and print the bounding box coordinates
        // of all tiles whose vector value length is greater than 10.
        //std::cout << iter.getCoord() << std::endl;
        iter.getValue();
        /*
        if (value.length() > 10.0) {
            if (iter.isVoxelValue()) {
                std::cout << iter.getCoord() << std::endl;
            } else {
                openvdb::CoordBBox bbox;
                iter.getBoundingBox(bbox);
                std::cout << bbox << std::endl;
            }
        }
    }*/


    std::cout<<"hi2 "<<x<<"\n";


    UT_Vector3 pos;
    pos(0) = 1;
    pos(1) = 2;
    pos(2) = 3;

    //GA_Offset ptoff = gdp->pointOffset(GA_Index(1));
    //GA_Offset ptoff = gdp->appendPointOffset();
    //gdp->setPos3(ptoff, pos);

    printf("done\n");

    return error();
}
