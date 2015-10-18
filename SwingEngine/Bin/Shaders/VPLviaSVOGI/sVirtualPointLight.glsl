#define PI 3.141593
#define VPL_INTENSITY 1.0
#define VPL_LUMINANCE_EPSILON 0.01

struct VPLBufferHead
{
    ivec2 prevLevelStartXY;
    ivec2 curLevelStartXY;
    float curLevelWarpExtension;
    float padding1;
    vec2 curLevelWarpStartXY;
    float importance00;
    float importance10;
    float importance01;
    float importance11;
};

//----------------------------------------------------------------------------
// Virtual Point Light
//----------------------------------------------------------------------------
struct VPL
{
    vec4 WorldPosition;
    vec4 WorldNormal;
    vec4 Flux;
    mat4 View;
};

layout(std430, binding = 0)  buffer _VPLBuffer
{
	VPLBufferHead head;
    VPL vpls[];
} VPLBuffer;

//----------------------------------------------------------------------------
mat4 GetVPLViewTransform(const vec3 E, const vec3 D)
{
    mat4 res;

    vec3 R, U;
    if( D.x > D.y )
    {
        R = vec3(D.z, 0.0, -D.x);
    }
    else
    {
        R = vec3(0.0, D.z, -D.y);
    }
    R = normalize(R);
    U = cross(D, R);

    float EdR = dot(E, R);
    float EdU = dot(E, U);
    float EdD = dot(E, D);

    res = mat4(vec4(R.x, U.x, D.x, 0.0),
        vec4(R.y, U.y, D.y, 0.0),
        vec4(R.z, U.z, D.z, 0.0),
        vec4(-EdR, -EdU, -EdD, 1.0));

    return res;
}
//----------------------------------------------------------------------------
float ComputeImportance(vec3 value)
{
    //if( value.r > 0.0 || value.g > 0.0 || value.b > 0.0 )
    //{
    //    return 1.0;
    //}
    //else
    //{
    //    return 0.0;
    //}
    return value.r*0.3 + value.g*0.4 + value.b*0.3;
    //return 1.0;
}
//----------------------------------------------------------------------------