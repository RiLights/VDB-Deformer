#ifndef __SOP_Star_h__
#define __SOP_Star_h__

#include <SOP/SOP_Node.h>


class VDB_Deformer : public SOP_Node
{
public:
    static OP_Node *myConstructor(OP_Network*, const char *, OP_Operator *);

    /// Stores the description of the interface of the SOP in Houdini.
    /// Each parm template refers to a parameter.
    static PRM_Template	myTemplateList[];
    static int testUI(void *op, int, fpreal time, const PRM_Template *);

protected:
    VDB_Deformer(OP_Network *net, const char *name, OP_Operator *op);
    virtual ~VDB_Deformer();


    /// cookMySop does the actual work of the SOP computing, in this
    /// case, a star shape.
    virtual OP_ERROR cookMySop(OP_Context &context);

private:
    /// The following list of accessors simplify evaluating the parameters
    /// of the SOP.
    //int		DIVISIONS(fpreal t)	{ return evalInt  ("divs", 0, t); }

    /// Member variables are stored in the actual SOP, not with the geometry
    /// In this case these are just used to transfer data to the local
    /// variable callback.
    /// Another use for local data is a cache to store expensive calculations.
    //int		myCurrPoint;

    void	ATTRIBNAME(UT_String &str, fpreal t)
                    { evalString(str, 0, 0, t); }
    fpreal	VALUE(fpreal t)		{ return evalFloat(1, 0, t); }
};


#endif
