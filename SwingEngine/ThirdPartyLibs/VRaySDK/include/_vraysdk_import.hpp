/// THIS IS AN INTERNAL IMPLEMENTATION HEADER. DO NOT INCLUDE DIRECTLY!
#if !defined(VRAY_RUNTIME_LOAD_PRIMARY) && !defined(VRAY_RUNTIME_LOAD_SECONDARY)
# define VRAYSDK_FUNC(ret, name, ...) extern ret name(__VA_ARGS__);
#else
# ifdef VRAY_RUNTIME_LOAD_SECONDARY
#   define VRAYSDK_FUNC(ret, name, ...) extern ret (*name)(__VA_ARGS__);
# else
#   define VRAYSDK_FUNC(ret, name, ...) ret (*name)(__VA_ARGS__) = NULL;
# endif
#endif

namespace Internal {

class VRayExportSettings_Internal;

struct CropRegion {
	int srcWidth;
	int srcHeight;
	float rgnLeft;
	float rgnTop;
	float rgnWidth;
	float rgnHeight;
};

struct RectRegion {
	int left, top, width, height;
};

#if !defined(VRAY_RUNTIME_LOAD_PRIMARY) && !defined(VRAY_RUNTIME_LOAD_SECONDARY)
extern "C" {
#endif

typedef void(*OnBucketInit)(int x, int y, int width, int height, const char* host, void* userObject);
typedef void(*OnBucketReady)(int i, int j, int width, int height, const char* host, VRayImage* colorBuffer, void* userObject);
typedef void(*OnRenderStart)(void* userObject);
typedef void(*OnImageReady)(void* userObject);
typedef void(*OnImageUpdated)(int width, int height, VRayImage* colorBuffer, void* userObject);
typedef void(*OnDumpMessage)(const char* message, int level, void* userObject);
typedef void(*OnRendererClose)(void* userObject);
typedef void(*OnSequenceStart)(void* userObject);
typedef void(*OnSequenceDone)(void* userObject);
typedef void(*OnProgress)(const char* message, int progress, int total, void* userObject);
typedef void(*OnRenderViewChanged)(const char* propName, void* userObject);
typedef void(*OnRenderLast)(int flags, void* userObject);
typedef void(*OnVFBClosed)(void* userObject);

VRAYSDK_FUNC(unsigned, VRay_getSDKVersion, void)
VRAYSDK_FUNC(const char*, VRay_getSDKVersionDetails, void)
VRAYSDK_FUNC(unsigned, VRay_getVRayVersion, void)
VRAYSDK_FUNC(const char*, VRay_getVRayVersionDetails, void)
VRAYSDK_FUNC(char*, VRay_getLicenseErrorTextStringFromErrorCode, size_t);
VRAYSDK_FUNC(const char*, VRay_getSimpleLicenseErrorTextStringFromErrorCode, int);
VRAYSDK_FUNC(int, VRay_encodeBRDFScannedParams, int* data, int size, const ScannedMaterialParams* params, size_t vrScansPrmSize, int* licErr);
VRAYSDK_FUNC(int, VRay_LicenseManager_setLicenseServers, const char*);
VRAYSDK_FUNC(const char*, VRay_LicenseManager_getLicenseServers);

// VRayRenderer
VRAYSDK_FUNC(void, VRay_enableFrameBuffer, Bool isEnabled)
VRAYSDK_FUNC(void, VRay_blendFrameBuffer)

VRAYSDK_FUNC(VRayRendererNative*, VRay_VRayRenderer_create, const RendererOptions* options, size_t* licErr)
VRAYSDK_FUNC(void, VRay_VRayRenderer_release, VRayRendererNative* renderer)
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_reset, VRayRendererNative* renderer, const RendererOptions* options)
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_setImprovedDefaults, VRayRendererNative* renderer)

VRAYSDK_FUNC(int, VRay_VRayRenderer_getLastErrorCode, VRayRendererNative* renderer)
VRAYSDK_FUNC(const char*, VRay_getErrorTextStringFromErrorCode, int errCode)

VRAYSDK_FUNC(const char*, VRay_VRayRenderer_getLastParserErrorFileName, VRayRendererNative* renderer)
VRAYSDK_FUNC(int, VRay_VRayRenderer_getLastParserErrorFileLine, VRayRendererNative* renderer)
VRAYSDK_FUNC(const char*, VRay_VRayRenderer_getLastParserErrorText, VRayRendererNative* renderer)

VRAYSDK_FUNC(Bool, VRay_VRayRenderer_setRenderMode, VRayRendererNative* renderer, int renderMode)
VRAYSDK_FUNC(int, VRay_VRayRenderer_getRenderMode, VRayRendererNative* renderer)
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_setRenderOptions, VRayRendererNative* renderer, const RendererOptions* options)
VRAYSDK_FUNC(RendererOptions*, VRay_VRayRenderer_getRenderOptions, VRayRendererNative* renderer)
VRAYSDK_FUNC(void, VRay_VRayRenderer_showFrameBuffer, VRayRendererNative* renderer, Bool show, Bool setFocus)
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_isFrameBufferShown, VRayRendererNative* renderer)
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_getBoundingBox, VRayRendererNative* renderer, Box* bbox)
VRAYSDK_FUNC(int, VRay_VRayRenderer_getState, VRayRendererNative* renderer)
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_pauseRT, VRayRendererNative* renderer, Bool flag) 
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_getPauseRTStatus, VRayRendererNative* renderer)
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_getRTStatistics, VRayRendererNative* renderer, RTStatistics* statistics)
VRAYSDK_FUNC(void, VRay_VRayRenderer_enableVFBInteractivity, VRayRendererNative* renderer, Bool enable)
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_isVFBInteractivityEnabled, VRayRendererNative* renderer)
VRAYSDK_FUNC(void*, VRay_VRayRenderer_getVfbWindowHandle, VRayRendererNative* renderer)
VRAYSDK_FUNC(void, VRay_VRayRenderer_setVfbParentWindowHandle, VRayRendererNative* renderer, void* hwnd)
VRAYSDK_FUNC(void, VRay_VRayRenderer_setVfbPositionAndSize, VRayRendererNative* renderer, void* posAndSize)
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_getVfbPositionAndSize, VRayRendererNative* renderer, void* posAndSize)
VRAYSDK_FUNC(void, VRay_VRayRenderer_setVfbAlwaysOnTop, VRayRendererNative* renderer, Bool flag)
VRAYSDK_FUNC(size_t, VRay_VRayRenderer_getVfbState, VRayRendererNative* renderer, void** buf, int parts)
VRAYSDK_FUNC(void, VRay_VRayRenderer_setVfbState, VRayRendererNative* renderer, const void* buf, size_t size, int parts)
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_loadVfbColorCorrectionsPreset, VRayRendererNative* renderer, const char* filename)
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_saveVfbColorCorrectionsPreset, VRayRendererNative* renderer, const char* filename)
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_setVfbTitlePrefix, VRayRendererNative* renderer, const char* prefix)
VRAYSDK_FUNC(void, VRay_VRayRenderer_setCurrentFrame, VRayRendererNative* renderer, int frame)
VRAYSDK_FUNC(int, VRay_VRayRenderer_getCurrentFrame, VRayRendererNative* renderer) 
VRAYSDK_FUNC(int, VRay_VRayRenderer_getCurrentSequenceFrame, VRayRendererNative* renderer) 
VRAYSDK_FUNC(void, VRay_VRayRenderer_setCurrentTime, VRayRendererNative* renderer, double time)
VRAYSDK_FUNC(double, VRay_VRayRenderer_getCurrentTime, VRayRendererNative* renderer)
VRAYSDK_FUNC(void, VRay_VRayRenderer_setAnimateProperties, VRayRendererNative* renderer, Bool flag)
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_getAnimateProperties, VRayRendererNative* renderer)
VRAYSDK_FUNC(int, VRay_VRayRenderer_clearAllFrameValuesUpToTime, VRayRendererNative* renderer, double time)
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_isPropertyAnimatedByID, VRayRendererNative* renderer, InstanceId pluginID, const char* propertyName)
VRAYSDK_FUNC(int, VRay_VRayRenderer_clientAddHosts, VRayRendererNative* renderer, const char* hosts)
VRAYSDK_FUNC(int, VRay_VRayRenderer_clientRemoveHosts, VRayRendererNative* renderer, const char* hosts)
VRAYSDK_FUNC(int, VRay_VRayRenderer_clientResetHosts, VRayRendererNative* renderer, const char* hosts)
VRAYSDK_FUNC(char*, VRay_VRayRenderer_clientGetAllHosts, VRayRendererNative* renderer, int* size)
VRAYSDK_FUNC(char*, VRay_VRayRenderer_clientGetActiveHosts, VRayRendererNative* renderer, int* size)
VRAYSDK_FUNC(char*, VRay_VRayRenderer_clientGetInactiveHosts, VRayRendererNative* renderer, int* size)
VRAYSDK_FUNC(int, VRay_VRayRenderer_load, VRayRendererNative* renderer, const char* vrsceneFileName, Bool commit)
VRAYSDK_FUNC(int, VRay_VRayRenderer_loadAsText, VRayRendererNative* renderer, const char* sceneText, Bool commit)
VRAYSDK_FUNC(int, VRay_VRayRenderer_append, VRayRendererNative* renderer, const char* vrsceneFileName, Bool commit)
VRAYSDK_FUNC(int, VRay_VRayRenderer_appendAsText, VRayRendererNative* renderer, const char* sceneText, Bool commit)
VRAYSDK_FUNC(void, VRay_VRayRenderer_renderImage, VRayRendererNative* renderer)
VRAYSDK_FUNC(void, VRay_VRayRenderer_render, VRayRendererNative* renderer)
VRAYSDK_FUNC(void, VRay_VRayRenderer_startSync, VRayRendererNative* renderer) 
VRAYSDK_FUNC(void, VRay_VRayRenderer_renderSequence, VRayRendererNative* renderer, SubSequenceDesc subSequences[], size_t subSequencesCount)
VRAYSDK_FUNC(void, VRay_VRayRenderer_continueSequence, VRayRendererNative* renderer)
VRAYSDK_FUNC(int, VRay_VRayRenderer_exportScene, VRayRendererNative* renderer, const char* destinationFilePath, const VRayExportSettings_Internal* settings)
VRAYSDK_FUNC(void, VRay_VRayRenderer_bucketRenderNearest, VRayRendererNative* renderer, int x, int y)
VRAYSDK_FUNC(int, VRay_VRayRenderer_setCurrentCameraByID, VRayRendererNative* renderer, InstanceId id)
VRAYSDK_FUNC(InstanceId, VRay_VRayRenderer_getCurrentCamera, VRayRendererNative* renderer)
VRAYSDK_FUNC(int, VRay_VRayRenderer_setPropertyValueByID, VRayRendererNative* renderer, InstanceId pluginID, const char* propertyName, const char* value, Bool commit)
VRAYSDK_FUNC(int, VRay_VRayRenderer_setPropertyValueAtTimeByID, VRayRendererNative* renderer, InstanceId pluginID, const char* propertyName, const char* value, double time, Bool commit)
VRAYSDK_FUNC(char*, VRay_VRayRenderer_getPropertyValue, VRayRendererNative* renderer, const char* pluginName, const char* propertyName)
VRAYSDK_FUNC(char*, VRay_VRayRenderer_getPropertyValueByID, VRayRendererNative* renderer, InstanceId pluginID, const char* propertyName)
VRAYSDK_FUNC(void*, VRay_VRayRenderer_getPluginNames, VRayRendererNative* renderer)
VRAYSDK_FUNC(void*, VRay_VRayRenderer_getPluginClassNames, VRayRendererNative* renderer)
VRAYSDK_FUNC(const char*, VRay_VRayRenderer_getPluginType, VRayRendererNative* renderer, const char* pluginName)
VRAYSDK_FUNC(const char*, VRay_VRayRenderer_getPluginTypeByID, VRayRendererNative* renderer, InstanceId pluginID)
VRAYSDK_FUNC(Type, VRay_VRayRenderer_getPropertyType, VRayRendererNative* renderer, const char* pluginName, const char* propertyName)
VRAYSDK_FUNC(Type, VRay_VRayRenderer_getPropertyTypeByID, VRayRendererNative* renderer, InstanceId pluginID, const char* propertyName)
VRAYSDK_FUNC(int, VRay_VRayRenderer_getPropertyElementsCountByID, VRayRendererNative* renderer, InstanceId pluginID, const char* propertyName)
VRAYSDK_FUNC(void*, VRay_VRayRenderer_getPluginIDsOfType, VRayRendererNative* renderer, const char* pluginType, int* count)
// VRAYSDK_FUNC(char*, VRay_VRayRenderer_getPropertyDefaultValue, VRayRendererNative* renderer, const char* pluginName, const char* propertyName)
VRAYSDK_FUNC(void*, VRay_VRayRenderer_getPropertyNames, VRayRendererNative* renderer, const char* pluginName)
VRAYSDK_FUNC(void*, VRay_VRayRenderer_getPropertyNamesByID, VRayRendererNative* renderer,  InstanceId pluginID)
VRAYSDK_FUNC(void*, VRay_VRayRenderer_getClassPropertyNames, VRayRendererNative* renderer, const char* pluginClassName)
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_pluginExists, VRayRendererNative* renderer, const char* pluginName)
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_pluginExistsByID, VRayRendererNative* renderer, InstanceId pluginID)
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_pluginClassExists, VRayRendererNative* renderer, const char* pluginClassName)
// VRAYSDK_FUNC(Bool, VRay_VRayRenderer_propertyExists, VRayRendererNative* renderer, const char* pluginName, const char* propertyName)
VRAYSDK_FUNC(InstanceId, VRay_VRayRenderer_getPluginID, VRayRendererNative* renderer, const char* pluginName)
VRAYSDK_FUNC(char*, VRay_VRayRenderer_getPluginName, VRayRendererNative* renderer, InstanceId pluginID)
VRAYSDK_FUNC(void, VRay_VRayRenderer_stop, VRayRendererNative* renderer)
VRAYSDK_FUNC(VRayImage*, VRay_VRayRenderer_getImage, VRayRendererNative* renderer, const GetImageOptions *options)
VRAYSDK_FUNC(int, VRay_VRayRenderer_saveVfbToFile, VRayRendererNative* renderer, const char* fileName, const ImageWriterOptions* options)
VRAYSDK_FUNC(int, VRay_VRayRenderer_getWidth, VRayRendererNative* renderer)
VRAYSDK_FUNC(int, VRay_VRayRenderer_getHeight, VRayRendererNative* renderer)
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_getImageSize, VRayRendererNative* renderer, int* width, int* height)
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_setImageSizeEx, VRayRendererNative* renderer, int width, int height, int resetRegions)
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_getRenderRegion, VRayRendererNative* renderer, RectRegion* renderRegion)
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_getCropRegion, VRayRendererNative* renderer, CropRegion* cropRegion)
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_setRenderRegion, VRayRendererNative* renderer, const RectRegion* renderRegion)
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_setCropRegion, VRayRendererNative* renderer, const CropRegion* cropRegion)
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_isImageReady, VRayRendererNative* renderer)
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_isRendering, VRayRendererNative* renderer)
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_isAborted, VRayRendererNative* renderer)
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_isSequenceDone, VRayRendererNative* renderer)
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_waitImageReady, VRayRendererNative* renderer, int time)
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_waitSequenceDone, VRayRendererNative* renderer, int time)
VRAYSDK_FUNC(const void*, VRay_VRayRenderer_getPropertyDefinition, VRayRendererNative* renderer, const char* pluginName, const char* propertyName)
VRAYSDK_FUNC(const void*, VRay_VRayRenderer_getPropertyDefinitionByID, VRayRendererNative* renderer, InstanceId pluginID, const char* propertyName)
VRAYSDK_FUNC(const void*, VRay_VRayRenderer_getPropertyDefinitionFromClass, VRayRendererNative* renderer, const char* pluginClassName, const char* propertyName)
VRAYSDK_FUNC(int, VRay_VRayRenderer_newPlugin, VRayRendererNative* renderer, const char* pluginName, const char* pluginClassName, Bool commit, InstanceId* pluginID)
VRAYSDK_FUNC(int, VRay_VRayRenderer_deletePlugin, VRayRendererNative* renderer, const char* pluginName, Bool commit)
VRAYSDK_FUNC(int, VRay_VRayRenderer_deletePluginByID, VRayRendererNative* renderer, InstanceId pluginID, Bool commit)
VRAYSDK_FUNC(int, VRay_VRayRenderer_replacePluginByID, VRayRendererNative* renderer, InstanceId oldID, InstanceId newID, Bool commit)

VRAYSDK_FUNC(void, VRay_VRayRenderer_setBucketInitCallback, VRayRendererNative* renderer, OnBucketInit onBucketInit, void* userObject)
VRAYSDK_FUNC(void, VRay_VRayRenderer_setBucketReadyCallback, VRayRendererNative* renderer, OnBucketReady onBucketReady, void* userObject)
VRAYSDK_FUNC(void, VRay_VRayRenderer_setBucketFailedCallback, VRayRendererNative* renderer, OnBucketInit onBucketFailed, void* userObject)
VRAYSDK_FUNC(void, VRay_VRayRenderer_setRenderStartCallback, VRayRendererNative* renderer, OnRenderStart onRenderStart, void* userObject)
VRAYSDK_FUNC(void, VRay_VRayRenderer_setImageUpdatedCallback, VRayRendererNative* renderer, OnImageUpdated onImageUpdated, void* userObject)
VRAYSDK_FUNC(void, VRay_VRayRenderer_setImageReadyCallback, VRayRendererNative* renderer, OnImageReady onImageReady, void* userObject)
VRAYSDK_FUNC(void, VRay_VRayRenderer_setDumpMessageCallback, VRayRendererNative* renderer, OnDumpMessage onDumpMessage, void* userObject)
VRAYSDK_FUNC(void, VRay_VRayRenderer_setRendererCloseCallback, VRayRendererNative* renderer, OnRendererClose onRendererClose, void* userObject)
VRAYSDK_FUNC(void, VRay_VRayRenderer_setSequenceStartCallback, VRayRendererNative* renderer, OnRenderStart onRenderStart, void* userObject)
VRAYSDK_FUNC(void, VRay_VRayRenderer_setSequenceDoneCallback, VRayRendererNative* renderer, OnRenderStart onRenderStart, void* userObject)
VRAYSDK_FUNC(void, VRay_VRayRenderer_setProgressCallback, VRayRendererNative* renderer, OnProgress onProgress, void* userObject)
VRAYSDK_FUNC(void, VRay_VRayRenderer_setRenderViewChangedCallback, VRayRendererNative* renderer, OnRenderViewChanged onRenderViewChanged, void* userObject)
VRAYSDK_FUNC(void, VRay_VRayRenderer_setRenderLastCallback, VRayRendererNative* renderer, OnRenderLast onRenderLast, void* userObject)
VRAYSDK_FUNC(void, VRay_VRayRenderer_setVFBClosedCallback, VRayRendererNative* renderer, OnVFBClosed onVFBClosed, void* userObject)

VRAYSDK_FUNC(float, VRay_VRayRenderer_setRTImageUpdateDifference, VRayRendererNative* renderer, float difference)
VRAYSDK_FUNC(unsigned long, VRay_VRayRenderer_setRTImageUpdateTimeout, VRayRendererNative* renderer, unsigned long timeout)
VRAYSDK_FUNC(void, VRay_VRayRenderer_setKeepRTframesInCallback, VRayRendererNative* renderer, Bool keep)
VRAYSDK_FUNC(void, VRay_VRayRenderer_setKeepBucketsInCallback, VRayRendererNative* renderer, Bool keep)

//VRAYSDK_FUNC(void*, VRay_VRayRenderer_setUserData, VRayRendererNative* renderer, int n, void* data)
//VRAYSDK_FUNC(void*, VRay_VRayRenderer_getUserData, VRayRendererNative* renderer, int n)
//VRAYSDK_FUNC(size_t, VRay_getUserDataCount, void)

VRAYSDK_FUNC(int, VRay_VRayRenderer_commit, VRayRendererNative* renderer, Bool force)
VRAYSDK_FUNC(void, VRay_getBlackBodyColorRGB, Color* color, float temperature, int colorSpace)

// VRayRenderer get specific parameters values
VRAYSDK_FUNC(Bool, VRay_getPropertyInt, VRayRendererNative* renderer, const char* pluginName, const char* propertyName, int* pInt)
VRAYSDK_FUNC(Bool, VRay_getPropertyIntByID, VRayRendererNative* renderer, InstanceId pluginID, const char* propertyName, int* pInt)
VRAYSDK_FUNC(Bool, VRay_getPropertyFloat, VRayRendererNative* renderer, const char* pluginName, const char* propertyName, float* pFloat)
VRAYSDK_FUNC(Bool, VRay_getPropertyFloatByID, VRayRendererNative* renderer, InstanceId pluginID, const char* propertyName, float* pFloat)
VRAYSDK_FUNC(Bool, VRay_getPropertyBool, VRayRendererNative* renderer, const char* pluginName, const char* propertyName, Bool* pBool)
VRAYSDK_FUNC(Bool, VRay_getPropertyBoolByID, VRayRendererNative* renderer, InstanceId pluginID, const char* propertyName, Bool* pBool)
VRAYSDK_FUNC(Color*, VRay_getPropertyColor, VRayRendererNative* renderer, const char* pluginName, const char* propertyName)
VRAYSDK_FUNC(Color*, VRay_getPropertyColorByID, VRayRendererNative* renderer, InstanceId pluginID, const char* propertyName)
VRAYSDK_FUNC(AColor*, VRay_getPropertyAColor, VRayRendererNative* renderer, const char* pluginName, const char* propertyName)
VRAYSDK_FUNC(AColor*, VRay_getPropertyAColorByID, VRayRendererNative* renderer, InstanceId pluginID, const char* propertyName)
VRAYSDK_FUNC(Vector*, VRay_getPropertyVector, VRayRendererNative* renderer, const char* pluginName, const char* propertyName)
VRAYSDK_FUNC(Vector*, VRay_getPropertyVectorByID, VRayRendererNative* renderer, InstanceId pluginID, const char* propertyName)
VRAYSDK_FUNC(Matrix*, VRay_getPropertyMatrix, VRayRendererNative* renderer, const char* pluginName, const char* propertyName)
VRAYSDK_FUNC(Matrix*, VRay_getPropertyMatrixByID, VRayRendererNative* renderer, InstanceId pluginID, const char* propertyName)
VRAYSDK_FUNC(Transform*, VRay_getPropertyTransform, VRayRendererNative* renderer, const char* pluginName, const char* propertyName)
VRAYSDK_FUNC(Transform*, VRay_getPropertyTransformByID, VRayRendererNative* renderer, InstanceId pluginID, const char* propertyName)
VRAYSDK_FUNC(Bool, VRay_getPropertyObjectByID, VRayRendererNative* renderer, InstanceId pluginID, const char* propertyName, InstanceId* idPtr, const char** subparam)

VRAYSDK_FUNC(void, VRay_getPropertyByteArrayByID, VRayRendererNative* renderer,  InstanceId pluginID, const char* propertyName, int** byteArray, int* size)
VRAYSDK_FUNC(int, VRay_VRayRenderer_setPropertyBytesValueByID, VRayRendererNative* renderer, InstanceId pluginID, const char* propertyName, const void* value, Bool commit)
VRAYSDK_FUNC(int, VRay_VRayRenderer_setPropertyBytesValueAtTimeByID, VRayRendererNative* renderer, InstanceId pluginID, const char* propertyName, const void* value, double time, Bool commit)
VRAYSDK_FUNC(int, VRay_VRayRenderer_setPropertyBytesValueRawByID, VRayRendererNative* renderer, InstanceId pluginID, const char* parameterName, const void* value, size_t count, Bool commit)
VRAYSDK_FUNC(int, VRay_VRayRenderer_setPropertyBytesValueRawAtTimeByID, VRayRendererNative* renderer, InstanceId pluginID, const char* parameterName, const void* value, size_t count, double time, Bool commit)

// Pick Plugin by coordinates
VRAYSDK_FUNC(InstanceId, VRay_VRayRenderer_pickPluginByXY, VRayRendererNative* renderer, int x, int y, int timeout)
VRAYSDK_FUNC(int, VRay_VRayRenderer_pickPluginsByXY, VRayRendererNative* renderer, double x, double y, int timeout, int maxcount, InstanceId** pluginsArray)
	
// Proxy
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_readProxyPreview, VRayRendererNative* renderer, int type, InstanceId proxyPluginId, void **primary, void **secondary, size_t *primCount, size_t *secCount, int countHint)
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_readProxyFullPrepare, VRayRendererNative* renderer, InstanceId proxyPluginId, void *proxyData, void **handle)
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_readProxyFullFinalize, VRayRendererNative* renderer, void *proxyData, void *handle)
VRAYSDK_FUNC(int, VRay_VRayRenderer_createProxyFile, VRayRendererNative* renderer, int type, int count, const InstanceId *geomPluginIds, const float *transforms, const void *params)

// Saving GI maps post-render
VRAYSDK_FUNC(size_t, VRay_VRayRenderer_getIrradianceMapSize, VRayRendererNative* renderer)
VRAYSDK_FUNC(size_t, VRay_VRayRenderer_getLightCacheSize, VRayRendererNative* renderer)
VRAYSDK_FUNC(size_t, VRay_VRayRenderer_getPhotonMapSize, VRayRendererNative* renderer)
VRAYSDK_FUNC(size_t, VRay_VRayRenderer_getCausticsSize, VRayRendererNative* renderer)
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_saveIrradianceMapFile, VRayRendererNative* renderer, const char* fileName)
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_saveLightCacheFile, VRayRendererNative* renderer, const char* fileName)
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_savePhotonMapFile, VRayRendererNative* renderer, const char* fileName)
VRAYSDK_FUNC(Bool, VRay_VRayRenderer_saveCausticsFile, VRayRendererNative* renderer, const char* fileName)

// Compute devices
VRAYSDK_FUNC(int, VRay_VRayRenderer_getDeviceListCUDA, VRayRendererNative* renderer, void **data)
VRAYSDK_FUNC(int, VRay_VRayRenderer_getDeviceListOpenCL, VRayRendererNative* renderer, void **data)
VRAYSDK_FUNC(int, VRay_VRayRenderer_setDeviceListCUDA, VRayRendererNative* renderer, const int *indices, size_t count)
VRAYSDK_FUNC(int, VRay_VRayRenderer_setDeviceListOpenCL, VRayRendererNative* renderer, const int *indices, size_t count)

// LicenseManager
VRAYSDK_FUNC(void, VRay_LicenseManager_releaseLicense, void)

// std_vector utils
VRAYSDK_FUNC(const char*, getVectorElement, void* elements, int index)
VRAYSDK_FUNC(unsigned, getVectorSize, void* elements)
VRAYSDK_FUNC(void, releaseVector, void* elements)

// Plugin utils
VRAYSDK_FUNC(char*, VRay_PluginUtils_getPropertyDefaultValue, const void* paramDef)
VRAYSDK_FUNC(const char*, VRay_PluginUtils_getPropertyDescription, const void* paramDef)
VRAYSDK_FUNC(const char*, VRay_PluginUtils_getPropertyUIGuides, const void* paramDef)
VRAYSDK_FUNC(const char*, VRay_PluginUtils_getPropertyName, const void* paramDef)
VRAYSDK_FUNC(Type, VRay_PluginUtils_getPropertyType, const void* paramDef)
VRAYSDK_FUNC(int, VRay_PluginUtils_getPropertyElementsCount, const void* paramDef)

// JPEG, PNG compression (renderer may be NULL)
VRAYSDK_FUNC(size_t, VRayImage_compressToJpeg, const VRayImage* img, int quality, byte** outAddr, VRayRendererNative* renderer) // if quality is negative, abs(quality) is used and progressive encoding is performed
VRAYSDK_FUNC(size_t, VRayImage_compressToPng, const VRayImage* img, Bool preserveAlpha, byte** outAddr, VRayRendererNative* renderer)
VRAYSDK_FUNC(size_t, VRayImage_convertToBmp, const VRayImage* img, Bool preserveAlpha, byte** outAddr, VRayRendererNative* renderer, int channelOrder)
VRAYSDK_FUNC(Bool, VRayImage_compressToJpegFile, const VRayImage* img, int quality, const char* fileName, VRayRendererNative* renderer)
VRAYSDK_FUNC(Bool, VRayImage_compressToPngFile, const VRayImage* img, Bool preserveAlpha, const char* fileName, VRayRendererNative* renderer)
VRAYSDK_FUNC(Bool, VRayImage_SaveToBmpFile, const VRayImage* img, Bool preserveAlpha, const char* fileName, VRayRendererNative* renderer, int channelOrder)

VRAYSDK_FUNC(size_t, VRayImage_toBitmapData, const VRayImage* img, Bool preserveAlpha, byte** outAddr, int channelOrder, int stride)

// VRayImage methods
VRAYSDK_FUNC(VRayImage*, VRayImage_crop, const VRayImage* img, int x, int y, int width, int height)
VRAYSDK_FUNC(VRayImage*, VRayImage_downscale, const VRayImage* img, int width, int height)
VRAYSDK_FUNC(VRayImage*, VRayImage_fitIn, const VRayImage* img, int width, int height)
VRAYSDK_FUNC(VRayImage*, VRayImage_fitOut, const VRayImage* img, int width, int height)
VRAYSDK_FUNC(VRayImage*, VRayImage_cutIn, const VRayImage* img, int width, int height)
VRAYSDK_FUNC(VRayImage*, VRayImage_crop_downscale, const VRayImage* img, int x, int y, int srcWidth, int srcHeight, int dstWidth, int dstHeight)
VRAYSDK_FUNC(VRayImage*, VRayImage_clone, const VRayImage* img)
VRAYSDK_FUNC(VRayImage*, VRayImage_create, int width, int height)
VRAYSDK_FUNC(Bool, VRayImage_getSize, const VRayImage* img, int* width, int* height)
VRAYSDK_FUNC(size_t, VRayImage_getMemorySize, const VRayImage* img)
VRAYSDK_FUNC(void*, VRayImage_getPixelDataPointerAndPixelCount, const VRayImage* img, size_t* size)
VRAYSDK_FUNC(Bool, VRayImage_drawImage, VRayImage* img0, const VRayImage* img1, int x, int y)
VRAYSDK_FUNC(Bool, VRayImage_addColor, VRayImage* img, const AColor* color)
VRAYSDK_FUNC(Bool, VRayImage_changeExposure, VRayImage* img, float ev)
VRAYSDK_FUNC(Bool, VRayImage_changeGamma, VRayImage* img, float gamma)
VRAYSDK_FUNC(Bool, VRayImage_changeContrast, VRayImage* img, float contrast)
VRAYSDK_FUNC(Bool, VRayImage_makeNegative, VRayImage* img)

// Render Elements
VRAYSDK_FUNC(int, VRay_newRenderElementPlugin, VRayRendererNative* renderer, int type, const char *instanceName, const char *displayName, InstanceId *instanceID)
VRAYSDK_FUNC(InstanceId, VRay_getRenderElementPlugin, VRayRendererNative* renderer, int type)
VRAYSDK_FUNC(InstanceId*, VRay_getRenderElementPluginsList, VRayRendererNative* renderer, int type, int *count)
VRAYSDK_FUNC(Bool, VRay_getRenderElementInfo, VRayRendererNative* renderer, InstanceId pluginID, RenderElement::Info* chInfo)
VRAYSDK_FUNC(size_t, VRay_getRenderElementData, VRayRendererNative* renderer, InstanceId pluginID, InstanceId alphaPluginID, int format, int rgbOrder, const ImageRegion* rgn, void** buf)
VRAYSDK_FUNC(VRayImage*, VRay_getRenderElementAsImage, VRayRendererNative* renderer, InstanceId pluginID, const ImageRegion* rgn)

// Utility
VRAYSDK_FUNC(void, C_memory_free, void* memory)
VRAYSDK_FUNC(void*, C_memory_alloc, size_t size)

// V-Ray SDK interoperability layer
#ifdef VRAY_SDK_INTEROPERABILITY
// RefCounter
VRAYSDK_FUNC(void*, RefCounter_create, int type)
VRAYSDK_FUNC(void , RefCounter_destroy, void* self, int type, void* ptr)
VRAYSDK_FUNC(void , RefCounter_increment, void* self, int type)
VRAYSDK_FUNC(int  , RefCounter_decrement, void* self, int type)
VRAYSDK_FUNC(void*, RefCounter_allocMem, void* self, int type, int n)

// CharStringBuf
VRAYSDK_FUNC(void*, CharStringBuf_create, int size)
VRAYSDK_FUNC(char*, CharStringBuf_resize, void* self, int s)
VRAYSDK_FUNC(void , CharStringBuf_incRefCount, void* self)
VRAYSDK_FUNC(void , CharStringBuf_decRefCount, void* self)
VRAYSDK_FUNC(int  , CharStringBuf_getRefCount, void* self)

// Additional Plugin property setters (V-Ray SDK interoperability layer)
VRAYSDK_FUNC(int, VRay_VRayRenderer_setPropertyValueByID_interop, VRayRendererNative* renderer, InstanceId pluginID, const char* parameterName, const void* value, int type, Bool commit)
VRAYSDK_FUNC(int, VRay_VRayRenderer_setPropertyValueAtTimeByID_interop, VRayRendererNative* renderer, InstanceId pluginID, const char* parameterName, const void* value, int type, double time, Bool commit)
#endif // VRAY_SDK_INTEROPERABILITY

#if !defined(VRAY_RUNTIME_LOAD_PRIMARY) && !defined(VRAY_RUNTIME_LOAD_SECONDARY)
} // extern "C"
#endif
} // namespace Internal
