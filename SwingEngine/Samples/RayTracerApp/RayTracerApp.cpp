#include "SwingEngineCore.h"
#include "SwingEngineBossaNova.h"

using namespace Swing;

class WorkerThread : public SEThread
{
public:
    WorkerThread(){}
    ~WorkerThread(){}

    int Run()
    {

        return 0;
    }
};

class MyTask : public SETask
{
public:
    MyTask(int iID){ m_iID = iID; }
    ~MyTask(){}

    void DoWork()
    {
        //for( int i = 0; i < 10; ++i )
        //{
        //    std::cout << "The " << i+1 << "th step of " << "task " << m_iID << " is being done" << "\n";
        //}
        //SEMatrix4f mat1, mat2, mat3;
        //for( int i = 0; i < 20000000; ++i )
        //{
        //    mat3 = mat1 * mat2;
        //}
    }

private:
    int m_iID;
};

class DebugVolumeShader : public SEBNVolumeShader
{
public:
    DebugVolumeShader(float attenuation)
    {
        Attenuation = attenuation;
    }

    // Volume shading interface.
    SESpectrum Li(const SEBNScene*, const SEBNRenderer*,
        const SERayDifferential&, const SEBNSample*, 
        SERandomNumberGenerator&, SESpectrum* T, 
        SEMemoryArena&) const
    {
        *T = Attenuation;
        return 0.0f;
    }

    // Transmittance shading interface.
    SESpectrum Transmittance(const SEBNScene*,
        const SEBNRenderer*, const SERayDifferential&,
        const SEBNSample*, SERandomNumberGenerator&, 
        SEMemoryArena&) const
    {
        return Attenuation;
    }

    float Attenuation;
};

class DebugSurfaceShader : public SEBNSurfaceShader
{
public:
    // Surface shading interface.
    virtual SESpectrum Li(const SEBNScene*, const SEBNRenderer*,
        const SERayDifferential&, const SEBNIntersection&,
        const SEBNSample*, SERandomNumberGenerator&, 
        SEMemoryArena&) const
    {
        return 1.0f;
    }
};

class DebugSampler : public SEBNSampler
{
public:
    DebugSampler(int xstart, int xend, int ystart, int yend,
        int xs, int ys, float sopen, float sclose)
        :
        SEBNSampler(xstart, xend, ystart, yend, xs * ys, sopen, sclose)
    {
        CurXPos = XPixelStart;
        CurYPos = YPixelStart;
        XPixelSamples = xs;
        YPixelSamples = ys;
    }

    ~DebugSampler()
    {
    }

    int RoundSize(int size) const
    {
        return size;
    }

    SEBNSampler* GetSubSampler(int num, int count)
    {
        int x0, x1, y0, y1;
        ComputeSubWindow(num, count, &x0, &x1, &y0, &y1);

        if( x0 == x1 || y0 == y1 )
        {
            return 0;
        }

        return SE_NEW DebugSampler(x0, x1, y0, y1, XPixelSamples,
            YPixelSamples, ShutterOpen, ShutterClose);
    }

    int GetMoreSamples(SEBNSample* samples, SERandomNumberGenerator&)
    {
        if( CurYPos == YPixelEnd )
        {
            return 0;
        }

        samples->ImageX = CurXPos + 0.5f;
        samples->ImageY = CurYPos + 0.5f;
        samples->LensU = 0.5f;
        samples->LensV = 0.5f;
        samples->Time = 0.5f;

        // Advance to next pixel for stratified sampling.
        if( ++CurXPos == XPixelEnd )
        {
            CurXPos = XPixelStart;
            ++CurYPos;
        }

        return 1;
    }

    int MaximumSampleCount()
    {
        return 1;
    }

private:
    int XPixelSamples, YPixelSamples;

    // Helper variables.
    int CurXPos, CurYPos;
};

//----------------------------------------------------------------------------
void CreateBox(std::vector<SEBNPrimitivePtr>& primitives, 
    float extension, float colorScale, SETransformation& worldTransform)
{
    SE_ASSERT( extension > 0.0f );

    int indexBuffer[6] = { 0, 1, 2, 0, 2, 3 };
    SEVector3f vertexBuffer[4];
    vertexBuffer[0] = SEVector3f(-extension, 0, -extension);
    vertexBuffer[1] = SEVector3f(-extension, 0,  extension);
    vertexBuffer[2] = SEVector3f( extension, 0,  extension);
    vertexBuffer[3] = SEVector3f( extension, 0, -extension);
    float uvBuffer[8] = { 0.0f, 1.0f,
                          0.0f, 0.0f,
                          1.0f, 0.0f,
                          1.0f, 1.0f };

    SETransformation localTransform;
    SETransformation finalTransform;
    SEMatrix3f rotation;

    // Create meshes.
    rotation.FromAxisAngle(SEVector3f(1.0f, 0.0f, 0.0f), -SEMathf::HALF_PI);
    localTransform.SetRotate(rotation);
    localTransform.SetTranslate(SEVector3f(0.0f, extension, -extension));
    finalTransform.Product(localTransform, worldTransform);
    SEBNTriangleMesh* frontWallMesh = SE_NEW SEBNTriangleMesh(&finalTransform, 
        false, 2, 4, indexBuffer, vertexBuffer, 0, 0, uvBuffer, 0);

    rotation.FromAxisAngle(SEVector3f(1.0f, 0.0f, 0.0f), SEMathf::HALF_PI);
    localTransform.SetRotate(rotation);
    localTransform.SetTranslate(SEVector3f(0.0f, extension, extension));
    finalTransform.Product(localTransform, worldTransform);
    SEBNTriangleMesh* backWallMesh = SE_NEW SEBNTriangleMesh(&finalTransform, 
        false, 2, 4, indexBuffer, vertexBuffer, 0, 0, uvBuffer, 0);
    
    rotation.FromAxisAngle(SEVector3f(1.0f, 0.0f, 0.0f), SEMathf::PI);
    localTransform.SetRotate(rotation);
    localTransform.SetTranslate(SEVector3f::ZERO);
    finalTransform.Product(localTransform, worldTransform);
    SEBNTriangleMesh* floorMesh = SE_NEW SEBNTriangleMesh(&finalTransform, 
        false, 2, 4, indexBuffer, vertexBuffer, 0, 0, uvBuffer, 0);

    rotation.FromAxisAngle(SEVector3f(0.0f, 0.0f, 1.0f), SEMathf::HALF_PI);
    localTransform.SetRotate(rotation);
    localTransform.SetTranslate(SEVector3f(-extension, extension, 0.0f));
    finalTransform.Product(localTransform, worldTransform);
    SEBNTriangleMesh* leftWallMesh = SE_NEW SEBNTriangleMesh(&finalTransform, 
        false, 2, 4, indexBuffer, vertexBuffer, 0, 0, uvBuffer, 0);

    rotation.FromAxisAngle(SEVector3f(0.0f, 0.0f, 1.0f), -SEMathf::HALF_PI);
    localTransform.SetRotate(rotation);
    localTransform.SetTranslate(SEVector3f(extension, extension, 0.0f));
    finalTransform.Product(localTransform, worldTransform);
    SEBNTriangleMesh* rightWallMesh = SE_NEW SEBNTriangleMesh(&finalTransform, 
        false, 2, 4, indexBuffer, vertexBuffer, 0, 0, uvBuffer, 0);

    localTransform.SetRotate(SEMatrix3f::IDENTITY);
    localTransform.SetTranslate(SEVector3f(0.0f, 2*extension, 0.0f));
    finalTransform.Product(localTransform, worldTransform);
    SEBNTriangleMesh* ceilingMesh = SE_NEW SEBNTriangleMesh(&finalTransform, 
        false, 2, 4, indexBuffer, vertexBuffer, 0, 0, uvBuffer, 0);

    // Create box material.
    SESpectrum color;
    color[0] = 1.0f * colorScale;
    color[1] = 1.0f * colorScale;
    color[2] = 1.0f * colorScale;
    SEBNConstantTexture<SESpectrum>* diffuseWhite = 
        SE_NEW SEBNConstantTexture<SESpectrum>(color);
    SEBNConstantTexture<float>* sigmaZero = 
        SE_NEW SEBNConstantTexture<float>(0.0f);
    SEBNMatteMaterial* whiteMaterial = SE_NEW SEBNMatteMaterial(diffuseWhite, 
        sigmaZero, 0);

    // Create primitives.
    SEBNPrimitive* frontWallPrimitive = SE_NEW SEBNGeometricPrimitive(
        frontWallMesh, whiteMaterial, 0);

    SEBNPrimitive* backWallPrimitive = SE_NEW SEBNGeometricPrimitive(
        backWallMesh, whiteMaterial, 0);

    SEBNPrimitive* floorPrimitive = SE_NEW SEBNGeometricPrimitive(
        floorMesh, whiteMaterial, 0);

    SEBNPrimitive* ceilingPrimitive = SE_NEW SEBNGeometricPrimitive(
        ceilingMesh, whiteMaterial, 0);

    SEBNPrimitive* leftWallPrimitive = SE_NEW SEBNGeometricPrimitive(
        leftWallMesh, whiteMaterial, 0);

    SEBNPrimitive* rightWallPrimitive = SE_NEW SEBNGeometricPrimitive(
        rightWallMesh, whiteMaterial, 0);

    primitives.push_back(frontWallPrimitive);
    primitives.push_back(backWallPrimitive);
    primitives.push_back(floorPrimitive);
    primitives.push_back(ceilingPrimitive);
    primitives.push_back(leftWallPrimitive);
    primitives.push_back(rightWallPrimitive);
}
//----------------------------------------------------------------------------
void CreateCornellBox(std::vector<SEBNPrimitivePtr>& primitives, 
    float extension, float colorScale)
{
    SE_ASSERT( extension > 0.0f );

    int indexBuffer[6] = { 0, 1, 2, 0, 2, 3 };
    SEVector3f vertexBuffer[4];
    vertexBuffer[0] = SEVector3f(-extension, 0, -extension);
    vertexBuffer[1] = SEVector3f(-extension, 0,  extension);
    vertexBuffer[2] = SEVector3f( extension, 0,  extension);
    vertexBuffer[3] = SEVector3f( extension, 0, -extension);
    float uvBuffer[8] = { 0.0f, 1.0f,
                          0.0f, 0.0f,
                          1.0f, 0.0f,
                          1.0f, 1.0f };

    //int indexBuffer1[3] = { 0, 1, 2 };
    //SEVector3f vertexBuffer1[3];
    //vertexBuffer1[0] = SEVector3f(-extension, 0, -extension);
    //vertexBuffer1[1] = SEVector3f(-extension, 0,  extension);
    //vertexBuffer1[2] = SEVector3f( extension, 0,  extension);
    //float uvBuffer1[6] = { 0.0f, 1.0f,
    //                      0.0f, 0.0f,
    //                      1.0f, 0.0f };

    SETransformation transform;
    SEMatrix3f rotation;

    // Create meshes.
    rotation.FromAxisAngle(SEVector3f(1.0f, 0.0f, 0.0f), -SEMathf::HALF_PI);
    transform.SetRotate(rotation);
    transform.SetTranslate(SEVector3f(0.0f, extension, extension));
    SEBNTriangleMesh* backWallMesh = SE_NEW SEBNTriangleMesh(&transform, false, 
        2, 4, indexBuffer, vertexBuffer, 0, 0, uvBuffer, 0);
    //SEBNTriangleMesh* backWallMesh = SE_NEW SEBNTriangleMesh(&transform, false, 
    //    1, 3, indexBuffer1, vertexBuffer1, 0, 0, uvBuffer1, 0);
    
    transform.SetRotate(SEMatrix3f::IDENTITY);
    transform.SetTranslate(SEVector3f::ZERO);
    SEBNTriangleMesh* floorMesh = SE_NEW SEBNTriangleMesh(&transform, false, 
        2, 4, indexBuffer, vertexBuffer, 0, 0, uvBuffer, 0);

    rotation.FromAxisAngle(SEVector3f(0.0f, 0.0f, 1.0f), -SEMathf::HALF_PI);
    transform.SetRotate(rotation);
    transform.SetTranslate(SEVector3f(-extension, extension, 0.0f));
    SEBNTriangleMesh* leftWallMesh = SE_NEW SEBNTriangleMesh(&transform, false, 
        2, 4, indexBuffer, vertexBuffer, 0, 0, uvBuffer, 0);

    rotation.FromAxisAngle(SEVector3f(0.0f, 0.0f, 1.0f), SEMathf::HALF_PI);
    transform.SetRotate(rotation);
    transform.SetTranslate(SEVector3f(extension, extension, 0.0f));
    SEBNTriangleMesh* rightWallMesh = SE_NEW SEBNTriangleMesh(&transform, false, 
        2, 4, indexBuffer, vertexBuffer, 0, 0, uvBuffer, 0);

    rotation.FromAxisAngle(SEVector3f(0.0f, 0.0f, 1.0f), SEMathf::PI);
    transform.SetRotate(rotation);
    transform.SetTranslate(SEVector3f(0.0f, 2*extension, 0.0f));
    SEBNTriangleMesh* ceilingMesh = SE_NEW SEBNTriangleMesh(&transform, false, 
        2, 4, indexBuffer, vertexBuffer, 0, 0, uvBuffer, 0);

    // Create materials.
    SESpectrum color;
    color[0] = 1.0f * colorScale;
    color[1] = 0.0f;
    color[2] = 0.0f;
    SEBNConstantTexture<SESpectrum>* diffuseRed = 
        SE_NEW SEBNConstantTexture<SESpectrum>(color);
    SEBNConstantTexture<float>* sigmaZero = 
        SE_NEW SEBNConstantTexture<float>(0.0f);
    SEBNMatteMaterial* redMaterial = SE_NEW SEBNMatteMaterial(diffuseRed, 
        sigmaZero, 0);

    color[0] = 1.0f * colorScale;
    color[1] = 1.0f * colorScale;
    color[2] = 1.0f * colorScale;
    SEBNConstantTexture<SESpectrum>* diffuseWhite = 
        SE_NEW SEBNConstantTexture<SESpectrum>(color);
    SEBNMatteMaterial* whiteMaterial = SE_NEW SEBNMatteMaterial(diffuseWhite, 
        sigmaZero, 0);

    color[0] = 0.0f;
    color[1] = 1.0f * colorScale;
    color[2] = 0.0f;
    SEBNConstantTexture<SESpectrum>* diffuseGreen = 
        SE_NEW SEBNConstantTexture<SESpectrum>(color);
    SEBNMatteMaterial* greenMaterial = SE_NEW SEBNMatteMaterial(diffuseGreen, 
        sigmaZero, 0);

    // Create primitives.
    SEBNPrimitive* backWallPrimitive = SE_NEW SEBNGeometricPrimitive(
        backWallMesh, whiteMaterial, 0);

    SEBNPrimitive* floorPrimitive = SE_NEW SEBNGeometricPrimitive(
        floorMesh, whiteMaterial, 0);

    SEBNPrimitive* ceilingPrimitive = SE_NEW SEBNGeometricPrimitive(
        ceilingMesh, whiteMaterial, 0);

    SEBNPrimitive* leftWallPrimitive = SE_NEW SEBNGeometricPrimitive(
        leftWallMesh, redMaterial, 0);

    SEBNPrimitive* rightWallPrimitive = SE_NEW SEBNGeometricPrimitive(
        rightWallMesh, greenMaterial, 0);

    primitives.push_back(backWallPrimitive);
    //primitives.push_back(floorPrimitive);
    //primitives.push_back(ceilingPrimitive);
    //primitives.push_back(leftWallPrimitive);
    //primitives.push_back(rightWallPrimitive);
    primitives.push_back(floorPrimitive);
    primitives.push_back(ceilingPrimitive);
    primitives.push_back(leftWallPrimitive);
    primitives.push_back(rightWallPrimitive);
}
//----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    // Create meshes.
    SEMatrix3f rotation;
    int indexBuffer[6] = { 0, 1, 2, 0, 2, 3 };
    SEVector3f vertexBuffer[4];
    float ext = 2.0f;
    float extScale = 1.0f;
    vertexBuffer[0] = SEVector3f(-extScale*ext, 0, -ext);
    vertexBuffer[1] = SEVector3f(-extScale*ext, 0,  ext);
    vertexBuffer[2] = SEVector3f( extScale*ext, 0,  ext);
    vertexBuffer[3] = SEVector3f( extScale*ext, 0, -ext);
    float uvBuffer[8] = { 0.0f, 1.0f,
                          0.0f, 0.0f,
                          1.0f, 0.0f,
                          1.0f, 1.0f };

    //int indexBuffer1[3] = { 0, 1, 2 };
    //SEVector3f vertexBuffer1[3];
    //vertexBuffer1[0] = SEVector3f(-extScale*ext, 0, -ext);
    //vertexBuffer1[1] = SEVector3f(-extScale*ext, 0,  ext);
    //vertexBuffer1[2] = SEVector3f( extScale*ext, 0,  ext);
    //float uvBuffer1[6] = { 0.0f, 1.0f,
    //                      0.0f, 0.0f,
    //                      1.0f, 0.0f };
    
    SETransformation transform;
    rotation.FromAxisAngle(SEVector3f(1.0f, 0.0f, 0.0f), -SEMathf::PI);
    transform.SetRotate(rotation);
    transform.SetTranslate(SEVector3f(0.0f, 19.98f, 0.0f));
    SEBNTriangleMeshPtr pLightMesh = SE_NEW SEBNTriangleMesh(&transform, false, 
        2, 4, indexBuffer, vertexBuffer, 0, 0, uvBuffer, 0);
    //SEBNTriangleMeshPtr pLightMesh = SE_NEW SEBNTriangleMesh(&transform, false, 
    //    1, 3, indexBuffer1, vertexBuffer1, 0, 0, uvBuffer1, 0);

    // Create light.
    // 224 188 110
    float lightColorScale = 15.0f;
    SESpectrum lightColor;
    lightColor[0] = 0.8784f * lightColorScale;
    lightColor[1] = 0.7372f * lightColorScale;
    lightColor[2] = 0.4313f * lightColorScale;
    SEBNDiffuseAreaLight* pLight = SE_NEW SEBNDiffuseAreaLight(transform, 
        lightColor, 4, pLightMesh);
    std::vector<SEBNLight*> lights;
    lights.push_back(pLight);

    SEBNConstantTexture<SESpectrum>* diffuseWhite = 
        SE_NEW SEBNConstantTexture<SESpectrum>(lightColor);
    SEBNConstantTexture<float>* sigmaZero = 
        SE_NEW SEBNConstantTexture<float>(0.0f);
    SEBNMatteMaterial* whiteMaterial = SE_NEW SEBNMatteMaterial(diffuseWhite, 
        sigmaZero, 0);
    SEBNPrimitive* lightPrimitive = SE_NEW SEBNGeometricPrimitive(
        pLightMesh, whiteMaterial, pLight);

    std::vector<SEBNPrimitivePtr> primitives;
    primitives.push_back(lightPrimitive);

    // Create cornell box.
    CreateCornellBox(primitives, 10.0f, 1.0f);

    // Create scene objects.
    SETransformation worldTransform;
    SEMatrix3f worldRotate(SEVector3f(0.0f, 1.0f, 0.0f), SEMathf::HALF_PI*0.25f);
    worldTransform.SetRotate(worldRotate);
    worldTransform.SetTranslate(SEVector3f(3.5f, 0.0f, -1.0f));
    //worldTransform.SetScale(SEVector3f(1.0f, 2.0f, 1.0f));
    CreateBox(primitives, 3.0f, 1.5f, worldTransform);

    SEMatrix3f worldRotate2(SEVector3f(0.0f, 1.0f, 0.0f), -SEMathf::HALF_PI*0.35f);
    worldTransform.SetRotate(worldRotate2);
    worldTransform.SetTranslate(SEVector3f(-3.5f, 0.0f, 3.0f));
    worldTransform.SetScale(SEVector3f(1.0f, 2.5f, 1.0f));
    CreateBox(primitives, 2.5f, 1.5f, worldTransform);

    // Create scene.
    SEBNKdTreePtr pKdTree = SE_NEW SEBNKdTree(primitives, 80, 1, 0.5, 1);
    SEBNScene* scene = SE_NEW SEBNScene(pKdTree, lights, 0);

    // Create camera.
    float crop[4] = { 0, 1, 0, 1 };
    SEFilter* filter = SE_NEW SEGaussianFilter(1.0f, 1.0f, 2.0f);
    int w = 512, h = 512;
    SEImageFilm* film = SE_NEW SEImageFilm(w, h, filter, crop, "test.tga", false);
    SEBNPerspectiveCamera* camera = SE_NEW SEBNPerspectiveCamera(0.0f, 1.0f, film, 0.0f, 1.0f); 
    camera->SetFrustum(40.0f, 1.0f, 0.01f, 1000.0f);
    camera->SetLocation(SEVector3f(0.0f, 10.0f, -38.0f));

    // Create sampler.
    SEBNSampler* sampler = SE_NEW SEBNStratifiedSampler(0, w, 0, h, 4, 4, true, 0.0f, 1.0f);

    // Create shaders.
    //SEBNSurfaceShader* surfaceShader = SE_NEW SEBNDirectLightingShader();
    //SEBNSurfaceShader* surfaceShader = SE_NEW SEBNPathTracingShader(2);
    SEBNSurfaceShader* surfaceShader = SE_NEW SEBNPhotonMapShader(0, 500000, 64, 1, 6, 1.5f, true, 64, 10);
    SEBNVolumeShader* volumeShader = SE_NEW DebugVolumeShader(0.6f);

    // Create renderer.
    SESamplerRenderer* renderer = SE_NEW SESamplerRenderer(sampler, camera, surfaceShader, volumeShader, false);
    renderer->Render(scene);

    getchar();

    return 0;
}