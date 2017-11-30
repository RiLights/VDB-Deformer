#ifndef __SOP_vdb_activate_from_points_h__
#define __SOP_vdb_activate_from_points_h__

#include <SOP/SOP_Node.h>

namespace VdbActivateFromPoints {
class SOP_VdbActivateFromPoints : public SOP_Node
{
public:
    static OP_Node		*myConstructor(OP_Network*, const char *, OP_Operator *);

    static PRM_Template		 myTemplateList[];

protected:
    SOP_VdbActivateFromPoints(OP_Network *net, const char *name, OP_Operator *op);

    virtual ~SOP_VdbActivateFromPoints();

    virtual const char *inputLabel(unsigned idx) const;

    virtual OP_ERROR cookMySop(OP_Context &context);

private:
    int DEBUG() { return evalInt("debug", 0, 0); }

};
}

#endif