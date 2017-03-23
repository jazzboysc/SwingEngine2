/// THIS IS AN INTERNAL IMPLEMENTATION HEADER. DO NOT INCLUDE DIRECTLY!

#ifdef VRAY_RUNTIME_LOAD_PRIMARY

namespace {
	std::string getLoaderError(const char *msg) {
		char errBuf[1024];
#ifdef _WIN32
		DWORD errorMessageID = GetLastError();
		LPSTR errAsString = NULL;
		FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					   NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR) &errAsString, 0, NULL);
		size_t len = errAsString ? strlen(errAsString) : 0;
		while (len--) {
			if (errAsString[len] != '\r' && errAsString[len] != '\n')
				break;
			errAsString[len] = 0;
		}
		if (errAsString) {
			sprintf_s(errBuf, 1024, "%s [%s]", msg, errAsString);
			LocalFree(errAsString);
		} else {
			sprintf_s(errBuf, 1024, "%s [%u]", msg, errorMessageID);
		}
#else
#   if __cplusplus >= 201103L
		snprintf(errBuf, 1024, "%s [%s]", msg, dlerror());
#   else
		sprintf(errBuf, "%s [%s]", msg, dlerror());
#   endif
#endif
		return errBuf;
	}
} // anon

#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(disable: 4706) // "assignment within conditional expression" - for VRayInit::initialize
#endif // _MSC_VER

void VRay::VRayInit::initialize(const char* libraryFileName) {
	if (libraryFileName == NULL) {
#	ifdef __APPLE__
		libraryFileName = "libVRaySDKLibrary.dylib";
#	elif defined(__linux__)
		libraryFileName = "libVRaySDKLibrary.so";
#	else // WIN32
		libraryFileName = "VRaySDKLibrary.dll";
#	endif
	}

#	undef GET_PROC_ADDR
#	undef PROC_TYPE
#	ifdef _WIN32
	hLib = ::LoadLibraryA(libraryFileName);
#		define GET_PROC_ADDR GetProcAddress
#		define PROC_TYPE (FARPROC&)
#	else
	hLib = ::dlopen(libraryFileName, RTLD_LAZY);
#		define GET_PROC_ADDR dlsym
#		define PROC_TYPE (void*&)
#	endif // _WIN32

#	ifdef VRAY_NOTHROW
#		define PROC_IMPORT_FAIL(message) return makeError(getLoaderError(message))
#	else
#		define PROC_IMPORT_FAIL(message) throw InitErr( (errString = getLoaderError(message)) )
#	endif

	if (!hLib)
		PROC_IMPORT_FAIL("Cannot load V-Ray SDK Library file.");

	if (!(PROC_TYPE Internal::VRay_getSDKVersion = GET_PROC_ADDR(hLib, "VRay_getSDKVersion")))
		PROC_IMPORT_FAIL("Cannot find VRay_getSDKVersion in library.");
	if (!(PROC_TYPE Internal::VRay_getSDKVersionDetails = GET_PROC_ADDR(hLib, "VRay_getSDKVersionDetails")))
		PROC_IMPORT_FAIL("Cannot find VRay_getSDKVersionDetails in library.");
	if (!(PROC_TYPE Internal::VRay_getVRayVersion = GET_PROC_ADDR(hLib, "VRay_getVRayVersion")))
		PROC_IMPORT_FAIL("Cannot find VRay_getVRayVersion in library.");
	if (!(PROC_TYPE Internal::VRay_getVRayVersionDetails = GET_PROC_ADDR(hLib, "VRay_getVRayVersionDetails")))
		PROC_IMPORT_FAIL("Cannot find VRay_getVRayVersionDetails in library.");
	if (!(PROC_TYPE Internal::VRay_LicenseManager_setLicenseServers = GET_PROC_ADDR(hLib, "VRay_LicenseManager_setLicenseServers")))
		PROC_IMPORT_FAIL("Cannot find VRay_LicenseManager_setLicenseServers in library.");
	if (!(PROC_TYPE Internal::VRay_LicenseManager_getLicenseServers = GET_PROC_ADDR(hLib, "VRay_LicenseManager_getLicenseServers")))
		PROC_IMPORT_FAIL("Cannot find VRay_LicenseManager_getLicenseServers in library.");

	if (!(PROC_TYPE Internal::VRay_getLicenseErrorTextStringFromErrorCode = GET_PROC_ADDR(hLib, "VRay_getLicenseErrorTextStringFromErrorCode")))
		PROC_IMPORT_FAIL("Cannot find VRay_getLicenseErrorTextStringFromErrorCode in library.");

	if (!(PROC_TYPE Internal::VRay_getSimpleLicenseErrorTextStringFromErrorCode = GET_PROC_ADDR(hLib, "VRay_getSimpleLicenseErrorTextStringFromErrorCode")))
		PROC_IMPORT_FAIL("Cannot find VRay_getSimpleLicenseErrorTextStringFromErrorCode in library.");

	if (!(PROC_TYPE Internal::VRay_encodeBRDFScannedParams = GET_PROC_ADDR(hLib, "VRay_encodeBRDFScannedParams")))
		PROC_IMPORT_FAIL("Cannot find VRay_encodeBRDFScannedParams in library.");

	// VRayRenderer
	if (!(PROC_TYPE Internal::VRay_enableFrameBuffer = GET_PROC_ADDR(hLib, "VRay_enableFrameBuffer")))
		PROC_IMPORT_FAIL("Cannot find VRay_enableFrameBuffer in library.");

	if (!(PROC_TYPE Internal::VRay_blendFrameBuffer = GET_PROC_ADDR(hLib, "VRay_blendFrameBuffer")))
		PROC_IMPORT_FAIL("Cannot find VRay_blendFrameBuffer in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_create = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_create")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_create in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_reset = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_reset")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_reset in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setImprovedDefaults = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setImprovedDefaults")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setImprovedDefaults in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_release = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_release")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_release in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getLastErrorCode = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getLastErrorCode")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getLastErrorCode in library.");

	if (!(PROC_TYPE Internal::VRay_getErrorTextStringFromErrorCode = GET_PROC_ADDR(hLib, "VRay_getErrorTextStringFromErrorCode")))
		PROC_IMPORT_FAIL("Cannot find VRay_getErrorTextStringFromErrorCode in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getLastParserErrorFileName = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getLastParserErrorFileName")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getLastParserErrorFileName in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getLastParserErrorFileLine = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getLastParserErrorFileLine")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getLastParserErrorFileLine in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getLastParserErrorText = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getLastParserErrorText")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getLastParserErrorText in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setRenderMode = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setRenderMode")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setRenderMode in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getRenderMode = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getRenderMode")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getRenderMode in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setRenderOptions = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setRenderOptions")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setRenderOptions in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getRenderOptions = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getRenderOptions")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getRenderOptions in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_showFrameBuffer = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_showFrameBuffer")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_showFrameBuffer in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_isFrameBufferShown = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_isFrameBufferShown")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_isFrameBufferShown in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getBoundingBox = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getBoundingBox")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getBoundingBox in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getState = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getState")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getState in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_pauseRT = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_pauseRT")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_pauseRT in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getPauseRTStatus = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getPauseRTStatus")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getPauseRTStatus in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getRTStatistics = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getRTStatistics")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getRTStatistics in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_enableVFBInteractivity = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_enableVFBInteractivity")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_enableVFBInteractivity in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_isVFBInteractivityEnabled = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_isVFBInteractivityEnabled")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_isVFBInteractivityEnabled in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getVfbWindowHandle = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getVfbWindowHandle")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getVfbWindowHandle in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setVfbParentWindowHandle = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setVfbParentWindowHandle")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setVfbParentWindowHandle in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setVfbPositionAndSize = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setVfbPositionAndSize")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setVfbPositionAndSize in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getVfbPositionAndSize = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getVfbPositionAndSize")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getVfbPositionAndSize in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setVfbAlwaysOnTop = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setVfbAlwaysOnTop")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setVfbAlwaysOnTop in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getVfbState = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getVfbState")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getVfbState in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setVfbState = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setVfbState")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setVfbState in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_loadVfbColorCorrectionsPreset = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_loadVfbColorCorrectionsPreset")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_loadVfbColorCorrectionsPreset in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_saveVfbColorCorrectionsPreset = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_saveVfbColorCorrectionsPreset")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_saveVfbColorCorrectionsPreset in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setVfbTitlePrefix = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setVfbTitlePrefix")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setVfbTitlePrefix in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setCurrentFrame = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setCurrentFrame")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setCurrentFrame in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getCurrentFrame = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getCurrentFrame")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getCurrentFrame in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getCurrentSequenceFrame = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getCurrentSequenceFrame")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getCurrentSequenceFrame in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setCurrentTime = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setCurrentTime")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setCurrentTime in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getCurrentTime = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getCurrentTime")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getCurrentTime in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setAnimateProperties = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setAnimateProperties")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setAnimateProperties in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getAnimateProperties = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getAnimateProperties")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getAnimateProperties in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_clearAllFrameValuesUpToTime = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_clearAllFrameValuesUpToTime")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_clearAllFrameValuesUpToTime in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_isPropertyAnimatedByID = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_isPropertyAnimatedByID")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_isPropertyAnimatedByID in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_clientAddHosts = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_clientAddHosts")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_clientAddHosts in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_clientRemoveHosts = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_clientRemoveHosts")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_clientRemoveHosts in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_clientResetHosts = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_clientResetHosts")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_clientResetHosts in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_clientGetAllHosts = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_clientGetAllHosts")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_clientGetAllHosts in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_clientGetActiveHosts = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_clientGetActiveHosts")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_clientGetActiveHosts in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_clientGetInactiveHosts = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_clientGetInactiveHosts")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_clientGetInactiveHosts in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_load = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_load")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_load in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_loadAsText = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_loadAsText")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_loadAsText in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_append = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_append")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_append in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_appendAsText = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_appendAsText")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_appendAsText in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_renderImage = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_renderImage")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_renderImage in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_render = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_render")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_render in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_startSync = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_startSync")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_startSync in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_renderSequence = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_renderSequence")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_renderSequence in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_continueSequence = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_continueSequence")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_continueSequence in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_exportScene = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_exportScene")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_exportScene in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_bucketRenderNearest = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_bucketRenderNearest")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_bucketRenderNearest in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setCurrentCameraByID = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setCurrentCameraByID")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setCurrentCameraByID in library.");			

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getCurrentCamera = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setCurrentCamera")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setCurrentCamera in library.");			

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setPropertyValueByID = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setPropertyValueByID")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setPropertyValueByID in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setPropertyValueAtTimeByID = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setPropertyValueAtTimeByID")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setPropertyValueAtTimeByID in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getPropertyValue = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getPropertyValue")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getPropertyValue in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getPropertyValueByID = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getPropertyValueByID")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getPropertyValueByID in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getPluginNames = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getPluginNames")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getPluginNames in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getPluginClassNames = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getPluginClassNames")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getPluginClassNames in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getPluginType = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getPluginType")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getPluginType in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getPluginTypeByID = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getPluginTypeByID")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getPluginTypeByID in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getPropertyType = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getPropertyType")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getPropertyType in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getPropertyTypeByID = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getPropertyTypeByID")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getPropertyTypeByID in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getPropertyElementsCountByID = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getPropertyElementsCountByID")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getPropertyElementsCountByID in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getPluginIDsOfType = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getPluginIDsOfType")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getPluginIDsOfType in library.");

//	if (!(Internal::PROC_TYPE VRay_VRayRenderer_getPropertyDefaultValue = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getPropertyDefaultValue")))
//		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getPropertyDefaultValue in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getPropertyNames = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getPropertyNames")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getPropertyNames in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getPropertyNamesByID = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getPropertyNamesByID")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getPropertyNamesByID in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getClassPropertyNames = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getClassPropertyNames")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getClassPropertyNames in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_pluginExists = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_pluginExists")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_pluginExists in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_pluginExistsByID = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_pluginExistsByID")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_pluginExistsByID in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_pluginClassExists = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_pluginClassExists")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_pluginClassExists in library.");

//	if (!(PROC_TYPE Internal::VRay_VRayRenderer_propertyExists = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_propertyExists")))
//		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_propertyExists in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getPluginID = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getPluginID")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getPluginID in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getPluginName = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getPluginName")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getPluginName in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_stop = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_stop")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_stop in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getImage = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getImage")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getImage in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_saveVfbToFile = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_saveVfbToFile")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_saveVfbToFile in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getWidth = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getWidth")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getWidth in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getHeight = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getHeight")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getHeight in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getImageSize = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getImageSize")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getImageSize in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setImageSizeEx = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setImageSizeEx")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setImageSizeEx in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setRenderRegion = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setRenderRegion")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setRenderRegion in library.");
		
	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getRenderRegion = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getRenderRegion")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getRenderRegion in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setCropRegion = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setCropRegion")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setCropRegion in library.");
		
	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getCropRegion = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getCropRegion")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getCropRegion in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_isImageReady = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_isImageReady")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_isImageReady in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_isRendering = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_isRendering")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_isRendering in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_isAborted = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_isAborted")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_isAborted in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_isSequenceDone = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_isSequenceDone")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_isSequenceDone in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_waitImageReady = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_waitImageReady")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_waitImageReady in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_waitSequenceDone = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_waitSequenceDone")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_waitSequenceDone in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getPropertyDefinition = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getPropertyDefinition")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getPropertyDefinition in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getPropertyDefinitionFromClass = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getPropertyDefinitionFromClass")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getPropertyDefinitionFromClass in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_newPlugin = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_newPlugin")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_newPlugin in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_deletePlugin = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_deletePlugin")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_deletePlugin in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_deletePluginByID = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_deletePluginByID")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_deletePluginByID in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_replacePluginByID = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_replacePluginByID")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_replacePluginByID in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setBucketInitCallback = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setBucketInitCallback")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setBucketInitCallback in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setBucketReadyCallback = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setBucketReadyCallback")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setBucketReadyCallback in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setBucketFailedCallback = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setBucketFailedCallback")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setBucketFailedCallback in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setRenderStartCallback = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setRenderStartCallback")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setRenderStartCallback in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setImageUpdatedCallback = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setImageUpdatedCallback")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setImageUpdatedCallback in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setImageReadyCallback = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setImageReadyCallback")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setImageReadyCallback in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setDumpMessageCallback = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setDumpMessageCallback")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setDumpMessageCallback in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setRendererCloseCallback = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setRendererCloseCallback")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setRendererCloseCallback in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setSequenceStartCallback = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setSequenceStartCallback")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setSequenceStartCallback in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setSequenceDoneCallback = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setSequenceDoneCallback")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setSequenceDoneCallback in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setProgressCallback = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setProgressCallback")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setProgressCallback in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setRenderViewChangedCallback = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setRenderViewChangedCallback")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setRenderViewChangedCallback in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setRenderLastCallback = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setRenderLastCallback")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setRenderLastCallback in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setVFBClosedCallback = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setVFBClosedCallback")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setVFBClosedCallback in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setRTImageUpdateDifference = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setRTImageUpdateDifference")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setRTImageUpdateDifference in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setRTImageUpdateTimeout = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setRTImageUpdateTimeout")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setRTImageUpdateTimeout in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setKeepRTframesInCallback = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setKeepRTframesInCallback")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setKeepRTframesInCallback in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setKeepBucketsInCallback = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setKeepBucketsInCallback")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setKeepBucketsInCallback in library.");

	//if (!(PROC_TYPE Internal::VRay_VRayRenderer_setUserData = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setUserData")))
	//	PROC_IMPORT_FAIL("Cannot find VRay_setUserData in library.");

	//if (!(PROC_TYPE Internal::VRay_VRayRenderer_getUserData = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getUserData")))
	//	PROC_IMPORT_FAIL("Cannot find VRay_getUserData in library.");

	//if (!(PROC_TYPE Internal::VRay_getUserDataCount = GET_PROC_ADDR(hLib, "VRay_getUserDataCount")))
	//	PROC_IMPORT_FAIL("Cannot find VRay_getUserDataCount in library.");

	if (!(PROC_TYPE Internal::VRay_getBlackBodyColorRGB = GET_PROC_ADDR(hLib, "VRay_getBlackBodyColorRGB")))
		PROC_IMPORT_FAIL("Cannot find VRay_getBlackBodyColorRGB in library.");

	// VRayRenderer get specific parameters values
	if (!(PROC_TYPE Internal::VRay_getPropertyInt = GET_PROC_ADDR(hLib, "VRay_getPropertyInt")))
		PROC_IMPORT_FAIL("Cannot find VRay_getPropertyInt in library.");

	if (!(PROC_TYPE Internal::VRay_getPropertyIntByID = GET_PROC_ADDR(hLib, "VRay_getPropertyIntByID")))
		PROC_IMPORT_FAIL("Cannot find VRay_getPropertyIntByID in library.");

	if (!(PROC_TYPE Internal::VRay_getPropertyFloat = GET_PROC_ADDR(hLib, "VRay_getPropertyFloat")))
		PROC_IMPORT_FAIL("Cannot find VRay_getPropertyFloat in library.");

	if (!(PROC_TYPE Internal::VRay_getPropertyFloatByID = GET_PROC_ADDR(hLib, "VRay_getPropertyFloatByID")))
		PROC_IMPORT_FAIL("Cannot find VRay_getPropertyFloatByID in library.");

	if (!(PROC_TYPE Internal::VRay_getPropertyBool = GET_PROC_ADDR(hLib, "VRay_getPropertyBool")))
		PROC_IMPORT_FAIL("Cannot find VRay_getPropertyBool in library.");

	if (!(PROC_TYPE Internal::VRay_getPropertyBoolByID = GET_PROC_ADDR(hLib, "VRay_getPropertyBoolByID")))
		PROC_IMPORT_FAIL("Cannot find VRay_getPropertyBoolByID in library.");

	if (!(PROC_TYPE Internal::VRay_getPropertyColor = GET_PROC_ADDR(hLib, "VRay_getPropertyColor")))
		PROC_IMPORT_FAIL("Cannot find VRay_getPropertyColor in library.");

	if (!(PROC_TYPE Internal::VRay_getPropertyColorByID = GET_PROC_ADDR(hLib, "VRay_getPropertyColorByID")))
		PROC_IMPORT_FAIL("Cannot find VRay_getPropertyColorByID in library.");

	if (!(PROC_TYPE Internal::VRay_getPropertyAColor = GET_PROC_ADDR(hLib, "VRay_getPropertyAColor")))
		PROC_IMPORT_FAIL("Cannot find VRay_getPropertyAColor in library.");

	if (!(PROC_TYPE Internal::VRay_getPropertyAColorByID = GET_PROC_ADDR(hLib, "VRay_getPropertyAColorByID")))
		PROC_IMPORT_FAIL("Cannot find VRay_getPropertyAColorByID in library.");

	if (!(PROC_TYPE Internal::VRay_getPropertyVector = GET_PROC_ADDR(hLib, "VRay_getPropertyVector")))
		PROC_IMPORT_FAIL("Cannot find VRay_getPropertyVector in library.");

	if (!(PROC_TYPE Internal::VRay_getPropertyVectorByID = GET_PROC_ADDR(hLib, "VRay_getPropertyVectorByID")))
		PROC_IMPORT_FAIL("Cannot find VRay_getPropertyVectorByID in library.");

	if (!(PROC_TYPE Internal::VRay_getPropertyMatrix = GET_PROC_ADDR(hLib, "VRay_getPropertyMatrix")))
		PROC_IMPORT_FAIL("Cannot find VRay_getPropertyMatrix in library.");

	if (!(PROC_TYPE Internal::VRay_getPropertyMatrixByID = GET_PROC_ADDR(hLib, "VRay_getPropertyMatrixByID")))
		PROC_IMPORT_FAIL("Cannot find VRay_getPropertyMatrixByID in library.");

	if (!(PROC_TYPE Internal::VRay_getPropertyTransform = GET_PROC_ADDR(hLib, "VRay_getPropertyTransform")))
		PROC_IMPORT_FAIL("Cannot find VRay_getPropertyTransform in library.");

	if (!(PROC_TYPE Internal::VRay_getPropertyTransformByID = GET_PROC_ADDR(hLib, "VRay_getPropertyTransformByID")))
		PROC_IMPORT_FAIL("Cannot find VRay_getPropertyTransformByID in library.");

	if (!(PROC_TYPE Internal::VRay_getPropertyObjectByID = GET_PROC_ADDR(hLib, "VRay_getPropertyObjectByID")))
		PROC_IMPORT_FAIL("Cannot find VRay_getPropertyObjectByID in library.");

	if (!(PROC_TYPE Internal::VRay_getPropertyByteArrayByID = GET_PROC_ADDR(hLib, "VRay_getPropertyByteArrayByID")))
		PROC_IMPORT_FAIL("Cannot find VRay_getPropertyByteArrayByID in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setPropertyBytesValueByID = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setPropertyBytesValueByID")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setPropertyBytesValueByID in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setPropertyBytesValueAtTimeByID = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setPropertyBytesValueAtTimeByID")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setPropertyBytesValueAtTimeByID in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setPropertyBytesValueRawByID = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setPropertyBytesValueRawByID")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setPropertyBytesValueRawByID in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setPropertyBytesValueRawAtTimeByID = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setPropertyBytesValueRawAtTimeByID")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setPropertyBytesValueRawAtTimeByID in library.");


	if (!(PROC_TYPE Internal::VRay_VRayRenderer_commit = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_commit")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_commit in library.");


	if (!(PROC_TYPE Internal::VRay_VRayRenderer_pickPluginByXY = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_pickPluginByXY")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_pickPluginByXY in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_pickPluginsByXY = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_pickPluginsByXY")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_pickPluginsByXY in library.");
			
	if (!(PROC_TYPE Internal::VRay_VRayRenderer_readProxyPreview = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_readProxyPreview")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_readProxyPreview in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_readProxyFullPrepare = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_readProxyFullPrepare")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_readProxyFullPrepare in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_readProxyFullFinalize = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_readProxyFullFinalize")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_readProxyFullFinalize in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_createProxyFile = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_createProxyFile")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_createProxyFile in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getIrradianceMapSize = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getIrradianceMapSize")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getIrradianceMapSize in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getLightCacheSize = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getLightCacheSize")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getLightCacheSize in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getPhotonMapSize = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getPhotonMapSize")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getPhotonMapSize in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getCausticsSize = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getCausticsSize")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getCausticsSize in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_saveIrradianceMapFile = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_saveIrradianceMapFile")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_saveIrradianceMapFile in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_saveLightCacheFile = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_saveLightCacheFile")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_saveLightCacheFile in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_savePhotonMapFile = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_savePhotonMapFile")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_savePhotonMapFile in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_saveCausticsFile = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_saveCausticsFile")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_saveCausticsFile in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getDeviceListCUDA = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getDeviceListCUDA")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getDeviceListCUDA in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_getDeviceListOpenCL = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_getDeviceListOpenCL")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_getDeviceListOpenCL in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setDeviceListCUDA = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setDeviceListCUDA")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setDeviceListCUDA in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setDeviceListOpenCL = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setDeviceListOpenCL")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setDeviceListOpenCL in library.");


	// LicenseManager
	if (!(PROC_TYPE Internal::VRay_LicenseManager_releaseLicense = GET_PROC_ADDR(hLib, "VRay_LicenseManager_releaseLicense")))
		PROC_IMPORT_FAIL("Cannot find VRay_LicenseManager_releaseLicense in library.");

	// std_vector utils
	if (!(PROC_TYPE Internal::getVectorElement = GET_PROC_ADDR(hLib, "getVectorElement")))
		PROC_IMPORT_FAIL("Cannot find getVectorElement in library.");

	if (!(PROC_TYPE Internal::getVectorSize = GET_PROC_ADDR(hLib, "getVectorSize")))
		PROC_IMPORT_FAIL("Cannot find getVectorSize in library.");

	if (!(PROC_TYPE Internal::releaseVector = GET_PROC_ADDR(hLib, "releaseVector")))
		PROC_IMPORT_FAIL("Cannot find releaseVector in library.");

	// Plugin utils
	if (!(PROC_TYPE Internal::VRay_PluginUtils_getPropertyDefaultValue = GET_PROC_ADDR(hLib, "VRay_PluginUtils_getPropertyDefaultValue")))
		PROC_IMPORT_FAIL("Cannot find VRay_PluginUtils_getPropertyDefaultValue in library.");

	if (!(PROC_TYPE Internal::VRay_PluginUtils_getPropertyDescription = GET_PROC_ADDR(hLib, "VRay_PluginUtils_getPropertyDescription")))
		PROC_IMPORT_FAIL("Cannot find VRay_PluginUtils_getPropertyDescription in library.");

	if (!(PROC_TYPE Internal::VRay_PluginUtils_getPropertyUIGuides = GET_PROC_ADDR(hLib, "VRay_PluginUtils_getPropertyUIGuides")))
		PROC_IMPORT_FAIL("Cannot find VRay_PluginUtils_getPropertyUIGuides in library.");

	if (!(PROC_TYPE Internal::VRay_PluginUtils_getPropertyName = GET_PROC_ADDR(hLib, "VRay_PluginUtils_getPropertyName")))
		PROC_IMPORT_FAIL("Cannot find VRay_PluginUtils_getPropertyName in library.");

	if (!(PROC_TYPE Internal::VRay_PluginUtils_getPropertyType = GET_PROC_ADDR(hLib, "VRay_PluginUtils_getPropertyType")))
		PROC_IMPORT_FAIL("Cannot find VRay_PluginUtils_getPropertyType in library.");

	if (!(PROC_TYPE Internal::VRay_PluginUtils_getPropertyElementsCount = GET_PROC_ADDR(hLib, "VRay_PluginUtils_getPropertyElementsCount")))
		PROC_IMPORT_FAIL("Cannot find VRay_PluginUtils_getPropertyElementsCount in library.");

	// JPEG, PNG compression
	if (!(PROC_TYPE Internal::VRayImage_compressToJpeg = GET_PROC_ADDR(hLib, "VRayImage_compressToJpeg")))
		PROC_IMPORT_FAIL("Cannot find VRayImage_compressToJpeg in library.");

	if (!(PROC_TYPE Internal::VRayImage_compressToPng = GET_PROC_ADDR(hLib, "VRayImage_compressToPng")))
		PROC_IMPORT_FAIL("Cannot find VRayImage_compressToPng in library.");

	if (!(PROC_TYPE Internal::VRayImage_convertToBmp = GET_PROC_ADDR(hLib, "VRayImage_convertToBmp")))
		PROC_IMPORT_FAIL("Cannot find VRayImage_convertToBmp in library.");

	if (!(PROC_TYPE Internal::VRayImage_compressToJpegFile = GET_PROC_ADDR(hLib, "VRayImage_compressToJpegFile")))
		PROC_IMPORT_FAIL("Cannot find VRayImage_compressToJpegFile in library.");

	if (!(PROC_TYPE Internal::VRayImage_compressToPngFile = GET_PROC_ADDR(hLib, "VRayImage_compressToPngFile")))
		PROC_IMPORT_FAIL("Cannot find VRayImage_compressToPngFile in library.");

	if (!(PROC_TYPE Internal::VRayImage_SaveToBmpFile = GET_PROC_ADDR(hLib, "VRayImage_SaveToBmpFile")))
		PROC_IMPORT_FAIL("Cannot find VRayImage_SaveToBmpFile in library.");

	if (!(PROC_TYPE Internal::VRayImage_toBitmapData = GET_PROC_ADDR(hLib, "VRayImage_toBitmapData")))
		PROC_IMPORT_FAIL("Cannot find VRayImage_toBitmapData in library.");

	// VRayImage methods
	if (!(PROC_TYPE Internal::VRayImage_crop = GET_PROC_ADDR(hLib, "VRayImage_crop")))
		PROC_IMPORT_FAIL("Cannot find VRayImage_crop in library.");

	if (!(PROC_TYPE Internal::VRayImage_downscale = GET_PROC_ADDR(hLib, "VRayImage_downscale")))
		PROC_IMPORT_FAIL("Cannot find VRayImage_downscale in library.");

	if (!(PROC_TYPE Internal::VRayImage_fitIn = GET_PROC_ADDR(hLib, "VRayImage_fitIn")))
		PROC_IMPORT_FAIL("Cannot find VRayImage_fitIn in library.");

	if (!(PROC_TYPE Internal::VRayImage_fitOut = GET_PROC_ADDR(hLib, "VRayImage_fitOut")))
		PROC_IMPORT_FAIL("Cannot find VRayImage_fitOut in library.");

	if (!(PROC_TYPE Internal::VRayImage_cutIn = GET_PROC_ADDR(hLib, "VRayImage_cutIn")))
		PROC_IMPORT_FAIL("Cannot find VRayImage_cutIn in library.");

	if (!(PROC_TYPE Internal::VRayImage_crop_downscale = GET_PROC_ADDR(hLib, "VRayImage_crop_downscale")))
		PROC_IMPORT_FAIL("Cannot find VRayImage_crop_downscale in library.");

	if (!(PROC_TYPE Internal::VRayImage_clone = GET_PROC_ADDR(hLib, "VRayImage_clone")))
		PROC_IMPORT_FAIL("Cannot find VRayImage_clone in library.");

	if (!(PROC_TYPE Internal::VRayImage_create = GET_PROC_ADDR(hLib, "VRayImage_create")))
		PROC_IMPORT_FAIL("Cannot find VRayImage_create in library.");

	if (!(PROC_TYPE Internal::VRayImage_getSize = GET_PROC_ADDR(hLib, "VRayImage_getSize")))
		PROC_IMPORT_FAIL("Cannot find VRayImage_getSize in library.");

	if (!(PROC_TYPE Internal::VRayImage_getMemorySize = GET_PROC_ADDR(hLib, "VRayImage_getMemorySize")))
		PROC_IMPORT_FAIL("Cannot find VRayImage_getMemorySize in library.");

	if (!(PROC_TYPE Internal::VRayImage_getPixelDataPointerAndPixelCount = GET_PROC_ADDR(hLib, "VRayImage_getPixelDataPointerAndPixelCount")))
		PROC_IMPORT_FAIL("Cannot find VRayImage_getPixelDataPointerAndPixelCount in library.");

	if (!(PROC_TYPE Internal::VRayImage_drawImage = GET_PROC_ADDR(hLib, "VRayImage_drawImage")))
		PROC_IMPORT_FAIL("Cannot find VRayImage_drawImage in library.");

	if (!(PROC_TYPE Internal::VRayImage_addColor = GET_PROC_ADDR(hLib, "VRayImage_addColor")))
		PROC_IMPORT_FAIL("Cannot find VRayImage_addColor in library.");

	if (!(PROC_TYPE Internal::VRayImage_changeExposure = GET_PROC_ADDR(hLib, "VRayImage_changeExposure")))
		PROC_IMPORT_FAIL("Cannot find VRayImage_changeExposure in library.");

	if (!(PROC_TYPE Internal::VRayImage_changeGamma = GET_PROC_ADDR(hLib, "VRayImage_changeGamma")))
		PROC_IMPORT_FAIL("Cannot find VRayImage_changeGamma in library.");

	if (!(PROC_TYPE Internal::VRayImage_changeContrast = GET_PROC_ADDR(hLib, "VRayImage_changeContrast")))
		PROC_IMPORT_FAIL("Cannot find VRayImage_changeContrast in library.");

	if (!(PROC_TYPE Internal::VRayImage_makeNegative = GET_PROC_ADDR(hLib, "VRayImage_makeNegative")))
		PROC_IMPORT_FAIL("Cannot find VRayImage_makeNegative in library.");

	// Render Elements
	if (!(PROC_TYPE Internal::VRay_newRenderElementPlugin = GET_PROC_ADDR(hLib, "VRay_newRenderElementPlugin")))
		PROC_IMPORT_FAIL("Cannot find VRay_newRenderElementPlugin in library.");
			
	if (!(PROC_TYPE Internal::VRay_getRenderElementPlugin = GET_PROC_ADDR(hLib, "VRay_getRenderElementPlugin")))
		PROC_IMPORT_FAIL("Cannot find VRay_getRenderElementPlugin in library.");
			
	if (!(PROC_TYPE Internal::VRay_getRenderElementPluginsList = GET_PROC_ADDR(hLib, "VRay_getRenderElementPluginsList")))
		PROC_IMPORT_FAIL("Cannot find VRay_getRenderElementPluginsList in library.");

	if (!(PROC_TYPE Internal::VRay_getRenderElementInfo = GET_PROC_ADDR(hLib, "VRay_getRenderElementInfo")))
		PROC_IMPORT_FAIL("Cannot find VRay_getRenderElementInfo in library.");

	if (!(PROC_TYPE Internal::VRay_getRenderElementData = GET_PROC_ADDR(hLib, "VRay_getRenderElementData")))
		PROC_IMPORT_FAIL("Cannot find VRay_getRenderElementData in library.");

	if (!(PROC_TYPE Internal::VRay_getRenderElementAsImage = GET_PROC_ADDR(hLib, "VRay_getRenderElementAsImage")))
		PROC_IMPORT_FAIL("Cannot find VRay_getRenderElementAsImage in library.");

	// Memory
	if (!(PROC_TYPE Internal::C_memory_free = GET_PROC_ADDR(hLib, "C_memory_free")))
		PROC_IMPORT_FAIL("Cannot find C_memory_free in library.");

	if (!(PROC_TYPE Internal::C_memory_alloc = GET_PROC_ADDR(hLib, "C_memory_alloc")))
		PROC_IMPORT_FAIL("Cannot find C_memory_alloc in library.");

	// V-Ray SDK interoperability layer
#ifdef VRAY_SDK_INTEROPERABILITY
	if (!(PROC_TYPE Internal::RefCounter_create = GET_PROC_ADDR(hLib, "RefCounter_create")))
		PROC_IMPORT_FAIL("Cannot find RefCounter_create in library.");

	if (!(PROC_TYPE Internal::RefCounter_destroy = GET_PROC_ADDR(hLib, "RefCounter_destroy")))
		PROC_IMPORT_FAIL("Cannot find RefCounter_destroy in library.");

	if (!(PROC_TYPE Internal::RefCounter_increment = GET_PROC_ADDR(hLib, "RefCounter_increment")))
		PROC_IMPORT_FAIL("Cannot find RefCounter_increment in library.");

	if (!(PROC_TYPE Internal::RefCounter_decrement = GET_PROC_ADDR(hLib, "RefCounter_decrement")))
		PROC_IMPORT_FAIL("Cannot find RefCounter_decrement in library.");

	if (!(PROC_TYPE Internal::RefCounter_allocMem = GET_PROC_ADDR(hLib, "RefCounter_allocMem")))
		PROC_IMPORT_FAIL("Cannot find RefCounter_allocMem in library.");

	if (!(PROC_TYPE Internal::CharStringBuf_create = GET_PROC_ADDR(hLib, "CharStringBuf_create")))
		PROC_IMPORT_FAIL("Cannot find CharStringBuf_create in library.");

	if (!(PROC_TYPE Internal::CharStringBuf_resize = GET_PROC_ADDR(hLib, "CharStringBuf_resize")))
		PROC_IMPORT_FAIL("Cannot find CharStringBuf_resize in library.");

	if (!(PROC_TYPE Internal::CharStringBuf_incRefCount = GET_PROC_ADDR(hLib, "CharStringBuf_incRefCount")))
		PROC_IMPORT_FAIL("Cannot find CharStringBuf_incRefCount in library.");

	if (!(PROC_TYPE Internal::CharStringBuf_decRefCount = GET_PROC_ADDR(hLib, "CharStringBuf_decRefCount")))
		PROC_IMPORT_FAIL("Cannot find CharStringBuf_decRefCount in library.");

	if (!(PROC_TYPE Internal::CharStringBuf_getRefCount = GET_PROC_ADDR(hLib, "CharStringBuf_getRefCount")))
		PROC_IMPORT_FAIL("Cannot find CharStringBuf_getRefCount in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setPropertyValueByID_interop = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setPropertyValueByID_interop")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setPropertyValueByID_interop in library.");

	if (!(PROC_TYPE Internal::VRay_VRayRenderer_setPropertyValueAtTimeByID_interop = GET_PROC_ADDR(hLib, "VRay_VRayRenderer_setPropertyValueAtTimeByID_interop")))
		PROC_IMPORT_FAIL("Cannot find VRay_VRayRenderer_setPropertyValueAtTimeByID_interop in library.");
#endif // VRAY_SDK_INTEROPERABILITY

#undef GET_PROC_ADDR
#undef PROC_TYPE
}

#ifdef _MSC_VER
#	pragma warning(pop)
#endif // _MSC_VER

#endif // VRAY_RUNTIME_LOAD_PRIMARY


namespace VRay {

	inline RendererOptions::RendererOptions(const RendererOptions& options) :
		imageWidth(options.imageWidth),
		imageHeight(options.imageHeight),
		renderMode(options.renderMode),
		rtSampleLevel(options.rtSampleLevel),
		rtNoiseThreshold(options.rtNoiseThreshold),
		rtTimeout(options.rtTimeout),
		numThreads(options.numThreads),
		vfbDrawStyle(options.vfbDrawStyle),
		flags(options.flags),
		pluginLibraryPath(options.pluginLibraryPath),
		gpuPluginPath(options.gpuPluginPath)
	{ }

	inline RendererOptions& RendererOptions::operator=(const RendererOptions& options) {
		imageWidth = options.imageWidth;
		imageHeight = options.imageHeight;
		renderMode = options.renderMode;
		rtSampleLevel = options.rtSampleLevel;
		rtNoiseThreshold = options.rtNoiseThreshold;
		rtTimeout = options.rtTimeout;
		numThreads = options.numThreads;
		flags = options.flags;
		pluginLibraryPath = options.pluginLibraryPath;
		gpuPluginPath = options.gpuPluginPath;
		vfbDrawStyle = options.vfbDrawStyle;
		return *this;
	}

	inline void RendererOptions::swap(RendererOptions& options) {
		std::swap(imageWidth, options.imageWidth);
		std::swap(imageHeight, options.imageHeight);
		std::swap(renderMode, options.renderMode);
		std::swap(rtSampleLevel, options.rtSampleLevel);
		std::swap(rtNoiseThreshold, options.rtNoiseThreshold);
		std::swap(rtTimeout, options.rtTimeout);
		std::swap(numThreads, options.numThreads);
		std::swap(flags, options.flags);
		std::swap(vfbDrawStyle, options.vfbDrawStyle);
		pluginLibraryPath.swap(options.pluginLibraryPath);
		gpuPluginPath.swap(options.gpuPluginPath);
	}

	inline const RendererOptions* RendererOptions::prepare() const {
		internal_pluginLibraryPath = pluginLibraryPath.c_str();
		internal_gpuPluginPath = gpuPluginPath.c_str();
		return this;
	}

	inline RendererOptions::RendererOptions(const RendererOptions* options)
		: imageWidth(options->imageWidth)
		, imageHeight(options->imageHeight)
		, renderMode(options->renderMode)
		, rtSampleLevel(options->rtSampleLevel)
		, rtNoiseThreshold(options->rtNoiseThreshold)
		, rtTimeout(options->rtTimeout)
		, numThreads(options->numThreads)
		, vfbDrawStyle(options->vfbDrawStyle)
		, flags(options->flags)
	{
		pluginLibraryPath = options->internal_pluginLibraryPath ? options->internal_pluginLibraryPath : "";
		gpuPluginPath = options->internal_gpuPluginPath ? options->internal_gpuPluginPath : "";
	}


namespace Internal {
	template<bool> struct StaticAssert;
	template<> struct StaticAssert<true> { };

	class BinaryValueParser {
		const VRayRenderer *pRenderer;
		int* plist;

	public:
		BinaryValueParser(const VRayRenderer *pRenderer, int* binaryValue) : pRenderer(pRenderer), plist(binaryValue) {
		}

		Value get() {
			Type type = static_cast<Type>(*(plist++));
			const float* const pfloat = reinterpret_cast<float*>(plist);

			switch (type) {

			case TYPE_BOOL:
				return Value(!!*(plist++));

			case TYPE_INT:
				return Value(*(plist++));

			case TYPE_FLOAT:
				++plist;
				return Value(*pfloat);

			case TYPE_COLOR:
				plist += 3;
				return Value(Color(pfloat[0], pfloat[1], pfloat[2]));

			case TYPE_ACOLOR:
				plist += 4;
				return Value(AColor(pfloat[0], pfloat[1], pfloat[2], pfloat[3]));

			case TYPE_VECTOR:
				plist += 3;
				return Value(Vector(pfloat[0], pfloat[1], pfloat[2]));

			case TYPE_MATRIX:
				plist += 9;
				return Value(Matrix(
					Vector(pfloat[0], pfloat[1], pfloat[2]),
					Vector(pfloat[3], pfloat[4], pfloat[5]),
					Vector(pfloat[6], pfloat[7], pfloat[8])
				));

			case TYPE_TRANSFORM:
				plist += 12;
				return Value(Transform(Matrix(
					Vector(pfloat[0], pfloat[1], pfloat[2]),
					Vector(pfloat[3], pfloat[4], pfloat[5]),
					Vector(pfloat[6], pfloat[7], pfloat[8])),
					Vector(pfloat[9], pfloat[10], pfloat[11])
				));

			case TYPE_STRING: {
				size_t len = *plist;
				plist += (len + sizeof(int) - 1) / sizeof(int) + 1;
				return Value(std::string(reinterpret_cast<const char*>(pfloat + 1), len));
			}

			case TYPE_OBJECT:
			case TYPE_TEXTURE:
			case TYPE_TEXTUREFLOAT:
			case TYPE_TEXTUREINT:
			case TYPE_TEXTUREMATRIX:
			case TYPE_TEXTURETRANSFORM:
				return Value(const_cast<VRayRenderer*>(pRenderer), *(plist++));

			default: // avoid warning here; fall through for list types
				break;
			}

			const unsigned count = *(plist++);
			Value value;

			switch (type) {

			case TYPE_INT_LIST:
				value.constructListWithIterators_<int, TYPE_INT_LIST>(plist, plist + count);
				plist += count;
				break;

			case TYPE_FLOAT_LIST:
				plist += count;
				value.constructListWithIterators_<float, TYPE_FLOAT_LIST>(pfloat, reinterpret_cast<float*>(plist));
				break;

			case TYPE_COLOR_LIST: {
					Color* pval = reinterpret_cast<Color*>(plist);
					Color* pval_end = pval + count;
					plist = reinterpret_cast<int*>(pval_end);
					value.constructListWithIterators_<Color, TYPE_COLOR_LIST>(pval, pval_end);
				}
				break;

			case TYPE_VECTOR_LIST: {
					Vector* pval = reinterpret_cast<Vector*>(plist);
					Vector* pval_end = pval + count;
					plist = reinterpret_cast<int*>(pval_end);
					value.constructListWithIterators_<Vector, TYPE_VECTOR_LIST>(pval, pval_end);
				}
				break;

			case TYPE_GENERAL_LIST:
				value.constructOnly_<ValueList, TYPE_GENERAL_LIST>();
				for (unsigned i = 0; i < count; ++i)
					value.interpretAs_<ValueList>().push_back(get());

			default: // remember to implement new list types above!
				break;
			}

			return value;
		}

		bool get(std::string& str) const {
			Type type = static_cast<Type>(plist[0]);
			if (type == TYPE_STRING) {
				size_t count = plist[1];
				const char* const pstr = reinterpret_cast<char*>(plist + 2);
				str.assign(pstr, count);
				return true;
			}
			else {
				return false;
			}
		}

		bool get(ValueList& valuelist) {
			Type type = static_cast<Type>(*(plist++));
			if (type == TYPE_GENERAL_LIST) {
				const unsigned count = *(plist++);
				valuelist.clear();
				valuelist.reserve(count);
				for (unsigned i = 0; i < count; ++i)
					valuelist.push_back(get());
				return true;
			}
			else {
				return false;
			}
		}

		bool get(IntList& intList) const {
			return getTypedList<int, TYPE_INT_LIST>(intList);
		}

		bool get(FloatList& floatList) const {
			return getTypedList<float, TYPE_FLOAT_LIST>(floatList);
		}

		bool get(ColorList& colorList) const {
			return getTypedList<Color, TYPE_COLOR_LIST>(colorList);
		}

		bool get(VectorList& vectorList) const {
			return getTypedList<Vector, TYPE_VECTOR_LIST>(vectorList);
		}

	private:
		template<typename T, Type LIST_TYPE>
		bool getTypedList(std::vector<T>& typedList) const {
			Type type = static_cast<Type>(plist[0]);
			if (type == LIST_TYPE) {
				size_t count = plist[1];
				T* data = reinterpret_cast<T*>(plist);
				typedList.assign(data, data + count);
				return true;
			}
			else {
				return false;
			}
		}
	};


	class BinaryValueBuilder {
		std::vector<int> buf;

	public:
		bool build(const Value& val) {
			const Type type = val.getType();
			buf.push_back(static_cast<int>(type));

			switch (type) {

			case TYPE_BOOL:
				buf.push_back(val.interpretAs_<bool>());
				break;

			case TYPE_INT:
				buf.push_back(val.interpretAs_<int>());
				break;

			case TYPE_FLOAT: {
					float f = val.interpretAs_<float>();
					buf.push_back(reinterpret_cast<int&>(f));
				}
				break;

			case TYPE_STRING: {
					const std::string& str = val.interpretAs_<const std::string>();
					const int* istr = reinterpret_cast<const int*>(str.c_str());
					size_t len = str.size();
					buf.push_back(static_cast<int>(len));

					while (len > sizeof(int) - 1) {
						buf.push_back(*(istr++));
						len -= sizeof(int);
					}
					union {
						char c[sizeof(int)];
						int i;
					} tmp = {};

					const char* &cstr = reinterpret_cast<const char*&>(istr);

					switch (len) {
					case 3: tmp.c[2] = cstr[2];
					case 2: tmp.c[1] = cstr[1];
					case 1: tmp.c[0] = cstr[0];
					}
					buf.push_back(tmp.i);
				}
				break;

			case TYPE_OBJECT:
			case TYPE_TEXTURE:
			case TYPE_TEXTUREFLOAT:
			case TYPE_TEXTUREINT:
			case TYPE_TEXTUREMATRIX:
			case TYPE_TEXTURETRANSFORM:
				buf.push_back(static_cast<int>(val.interpretAs_<InstanceId>()));
				break;

			case TYPE_COLOR: {
					const Color& color = val.interpretAs_<const Color>();
					buf.push_back(reinterpret_cast<const int&>(color.r));
					buf.push_back(reinterpret_cast<const int&>(color.g));
					buf.push_back(reinterpret_cast<const int&>(color.b));
				}
				break;

			case TYPE_ACOLOR: {
					const AColor& acolor = val.interpretAs_<const AColor>();
					buf.push_back(reinterpret_cast<const int&>(acolor.color.r));
					buf.push_back(reinterpret_cast<const int&>(acolor.color.g));
					buf.push_back(reinterpret_cast<const int&>(acolor.color.b));
					buf.push_back(reinterpret_cast<const int&>(acolor.alpha));
				}
				break;

			case TYPE_VECTOR: {
					const Vector& vector = val.interpretAs_<const Vector>();
					buf.push_back(reinterpret_cast<const int&>(vector.x));
					buf.push_back(reinterpret_cast<const int&>(vector.y));
					buf.push_back(reinterpret_cast<const int&>(vector.z));
				}
				break;

			case TYPE_MATRIX: {
					const Matrix& matrix = val.interpretAs_<const Matrix>();
					const int* iptr = reinterpret_cast<const int*>(&matrix);
					for (unsigned i = 9; i; --i)
						buf.push_back(*(iptr++));
				}
				break;

			case TYPE_TRANSFORM: {
					const Transform& transform = val.interpretAs_<const Transform>();
					const int* iptr = reinterpret_cast<const int*>(&transform);
					for (unsigned i = 12; i; --i)
						buf.push_back(*(iptr++));
				}
				break;

			case TYPE_INT_LIST: {
					const IntList& list = val.interpretAs_<const IntList>();
					const int count = static_cast<int>(list.size());
					buf.push_back(count);
					IntList::const_iterator iter = list.begin();
					for (int i = 0; i < count; ++i)
						buf.push_back(*(iter++));
				}
				break;

			case TYPE_FLOAT_LIST: {
					const FloatList& list = val.interpretAs_<const FloatList>();
					const int count = static_cast<int>(list.size());
					buf.push_back(count);
					FloatList::const_iterator iter = list.begin();
					for (int i = 0; i < count; ++i)
						buf.push_back(reinterpret_cast<const int&>(*(iter++)));
				}
				break;

			case TYPE_COLOR_LIST: {
					const ColorList& list = val.interpretAs_<const ColorList>();
					const int count = static_cast<int>(list.size());
					buf.push_back(count);
					ColorList::const_iterator iter = list.begin();
					for (int i = 0; i < count; ++i) {
						const Color& color = *(iter++);
						buf.push_back(reinterpret_cast<const int&>(color.r));
						buf.push_back(reinterpret_cast<const int&>(color.g));
						buf.push_back(reinterpret_cast<const int&>(color.b));
					}
				}
				break;

			case TYPE_VECTOR_LIST: {
					const VectorList& list = val.interpretAs_<const VectorList>();
					const int count = static_cast<int>(list.size());
					buf.push_back(count);
					VectorList::const_iterator iter = list.begin();
					for (int i = 0; i < count; ++i) {
						const Vector& vector = *(iter++);
						buf.push_back(reinterpret_cast<const int&>(vector.x));
						buf.push_back(reinterpret_cast<const int&>(vector.y));
						buf.push_back(reinterpret_cast<const int&>(vector.z));
					}
				}
				break;

			case TYPE_GENERAL_LIST: {
					const ValueList& list = val.interpretAs_<const ValueList>();
					const int count = static_cast<int>(list.size());
					buf.push_back(count);
					ValueList::const_iterator iter = list.begin();
					for (int i = 0; i < count; ++i)
						if (!build(*(iter++)))
							return false;
				}
				break;

			default:
				return false;
			}
			return true;
		}

		bool build(const ValueList& list) {
			buf.push_back(TYPE_GENERAL_LIST);
			const int count = static_cast<int>(list.size());
			buf.push_back(count);
			ValueList::const_iterator iter = list.begin();
			for (int i = 0; i < count; ++i)
				if (!build(*(iter++)))
					return false;
			return true;
		}

		const int* getPointer() {
			return &buf[0];
		}
	};

	struct SubFileInfo_Internal {
		const char* pluginType;
		const char* fileNameSuffix;
	};

	class VRayExportSettings_Internal {
		Bool compressed;
		Bool framesInSeparateFiles;
		Bool useHexFormat;
		Bool renderElementsSeparateFolders;
		int subFileInfoCount;
		SubFileInfo_Internal* subFileInfos;

	public:
		VRayExportSettings_Internal(const VRayExportSettings& exportSettings) {
			compressed = exportSettings.compressed;
			framesInSeparateFiles = exportSettings.framesInSeparateFiles;
			useHexFormat = exportSettings.useHexFormat;
			renderElementsSeparateFolders = exportSettings.renderElementsSeparateFolders;
			subFileInfoCount = static_cast<int>(exportSettings.subFileInfos.size());
			subFileInfos = NULL;
			if (subFileInfoCount) {
				subFileInfos = new SubFileInfo_Internal[subFileInfoCount];
				for (int i = 0; i < subFileInfoCount; ++i) {
					subFileInfos[i].pluginType = &exportSettings.subFileInfos[i].pluginType[0];
					subFileInfos[i].fileNameSuffix = &exportSettings.subFileInfos[i].fileNameSuffix[0];
				}
			}
		}

		~VRayExportSettings_Internal() {
			if (subFileInfos) delete[] subFileInfos;
		}
	};
} // namespace Internal

inline std::string LicenseError::toString() const {
	char* str = Internal::VRay_getLicenseErrorTextStringFromErrorCode(errs);
	std::string res = str;
	Internal::C_memory_free(str);
	return res;
}

inline std::ostream& operator <<(std::ostream& stream, const LicenseError& err) {
	char* str = Internal::VRay_getLicenseErrorTextStringFromErrorCode(err.errs);
	stream << str;
	Internal::C_memory_free(str);
	return stream;
}

inline const char* ScannedMaterialLicenseError::toString() const {
	return Internal::VRay_getSimpleLicenseErrorTextStringFromErrorCode(static_cast<int>(errorCode));
}

inline std::ostream& operator <<(std::ostream& stream, const ScannedMaterialLicenseError& err) {
	return stream << err.toString();
}

inline bool encodeScannedMaterialParams(const ScannedMaterialParams& mtlParams, IntList& paramBlock) {
	int size = Internal::VRay_encodeBRDFScannedParams(NULL, 0, &mtlParams, sizeof(mtlParams), NULL);
	if (!size) return false;

	IntList block(size);
	size = Internal::VRay_encodeBRDFScannedParams(&block[0], size, &mtlParams, sizeof(mtlParams), NULL);
	if (!size) return false;

	paramBlock.swap(block);
	return true;
}

inline bool encodeScannedMaterialParams(const ScannedMaterialParams& mtlParams, IntList& paramBlock, ScannedMaterialLicenseError& licError) {
	int size = Internal::VRay_encodeBRDFScannedParams(NULL, 0, &mtlParams, sizeof(mtlParams), reinterpret_cast<int*>(&licError.errorCode));
	if (!size) return false;

	IntList block(size);
	size = Internal::VRay_encodeBRDFScannedParams(&block[0], size, &mtlParams, sizeof(mtlParams), reinterpret_cast<int*>(&licError.errorCode));
	if (!size) return false;

	paramBlock.swap(block);
	return true;
}

inline Color Color::fromTemperature(float temperature, RGBColorSpace colorSpace) {
	Color color;
	Internal::VRay_getBlackBodyColorRGB(&color, temperature, colorSpace);
	return color;
}

inline std::ostream& operator <<(std::ostream& stream, const Color& color) {
	stream << "Color(" << color.r << ", " << color.g << ", " << color.b << ")";
	return stream;
}

inline std::string Color::toString() const {
	std::ostringstream stream;
	stream << *this;
	return stream.str();
}

inline void Value::copyValue(const Value& value) {
	switch (value.type) {
	case TYPE_INT:
		constructWithValue_<int, TYPE_INT>(value.interpretAs_<int>());
		break;
	case TYPE_FLOAT:
		constructWithValue_<float, TYPE_FLOAT>(value.interpretAs_<float>());
		break;
	case TYPE_BOOL:
		constructWithValue_<bool, TYPE_BOOL>(value.interpretAs_<bool>());
		break;
	case TYPE_VECTOR:
		constructWithValue_<Vector, TYPE_VECTOR>(value.interpretAs_<Vector>());
		break;
	case TYPE_COLOR:
		constructWithValue_<Color, TYPE_COLOR>(value.interpretAs_<Color>());
		break;
	case TYPE_MATRIX:
		constructWithValue_<Matrix, TYPE_MATRIX>(value.interpretAs_<Matrix>());
		break;
	case TYPE_TRANSFORM:
		constructWithValue_<Transform, TYPE_TRANSFORM>(value.interpretAs_<Transform>());
		break;
	case TYPE_STRING:
		constructWithValue_<std::string, TYPE_STRING>(value.interpretAs_<std::string>());
		break;
	case TYPE_OBJECT:
		constructWithValue_<Plugin_t, TYPE_OBJECT>(value.plugin);
		break;
	case TYPE_INT_LIST:
		constructWithValue_<IntList, TYPE_INT_LIST>(value.interpretAs_<IntList>());
		break;
	case TYPE_FLOAT_LIST:
		constructWithValue_<FloatList, TYPE_FLOAT_LIST>(value.interpretAs_<FloatList>());
		break;
	case TYPE_VECTOR_LIST:
		constructWithValue_<VectorList, TYPE_VECTOR_LIST>(value.interpretAs_<VectorList>());
		break;
	case TYPE_COLOR_LIST:
		constructWithValue_<ColorList, TYPE_COLOR_LIST>(value.interpretAs_<ColorList>());
		break;
	case TYPE_GENERAL_LIST:
		constructWithValue_<ValueList, TYPE_GENERAL_LIST>(value.interpretAs_<ValueList>());
		break;

	default: // remember to implement new types above!
		type = TYPE_ERROR;
		break;
	}
}

inline void Value::destroyValue() {
	switch (type) {
	case TYPE_STRING:
		reinterpret_cast<std::string*>(val)->~basic_string();
		break;
	case TYPE_INT_LIST:
		reinterpret_cast<IntList*>(val)->~IntList();
		break;
	case TYPE_FLOAT_LIST:
		reinterpret_cast<FloatList*>(val)->~FloatList();
		break;
	case TYPE_VECTOR_LIST:
		reinterpret_cast<VectorList*>(val)->~VectorList();
		break;
	case TYPE_COLOR_LIST:
		reinterpret_cast<ColorList*>(val)->~ColorList();
		break;
	case TYPE_GENERAL_LIST:
		reinterpret_cast<ValueList*>(val)->~ValueList();
		break;
	default: // this prevents warnings, but implement new list types above to avoid leaks!
		break;
	};
	type = TYPE_ERROR;
}

template<typename T, Type TYPE>
void Value::constructOnly_() {
#ifdef new
#pragma push_macro("new")
#undef new
	new(val) T;
#pragma pop_macro("new")
#else
	new(val) T;
#endif
	type = TYPE;
}

template<typename T, Type TYPE>
void Value::constructListWithIterators_(const T* first, const T* last) {
#ifdef new
#pragma push_macro("new")
#undef new
	new(val) std::vector<T>(first, last);
#pragma pop_macro("new")
#else
	new(val) std::vector<T>(first, last);
#endif
	type = TYPE;
}

template<typename T, Type TYPE>
void Value::constructWithValue_(const T& value) {
#ifdef new
#pragma push_macro("new")
#undef new
	new(val) T(value);
#pragma pop_macro("new")
#else
	new(val) T(value);
#endif
	type = TYPE;
}

template<> inline void Value::constructWithValue_<const char*, TYPE_STRING>(const char* const &value) {
#ifdef new
#pragma push_macro("new")
#undef new
	new(val) T(value);
#pragma pop_macro("new")
#else
	new(val) std::string(value);
#endif
	type = TYPE_STRING;
}

template<typename T, Type TYPE>
void Value::setToValue_(const T& value) {
	destroyValue();
	constructWithValue_<T, TYPE>(value);
}

template<typename T>
T& Value::interpretAs_() {
	return *reinterpret_cast<T*>(val);
}

template<typename T>
const T& Value::interpretAs_() const {
	return *reinterpret_cast<const T*>(val);
}

template<typename T, Type TYPE>
T& Value::viewAs_() {
#ifndef VRAY_NOTHROW
	if (type != TYPE) throw InvalidTypeErr();
#endif
	return interpretAs_<T>();
}

template<typename T, Type TYPE>
T Value::getValue_() const {
#ifdef VRAY_NOTHROW
	return type == TYPE ? *reinterpret_cast<const T*>(val) : T();
#else
	if (type != TYPE) throw InvalidTypeErr();
	return *reinterpret_cast<const T*>(val);
#endif
}

inline Value::Value() : type(TYPE_ERROR) {
};

inline Value::Value(const Value& value) {
	copyValue(value);
}

inline Value::Value(int value) {
	constructWithValue_<int, TYPE_INT>(value);
};

inline Value::Value(float value) {
	constructWithValue_<float, TYPE_FLOAT>(value);
};

inline Value::Value(bool value) {
	constructWithValue_<bool, TYPE_BOOL>(value);
};

inline Value::Value(const Color& value) {
	constructWithValue_<Color, TYPE_COLOR>(value);
};

inline Value::Value(const AColor& value) {
	constructWithValue_<AColor, TYPE_ACOLOR>(value);
};

inline Value::Value(const Vector& value) {
	constructWithValue_<Vector, TYPE_VECTOR>(value);
};

inline Value::Value(const Matrix& value) {
	constructWithValue_<Matrix, TYPE_MATRIX>(value);
};

inline Value::Value(const Transform& value) {
	constructWithValue_<Transform, TYPE_TRANSFORM>(value);
};

inline Value::Value(const IntList& value) {
	constructWithValue_<IntList, TYPE_INT_LIST>(value);
};

inline Value::Value(const FloatList& value) {
	constructWithValue_<FloatList, TYPE_FLOAT_LIST>(value);
};

inline Value::Value(const VectorList& value) {
	constructWithValue_<VectorList, TYPE_VECTOR_LIST>(value);
};

inline Value::Value(const ColorList& value) {
	constructWithValue_<ColorList, TYPE_COLOR_LIST>(value);
};

inline Value::Value(const ValueList& value) {
	constructWithValue_<ValueList, TYPE_GENERAL_LIST>(value);
};

inline Value::Value(const char* value) {
	constructWithValue_<const char*, TYPE_STRING>(value);
};

inline Value::Value(const std::string& value) {
	constructWithValue_<std::string, TYPE_STRING>(value);
};

inline Value::Value(const Plugin& value) {
	plugin.renderer = value.getRenderer();
	plugin.id = value.getId();
	type = TYPE_OBJECT;
};

inline Value::Value(VRayRenderer *renderer, InstanceId id) {
	plugin.renderer = renderer;
	plugin.id = id;
	type = TYPE_OBJECT;
};

inline Value::Value(const int* values, size_t count) {
	constructListWithIterators_<int, TYPE_INT_LIST>(values, values + count);
}

inline Value::Value(const float* values, size_t count) {
	constructListWithIterators_<float, TYPE_FLOAT_LIST>(values, values + count);
}

inline Value::Value(const Vector* values, size_t count) {
	constructListWithIterators_<Vector, TYPE_VECTOR_LIST>(values, values + count);
}

inline Value::Value(const Color* values, size_t count) {
	constructListWithIterators_<Color, TYPE_COLOR_LIST>(values, values + count);
}

inline Value::~Value() {
	destroyValue();
}

inline void Value::swap(Value& val) {
	Internal::StaticAssert<sizeof(Value) == sizeof(Value) / sizeof(size_t) * sizeof(size_t)>();
	int len = sizeof(Value) / sizeof(size_t);
	size_t* a = reinterpret_cast<size_t*>(this);
	size_t* b = reinterpret_cast<size_t*>(&val);
	while (len) {
		std::swap(*a, *b);
		++a, ++b, --len;
	}
}

inline Value& Value::operator=(const Value& value) {
	destroyValue();
	copyValue(value);
	return *this;
}

inline Type Value::getType() const {
	return type;
}

inline size_t Value::getCount() const {
	switch (type) {

	case TYPE_INT_LIST:
		return reinterpret_cast<const IntList*>(val)->size();

	case TYPE_FLOAT_LIST:
		return reinterpret_cast<const FloatList*>(val)->size();

	case TYPE_VECTOR_LIST:
		return reinterpret_cast<const VectorList*>(val)->size();

	case TYPE_COLOR_LIST:
		return reinterpret_cast<const ColorList*>(val)->size();

	case TYPE_GENERAL_LIST:
		return reinterpret_cast<const ValueList*>(val)->size();

	default: // avoid warning
		break;
	};
	return 0;
}

inline bool Value::isList() const {
	return type == TYPE_LIST || (type >= TYPE_INT_LIST && type <= TYPE_TRANSFORM_LIST);
}

inline int Value::getInt() const {
	switch (type) {
	case TYPE_INT:   return static_cast<int>(interpretAs_<int>());
	case TYPE_FLOAT: return static_cast<int>(interpretAs_<float>());
	case TYPE_BOOL:  return static_cast<int>(interpretAs_<bool>());
	default: break; // avoid warning
	}
	return 0;
}

inline float Value::getFloat() const {
	switch (type) {
	case TYPE_INT:   return static_cast<float>(interpretAs_<int>());
	case TYPE_FLOAT: return static_cast<float>(interpretAs_<float>());
	case TYPE_BOOL:  return static_cast<float>(interpretAs_<bool>());
	default: break; // avoid warning
	}
	return 0.0f;
}

inline bool Value::getBool() const {
	switch (type) {
	case TYPE_INT:   return !!interpretAs_<int>();
	case TYPE_FLOAT: return !!interpretAs_<float>();
	case TYPE_BOOL:  return   interpretAs_<bool>();
	default: break; // avoid warning
	}
	return false;
}

inline Color Value::getColor() const {
	switch (type) {
	case TYPE_COLOR: return interpretAs_<Color>();
	case TYPE_ACOLOR: return interpretAs_<AColor>().color;
	default: break; // avoid warning
	}
	return Color();
}

inline AColor Value::getAColor() const {
	return type == TYPE_ACOLOR ? interpretAs_<AColor>() : AColor();
}

inline Vector Value::getVector() const {
	return type == TYPE_VECTOR ? interpretAs_<Vector>() : Vector();
}

inline Matrix Value::getMatrix() const {
	return type == TYPE_MATRIX ? interpretAs_<Matrix>() : Matrix();
}

inline Transform Value::getTransform() const {
	return type == TYPE_TRANSFORM ? interpretAs_<Transform>() : Transform();
}

inline IntList Value::getIntList() const {
	return type == TYPE_INT_LIST ? interpretAs_<IntList>() : IntList();
}

inline FloatList Value::getFloatList() const {
	return type == TYPE_FLOAT_LIST ? interpretAs_<FloatList>() : FloatList();
}

inline ColorList Value::getColorList() const {
	return type == TYPE_COLOR_LIST ? interpretAs_<ColorList>() : ColorList();
}

inline VectorList Value::getVectorList() const {
	return type == TYPE_VECTOR_LIST ? interpretAs_<VectorList>() : VectorList();
}

inline ValueList Value::getValueList() const {
	return type == TYPE_GENERAL_LIST ? interpretAs_<ValueList>() : ValueList();
}

inline std::string Value::getString() const {
	return type == TYPE_STRING ? interpretAs_<std::string>() : std::string();
}

inline InstanceId Value::getInstanceId() const {
	return type == TYPE_OBJECT ? interpretAs_<InstanceId>() : NO_ID;
}

inline Plugin Value::getPlugin() const {
	return Plugin(*plugin.renderer, type == TYPE_OBJECT ? plugin.id : NO_ID);
}

inline bool Value::isOK() const {
	return type != TYPE_ERROR;
}

inline bool Value::isBad() const {
	return !isOK();
}

inline const char* Value::getStringType() const {
	switch (getType()) {

	case TYPE_INT:
		return "int";

	case TYPE_FLOAT:
		return "float";

	case TYPE_BOOL:
		return "bool";

	case TYPE_VECTOR:
		return "Vector";

	case TYPE_COLOR:
		return "Color";

	case TYPE_ACOLOR:
		return "Color";

	case TYPE_MATRIX:
		return "Color";

	case TYPE_TRANSFORM:
		return "Color";

	case TYPE_STRING:
		return "string";

	case TYPE_OBJECT:
		return "Plugin";

	case TYPE_LIST:
		return "List";

	case TYPE_INT_LIST:
		return "IntList";

	case TYPE_FLOAT_LIST:
		return "FloatList";

	case TYPE_VECTOR_LIST:
		return "VectorList";

	case TYPE_COLOR_LIST:
		return "ColorList";

	default: // avoid warning
		break;
	}
	return "";
}

template<> inline void Value::set<int>(const int* values, size_t count) {
	destroyValue();
	constructListWithIterators_<int, TYPE_INT_LIST>(values, values + count);
}

template<> inline void Value::set<float>(const float* values, size_t count) {
	destroyValue();
	constructListWithIterators_<float, TYPE_FLOAT_LIST>(values, values + count);
}

template<> inline void Value::set<Vector>(const Vector* values, size_t count) {
	destroyValue();
	constructListWithIterators_<Vector, TYPE_VECTOR_LIST>(values, values + count);
}

template<> inline void Value::set<Color>(const Color* values, size_t count) {
	destroyValue();
	constructListWithIterators_<Color, TYPE_COLOR_LIST>(values, values + count);
}


template<> inline void Value::set<int>(const int& value) {
	setToValue_<int, TYPE_INT>(value);
}

template<> inline void Value::set<float>(const float& value) {
	setToValue_<float, TYPE_FLOAT>(value);
}

template<> inline void Value::set<bool>(const bool& value) {
	setToValue_<bool, TYPE_BOOL>(value);
}

template<> inline void Value::set<Color>(const Color& value) {
	setToValue_<Color, TYPE_COLOR>(value);
}

template<> inline void Value::set<AColor>(const AColor& value) {
	setToValue_<AColor, TYPE_ACOLOR>(value);
}

template<> inline void Value::set<Vector>(const Vector& value) {
	setToValue_<Vector, TYPE_VECTOR>(value);
}

template<> inline void Value::set<Matrix>(const Matrix& value) {
	setToValue_<Matrix, TYPE_MATRIX>(value);
}

template<> inline void Value::set<Transform>(const Transform& value) {
	setToValue_<Transform, TYPE_TRANSFORM>(value);
}

template<> inline void Value::set<IntList>(const IntList& value) {
	setToValue_<IntList, TYPE_INT_LIST>(value);
}

template<> inline void Value::set<FloatList>(const FloatList& value) {
	setToValue_<FloatList, TYPE_FLOAT_LIST>(value);
}

template<> inline void Value::set<ColorList>(const ColorList& value) {
	setToValue_<ColorList, TYPE_COLOR_LIST>(value);
}

template<> inline void Value::set<VectorList>(const VectorList& value) {
	setToValue_<VectorList, TYPE_VECTOR_LIST>(value);
}

template<> inline void Value::set<ValueList>(const ValueList& value) {
	setToValue_<ValueList, TYPE_GENERAL_LIST>(value);
}

template<> inline void Value::set<const char*>(const char* const &value) {
	setToValue_<const char*, TYPE_STRING>(value);
}

template<> inline void Value::set<char*>(char* const &value) {
	setToValue_<const char*, TYPE_STRING>(value);
}

template<> inline void Value::set<std::string>(const std::string& value) {
	setToValue_<std::string, TYPE_STRING>(value);
}

template<> inline void Value::set<Plugin>(const Plugin& value) {
	destroyValue();
	plugin.renderer = value.getRenderer();
	plugin.id = value.getId();
	type = TYPE_OBJECT;
};

template<> inline int& Value::as() {
	return viewAs_<int, TYPE_INT>();
}

template<> inline float& Value::as() {
	return viewAs_<float, TYPE_FLOAT>();
}

template<> inline bool& Value::as() {
	return viewAs_<bool, TYPE_BOOL>();
}

template<> inline Color& Value::as() {
	return viewAs_<Color, TYPE_COLOR>();
}

template<> inline AColor& Value::as() {
	return viewAs_<AColor, TYPE_ACOLOR>();
}

template<> inline Vector& Value::as() {
	return viewAs_<Vector, TYPE_VECTOR>();
}

template<> inline Matrix& Value::as() {
	return viewAs_<Matrix, TYPE_MATRIX>();
}

template<> inline Transform& Value::as() {
	return viewAs_<Transform, TYPE_TRANSFORM>();
}

template<> inline IntList& Value::as() {
	return viewAs_<IntList, TYPE_INT_LIST>();
}

template<> inline FloatList& Value::as() {
	return viewAs_<FloatList, TYPE_FLOAT_LIST>();
}

template<> inline ColorList& Value::as() {
	return viewAs_<ColorList, TYPE_COLOR_LIST>();
}

template<> inline VectorList& Value::as() {
	return viewAs_<VectorList, TYPE_VECTOR_LIST>();
}

template<> inline ValueList& Value::as() {
	return viewAs_<ValueList, TYPE_GENERAL_LIST>();
}

template<> inline std::string& Value::as() {
	return viewAs_<std::string, TYPE_STRING>();
}


template<> inline int Value::get() const {
	return getValue_<int, TYPE_INT>();
}

template<> inline float Value::get() const {
	return getValue_<float, TYPE_FLOAT>();
}

template<> inline bool Value::get() const {
	return getValue_<bool, TYPE_BOOL>();
}

template<> inline Color Value::get() const {
	return getValue_<Color, TYPE_COLOR>();
}

template<> inline AColor Value::get() const {
	return getValue_<AColor, TYPE_ACOLOR>();
}

template<> inline Vector Value::get() const {
	return getValue_<Vector, TYPE_VECTOR>();
}

template<> inline Matrix Value::get() const {
	return getValue_<Matrix, TYPE_MATRIX>();
}

template<> inline Transform Value::get() const {
	return getValue_<Transform, TYPE_TRANSFORM>();
}

template<> inline IntList Value::get() const {
	return getValue_<IntList, TYPE_INT_LIST>();
}

template<> inline FloatList Value::get() const {
	return getValue_<FloatList, TYPE_FLOAT_LIST>();
}

template<> inline ColorList Value::get() const {
	return getValue_<ColorList, TYPE_COLOR_LIST>();
}

template<> inline VectorList Value::get() const {
	return getValue_<VectorList, TYPE_VECTOR_LIST>();
}

template<> inline ValueList Value::get() const {
	return getValue_<ValueList, TYPE_GENERAL_LIST>();
}

template<> inline std::string Value::get() const {
	return getValue_<std::string, TYPE_STRING>();
}

template<> inline InstanceId Value::get() const {
	return getValue_<InstanceId, TYPE_OBJECT>();
}

template<> inline Plugin Value::get() const {
#ifdef VRAY_NOTHROW
	return Plugin(*plugin.renderer, TYPE_OBJECT ? plugin.id : NO_ID);
#else
	if (type != TYPE_OBJECT) throw InvalidTypeErr();
	return Plugin(*plugin.renderer, plugin.id);
#endif
}

inline Value& Value::operator[](int i) {
#ifdef VRAY_NOTHROW
	if (type != TYPE_GENERAL_LIST) return *this;
#else
	if (type != TYPE_GENERAL_LIST) throw InvalidTypeErr();
#endif
	return as<ValueList>().operator[](i);
}

inline std::ostream& operator <<(std::ostream& stream, const IntList& list) {
	stream << "IntList(";
	size_t end = list.size();
	if (end) {
		for (size_t i = 0; i < end - 1; ++i)
			stream << list[i] << ", ";
		stream << list[end - 1];
	}
	stream << ")";
	return stream;
}

inline std::ostream& operator <<(std::ostream& stream, const FloatList& list) {
	stream << "FloatList(";
	size_t end = list.size() - 1;
	for (size_t i = 0; i < end; ++i)
		stream << list[i] << ", ";
	if (end != -1)
		stream << list[end - 1];
	stream << ")";
	return stream;
}

inline std::ostream& operator <<(std::ostream& stream, const VectorList& list) {
	stream << "VectorList(";
	size_t end = list.size();
	if (end) {
		for (size_t i = 0; i < end - 1; ++i)
			stream << list[i] << ", ";
		stream << list[end - 1];
	}
	stream << ")";
	return stream;
}

inline std::ostream& operator <<(std::ostream& stream, const ColorList& list) {
	stream << "ColorList(";
	size_t end = list.size();
	if (end) {
		for (size_t i = 0; i < end - 1; ++i)
			stream << list[i] << ", ";
		stream << list[end - 1];
	}
	stream << ")";
	return stream;
}

inline std::ostream& operator <<(std::ostream& stream, const ValueList& list) {
	stream << "List(";
	size_t end = list.size();
	if (end) {
		for (size_t i = 0; i < end - 1; ++i)
			stream << list[i] << ", ";
		stream << list[end - 1];
	}
	stream << ")";
	return stream;
}

inline std::ostream& operator <<(std::ostream& stream, const Value& value) {
	return value.toString(stream);
}

inline std::ostream& Value::toString(std::ostream& stream) const {
	switch (type) {

	case TYPE_INT:
		stream << *reinterpret_cast<const int*>(val);
		break;

	case TYPE_FLOAT:
		stream << *reinterpret_cast<const float*>(val);
		break;

	case TYPE_BOOL:
		stream << *reinterpret_cast<const bool*>(val);
		break;

	case TYPE_VECTOR:
		stream << *reinterpret_cast<const Vector*>(val);
		break;

	case TYPE_COLOR:
		stream << *reinterpret_cast<const Color*>(val);
		break;

	case TYPE_ACOLOR:
		stream << *reinterpret_cast<const AColor*>(val);
		break;

	case TYPE_MATRIX:
		stream << *reinterpret_cast<const Matrix*>(val);
		break;

	case TYPE_TRANSFORM:
		stream << *reinterpret_cast<const Transform*>(val);
		break;

	case TYPE_STRING:
		stream << *reinterpret_cast<const std::string*>(val);
		break;

	case TYPE_OBJECT: {
			Plugin p = get<Plugin>();
			stream << (p.isValid() ? p.getName() : "#InvalidPlugin");
		}
		break;

	case TYPE_INT_LIST:
		stream << *reinterpret_cast<const IntList*>(val);
		break;

	case TYPE_FLOAT_LIST:
		stream << *reinterpret_cast<const FloatList*>(val);
		break;

	case TYPE_VECTOR_LIST:
		stream << *reinterpret_cast<const VectorList*>(val);
		break;

	case TYPE_COLOR_LIST:
		stream << *reinterpret_cast<const ColorList*>(val);
		break;

	case TYPE_LIST:
		stream << *reinterpret_cast<const ValueList*>(val);
		break;

	case TYPE_ERROR:
		stream << "#BadValue";
		break;

	default:
		stream << "<?>";
		break;
	}

	return stream;
}

inline std::string Value::toString() const {
	std::ostringstream stream;
	toString(stream);
	return stream.str();
}


inline ProxyReadData::ProxyReadData(int flags) {
	memset(&d, 0, sizeof(d));
	d.version = 0;
	d.flags = flags;
}

inline void ProxyReadData::allocateArrays() {
	if (d.use_vertices && d.vertices_count) {
		vertices.resize(d.vertices_count);
		indices.resize(d.indices_count);
		d.vertices = &vertices[0][0];
		d.indices = &indices[0];
	}
	if (d.use_normals && d.normals_count) {
		normals.resize(d.normals_count);
		normalIndices.resize(d.normalIndices_count);
		d.normals = &normals[0][0];
		d.normalIndices = &normalIndices[0];
	}
	if (d.use_UVs && d.uvSets_count) {
		size_t totalUVvalues = 0;
		size_t totalUVindices = 0;
		for (size_t i = 0; i < d.uvSets_count; ++i) {
			totalUVvalues += d.uvValues_counts[i];
			totalUVindices += d.uvValueIndices_counts[i];
		}
		uvSetNames.resize(d.uvSetNames_length);
		uvSetIndices.resize(d.uvSets_count);
		uvValues.resize(totalUVvalues);
		uvValueIndices.resize(totalUVindices);
		d.uvSetNames = &uvSetNames[0];
		d.uvSetIndices = &uvSetIndices[0];
		d.uvValues = &uvValues[0][0];
		d.uvValueIndices = &uvValueIndices[0];
	}
	if (d.use_mtlIDs && d.materialIDs_count) {
		materialIDs.resize(d.materialIDs_count);
		d.materialIDs = &materialIDs[0];
	}
	if (d.use_velocities && d.velocities_count) {
		velocities.resize(d.velocities_count);
		d.velocities = &velocities[0][0];
	}
}

inline int ProxyReadData::getUVSetsCount() const {
	return (int) d.uvSets_count;
}

inline std::string ProxyReadData::getUVSetName(int setIndex) const {
	if ((size_t) setIndex >= d.uvSets_count)
		return std::string();
	int idx = 0;
	while (setIndex > 0 && (size_t) idx < d.uvSetNames_length) {
		if (0 == d.uvSetNames[++idx])
			--setIndex;
	}
	return std::string(&d.uvSetNames[idx ? idx-1 : 0]);
}

inline std::vector<int> ProxyReadData::getUVOriginalIndices() const {
	std::vector<int> result(d.uvSets_count);
	for (size_t i = 0; i < d.uvSets_count; ++i)
		result[i] = d.uvSetIndices[i];
	return result;
}

inline size_t ProxyReadData::getUVValuesCount(int setIndex) const {
	if ((size_t) setIndex >= d.uvSets_count)
		return 0;
	return d.uvValues_counts[setIndex];
}

inline size_t ProxyReadData::getUVValueIndicesCount(int setIndex) const {
	if ((size_t) setIndex >= d.uvSets_count)
		return 0;
	return d.uvValueIndices_counts[setIndex];
}

inline const Vector *ProxyReadData::getUVValues(int setIndex) const {
	if ((size_t) setIndex >= d.uvSets_count)
		return NULL;
	size_t offset = 0;
	for (int i = 0; i < setIndex; ++i) {
		offset += d.uvValues_counts[i];
	}
	return reinterpret_cast<Vector *>(d.uvValues + offset);
}

inline const int *ProxyReadData::getUVValueIndices(int setIndex) const {
	if ((size_t) setIndex >= d.uvSets_count)
		return NULL;
	size_t offset = 0;
	for (int i = 0; i < setIndex; ++i) {
		offset += d.uvValueIndices_counts[i];
	}
	return d.uvValueIndices + offset;
}

inline void VRayRenderer::renderStartCallback(void* r) {
	VRayRenderer* &renderer = reinterpret_cast<VRayRenderer*&>(r);
	renderer->renderStartDelegate(*renderer);
}

inline void VRayRenderer::imageReadyCallback(void* r) {
	VRayRenderer* &renderer = reinterpret_cast<VRayRenderer*&>(r);
	renderer->imageReadyDelegate(*renderer);
}

inline void VRayRenderer::rendererCloseCallback(void* r) {
	VRayRenderer* &renderer = reinterpret_cast<VRayRenderer*&>(r);
	renderer->rendererCloseDelegate(*renderer);
}

inline void VRayRenderer::rtImageUpdatedCallback(int, int, VRayImage* img, void* r) {
	VRayRenderer* &renderer = reinterpret_cast<VRayRenderer*&>(r);
	renderer->rtImageUpdatedDelegate(*renderer, img);
}

inline void VRayRenderer::dumpMessageCallback(const char* msg, int level, void* r) {
	VRayRenderer* &renderer = reinterpret_cast<VRayRenderer*&>(r);
	renderer->dumpMessageDelegate(*renderer, msg, level);
}

inline void VRayRenderer::bucketInitCallback(int x, int y, int width, int height, const char* host, void* r) {
	VRayRenderer* &renderer = reinterpret_cast<VRayRenderer*&>(r);
	renderer->bucketInitDelegate(*renderer, x, y, width, height, host);
}

inline void VRayRenderer::bucketReadyCallback(int x, int y, int, int, const char* host, VRayImage* img, void* r) {
	VRayRenderer* &renderer = reinterpret_cast<VRayRenderer*&>(r);
	renderer->bucketReadyDelegate(*renderer, x, y, host, img);
}

inline void VRayRenderer::bucketFailedCallback(int x, int y, int width, int height, const char* host, void* r) {
	VRayRenderer* &renderer = reinterpret_cast<VRayRenderer*&>(r);
	renderer->bucketFailedDelegate(*renderer, x, y, width, height, host);
}

inline void VRayRenderer::sequenceStartCallback(void* r) {
	VRayRenderer* &renderer = reinterpret_cast<VRayRenderer*&>(r);
	renderer->sequenceStartDelegate(*renderer);
}

inline void VRayRenderer::sequenceDoneCallback(void* r) {
	VRayRenderer* &renderer = reinterpret_cast<VRayRenderer*&>(r);
	renderer->sequenceDoneDelegate(*renderer);
}

inline void VRayRenderer::progressCallback(const char* msg, int progress, int total, void* r) {
	VRayRenderer* &renderer = reinterpret_cast<VRayRenderer*&>(r);
	renderer->progressDelegate(*renderer, msg, progress, total);
}

inline void VRayRenderer::renderViewChangedCallback(const char* propName, void* r) {
	VRayRenderer* &renderer = reinterpret_cast<VRayRenderer*&>(r);
	renderer->renderViewChangedDelegate(*renderer, propName);
}

inline void VRayRenderer::renderLastCallback(int flags, void* r) {
	VRayRenderer* &renderer = reinterpret_cast<VRayRenderer*&>(r);
	renderer->renderLastDelegate(*renderer, flags);
}

inline void VRayRenderer::vfbClosedCallback(void* r) {
	VRayRenderer* &renderer = reinterpret_cast<VRayRenderer*&>(r);
	renderer->vfbClosedDelegate(*renderer);
}

inline void VRayRenderer::createInstance(const RendererOptions* options) {
	VRayRendererNative* r = Internal::VRay_VRayRenderer_create(options, &licErr.errs);
	vfb.rendererNative = r;
#ifndef VRAY_NOTHROW
	if (!r) {
		if (licErr.error())
			throw InstantiationErr(licErr.toString());
		else
			throw InstantiationErr();
	}
#endif
}

inline VRayRenderer::VRayRenderer() : autoCommit(true) {
	createInstance(NULL);
};

inline VRayRenderer::VRayRenderer(const RendererOptions &rendererOptions) : autoCommit(true) {
	createInstance(rendererOptions.prepare());
}

inline VRayRenderer::~VRayRenderer() {
	if (getNativeRenderer()) {
		Internal::VRay_VRayRenderer_setBucketInitCallback(getNativeRenderer(), NULL, NULL);
		Internal::VRay_VRayRenderer_setBucketReadyCallback(getNativeRenderer(), NULL, NULL);
		Internal::VRay_VRayRenderer_setBucketFailedCallback(getNativeRenderer(), NULL, NULL);
		Internal::VRay_VRayRenderer_setRenderStartCallback(getNativeRenderer(), NULL, NULL);
		Internal::VRay_VRayRenderer_setImageUpdatedCallback(getNativeRenderer(), NULL, NULL);
		Internal::VRay_VRayRenderer_setImageReadyCallback(getNativeRenderer(), NULL, NULL);
		Internal::VRay_VRayRenderer_setDumpMessageCallback(getNativeRenderer(), NULL, NULL);
		//Internal::VRay_VRayRenderer_setRendererCloseCallback(getNativeRenderer(), NULL, NULL);
		Internal::VRay_VRayRenderer_setSequenceStartCallback(getNativeRenderer(), NULL, NULL);
		Internal::VRay_VRayRenderer_setSequenceDoneCallback(getNativeRenderer(), NULL, NULL);
		Internal::VRay_VRayRenderer_setProgressCallback(getNativeRenderer(), NULL, NULL);
		Internal::VRay_VRayRenderer_setRenderViewChangedCallback(getNativeRenderer(), NULL, NULL);
		Internal::VRay_VRayRenderer_setRenderLastCallback(getNativeRenderer(), NULL, NULL);

		Internal::VRay_VRayRenderer_release(getNativeRenderer());
	}
}

inline void VRayRenderer::VFB::show(bool show, bool setFocus) {
	Internal::VRay_VRayRenderer_showFrameBuffer(rendererNative, show, setFocus);
}

inline bool VRayRenderer::VFB::isShown() const {
	return !!Internal::VRay_VRayRenderer_isFrameBufferShown(rendererNative);
}

inline void VRayRenderer::VFB::setAlwaysOnTop(bool set) {
	Internal::VRay_VRayRenderer_setVfbAlwaysOnTop(rendererNative, set);
}

inline void VRayRenderer::VFB::enableInteractivity(bool enable) {
	Internal::VRay_VRayRenderer_enableVFBInteractivity(rendererNative, enable);
}

inline bool VRayRenderer::VFB::isInteractivityEnabled() const {
	return !!Internal::VRay_VRayRenderer_isVFBInteractivityEnabled(rendererNative);
}

inline void VRayRenderer::VFB::setPosition(int x, int y) {
	struct { int x, y, w, h; } pos = { x, y };
	Internal::VRay_VRayRenderer_setVfbPositionAndSize(rendererNative, &pos);
}

inline void VRayRenderer::VFB::setPositionAndSize(int x, int y, int w, int h) {
	struct { int x, y, w, h; } posAndSize = { x, y, w, h };
	Internal::VRay_VRayRenderer_setVfbPositionAndSize(rendererNative, &posAndSize);
}

inline bool VRayRenderer::VFB::getPositionAndSize(int& x, int& y, int& w, int& h) const {
	struct { int x, y, w, h; } posAndSize = {};
	if (Internal::VRay_VRayRenderer_getVfbPositionAndSize(rendererNative, &posAndSize)) {
		x = posAndSize.x;
		y = posAndSize.y;
		w = posAndSize.w;
		h = posAndSize.h;
		return true;
	}
	return false;
}

inline VFBState* VRayRenderer::VFB::getState(size_t& dataSize, bool includeMain, bool includeHistory) const {
	void* buf;
	int parts = 0;
	if (includeMain) parts += 1;
	if (includeHistory) parts += 2;
	dataSize = Internal::VRay_VRayRenderer_getVfbState(rendererNative, &buf, parts);
	return static_cast<VFBState*>(buf);
}

inline void VRayRenderer::VFB::setState(const void* vfbStateData, size_t dataSize, bool setMain, bool setHistory) {
	int parts = 0;
	if (setMain) parts += 1;
	if (setHistory) parts += 2;
	Internal::VRay_VRayRenderer_setVfbState(rendererNative, vfbStateData, dataSize, parts);
}

inline void* VRayRenderer::VFB::getWindowHandle() const {
	return Internal::VRay_VRayRenderer_getVfbWindowHandle(rendererNative);
}

inline void VRayRenderer::VFB::setParentWindow(void* hWnd) {
	Internal::VRay_VRayRenderer_setVfbParentWindowHandle(rendererNative, hWnd);
}

inline bool VRayRenderer::VFB::loadColorCorrectionsPreset(const char* filename) {
	return !!Internal::VRay_VRayRenderer_loadVfbColorCorrectionsPreset(rendererNative, filename);
}

inline bool VRayRenderer::VFB::saveColorCorrectionsPreset(const char* filename) const {
	return !!Internal::VRay_VRayRenderer_saveVfbColorCorrectionsPreset(rendererNative, filename);
}

inline bool VRayRenderer::VFB::setTitlePrefix(const char* prefix) {
	return !!Internal::VRay_VRayRenderer_setVfbTitlePrefix(rendererNative, prefix);
}

/// If exceptions are not enabled use this function to test whether you have a valid renderer
/// @retval: true - renderer instance is valid, false - renderer instance is invalid
inline VRayRenderer::operator bool() const {
	return !!getNativeRenderer();
}

inline Error VRayRenderer::getLastError() const {
	return Internal::VRay_VRayRenderer_getLastErrorCode(getNativeRenderer());
}

inline LicenseError VRayRenderer::getLicenseError() const {
	return licErr;
}

inline ParserError VRayRenderer::getLastParserError() const {
	ParserError err;
	err.fileName = Internal::VRay_VRayRenderer_getLastParserErrorFileName(getNativeRenderer());
	err.errorText = Internal::VRay_VRayRenderer_getLastParserErrorText(getNativeRenderer());
	err.fileLine = Internal::VRay_VRayRenderer_getLastParserErrorFileLine(getNativeRenderer());
	return err;
}

inline InstanceId VRayRenderer::getPluginId_internal(const char* pluginName) const {
	return Internal::VRay_VRayRenderer_getPluginID(getNativeRenderer(), pluginName);
}

inline InstanceId VRayRenderer::getPluginId(const char* pluginName) const {
	return getPluginId_internal(pluginName);
}

inline InstanceId VRayRenderer::getPluginId(const std::string& pluginName) const {
	return getPluginId_internal(pluginName.c_str());
}

inline const char* VRayRenderer::getPluginName_internal(InstanceId pluginID) const {
	const char* pluginName = Internal::VRay_VRayRenderer_getPluginName(getNativeRenderer(), pluginID);
	return pluginName ? pluginName : "";
}

inline const char* VRayRenderer::getPluginName(InstanceId pluginID) const {
	return getPluginName_internal(pluginID);
}

inline bool VRayRenderer::setRenderMode(RendererOptions::RenderMode mode) {
	return !!Internal::VRay_VRayRenderer_setRenderMode(getNativeRenderer(), mode);
}

inline RendererOptions::RenderMode VRayRenderer::getRenderMode() {
	return static_cast<RendererOptions::RenderMode>(Internal::VRay_VRayRenderer_getRenderMode(getNativeRenderer()));
}

inline bool VRayRenderer::setOptions(const RendererOptions &options) {
	return !!Internal::VRay_VRayRenderer_setRenderOptions(getNativeRenderer(), options.prepare());
}

inline RendererOptions VRayRenderer::getOptions() {
	RendererOptions* options = Internal::VRay_VRayRenderer_getRenderOptions(getNativeRenderer());
	RendererOptions result(options);
	Internal::C_memory_free(options);
	return result;
}

inline bool VRayRenderer::reset() {
	return !!Internal::VRay_VRayRenderer_reset(getNativeRenderer(), NULL);
}

inline bool VRayRenderer::reset(const RendererOptions &options) {
	return !!Internal::VRay_VRayRenderer_reset(getNativeRenderer(), &options);
}

inline bool VRayRenderer::setImprovedDefaultSettings() {
	return !!Internal::VRay_VRayRenderer_setImprovedDefaults(getNativeRenderer());
}

inline int VRayRenderer::getCurrentFrame() const {
	return Internal::VRay_VRayRenderer_getCurrentFrame(getNativeRenderer());
}

inline void VRayRenderer::setCurrentFrame(int frame) {
	Internal::VRay_VRayRenderer_setCurrentFrame(getNativeRenderer(), frame);
}

inline double VRayRenderer::getCurrentTime() const {
	return Internal::VRay_VRayRenderer_getCurrentTime(getNativeRenderer());
}

inline void VRayRenderer::setCurrentTime(double time) {
	Internal::VRay_VRayRenderer_setCurrentTime(getNativeRenderer(), time);
}

inline bool VRayRenderer::clearAllPropertyValuesUpToTime(double time) {
	return !Internal::VRay_VRayRenderer_clearAllFrameValuesUpToTime(getNativeRenderer(), time);
}

inline void VRayRenderer::useAnimatedValues(bool on) {
	Internal::VRay_VRayRenderer_setAnimateProperties(getNativeRenderer(), on);
}

inline bool VRayRenderer::getUseAnimatedValuesState() const {
	return !!Internal::VRay_VRayRenderer_getAnimateProperties(getNativeRenderer());
}

inline int VRayRenderer::getSequenceFrame() const {
	return Internal::VRay_VRayRenderer_getCurrentSequenceFrame(getNativeRenderer());
}

inline Box VRayRenderer::getBoundingBox() const {
	Box bbox;
	Internal::VRay_VRayRenderer_getBoundingBox(getNativeRenderer(), &bbox);
	return bbox;
}

inline Box VRayRenderer::getBoundingBox(bool& ok) const {
	Box bbox;
	ok = !!Internal::VRay_VRayRenderer_getBoundingBox(getNativeRenderer(), &bbox);
	return bbox;
}

inline bool VRayRenderer::setCamera(const Plugin& plugin) {
	return !Internal::VRay_VRayRenderer_setCurrentCameraByID(getNativeRenderer(), plugin.getId());
}

inline Plugin VRayRenderer::getCamera() const {
	return Plugin(*const_cast<VRayRenderer*>(this), Internal::VRay_VRayRenderer_getCurrentCamera(getNativeRenderer()));
}

inline RTStatistics VRayRenderer::getRTStatistics() const {
	RTStatistics rtStatistics = {};
	Internal::VRay_VRayRenderer_getRTStatistics(getNativeRenderer(), &rtStatistics);
	return rtStatistics;
}

inline RendererState VRayRenderer::getState() const {
	return (RendererState) Internal::VRay_VRayRenderer_getState(getNativeRenderer());
}

inline bool VRayRenderer::pause() const {
	return !!Internal::VRay_VRayRenderer_pauseRT(getNativeRenderer(), true);
}

inline bool VRayRenderer::resume() const {
	return !!Internal::VRay_VRayRenderer_pauseRT(getNativeRenderer(), false);
}

inline bool VRayRenderer::isPaused() const {
	return !!Internal::VRay_VRayRenderer_getPauseRTStatus(getNativeRenderer());
}

inline int VRayRenderer::getWidth() const {
	return Internal::VRay_VRayRenderer_getWidth(getNativeRenderer());
}

inline int VRayRenderer::getHeight() const {
	return Internal::VRay_VRayRenderer_getHeight(getNativeRenderer());
}

inline bool VRayRenderer::getImageSize(int& width, int& height) const {
	return !!Internal::VRay_VRayRenderer_getImageSize(getNativeRenderer(), &width, &height);
}

inline bool VRayRenderer::setWidth(int width, bool resetCropRegion, bool resetRenderRegion) {
	return setImageSize(width, 0, resetCropRegion, resetRenderRegion);
}

inline bool VRayRenderer::setHeight(int height, bool resetCropRegion, bool resetRenderRegion) {
	return setImageSize(0, height, resetCropRegion, resetRenderRegion);
}

inline bool VRayRenderer::setImageSize(int width, int height, bool resetCropRegion, bool resetRenderRegion) {
	const int CROP_REGION_BITMASK = 1;
	const int RENDER_REGION_BITMASK = 2;
	int resetRegions = (resetCropRegion ? CROP_REGION_BITMASK : 0) | (resetRenderRegion ? RENDER_REGION_BITMASK : 0);
	return !!Internal::VRay_VRayRenderer_setImageSizeEx(getNativeRenderer(), width, height, resetRegions);
}

inline bool VRayRenderer::getRenderRegion(int& rgnLeft, int& rgnTop, int& rgnWidth, int& rgnHeight) {
	Internal::RectRegion renderRegion = {};
	bool res = !!Internal::VRay_VRayRenderer_getRenderRegion(getNativeRenderer(), &renderRegion);
	rgnLeft = renderRegion.left;
	rgnTop = renderRegion.top;
	rgnWidth = renderRegion.width;
	rgnHeight = renderRegion.height;
	return res;
}

inline bool VRayRenderer::setRenderRegion(int rgnLeft, int rgnTop, int rgnWidth, int rgnHeight) {
	Internal::RectRegion renderRegion = { rgnLeft, rgnTop, rgnWidth, rgnHeight };
	return !!Internal::VRay_VRayRenderer_setRenderRegion(getNativeRenderer(), &renderRegion);
}

inline bool VRayRenderer::getCropRegion(int& srcWidth, int& srcHeight, float& rgnLeft, float& rgnTop, float& rgnWidth, float& rgnHeight) {
	Internal::CropRegion cropRegion = {};
	bool res = !!Internal::VRay_VRayRenderer_getCropRegion(getNativeRenderer(), &cropRegion);
	srcWidth = cropRegion.srcWidth;
	srcHeight = cropRegion.srcHeight;
	rgnLeft = cropRegion.rgnLeft;
	rgnTop = cropRegion.rgnTop;
	rgnWidth = cropRegion.rgnWidth;
	rgnHeight = cropRegion.rgnHeight;
	return res;
}

inline bool VRayRenderer::setCropRegion(int srcWidth, int srcHeight, float rgnLeft, float rgnTop) {
	Internal::CropRegion cropRegion = { srcWidth, srcHeight, rgnLeft, rgnTop, -0.0f, -0.0f };
	return !!Internal::VRay_VRayRenderer_setCropRegion(getNativeRenderer(), &cropRegion);
}

inline bool VRayRenderer::setCropRegion(int srcWidth, int srcHeight, float rgnLeft, float rgnTop, float rgnWidth, float rgnHeight) {
	Internal::CropRegion cropRegion = { srcWidth, srcHeight, rgnLeft, rgnTop, rgnWidth, rgnHeight };
	return !!Internal::VRay_VRayRenderer_setCropRegion(getNativeRenderer(), &cropRegion);
}

inline int VRayRenderer::load(const char* fileName) const {
	return Internal::VRay_VRayRenderer_load(getNativeRenderer(), fileName, autoCommit);
}

inline int VRayRenderer::load(const std::string& fileName) const {
	return Internal::VRay_VRayRenderer_load(getNativeRenderer(), fileName.c_str(), autoCommit);
}

inline int VRayRenderer::loadAsText(const char* text) const {
	return Internal::VRay_VRayRenderer_loadAsText(getNativeRenderer(), text, autoCommit);
}

inline int VRayRenderer::loadAsText(const std::string& text) const {
	return Internal::VRay_VRayRenderer_loadAsText(getNativeRenderer(), text.c_str(), autoCommit);
}

inline int VRayRenderer::append(const char* fileName) const {
	return Internal::VRay_VRayRenderer_append(getNativeRenderer(), fileName, autoCommit);
}

inline int VRayRenderer::append(const std::string& fileName) const {
	return Internal::VRay_VRayRenderer_append(getNativeRenderer(), fileName.c_str(), autoCommit);
}

inline int VRayRenderer::appendAsText(const char* text) const {
	return Internal::VRay_VRayRenderer_appendAsText(getNativeRenderer(), text, autoCommit);
}

inline int VRayRenderer::appendAsText(const std::string& text) const {
	return Internal::VRay_VRayRenderer_appendAsText(getNativeRenderer(), text.c_str(), autoCommit);
}

inline void VRayRenderer::start() const {
	Internal::VRay_VRayRenderer_render(getNativeRenderer());
}

inline void VRayRenderer::startSync() const {
	Internal::VRay_VRayRenderer_startSync(getNativeRenderer());
}

inline void VRayRenderer::run() const {
	Internal::VRay_VRayRenderer_renderImage(getNativeRenderer());
}

inline void VRayRenderer::renderSequence() const {
	Internal::VRay_VRayRenderer_renderSequence(getNativeRenderer(), NULL, 0);
}

inline void VRayRenderer::renderSequence(SubSequenceDesc descriptions[], size_t count) const {
	Internal::VRay_VRayRenderer_renderSequence(getNativeRenderer(), descriptions, count);
}

inline void VRayRenderer::continueSequence() const {
	Internal::VRay_VRayRenderer_continueSequence(getNativeRenderer());
}

inline void VRayRenderer::stop() const {
	Internal::VRay_VRayRenderer_stop(getNativeRenderer());
}

inline int VRayRenderer::exportScene(const char* filePath) const {
	return Internal::VRay_VRayRenderer_exportScene(getNativeRenderer(), filePath, NULL);
}

inline int VRayRenderer::exportScene(const char* filePath, const VRayExportSettings& settings) const {
	Internal::VRayExportSettings_Internal s(settings);
	return Internal::VRay_VRayRenderer_exportScene(getNativeRenderer(), filePath, &s);
}

inline int VRayRenderer::exportScene(const std::string& filePath) const {
	return exportScene(filePath.c_str());
}

inline int VRayRenderer::exportScene(const std::string& filePath, const VRayExportSettings& settings) const {
	return exportScene(filePath.c_str(), settings);
}

inline void VRayRenderer::bucketRenderNearest(int x, int y) const {
	Internal::VRay_VRayRenderer_bucketRenderNearest(getNativeRenderer(), x, y);
}

inline int VRayRenderer::addHosts(const char* hosts) const {
	return Internal::VRay_VRayRenderer_clientAddHosts(getNativeRenderer(), hosts);
}

inline int VRayRenderer::addHosts(const std::string& hosts) const {
	return Internal::VRay_VRayRenderer_clientAddHosts(getNativeRenderer(), hosts.c_str());
}

inline int VRayRenderer::removeHosts(const char* hosts) const {
	return Internal::VRay_VRayRenderer_clientRemoveHosts(getNativeRenderer(), hosts);
}

inline int VRayRenderer::removeHosts(const std::string& hosts) const {
	return Internal::VRay_VRayRenderer_clientRemoveHosts(getNativeRenderer(), hosts.c_str());
}

inline int VRayRenderer::resetHosts(const char* hosts) const {
	return Internal::VRay_VRayRenderer_clientResetHosts(getNativeRenderer(), hosts);
}

inline int VRayRenderer::resetHosts(const std::string& hosts) const {
	return Internal::VRay_VRayRenderer_clientResetHosts(getNativeRenderer(), hosts.c_str());
}

inline std::string VRayRenderer::getAllHosts() const {
	int size;
	char* hosts = Internal::VRay_VRayRenderer_clientGetAllHosts(getNativeRenderer(), &size);
	std::string hostsStr(hosts ? hosts : "");
	Internal::C_memory_free(hosts);
	return hostsStr;
}

inline std::string VRayRenderer::getActiveHosts() const {
	int size;
	char* hosts = Internal::VRay_VRayRenderer_clientGetActiveHosts(getNativeRenderer(), &size);
	std::string hostsStr(hosts ? hosts : "");
	Internal::C_memory_free(hosts);
	return hostsStr;
}

inline std::string VRayRenderer::getInactiveHosts() const {
	int size;
	char* hosts = Internal::VRay_VRayRenderer_clientGetInactiveHosts(getNativeRenderer(), &size);
	std::string hostsStr(hosts ? hosts : "");
	Internal::C_memory_free(hosts);
	return hostsStr;
}

inline bool VRayRenderer::commit(bool force) {
	return Internal::VRay_VRayRenderer_commit(getNativeRenderer(), force) == 0;
}

inline VRayImage* VRayRenderer::getImage() const {
	return Internal::VRay_VRayRenderer_getImage(getNativeRenderer(), NULL);
}

inline VRayImage* VRayRenderer::getImage(const GetImageOptions &options) const {
	return Internal::VRay_VRayRenderer_getImage(getNativeRenderer(), &options);
}

inline bool VRayRenderer::saveImage(const char* fileName) const {
	return !Internal::VRay_VRayRenderer_saveVfbToFile(getNativeRenderer(), fileName, NULL);
}

inline bool VRayRenderer::saveImage(const std::string& fileName) const {
	return !Internal::VRay_VRayRenderer_saveVfbToFile(getNativeRenderer(), fileName.c_str(), NULL);
}

inline bool VRayRenderer::saveImage(const char* fileName, const ImageWriterOptions& options) const {
	return !Internal::VRay_VRayRenderer_saveVfbToFile(getNativeRenderer(), fileName, &options);
}

inline bool VRayRenderer::saveImage(const std::string& fileName, const ImageWriterOptions& options) const {
	return !Internal::VRay_VRayRenderer_saveVfbToFile(getNativeRenderer(), fileName.c_str(), &options);
}

inline bool VRayRenderer::isImageReady() const {
	return !!Internal::VRay_VRayRenderer_isImageReady(getNativeRenderer());
}

inline bool VRayRenderer::isRendering() const {
	return !!Internal::VRay_VRayRenderer_isRendering(getNativeRenderer());
}

inline bool VRayRenderer::isAborted() const {
	return !!Internal::VRay_VRayRenderer_isAborted(getNativeRenderer());
}

inline bool VRayRenderer::isSequenceDone() const {
	return !!Internal::VRay_VRayRenderer_isSequenceDone(getNativeRenderer());
}

inline bool VRayRenderer::waitForImageReady() const {
	return !!Internal::VRay_VRayRenderer_waitImageReady(getNativeRenderer(), -1);
}

inline bool VRayRenderer::waitForSequenceDone() const {
	return !!Internal::VRay_VRayRenderer_waitSequenceDone(getNativeRenderer(), -1);
}

inline bool VRayRenderer::waitForImageReady(const int timeout) const {
	return !!Internal::VRay_VRayRenderer_waitImageReady(getNativeRenderer(), timeout);
}

inline bool VRayRenderer::waitForSequenceDone(const int timeout) const {
	return !!Internal::VRay_VRayRenderer_waitSequenceDone(getNativeRenderer(), timeout);
}

inline std::vector<std::string> VRayRenderer::getPluginNames() const {
	return getStringVector(Internal::VRay_VRayRenderer_getPluginNames(getNativeRenderer()));
}

inline std::vector<InstanceId> VRayRenderer::getPluginIds() const {
	return getPluginIdsOfType(getNativeRenderer());
}

inline std::vector<InstanceId> VRayRenderer::getPluginIds(const char* pluginClassName) const {
	return getPluginIdsOfType(getNativeRenderer(), pluginClassName);
}

inline std::vector<InstanceId> VRayRenderer::getPluginIds(const std::string& pluginClassName) const {
	return getPluginIdsOfType(getNativeRenderer(), pluginClassName.c_str());
}

inline std::vector<Plugin> VRayRenderer::getPlugins() const {
	return getPluginsOfType<Plugin>(*const_cast<VRayRenderer*>(this), NULL);
}

inline std::vector<Plugin> VRayRenderer::getPlugins(const char* pluginClassName) const {
	return getPluginsOfType<Plugin>(*const_cast<VRayRenderer*>(this), pluginClassName);
}

inline std::vector<Plugin> VRayRenderer::getPlugins(const std::string& pluginClassName) const {
	return getPluginsOfType<Plugin>(*const_cast<VRayRenderer*>(this), pluginClassName.c_str());
}

template<class T>
inline std::vector<T> VRayRenderer::getPlugins() const {
	return getPluginsOfType<T>(*const_cast<VRayRenderer*>(this), T::getType());
}

inline std::vector<std::string> VRayRenderer::getAvailablePlugins() const {
	return getStringVector(Internal::VRay_VRayRenderer_getPluginClassNames(getNativeRenderer()));
}

inline Plugin VRayRenderer::getInstanceOf(const char* pluginType) const {
	InstanceId id = NO_ID;
	if (pluginType && *pluginType) {
		int count;
		InstanceId* ids = static_cast<InstanceId*>(Internal::VRay_VRayRenderer_getPluginIDsOfType(getNativeRenderer(), pluginType, &count));
		if (count > 0) id = ids[count - 1];
		Internal::C_memory_free(ids);
	}
	return Plugin(*const_cast<VRayRenderer*>(this), id);
}

inline Plugin VRayRenderer::getInstanceOf(const std::string& pluginType) const {
	return getInstanceOf(pluginType.c_str());
}

template<class T>
inline T VRayRenderer::getInstanceOf() const {
	return plugin_cast<T>(getInstanceOf(T::getType()));
}

inline Plugin VRayRenderer::getInstanceOrCreate(const char* pluginType) {
	int count;
	InstanceId* ids = static_cast<InstanceId*>(Internal::VRay_VRayRenderer_getPluginIDsOfType(getNativeRenderer(), pluginType, &count));
	if (count > 0) {
		InstanceId id = ids[count - 1];
		Internal::C_memory_free(ids);
		return Plugin(*this, id);
	}
	return newPlugin(pluginType);
}

inline Plugin VRayRenderer::getInstanceOrCreate(const std::string& pluginType) {
	return getInstanceOrCreate(pluginType.c_str());
}

template<class T>
T VRayRenderer::getInstanceOrCreate() {
	return plugin_cast<T>(getInstanceOrCreate(T::getType()));
}

inline Plugin VRayRenderer::getInstanceOrCreate(const char* pluginName, const char* pluginType) {
	if (!pluginName || !*pluginName) return getInstanceOrCreate(pluginType);
	int count;
	InstanceId* ids = static_cast<InstanceId*>(Internal::VRay_VRayRenderer_getPluginIDsOfType(getNativeRenderer(), pluginType, &count));
	if (count > 0) {
		InstanceId id = ids[count - 1];
		Internal::C_memory_free(ids);
		return Plugin(*this, id);
	}
	return newPlugin(pluginName, pluginType);
}

inline Plugin VRayRenderer::getInstanceOrCreate(const std::string& pluginName, const char* pluginType) {
	return getInstanceOrCreate(pluginName.c_str(), pluginType);
}

inline Plugin VRayRenderer::getInstanceOrCreate(const char* pluginName, const std::string& pluginType) {
	return getInstanceOrCreate(pluginName, pluginType.c_str());
}

inline Plugin VRayRenderer::getInstanceOrCreate(const std::string& pluginName, const std::string& pluginType) {
	return getInstanceOrCreate(pluginName.c_str(), pluginType.c_str());
}

template<class T>
inline T VRayRenderer::getInstanceOrCreate(const char* pluginName) {
	const char* pluginType = T::getType();
	return plugin_cast<T>(getInstanceOrCreate(pluginName, pluginType));
}

template<class T>
inline T VRayRenderer::getInstanceOrCreate(const std::string& pluginName) {
	return getInstanceOrCreate<T>(pluginName.c_str());
}

inline void VRayRenderer::setOnRenderStart(void(*callback)(VRayRenderer&, void*), const void* userData) {
	renderStartDelegate = Delegate1::from_function(callback, const_cast<void*>(userData));
	Internal::VRay_VRayRenderer_setRenderStartCallback(getNativeRenderer(), callback ? &renderStartCallback : NULL, this);
}

template<class T, void (T::*TMethod)(VRayRenderer&, void*)>
void VRayRenderer::setOnRenderStart(T& cls, const void* userData) {
	renderStartDelegate = Delegate1::from_method<T, TMethod>(&cls, const_cast<void*>(userData));
	Internal::VRay_VRayRenderer_setRenderStartCallback(getNativeRenderer(), &renderStartCallback, this);
}

inline void VRayRenderer::setOnImageReady(void(*callback)(VRayRenderer&, void*), const void* userData) {
	imageReadyDelegate = Delegate1::from_function(callback, const_cast<void*>(userData));
	Internal::VRay_VRayRenderer_setImageReadyCallback(getNativeRenderer(), callback ? &imageReadyCallback : NULL, this);
}

template<class T, void (T::*TMethod)(VRayRenderer&, void*)>
void VRayRenderer::setOnImageReady(T& cls, const void* userData) {
	imageReadyDelegate = Delegate1::from_method<T, TMethod>(&cls, const_cast<void*>(userData));
	Internal::VRay_VRayRenderer_setImageReadyCallback(getNativeRenderer(), &imageReadyCallback, this);
}

inline void VRayRenderer::setOnRendererClose(void(*callback)(VRayRenderer&, void*), const void* userData) {
	rendererCloseDelegate = Delegate1::from_function(callback, const_cast<void*>(userData));
	Internal::VRay_VRayRenderer_setRendererCloseCallback(getNativeRenderer(), callback ? &rendererCloseCallback : NULL, this);
}

template<class T, void (T::*TMethod)(VRayRenderer&, void*)>
void VRayRenderer::setOnRendererClose(T& cls, const void* userData) {
	rendererCloseDelegate = Delegate1::from_method<T, TMethod>(&cls, const_cast<void*>(userData));
	Internal::VRay_VRayRenderer_setRendererCloseCallback(getNativeRenderer(), &rendererCloseCallback, this);
}

inline void VRayRenderer::setOnRTImageUpdated(void(*callback)(VRayRenderer&, VRayImage* img, void*), const void* userData) {
	rtImageUpdatedDelegate = Delegate2::from_function(callback, const_cast<void*>(userData));
	Internal::VRay_VRayRenderer_setImageUpdatedCallback(getNativeRenderer(), callback ? &rtImageUpdatedCallback : NULL, this);
}

template<class T, void (T::*TMethod)(VRayRenderer&, VRayImage* img, void*)>
void VRayRenderer::setOnRTImageUpdated(T& cls, const void* userData) {
	rtImageUpdatedDelegate = Delegate2::from_method<T, TMethod>(&cls, const_cast<void*>(userData));
	Internal::VRay_VRayRenderer_setImageUpdatedCallback(getNativeRenderer(), &rtImageUpdatedCallback, this);
}

inline void VRayRenderer::setOnDumpMessage(void(*callback)(VRayRenderer&, const char* msg, int level, void*), const void* userData) {
	dumpMessageDelegate = Delegate3::from_function(callback, const_cast<void*>(userData));
	Internal::VRay_VRayRenderer_setDumpMessageCallback(getNativeRenderer(), callback ? &dumpMessageCallback : NULL, this);
}

template<class T, void (T::*TMethod)(VRayRenderer&, const char* msg, int level, void*)>
void VRayRenderer::setOnDumpMessage(T& cls, const void* userData) {
	dumpMessageDelegate = Delegate3::from_method<T, TMethod>(&cls, const_cast<void*>(userData));
	Internal::VRay_VRayRenderer_setDumpMessageCallback(getNativeRenderer(), &dumpMessageCallback, this);
}

inline void VRayRenderer::setOnBucketInit(void(*callback)(VRayRenderer&, int x, int y, int width, int height, const char* host, void*), const void* userData) {
	bucketInitDelegate = Delegate4::from_function(callback, const_cast<void*>(userData));
	Internal::VRay_VRayRenderer_setBucketInitCallback(getNativeRenderer(), callback ? &bucketInitCallback : NULL, this);
}

template<class T, void (T::*TMethod)(VRayRenderer&, int x, int y, int width, int height, const char* host, void*)>
void VRayRenderer::setOnBucketInit(T& cls, const void* userData) {
	bucketInitDelegate = Delegate4::from_method<T, TMethod>(&cls, const_cast<void*>(userData));
	Internal::VRay_VRayRenderer_setBucketInitCallback(getNativeRenderer(), &bucketInitCallback, this);
}

inline void VRayRenderer::setOnBucketReady(void(*callback)(VRayRenderer&, int x, int y, const char* host, VRayImage* img, void*), const void* userData) {
	bucketReadyDelegate = Delegate5::from_function(callback, const_cast<void*>(userData));
	Internal::VRay_VRayRenderer_setBucketReadyCallback(getNativeRenderer(), callback ? &bucketReadyCallback : NULL, this);
}

template<class T, void (T::*TMethod)(VRayRenderer&, int x, int y, const char* host, VRayImage* img, void*)>
void VRayRenderer::setOnBucketReady(T& cls, const void* userData) {
	bucketReadyDelegate = Delegate5::from_method<T, TMethod>(&cls, const_cast<void*>(userData));
	Internal::VRay_VRayRenderer_setBucketReadyCallback(getNativeRenderer(), &bucketReadyCallback, this);
}

inline void VRayRenderer::setOnBucketFailed(void(*callback)(VRayRenderer&, int x, int y, int width, int height, const char* host, void*), const void* userData) {
	bucketFailedDelegate = Delegate4::from_function(callback, const_cast<void*>(userData));
	Internal::VRay_VRayRenderer_setBucketFailedCallback(getNativeRenderer(), callback ? &bucketFailedCallback : NULL, this);
}

template<class T, void (T::*TMethod)(VRayRenderer&, int x, int y, int width, int height, const char* host, void*)>
void VRayRenderer::setOnBucketFailed(T& cls, const void* userData) {
	bucketFailedDelegate = Delegate4::from_method<T, TMethod>(&cls, const_cast<void*>(userData));
	Internal::VRay_VRayRenderer_setBucketFailedCallback(getNativeRenderer(), &bucketFailedCallback, this);
}

inline void VRayRenderer::setOnSequenceStart(void(*callback)(VRayRenderer&, void*), const void* userData) {
	sequenceStartDelegate = Delegate1::from_function(callback, const_cast<void*>(userData));
	Internal::VRay_VRayRenderer_setSequenceStartCallback(getNativeRenderer(), callback ? &sequenceStartCallback : NULL, this);
}

template<class T, void (T::*TMethod)(VRayRenderer&, void*)>
void VRayRenderer::setOnSequenceStart(T& cls, const void* userData) {
	sequenceStartDelegate = Delegate1::from_method<T, TMethod>(&cls, const_cast<void*>(userData));
	Internal::VRay_VRayRenderer_setSequenceStartCallback(getNativeRenderer(), &sequenceStartCallback, this);
}

inline void VRayRenderer::setOnSequenceDone(void(*callback)(VRayRenderer&, void*), const void* userData) {
	sequenceDoneDelegate = Delegate1::from_function(callback, const_cast<void*>(userData));
	Internal::VRay_VRayRenderer_setSequenceDoneCallback(getNativeRenderer(), callback ? &sequenceDoneCallback : NULL, this);
}

template<class T, void (T::*TMethod)(VRayRenderer&, void*)>
void VRayRenderer::setOnSequenceDone(T& cls, const void* userData) {
	sequenceDoneDelegate = Delegate1::from_method<T, TMethod>(&cls, const_cast<void*>(userData));
	Internal::VRay_VRayRenderer_setSequenceDoneCallback(getNativeRenderer(), &sequenceStartCallback, this);
}

inline void VRayRenderer::setOnProgress(void(*callback)(VRayRenderer&, const char* msg, int elementNumber, int elementsCount, void*), const void* userData) {
	progressDelegate = Delegate6::from_function(callback, const_cast<void*>(userData));
	Internal::VRay_VRayRenderer_setProgressCallback(getNativeRenderer(), callback ? &progressCallback : NULL, this);
}

template<class T, void (T::*TMethod)(VRayRenderer&, const char* msg, int elementNumber, int elementsCount, void*)>
void VRayRenderer::setOnProgress(T& cls, const void* userData) {
	progressDelegate = Delegate6::from_method<T, TMethod>(&cls, const_cast<void*>(userData));
	Internal::VRay_VRayRenderer_setProgressCallback(getNativeRenderer(), &progressCallback, this);
}

inline void VRayRenderer::setOnRenderViewChanged(void(*callback)(VRayRenderer&, const char* propName, void*), const void* userData) {
	renderViewChangedDelegate = Delegate7::from_function(callback, const_cast<void*>(userData));
	Internal::VRay_VRayRenderer_setRenderViewChangedCallback(getNativeRenderer(), callback ? &renderViewChangedCallback : NULL, this);
}

template<class T, void (T::*TMethod)(VRayRenderer&, const char* propName, void*)>
void VRayRenderer::setOnRenderViewChanged(T& cls, const void* userData) {
	renderViewChangedDelegate = Delegate7::from_method<T, TMethod>(&cls, const_cast<void*>(userData));
	Internal::VRay_VRayRenderer_setRenderViewChangedCallback(getNativeRenderer(), &renderViewChangedCallback, this);
}

inline void VRayRenderer::setOnRenderLast(void(*callback)(VRayRenderer&, bool isRendering, void*), const void* userData) {
	renderLastDelegate = Delegate8::from_function(callback, const_cast<void*>(userData));
	Internal::VRay_VRayRenderer_setRenderLastCallback(getNativeRenderer(), callback ? &renderLastCallback : NULL, this);
}

template<class T, void (T::*TMethod)(VRayRenderer&, bool isRendering, void*)>
void VRayRenderer::setOnRenderLast(T& cls, const void* userData) {
	renderLastDelegate = Delegate8::from_method<T, TMethod>(&cls, const_cast<void*>(userData));
	Internal::VRay_VRayRenderer_setRenderLastCallback(getNativeRenderer(), &renderLastCallback, this);
}

inline void VRayRenderer::setOnVFBClosed(void(*callback)(VRayRenderer&, void*), const void* userData) {
	vfbClosedDelegate = Delegate1::from_function(callback, const_cast<void*>(userData));
	Internal::VRay_VRayRenderer_setVFBClosedCallback(getNativeRenderer(), callback ? &vfbClosedCallback : NULL, this);
}

template<class T, void (T::*TMethod)(VRayRenderer&, void*)>
void VRayRenderer::setOnVFBClosed(T& cls, const void* userData) {
	vfbClosedDelegate = Delegate1::from_method<T, TMethod>(&cls, const_cast<void*>(userData));
	Internal::VRay_VRayRenderer_setVFBClosedCallback(getNativeRenderer(), &vfbClosedCallback, this);
}

inline float VRayRenderer::setRTImageUpdateDifference(float difference) {
	return Internal::VRay_VRayRenderer_setRTImageUpdateDifference(getNativeRenderer(), difference);
}

inline unsigned long VRayRenderer::setRTImageUpdateTimeout(unsigned long timeout) {
	return Internal::VRay_VRayRenderer_setRTImageUpdateTimeout(getNativeRenderer(), timeout);
}

inline void VRayRenderer::setKeepRTframesInCallback(bool keep) {
	Internal::VRay_VRayRenderer_setKeepRTframesInCallback(getNativeRenderer(), keep);
}

inline void VRayRenderer::setKeepBucketsInCallback(bool keep) {
	Internal::VRay_VRayRenderer_setKeepBucketsInCallback(getNativeRenderer(), keep);
}

inline Plugin VRayRenderer::getPlugin(const char *pluginName) const {
	return Plugin(*const_cast<VRayRenderer*>(this), pluginName);
}

inline Plugin VRayRenderer::getPlugin(const std::string &pluginName) const {
	return Plugin(*const_cast<VRayRenderer*>(this), pluginName);
}

inline Plugin VRayRenderer::getPlugin_internal(InstanceId pluginID) const {
	return pluginExists_internal(pluginID) ? Plugin(*const_cast<VRayRenderer*>(this), pluginID) : Plugin();
}

inline Plugin VRayRenderer::getPlugin(InstanceId pluginID) const {
	return getPlugin_internal(pluginID);
}

template<class T>
T VRayRenderer::getPlugin(const char *pluginName) const {
	return plugin_cast<T>(Plugin(*const_cast<VRayRenderer*>(this), pluginName));
}

template<class T>
T VRayRenderer::getPlugin(const std::string &pluginName) const {
	return plugin_cast<T>(Plugin(*const_cast<VRayRenderer*>(this), pluginName));
}

template<class T>
T VRayRenderer::getPlugin_internal(InstanceId pluginID) const {
	const char* type = getPluginType(pluginID);
	return plugin_cast<T>(!strcmp(type, T::getType()) ? Plugin(*const_cast<VRayRenderer*>(this), pluginID) : Plugin());
}

template<class T>
T VRayRenderer::getPlugin(InstanceId pluginID) const {
	return getPlugin_internal(pluginID);
}

inline PluginMeta VRayRenderer::getPluginMeta(const std::string &pluginClassName) const {
	return PluginMeta(this, pluginClassName);
}

inline Plugin VRayRenderer::newPlugin(const char* pluginName, const char* pluginType) {
	InstanceId pluginID;
	int err = Internal::VRay_VRayRenderer_newPlugin(getNativeRenderer(), pluginName, pluginType, autoCommit, &pluginID);
	return !err ? Plugin(*this, pluginID) : Plugin();
}

inline Plugin VRayRenderer::newPlugin(const std::string& pluginName, const std::string& pluginType) {
	return newPlugin(pluginName.c_str(), pluginType.c_str());
}

inline Plugin VRayRenderer::newPlugin(const char* pluginName, const std::string& pluginType) {
	return newPlugin(pluginName, pluginType.c_str());
}

inline Plugin VRayRenderer::newPlugin(const std::string& pluginName, const char* pluginType) {
	return newPlugin(pluginName.c_str(), pluginType);
}

inline Plugin VRayRenderer::newPlugin(const char* pluginType) {
	return newPlugin(NULL, pluginType);
}

inline Plugin VRayRenderer::newPlugin(const std::string &pluginType) {
	return newPlugin(pluginType.c_str());
}

template<class T>
T VRayRenderer::newPlugin(const char* pluginName) {
	const char* pluginType = T::getType();
	return plugin_cast<T>(newPlugin(pluginName, pluginType));
}

template<class T>
T VRayRenderer::newPlugin(const std::string& pluginName) {
	return newPlugin<T>(pluginName.c_str());
}

template<class T>
T VRayRenderer::newPlugin() {
	const char* pluginType = T::getType();
	return plugin_cast<T>(newPlugin(pluginType));
}

inline Plugin VRayRenderer::getOrCreatePlugin(const char* pluginName, const char* pluginType) {
	if (!pluginName || !*pluginName) return getInstanceOrCreate(pluginType);
	InstanceId pluginID;
	Internal::VRay_VRayRenderer_newPlugin(getNativeRenderer(), pluginName, pluginType, autoCommit, &pluginID);
	return Plugin(*this, pluginID);
}

inline Plugin VRayRenderer::getOrCreatePlugin(const std::string& pluginName, const char* pluginType) {
	return getOrCreatePlugin(pluginName.c_str(), pluginType);
}

inline Plugin VRayRenderer::getOrCreatePlugin(const char* pluginName, const std::string& pluginType) {
	return getOrCreatePlugin(pluginName, pluginType.c_str());
}

inline Plugin VRayRenderer::getOrCreatePlugin(const std::string& pluginName, const std::string& pluginType) {
	return getOrCreatePlugin(pluginName.c_str(), pluginType.c_str());
}

template<class T>
T VRayRenderer::getOrCreatePlugin(const char* pluginName) {
	const char* pluginType = T::getType();
	return plugin_cast<T>(getOrCreatePlugin(pluginName, pluginType));
}

inline Plugin VRayRenderer::pickPlugin(int x, int y) const {
	return Plugin(*const_cast<VRayRenderer*>(this), Internal::VRay_VRayRenderer_pickPluginByXY(getNativeRenderer(), x, y, -1));
}

inline Plugin VRayRenderer::pickPlugin(int x, int y, int timeout) const {
	return Plugin(*const_cast<VRayRenderer*>(this), Internal::VRay_VRayRenderer_pickPluginByXY(getNativeRenderer(), x, y, timeout));
}

inline std::vector<PluginDistancePair> VRayRenderer::pickPlugins(double x, double y, int maxcount, int timeout) const {
	InstanceId* list;
	int r = Internal::VRay_VRayRenderer_pickPluginsByXY(getNativeRenderer(), x, y, timeout, maxcount, &list);
	if (r < 0 || !*list) return std::vector<PluginDistancePair>();
	std::vector<PluginDistancePair> pairs((size_t)*list);
	for (size_t i = 0; i < *list; ++i) {
		pairs[i].plugin = Plugin(*const_cast<VRayRenderer*>(this), list[i + i + 1]);
		pairs[i].distance = reinterpret_cast<double*>(list)[i + i + 2];
	}
	Internal::C_memory_free(list);
	return pairs;
}

inline bool VRayRenderer::removePlugin(const Plugin& plugin) {
	return !Internal::VRay_VRayRenderer_deletePluginByID(getNativeRenderer(), plugin.id, autoCommit);
}

inline bool VRayRenderer::replacePlugin(const Plugin& oldPlugin, const Plugin& newPlugin) {
	return !Internal::VRay_VRayRenderer_replacePluginByID(getNativeRenderer(), oldPlugin.id, newPlugin.id, autoCommit);
}

#ifndef VRAY_NOTHROW
inline Plugin VRayRenderer::getPlugin(const char* pluginName, bool throw_flag) const {
	if (pluginExists(pluginName))
		return Plugin(*const_cast<VRayRenderer*>(this), pluginName);
	if (throw_flag)
		throw PluginUnavailableErr();
	return Plugin();
}

inline Plugin VRayRenderer::getPlugin(const std::string& pluginName, bool throw_flag) const {
	if (pluginExists(pluginName))
		return Plugin(*const_cast<VRayRenderer*>(this), pluginName);
	if (throw_flag)
		throw PluginUnavailableErr();
	return Plugin();
}

inline Plugin VRayRenderer::getPlugin(InstanceId pluginID, bool throw_flag) const {
	if (pluginExists_internal(pluginID))
		return Plugin(*const_cast<VRayRenderer*>(this), pluginID);
	if (throw_flag)
		throw PluginUnavailableErr();
	return Plugin();
}

inline Plugin VRayRenderer::pickPlugin(int x, int y, bool throw_flag) const {
	const InstanceId pluginID = Internal::VRay_VRayRenderer_pickPluginByXY(getNativeRenderer(), x, y, -1);

	if (throw_flag && pluginID == NO_ID)
		throw PluginUnavailableErr();

	return Plugin(*const_cast<VRayRenderer*>(this), pluginID);
}

inline Plugin VRayRenderer::pickPlugin(int x, int y, int timeout, bool throw_flag) const {
	const InstanceId pluginID = Internal::VRay_VRayRenderer_pickPluginByXY(getNativeRenderer(), x, y, timeout);

	if (throw_flag && pluginID == NO_ID)
		throw PluginUnavailableErr();

	return Plugin(*const_cast<VRayRenderer*>(this), pluginID);
}
#endif // VRAY_NOTHROW

inline bool VRayRenderer::pluginExists(const char* pluginName) const {
	return !!Internal::VRay_VRayRenderer_pluginExists(getNativeRenderer(), pluginName);
}

inline bool VRayRenderer::pluginExists(const std::string& pluginName) const {
	return !!Internal::VRay_VRayRenderer_pluginExists(getNativeRenderer(), pluginName.c_str());
}

inline bool VRayRenderer::pluginExists_internal(InstanceId pluginID) const {
	return !!Internal::VRay_VRayRenderer_pluginExistsByID(getNativeRenderer(), pluginID);
}

inline bool VRayRenderer::pluginExists(InstanceId pluginID) const {
	return pluginExists_internal(pluginID);
}

inline bool VRayRenderer::getAutoCommit() const {
	return autoCommit;
}

inline void VRayRenderer::setAutoCommit(bool autoCommit) {
	this->autoCommit = autoCommit;
}

inline VRayRendererNative* VRayRenderer::getNativeRenderer() const {
	return vfb.rendererNative;
}

inline RenderElements VRayRenderer::getRenderElements() {
	return RenderElements(*this);
}

// @param proxyPlugin A GeomMeshFile plugin instance to read the file path and parameters from.
// @param vertices Output vertex preview data. The array can be passed on directly to DirectX/OpenGL etc. (and the indices too)
// @param indices Output face index data. Each three consecutive elements in make up one triangle.
// @param countHint An optional number of triangles to return. The exact number of elements in 'vertices' may differ slightly.
// If countHint is not specified, proxyPlugin::num_preview_faces is used.
inline bool VRayRenderer::readProxyPreviewGeometry(const Plugin &proxyPlugin, VectorList &vertices, IntList &indices, int countHint) const {
	vertices.clear();
	indices.clear();
	if (proxyPlugin.getId() == NO_ID)
		return false;
	Vector *tmpVertices = NULL;
	int *tmpIndices = NULL;
	size_t vertCount = 0;
	size_t faceCount = 0;
	bool res = !!Internal::VRay_VRayRenderer_readProxyPreview(getNativeRenderer(), 0, proxyPlugin.getId(), (void **)&tmpVertices, (void **)&tmpIndices, &vertCount, &faceCount, countHint);
	if (res && tmpVertices && tmpIndices) {
		VectorList verticesTmp(tmpVertices, tmpVertices + vertCount);
		vertices.swap(verticesTmp);
		IntList indicesTmp(tmpIndices, tmpIndices + 3 * faceCount);
		indices.swap(indicesTmp);
	}
	Internal::C_memory_free(tmpVertices);
	Internal::C_memory_free(tmpIndices);
	return res;
}

// @param proxyPlugin A GeomMeshFile plugin instance to read the file path and parameters from.
// @param vertices Output vertex preview data. Consecutive vertices form hair strands. The i-th strand has lengths[i] consecutive vertices.
// @param lengths Output hair strand length data. Each strand can have a different number of vertices.
// @param countHint The number of hair strands to return. The exact number of elements in 'lengths' may differ slightly.
// If countHint is not specified, proxyPlugin::num_preview_faces is used.
inline bool VRayRenderer::readProxyPreviewHair(const Plugin &proxyPlugin, VectorList &vertices, IntList &lengths, int countHint) const {
	vertices.clear();
	lengths.clear();
	if (proxyPlugin.getId() == NO_ID)
		return false;
	Vector *tmpVertices = NULL;
	int *tmpLengths = NULL;
	size_t vertCount = 0;
	size_t hairCount = 0;
	bool res = !!Internal::VRay_VRayRenderer_readProxyPreview(getNativeRenderer(), 1, proxyPlugin.getId(), (void **)&tmpVertices, (void **)&tmpLengths, &vertCount, &hairCount, countHint);
	if (res && tmpVertices && tmpLengths) {
		VectorList verticesTmp(tmpVertices, tmpVertices + vertCount);
		vertices.swap(verticesTmp);
		IntList lengthsTmp(tmpLengths, tmpLengths + hairCount);
		lengths.swap(lengthsTmp);
	}
	Internal::C_memory_free(tmpVertices);
	Internal::C_memory_free(tmpLengths);
	return res;
}

inline bool VRayRenderer::readProxyPreviewParticles(const Plugin &proxyPlugin, VectorList &positions, int countHint) const {
	positions.clear();
	if (proxyPlugin.getId() == NO_ID)
		return false;
	Vector *tmpPositions = NULL;
	size_t particleCount = 0;
	bool res = !!Internal::VRay_VRayRenderer_readProxyPreview(getNativeRenderer(), 2, proxyPlugin.getId(), (void **)&tmpPositions, NULL, &particleCount, NULL, countHint);
	if (res && tmpPositions) {
		VectorList positionsTmp(tmpPositions, tmpPositions + particleCount);
		positions.swap(positionsTmp);
	}
	Internal::C_memory_free(tmpPositions);
	return res;
}

inline bool VRayRenderer::readProxyFullData(const Plugin &proxyPlugin, ProxyReadData &readData) const {
	if (proxyPlugin.getId() == NO_ID)
		return false;

	void *handle = NULL;
	bool res = !!Internal::VRay_VRayRenderer_readProxyFullPrepare(getNativeRenderer(), proxyPlugin.getId(), &readData, &handle);
	if (res == false)
		return false;
	readData.allocateArrays();
	res = !!Internal::VRay_VRayRenderer_readProxyFullFinalize(getNativeRenderer(), &readData, handle);
	return res;
}

inline bool VRayRenderer::createProxyFileMesh(const Plugin &geomStaticMeshPlugin, const Transform *transform, const ProxyCreateParams &params) {
	InstanceId id = geomStaticMeshPlugin.getId();
	bool success = (0 == Internal::VRay_VRayRenderer_createProxyFile(getNativeRenderer(), 0, 1, &id, reinterpret_cast<const float *>(transform), &params));
	return success;
}

inline bool VRayRenderer::createCombinedProxyFileMesh(const std::vector<Plugin> &geomStaticMeshPlugins, const std::vector<Transform> *transforms, const ProxyCreateParams &params) {
	std::vector<InstanceId> ids;
	ids.reserve(geomStaticMeshPlugins.size());
	for (size_t i = 0; i < geomStaticMeshPlugins.size(); ++i)
		ids.push_back(geomStaticMeshPlugins[i].getId());
	const float *tms = NULL;
	if (transforms && transforms->size()) {
		if (transforms->size() != geomStaticMeshPlugins.size())
			return false;
		tms = reinterpret_cast<const float *>(&(*transforms)[0]);
	}
	bool success = (0 == Internal::VRay_VRayRenderer_createProxyFile(getNativeRenderer(), 0, (int)geomStaticMeshPlugins.size(), &ids[0], tms, &params));
	return success;
}

inline std::vector<InstanceId> VRayRenderer::getPluginIdsOfType(VRayRendererNative *rendererNative, const char* pluginType) {
	int count;
	InstanceId* ids = static_cast<InstanceId*>(Internal::VRay_VRayRenderer_getPluginIDsOfType(rendererNative, pluginType, &count));
	std::vector<InstanceId> vids(ids, ids + count);
	Internal::C_memory_free(ids);
	return vids;
}

template<class T>
inline std::vector<T> VRayRenderer::getPluginsOfType(VRayRenderer &renderer, const char* pluginType) {
	int count;
	InstanceId* ids = static_cast<InstanceId*>(Internal::VRay_VRayRenderer_getPluginIDsOfType(renderer.getNativeRenderer(), pluginType, &count));
	std::vector<T> plugins(count);
	InstanceId* idsEnd = ids + count;
	for (int i = 0; i < count; ++i) {
		plugins[i] = plugin_cast<T>(Plugin(renderer, ids[i]));
	}
	Internal::C_memory_free(ids);
	return plugins;
}

inline std::vector<std::string> VRayRenderer::getStringVector(void *pvector) {
	std::vector<std::string> vector;
	if (pvector) {
		unsigned size = Internal::getVectorSize(pvector);
		for (unsigned i = 0; i < size; ++i)
			vector.push_back(Internal::getVectorElement(pvector, i));
		Internal::releaseVector(pvector);
	}
	return vector;
}

inline std::vector<std::string> VRayRenderer::getPluginPropertyNames(const char* pluginClassName) const {
	return getStringVector(Internal::VRay_VRayRenderer_getClassPropertyNames(getNativeRenderer(), pluginClassName));
}

inline const char* VRayRenderer::getPluginType(const char* name) const {
	const char* const type = Internal::VRay_VRayRenderer_getPluginType(getNativeRenderer(), name);
	return type ? type : "";
}

inline const char* VRayRenderer::getPluginType(InstanceId pluginID) const {
	const char* const type = Internal::VRay_VRayRenderer_getPluginTypeByID(getNativeRenderer(), pluginID);
	return type ? type : "";
}

inline Type VRayRenderer::getPluginPropertyRuntimeType(const char* pluginName, const char* propertyName) const {
	return Internal::VRay_VRayRenderer_getPropertyType(getNativeRenderer(), pluginName, propertyName);
}

inline Type VRayRenderer::getPluginPropertyRuntimeType(InstanceId pluginID, const char* propertyName) const {
	return Internal::VRay_VRayRenderer_getPropertyTypeByID(getNativeRenderer(), pluginID, propertyName);
}

inline int VRayRenderer::getPluginPropertyElementsCount(InstanceId pluginID, const char* propertyName) const {
	return Internal::VRay_VRayRenderer_getPropertyElementsCountByID(getNativeRenderer(), pluginID, propertyName);
}

inline const void* VRayRenderer::getPluginPropertyDefinition(const char* pluginName, const char* propertyName) const {
	return Internal::VRay_VRayRenderer_getPropertyDefinition(getNativeRenderer(), pluginName, propertyName);
}

inline const void* VRayRenderer::getPluginPropertyDefinition(InstanceId pluginID, const char* propertyName) const {
	return Internal::VRay_VRayRenderer_getPropertyDefinitionByID(getNativeRenderer(), pluginID, propertyName);
}

inline const void* VRayRenderer::getPluginPropertyDefinitionFromClass(const char* pluginClassName, const char* propertyName) const {
	return Internal::VRay_VRayRenderer_getPropertyDefinitionFromClass(getNativeRenderer(), pluginClassName, propertyName);
}

inline std::string VRayRenderer::getPluginPropertyValueAsString(const char* pluginName, const char* propertyName) const {
	char *pValue = Internal::VRay_VRayRenderer_getPropertyValue(getNativeRenderer(), pluginName, propertyName);
	std::string retval(pValue ? pValue : "");
	Internal::C_memory_free(pValue);
	return retval;
}

inline std::string VRayRenderer::getPluginPropertyValueAsString(InstanceId pluginID, const char* propertyName, bool& ok) const {
	char *pValue = Internal::VRay_VRayRenderer_getPropertyValueByID(getNativeRenderer(), pluginID, propertyName);
	ok = !!pValue;
	std::string retval(pValue ? pValue : "");
	Internal::C_memory_free(pValue);
	return retval;
}

inline bool VRayRenderer::setPluginPropertyValueAsString(InstanceId pluginID, const char* propertyName, const char* value) const {
	return !Internal::VRay_VRayRenderer_setPropertyValueByID(getNativeRenderer(), pluginID, propertyName, value, autoCommit);
}

inline bool VRayRenderer::setPluginPropertyValueAsStringAtTime(InstanceId pluginID, const char* propertyName, const char* value, double time) const {
	return !Internal::VRay_VRayRenderer_setPropertyValueAtTimeByID(getNativeRenderer(), pluginID, propertyName, value, time, autoCommit);
}

inline bool VRayRenderer::getPluginPropertyBool(const char* pluginName, const char* propertyName, bool& ok) const {
	Bool result;
	ok = !!Internal::VRay_getPropertyBool(getNativeRenderer(), pluginName, propertyName, &result);
	return !!result;
}

inline bool VRayRenderer::getPluginPropertyBool(InstanceId pluginID, const char* propertyName, bool& ok) const {
	Bool result;
	ok = !!Internal::VRay_getPropertyBoolByID(getNativeRenderer(), pluginID, propertyName, &result);
	return !!result;
}

inline int VRayRenderer::getPluginPropertyInt(const char* pluginName, const char* propertyName, bool& ok) const {
	int result;
	ok = !!Internal::VRay_getPropertyInt(getNativeRenderer(), pluginName, propertyName, &result);
	return result;
}

inline int VRayRenderer::getPluginPropertyInt(InstanceId pluginID, const char* propertyName, bool& ok) const {
	int result;
	ok = !!Internal::VRay_getPropertyIntByID(getNativeRenderer(), pluginID, propertyName, &result);
	return result;
}

inline float VRayRenderer::getPluginPropertyFloat(const char* pluginName, const char* propertyName, bool& ok) const {
	float result;
	ok = !!Internal::VRay_getPropertyFloat(getNativeRenderer(), pluginName, propertyName, &result);
	return result;
}

inline float VRayRenderer::getPluginPropertyFloat(InstanceId pluginID, const char* propertyName, bool& ok) const {
	float result;
	ok = !!Internal::VRay_getPropertyFloatByID(getNativeRenderer(), pluginID, propertyName, &result);
	return result;
}

inline Color VRayRenderer::getPluginPropertyColor(const char* pluginName, const char* propertyName, bool& ok) const {
	Color *pcolor = Internal::VRay_getPropertyColor(getNativeRenderer(), pluginName, propertyName);
	if (pcolor) {
		Color color(*pcolor);
		Internal::C_memory_free(pcolor);
		ok = true;
		return color;
	}
	ok = false;
	return Color();
}

inline Color VRayRenderer::getPluginPropertyColor(InstanceId pluginID, const char* propertyName, bool& ok) const {
	Color *pcolor = Internal::VRay_getPropertyColorByID(getNativeRenderer(), pluginID, propertyName);
	if (pcolor) {
		Color color(*pcolor);
		Internal::C_memory_free(pcolor);
		ok = true;
		return color;
	}
	ok = false;
	return Color();
}

inline AColor VRayRenderer::getPluginPropertyAColor(const char* pluginName, const char* propertyName, bool& ok) const {
	AColor *pacolor = Internal::VRay_getPropertyAColor(getNativeRenderer(), pluginName, propertyName);
	if (pacolor) {
		AColor acolor(*pacolor);
		Internal::C_memory_free(pacolor);
		ok = true;
		return acolor;
	}
	ok = false;
	return AColor();
}

inline AColor VRayRenderer::getPluginPropertyAColor(InstanceId pluginID, const char* propertyName, bool& ok) const {
	AColor *pacolor = Internal::VRay_getPropertyAColorByID(getNativeRenderer(), pluginID, propertyName);
	if (pacolor) {
		AColor acolor(*pacolor);
		Internal::C_memory_free(pacolor);
		ok = true;
		return acolor;
	}
	ok = false;
	return AColor();
}

inline Vector VRayRenderer::getPluginPropertyVector(const char* pluginName, const char* propertyName, bool& ok) const {
	Vector *pvector = Internal::VRay_getPropertyVector(getNativeRenderer(), pluginName, propertyName);
	if (pvector) {
		Vector vector(*pvector);
		Internal::C_memory_free(pvector);
		ok = true;
		return vector;
	}
	ok = false;
	return Vector();
}

inline Vector VRayRenderer::getPluginPropertyVector(InstanceId pluginID, const char* propertyName, bool& ok) const {
	Vector *pvector = Internal::VRay_getPropertyVectorByID(getNativeRenderer(), pluginID, propertyName);
	if (pvector) {
		Vector vector(*pvector);
		Internal::C_memory_free(pvector);
		ok = true;
		return vector;
	}
	ok = false;
	return Vector();
}

inline Matrix VRayRenderer::getPluginPropertyMatrix(const char* pluginName, const char* propertyName, bool& ok) const {
	Matrix *pmatrix = Internal::VRay_getPropertyMatrix(getNativeRenderer(), pluginName, propertyName);
	if (pmatrix) {
		Matrix matrix(*pmatrix);
		Internal::C_memory_free(pmatrix);
		ok = true;
		return matrix;
	}
	ok = false;
	return Matrix();
}

inline Matrix VRayRenderer::getPluginPropertyMatrix(InstanceId pluginID, const char* propertyName, bool& ok) const {
	Matrix *pmatrix = Internal::VRay_getPropertyMatrixByID(getNativeRenderer(), pluginID, propertyName);
	if (pmatrix) {
		Matrix matrix(*pmatrix);
		Internal::C_memory_free(pmatrix);
		ok = true;
		return matrix;
	}
	ok = false;
	return Matrix();
}

inline Transform VRayRenderer::getPluginPropertyTransform(const char* pluginName, const char* propertyName, bool& ok) const {
	Transform *ptransform = Internal::VRay_getPropertyTransform(getNativeRenderer(), pluginName, propertyName);
	if (ptransform) {
		Transform transform(*ptransform);
		Internal::C_memory_free(ptransform);
		ok = true;
		return transform;
	}
	ok = false;
	return Transform();
}

inline Transform VRayRenderer::getPluginPropertyTransform(InstanceId pluginID, const char* propertyName, bool& ok) const {
	Transform *ptransform = Internal::VRay_getPropertyTransformByID(getNativeRenderer(), pluginID, propertyName);
	if (ptransform) {
		Transform transform(*ptransform);
		Internal::C_memory_free(ptransform);
		ok = true;
		return transform;
	}
	ok = false;
	return Transform();
}

inline Plugin VRayRenderer::getPluginPropertyPluginProperty(InstanceId pluginID, const char* propertyName, const char** outPropertyName, bool& ok) const {
	InstanceId outID;
	ok = !!Internal::VRay_getPropertyObjectByID(getNativeRenderer(), pluginID, propertyName, &outID, outPropertyName);
	return Plugin(*const_cast<VRayRenderer*>(this), outID);
}

inline Value VRayRenderer::getPluginPropertyValue(InstanceId pluginID, const char* propertyName, bool& ok) const {
	int *plist, size;
	Internal::VRay_getPropertyByteArrayByID(getNativeRenderer(), pluginID, propertyName, &plist, &size);
	if (plist) {
		Internal::BinaryValueParser parser(this, plist);
		Value result = parser.get();
		Internal::C_memory_free(plist);
		ok = true;
		return result;
	}
	ok = false;
	return Value();
}

template<typename T>
T VRayRenderer::getPluginPropertyTyped(InstanceId pluginID, const char* propertyName, bool& ok) const {
	T result;
	ok = false;
	int *plist, size;
	Internal::VRay_getPropertyByteArrayByID(getNativeRenderer(), pluginID, propertyName, &plist, &size);
	if (plist) {
		Internal::BinaryValueParser parser(this, plist);
		ok = parser.get(result);
		Internal::C_memory_free(plist);
	}
	return result;
}

inline bool VRayRenderer::isPropertyAnimated(InstanceId pluginID, const char* propertyName) const {
	return !!Internal::VRay_VRayRenderer_isPropertyAnimatedByID(getNativeRenderer(), pluginID, propertyName);
}

inline bool VRayRenderer::setPluginPropertyValue(InstanceId pluginID, const char* propertyName, const void *pval) const {
	return !Internal::VRay_VRayRenderer_setPropertyBytesValueByID(getNativeRenderer(), pluginID, propertyName, pval, autoCommit);
}

inline bool VRayRenderer::setPluginPropertyValueAtTime(InstanceId pluginID, const char* propertyName, const void *pval, double time) const {
	return !Internal::VRay_VRayRenderer_setPropertyBytesValueAtTimeByID(getNativeRenderer(), pluginID, propertyName, pval, time, autoCommit);
}

inline bool VRayRenderer::setPluginPropertyValueRaw(InstanceId pluginID, const char* propertyName, const void *pval, size_t count) const {
	return !Internal::VRay_VRayRenderer_setPropertyBytesValueRawByID(getNativeRenderer(), pluginID, propertyName, pval, count, autoCommit);
}

inline bool VRayRenderer::setPluginPropertyValueRawAtTime(InstanceId pluginID, const char* propertyName, const void *pval, size_t count, double time) const {
	return !Internal::VRay_VRayRenderer_setPropertyBytesValueRawAtTimeByID(getNativeRenderer(), pluginID, propertyName, pval, count, time, autoCommit);
}

#ifdef VRAY_SDK_INTEROPERABILITY
inline bool VRayRenderer::setPluginPropertyValueSDKInterop(InstanceId pluginID, const char* propertyName, const void *pval, int type) const {
	return !Internal::VRay_VRayRenderer_setPropertyValueByID_interop(getNativeRenderer(), pluginID, propertyName, pval, type, autoCommit);
}

inline bool VRayRenderer::setPluginPropertyValueAtTimeSDKInterop(InstanceId pluginID, const char* propertyName, const void *pval, int type, double time) const {
	return !Internal::VRay_VRayRenderer_setPropertyValueAtTimeByID_interop(getNativeRenderer(), pluginID, propertyName, pval, type, time, autoCommit);
}
#endif // VRAY_SDK_INTEROPERABILITY

inline Plugin VRayRenderer::newRenderElementPlugin(int type, const char *instanceName, const char *displayName) {
	InstanceId pluginID;
	int err = Internal::VRay_newRenderElementPlugin(getNativeRenderer(), type, instanceName, displayName, &pluginID);
	return !err ? Plugin(*this, pluginID) : Plugin();
}

inline Plugin VRayRenderer::getRenderElementPlugin(int type) const {
	InstanceId id = Internal::VRay_getRenderElementPlugin(getNativeRenderer(), type);
	if (id == NO_ID)
		return Plugin();
	return Plugin(*const_cast<VRayRenderer*>(this), id);
}

inline std::vector<Plugin> VRayRenderer::getRenderElementPluginsList(int type) const {
	std::vector<Plugin> result;
	int count = 0;
	InstanceId* idArray = Internal::VRay_getRenderElementPluginsList(getNativeRenderer(), type, &count);
	if (idArray) {
		result.reserve(count);
		for (int i = 0; i < count; ++i) {
			result.push_back(Plugin(*const_cast<VRayRenderer*>(this), idArray[i]));
		}
		Internal::C_memory_free(idArray);
	}
	return result;
}

inline bool VRayRenderer::getRenderElementInfo(InstanceId pluginID, RenderElement::Info& chInfo) const {
	return !!Internal::VRay_getRenderElementInfo(getNativeRenderer(), pluginID, &chInfo);
}

inline size_t VRayRenderer::getRenderElementData(InstanceId pluginID, InstanceId alphaPluginID, RenderElement::PixelFormat format, bool rgbOrder, const ImageRegion* rgn, void** buf) const {
	return Internal::VRay_getRenderElementData(getNativeRenderer(), pluginID, alphaPluginID, format, rgbOrder, rgn, buf);
}

inline VRayImage* VRayRenderer::getRenderElementAsImage(InstanceId pluginID, const ImageRegion* rgn) const {
	return Internal::VRay_getRenderElementAsImage(getNativeRenderer(), pluginID, rgn);
}

inline size_t VRayRenderer::getIrradianceMapSize() { return Internal::VRay_VRayRenderer_getIrradianceMapSize(getNativeRenderer()); }
inline bool VRayRenderer::saveIrradianceMapFile(const std::string &fileName) { return saveIrradianceMapFile(fileName.c_str()); }
inline bool VRayRenderer::saveIrradianceMapFile(const char* fileName) {
	return !!Internal::VRay_VRayRenderer_saveIrradianceMapFile(getNativeRenderer(), fileName);
}

inline size_t VRayRenderer::getLightCacheSize() { return Internal::VRay_VRayRenderer_getLightCacheSize(getNativeRenderer()); }
inline bool VRayRenderer::saveLightCacheFile(const std::string &fileName) { return saveLightCacheFile(fileName.c_str()); }
inline bool VRayRenderer::saveLightCacheFile(const char* fileName) {
	return !!Internal::VRay_VRayRenderer_saveLightCacheFile(getNativeRenderer(), fileName);
}

inline size_t VRayRenderer::getPhotonMapSize() { return Internal::VRay_VRayRenderer_getPhotonMapSize(getNativeRenderer()); }
inline bool VRayRenderer::savePhotonMapFile(const std::string &fileName) { return savePhotonMapFile(fileName.c_str()); }
inline bool VRayRenderer::savePhotonMapFile(const char* fileName) {
	return !!Internal::VRay_VRayRenderer_savePhotonMapFile(getNativeRenderer(), fileName);
}

inline size_t VRayRenderer::getCausticsSize() { return Internal::VRay_VRayRenderer_getCausticsSize(getNativeRenderer()); }
inline bool VRayRenderer::saveCausticsFile(const std::string &fileName) { return saveCausticsFile(fileName.c_str()); }
inline bool VRayRenderer::saveCausticsFile(const char* fileName) {
	return !!Internal::VRay_VRayRenderer_saveCausticsFile(getNativeRenderer(), fileName);
}

namespace Internal {
	struct OCLDevInfo {
		char *name;
		int enabled;
		int vendor;
		int deviceType;
		unsigned long long totalMemory;
		unsigned long long freeMemory;
		int deviceIdx;
		int numConnectedDisplays;
	};

	inline std::vector<ComputeDeviceInfo> getComputeDevicesCommon(void *serialized, int count) {
		std::vector<ComputeDeviceInfo> result;
		if (count <= 0)
			return result;
		result.resize(count);
		OCLDevInfo *infos = reinterpret_cast<OCLDevInfo *>(serialized);
		for (int i = 0; i < count; ++i) {
			result[i].name = infos[i].name;
			result[i].enabled = infos[i].enabled;
			result[i].type = (ComputeDeviceInfo::ComputeDeviceType) infos[i].deviceType;
			result[i].totalMemoryMB = infos[i].totalMemory;
			result[i].numConnectedDisplays = infos[i].numConnectedDisplays;
		}
		C_memory_free(infos);
		return result;
	}
}

inline std::vector<ComputeDeviceInfo> VRayRenderer::getComputeDevicesCUDA() {
	void *serialized = NULL;
	int count = Internal::VRay_VRayRenderer_getDeviceListCUDA(getNativeRenderer(), &serialized);
	return Internal::getComputeDevicesCommon(serialized, count);
}

inline std::vector<ComputeDeviceInfo> VRayRenderer::getComputeDevicesOpenCL() {
	void *serialized = NULL;
	int count = Internal::VRay_VRayRenderer_getDeviceListOpenCL(getNativeRenderer(), &serialized);
	return Internal::getComputeDevicesCommon(serialized, count);
}

inline std::vector<ComputeDeviceInfo> VRayRenderer::getComputeDevicesCurrentEngine() {
	RendererOptions::RenderMode mode = getRenderMode();
	switch (mode) {
		case RendererOptions::RENDER_MODE_RT_GPU_OPENCL:
		case RendererOptions::RENDER_MODE_PRODUCTION_OPENCL:
			return getComputeDevicesOpenCL();
		case RendererOptions::RENDER_MODE_RT_GPU_CUDA:
		case RendererOptions::RENDER_MODE_PRODUCTION_CUDA:
			return getComputeDevicesCUDA();
		default: break;
	}
	return std::vector<ComputeDeviceInfo>();
}

inline bool VRayRenderer::setComputeDevicesCUDA(const std::vector<int> &indices) {
	if (indices.size() > 0)
		return 0 == Internal::VRay_VRayRenderer_setDeviceListCUDA(getNativeRenderer(), &indices[0], indices.size());
	else
		return 0 == Internal::VRay_VRayRenderer_setDeviceListCUDA(getNativeRenderer(), NULL, indices.size());
}

inline bool VRayRenderer::setComputeDevicesOpenCL(const std::vector<int> &indices) {
	if (indices.size() > 0)
		return 0 == Internal::VRay_VRayRenderer_setDeviceListOpenCL(getNativeRenderer(), &indices[0], indices.size());
	else
		return 0 == Internal::VRay_VRayRenderer_setDeviceListOpenCL(getNativeRenderer(), NULL, indices.size());
}

inline bool VRayRenderer::setComputeDevicesCurrentEngine(const std::vector<int> &indices) {
	RendererOptions::RenderMode mode = getRenderMode();
	switch (mode) {
		case RendererOptions::RENDER_MODE_RT_GPU_OPENCL:
		case RendererOptions::RENDER_MODE_PRODUCTION_OPENCL:
			return setComputeDevicesCUDA(indices);
		case RendererOptions::RENDER_MODE_RT_GPU_CUDA:
		case RendererOptions::RENDER_MODE_PRODUCTION_CUDA:
			return setComputeDevicesCUDA(indices);
		default: break;
	}
	return false;
}

inline Plugin::Plugin(VRayRenderer& renderer, const char* name) :
	pRenderer(&renderer), id(renderer.getPluginId_internal(name)) { }

inline Plugin::Plugin(VRayRenderer& renderer, const std::string& name) :
	pRenderer(&renderer), id(renderer.getPluginId_internal(name.c_str())) { }

inline Plugin::Plugin(VRayRenderer& renderer, InstanceId id) :
	pRenderer(&renderer), id(id) { }

inline Plugin::Plugin(const Plugin &plugin) :
	pRenderer(plugin.pRenderer), id(plugin.id) { }

inline Plugin& Plugin::operator=(const Plugin &plugin) {
	pRenderer = plugin.pRenderer;
	id = plugin.id;
	return *this;
}

inline bool Plugin::operator==(const Plugin &plugin) const {
	return id == plugin.id;
}

inline void Plugin::swap(Plugin &plugin) {
	std::swap(pRenderer, plugin.pRenderer);
	std::swap(id, plugin.id);
}

inline VRayRenderer* Plugin::getRenderer() const {
	return const_cast<VRayRenderer*>(pRenderer);
}

inline Plugin::operator bool() const {
	return pRenderer && id && id != NO_ID;
}

inline bool Plugin::isValid() const {
	return operator bool() && pRenderer->pluginExists_internal(id);
}

inline const char* Plugin::getName() const {
	return pRenderer->getPluginName_internal(id);
}

inline const char* Plugin::getType() const {
	return pRenderer->getPluginType(id);
}

inline InstanceId Plugin::getId() const {
	return id;
}

inline PluginMeta Plugin::getMeta() const {
	return PluginMeta(*this);
}

inline PropertyRuntimeMeta Plugin::getPropertyRuntimeMeta(const std::string &propertyName) const {
	return PropertyRuntimeMeta(*this, propertyName);
}

inline std::string Plugin::getValueAsString(const char* propertyName) const {
	bool ok;
	return pRenderer->getPluginPropertyValueAsString(id, propertyName, ok);
}

inline std::string Plugin::getValueAsString(const std::string& propertyName) const {
	return getValueAsString(propertyName.c_str());
}

inline std::string Plugin::getValueAsString(const char* propertyName, bool& ok) const {
	return pRenderer->getPluginPropertyValueAsString(id, propertyName, ok);
}

inline std::string Plugin::getValueAsString(const std::string& propertyName, bool& ok) const {
	return getValueAsString(propertyName.c_str(), ok);
}

inline bool Plugin::setValueAsString(const char* propertyName, const char* value) {
	return pRenderer->setPluginPropertyValueAsString(id, propertyName, value);
}

inline bool Plugin::setValueAsStringAtTime(const char* propertyName, const char* value, double time) {
	return pRenderer->setPluginPropertyValueAsStringAtTime(id, propertyName, value, time);
}

inline bool Plugin::setValueAsString(const char* propertyName, const std::string& value) {
	return setValueAsString(propertyName, value.c_str());
}

inline bool Plugin::setValueAsStringAtTime(const char* propertyName, const std::string& value, double time) {
	return setValueAsStringAtTime(propertyName, value.c_str(), time);
}

inline bool Plugin::setValueAsString(const std::string& propertyName, const char* value) {
	return setValueAsString(propertyName.c_str(), value);
}

inline bool Plugin::setValueAsStringAtTime(const std::string& propertyName, const char* value, double time) {
	return setValueAsStringAtTime(propertyName.c_str(), value, time);
}

inline bool Plugin::setValueAsString(const std::string& propertyName, const std::string& value) {
	return setValueAsString(propertyName.c_str(), value.c_str());
}

inline bool Plugin::setValueAsStringAtTime(const std::string& propertyName, const std::string& value, double time) {
	return setValueAsStringAtTime(propertyName.c_str(), value.c_str(), time);
}

inline bool Plugin::getBool(const char* propertyName) const {
	bool ok;
	return pRenderer->getPluginPropertyBool(id, propertyName, ok);
}

inline bool Plugin::getBool(const std::string &propertyName) const {
	return getBool(propertyName.c_str());
}

inline bool Plugin::getBool(const char* propertyName, bool& ok) const {
	return pRenderer->getPluginPropertyBool(id, propertyName, ok);
}

inline bool Plugin::getBool(const std::string &propertyName, bool& ok) const {
	return getBool(propertyName.c_str(), ok);
}

inline int Plugin::getInt(const char* propertyName) const {
	bool ok;
	return pRenderer->getPluginPropertyInt(id, propertyName, ok);
}

inline int Plugin::getInt(const std::string &propertyName) const {
	return getInt(propertyName.c_str());
}

inline int Plugin::getInt(const char* propertyName, bool& ok) const {
	return pRenderer->getPluginPropertyInt(id, propertyName, ok);
}

inline int Plugin::getInt(const std::string &propertyName, bool& ok) const {
	return getInt(propertyName.c_str(), ok);
}

inline float Plugin::getFloat(const char* propertyName) const {
	bool ok;
	return pRenderer->getPluginPropertyFloat(id, propertyName, ok);
}

inline float Plugin::getFloat(const std::string& propertyName) const {
	return getFloat(propertyName.c_str());
}

inline float Plugin::getFloat(const char* propertyName, bool& ok) const {
	return pRenderer->getPluginPropertyFloat(id, propertyName, ok);
}

inline float Plugin::getFloat(const std::string& propertyName, bool& ok) const {
	return getFloat(propertyName.c_str(), ok);
}

inline Color Plugin::getColor(const char* propertyName) const {
	bool ok;
	return pRenderer->getPluginPropertyColor(id, propertyName, ok);
}

inline Color Plugin::getColor(const std::string& propertyName) const {
	return getColor(propertyName.c_str());
}

inline Color Plugin::getColor(const char* propertyName, bool& ok) const {
	return pRenderer->getPluginPropertyColor(id, propertyName, ok);
}

inline Color Plugin::getColor(const std::string& propertyName, bool& ok) const {
	return getColor(propertyName.c_str(), ok);
}

inline AColor Plugin::getAColor(const char* propertyName) const {
	bool ok;
	return pRenderer->getPluginPropertyAColor(id, propertyName, ok);
}

inline AColor Plugin::getAColor(const std::string& propertyName) const {
	return getAColor(propertyName.c_str());
}

inline AColor Plugin::getAColor(const char* propertyName, bool& ok) const {
	return pRenderer->getPluginPropertyAColor(id, propertyName, ok);
}

inline AColor Plugin::getAColor(const std::string& propertyName, bool& ok) const {
	return getAColor(propertyName.c_str(), ok);
}

inline Vector Plugin::getVector(const char* propertyName) const {
	bool ok;
	return pRenderer->getPluginPropertyVector(id, propertyName, ok);
}

inline Vector Plugin::getVector(const std::string &propertyName) const {
	return getVector(propertyName.c_str());
}

inline Vector Plugin::getVector(const char* propertyName, bool& ok) const {
	return pRenderer->getPluginPropertyVector(id, propertyName, ok);
}

inline Vector Plugin::getVector(const std::string &propertyName, bool& ok) const {
	return getVector(propertyName.c_str(), ok);
}

inline Matrix Plugin::getMatrix(const char* propertyName) const {
	bool ok;
	return pRenderer->getPluginPropertyMatrix(id, propertyName, ok);
}

inline Matrix Plugin::getMatrix(const std::string& propertyName) const {
	return getMatrix(propertyName.c_str());
}

inline Matrix Plugin::getMatrix(const char* propertyName, bool& ok) const {
	return pRenderer->getPluginPropertyMatrix(id, propertyName, ok);
}

inline Matrix Plugin::getMatrix(const std::string& propertyName, bool& ok) const {
	return getMatrix(propertyName.c_str(), ok);
}

inline Transform Plugin::getTransform(const char* propertyName) const {
	bool ok;
	return pRenderer->getPluginPropertyTransform(id, propertyName, ok);
}

inline Transform Plugin::getTransform(const std::string& propertyName) const {
	return getTransform(propertyName.c_str());
}

inline Transform Plugin::getTransform(const char* propertyName, bool& ok) const {
	return pRenderer->getPluginPropertyTransform(id, propertyName, ok);
}

inline Transform Plugin::getTransform(const std::string& propertyName, bool& ok) const {
	return getTransform(propertyName.c_str(), ok);
}

inline Plugin Plugin::getPlugin(const char* propertyName) const {
	bool ok;
	return pRenderer->getPluginPropertyPluginProperty(id, propertyName, NULL, ok);
}

inline Plugin Plugin::getPlugin(const std::string& propertyName) const {
	return getPlugin(propertyName.c_str());
}

inline Plugin Plugin::getPlugin(const char* propertyName, bool& ok) const {
	return pRenderer->getPluginPropertyPluginProperty(id, propertyName, NULL, ok);
}

inline Plugin Plugin::getPlugin(const std::string& propertyName, bool& ok) const {
	return getPlugin(propertyName.c_str(), ok);
}

inline Plugin Plugin::getPluginProperty(const char* propertyName, std::string& outPropertyName) const {
	bool ok;
	const char* outputPropertyName;
	Plugin plugin = pRenderer->getPluginPropertyPluginProperty(id, propertyName, &outputPropertyName, ok);
	outPropertyName = ok && outputPropertyName ? outputPropertyName : "";
	return plugin;
}

inline Plugin Plugin::getPluginProperty(const std::string& propertyName, std::string& outPropertyName) const {
	return getPluginProperty(propertyName.c_str(), outPropertyName);
}

inline Plugin Plugin::getPluginProperty(const char* propertyName, std::string& outPropertyName, bool& ok) const {
	const char* outputPropertyName;
	Plugin plugin = pRenderer->getPluginPropertyPluginProperty(id, propertyName, &outputPropertyName, ok);
	outPropertyName = ok && outputPropertyName ? outputPropertyName : "";
	return plugin;
}

inline Plugin Plugin::getPluginProperty(const std::string& propertyName, std::string& outPropertyName, bool& ok) const {
	return getPluginProperty(propertyName.c_str(), outPropertyName, ok);
}

inline std::string Plugin::getString(const char* propertyName) const {
	bool ok;
	return getString(propertyName, ok);
}

inline std::string Plugin::getString(const std::string& propertyName) const {
	return getString(propertyName.c_str());
}

inline std::string Plugin::getString(const char* propertyName, bool& ok) const {
	return pRenderer->getPluginPropertyTyped<std::string>(id, propertyName, ok);
}

inline std::string Plugin::getString(const std::string& propertyName, bool& ok) const {
	return getString(propertyName.c_str(), ok);
}

inline IntList Plugin::getIntList(const char* propertyName) const {
	bool ok;
	return getIntList(propertyName, ok);
}

inline IntList Plugin::getIntList(const std::string& propertyName) const {
	return getIntList(propertyName.c_str());
}

inline IntList Plugin::getIntList(const char* propertyName, bool& ok) const {
	return pRenderer->getPluginPropertyTyped<IntList>(id, propertyName, ok);
}

inline IntList Plugin::getIntList(const std::string& propertyName, bool& ok) const {
	return getIntList(propertyName.c_str(), ok);
}

inline FloatList Plugin::getFloatList(const char* propertyName) const {
	bool ok;
	return getFloatList(propertyName, ok);
}

inline FloatList Plugin::getFloatList(const std::string& propertyName) const {
	return getFloatList(propertyName.c_str());
}

inline FloatList Plugin::getFloatList(const char* propertyName, bool& ok) const {
	return pRenderer->getPluginPropertyTyped<FloatList>(id, propertyName, ok);
}

inline FloatList Plugin::getFloatList(const std::string& propertyName, bool& ok) const {
	return getFloatList(propertyName.c_str(), ok);
}

inline ColorList Plugin::getColorList(const char* propertyName) const {
	bool ok;
	return getColorList(propertyName, ok);
}

inline ColorList Plugin::getColorList(const std::string& propertyName) const {
	return getColorList(propertyName.c_str());
}

inline ColorList Plugin::getColorList(const char* propertyName, bool& ok) const {
	return pRenderer->getPluginPropertyTyped<ColorList>(id, propertyName, ok);
}

inline ColorList Plugin::getColorList(const std::string& propertyName, bool& ok) const {
	return getColorList(propertyName.c_str(), ok);
}

inline VectorList Plugin::getVectorList(const char* propertyName) const {
	bool ok;
	return getVectorList(propertyName, ok);
}

inline VectorList Plugin::getVectorList(const std::string& propertyName) const {
	return getVectorList(propertyName.c_str());
}

inline VectorList Plugin::getVectorList(const char* propertyName, bool& ok) const {
	return pRenderer->getPluginPropertyTyped<VectorList>(id, propertyName, ok);
}

inline VectorList Plugin::getVectorList(const std::string& propertyName, bool& ok) const {
	return getVectorList(propertyName.c_str(), ok);
}

inline ValueList Plugin::getValueList(const char* propertyName) const {
	bool ok;
	return getValueList(propertyName, ok);
}

inline ValueList Plugin::getValueList(const std::string& propertyName) const {
	return getValueList(propertyName.c_str());
}

inline ValueList Plugin::getValueList(const char* propertyName, bool& ok) const {
	return pRenderer->getPluginPropertyTyped<ValueList>(id, propertyName, ok);
}

inline ValueList Plugin::getValueList(const std::string& propertyName, bool& ok) const {
	return getValueList(propertyName.c_str(), ok);
}

inline Value Plugin::getValue(const char* propertyName) const {
	bool ok;
	return pRenderer->getPluginPropertyValue(id, propertyName, ok);
}

inline Value Plugin::getValue(const std::string &propertyName) const {
	return getValue(propertyName.c_str());
}

inline Value Plugin::getValue(const char* propertyName, bool& ok) const {
	return pRenderer->getPluginPropertyValue(id, propertyName, ok);
}

inline Value Plugin::getValue(const std::string &propertyName, bool& ok) const {
	return getValue(propertyName.c_str(), ok);
}

inline bool Plugin::isAnimated(const char* propertyName) const {
	return pRenderer->isPropertyAnimated(id, propertyName);
}

inline bool Plugin::isAnimated(const std::string& propertyName) const {
	return isAnimated(propertyName.c_str());
}

template<typename T, int TYPE>
inline bool Plugin::setValueTemplate(const char* propertyName, const T& value) {
#pragma pack(push, 4)
	struct {
		int type;
		T   value;
	} buf = { TYPE, value };
#pragma pack(pop)
	return pRenderer->setPluginPropertyValue(id, propertyName, &buf);
}

template<typename T, int TYPE>
inline bool Plugin::setValueAtTimeTemplate(const char* propertyName, const T& value, double time) {
#pragma pack(push, 4)
	struct {
		int type;
		T   value;
	} buf = { TYPE, value };
#pragma pack(pop)
	return pRenderer->setPluginPropertyValueAtTime(id, propertyName, &buf, time);
}

template<typename T, int TYPE>
inline bool Plugin::setArrayTemplate(const char* propertyName, size_t count, const T& value) {
#pragma pack(push, 4)
	struct {
		int type;
		int count;
		T   value;
	} buf = { TYPE, static_cast<int>(count), value };
#pragma pack(pop)
	return pRenderer->setPluginPropertyValue(id, propertyName, &buf);
}

template<typename T, int TYPE>
inline bool Plugin::setArrayAtTimeTemplate(const char* propertyName, size_t count, const T& value, double time) {
#pragma pack(push, 4)
	struct {
		int type;
		int count;
		T   value;
	} buf = { TYPE, static_cast<int>(count), value };
#pragma pack(pop)
	return pRenderer->setPluginPropertyValueAtTime(id, propertyName, &buf, time);
}

inline bool Plugin::setValue(const char* propertyName, const bool value) {
	return setValueTemplate<Bool, TYPE_BOOL>(propertyName, value);
}

inline bool Plugin::setValueAtTime(const char* propertyName, const bool value, double time) {
	return setValueAtTimeTemplate<Bool, TYPE_BOOL>(propertyName, value, time);
}

inline bool Plugin::setValue(const std::string& propertyName, const bool value) {
	return setValue(propertyName.c_str(), value);
}

inline bool Plugin::setValueAtTime(const std::string& propertyName, const bool value, double time) {
	return setValueAtTime(propertyName.c_str(), value, time);
}

inline bool Plugin::setValue(const char* propertyName, const int value) {
	return setValueTemplate<int, TYPE_INT>(propertyName, value);
}

inline bool Plugin::setValueAtTime(const char* propertyName, const int value, double time) {
	return setValueAtTimeTemplate<int, TYPE_INT>(propertyName, value, time);
}

inline bool Plugin::setValue(const std::string& propertyName, const int value) {
	return setValue(propertyName.c_str(), value);
}

inline bool Plugin::setValueAtTime(const std::string& propertyName, const int value, double time) {
	return setValueAtTime(propertyName.c_str(), value, time);
}

inline bool Plugin::setValue(const char* propertyName, const float value) {
	return setValueTemplate<float, TYPE_FLOAT>(propertyName, value);
}

inline bool Plugin::setValueAtTime(const char* propertyName, const float value, double time) {
	return setValueAtTimeTemplate<float, TYPE_FLOAT>(propertyName, value, time);
}

inline bool Plugin::setValue(const std::string& propertyName, const float value) {
	return setValue(propertyName.c_str(), value);
}

inline bool Plugin::setValueAtTime(const std::string& propertyName, const float value, double time) {
	return setValueAtTime(propertyName.c_str(), value, time);
}

inline bool Plugin::setValue(const char* propertyName, const Color& color) {
	return setValueTemplate<Color, TYPE_COLOR>(propertyName, color);
}

inline bool Plugin::setValueAtTime(const char* propertyName, const Color& color, double time) {
	return setValueAtTimeTemplate<Color, TYPE_COLOR>(propertyName, color, time);
}

inline bool Plugin::setValue(const std::string& propertyName, const Color& color) {
	return setValue(propertyName.c_str(), color);
}

inline bool Plugin::setValueAtTime(const std::string& propertyName, const Color& color, double time) {
	return setValueAtTime(propertyName.c_str(), color, time);
}

inline bool Plugin::setValue(const char* propertyName, const AColor& acolor) {
	return setValueTemplate<AColor, TYPE_ACOLOR>(propertyName, acolor);
}

inline bool Plugin::setValueAtTime(const char* propertyName, const AColor& acolor, double time) {
	return setValueAtTimeTemplate<AColor, TYPE_ACOLOR>(propertyName, acolor, time);
}

inline bool Plugin::setValue(const std::string& propertyName, const AColor& acolor) {
	return setValue(propertyName.c_str(), acolor);
}

inline bool Plugin::setValueAtTime(const std::string& propertyName, const AColor& acolor, double time) {
	return setValueAtTime(propertyName.c_str(), acolor, time);
}

inline bool Plugin::setValue(const char* propertyName, const Vector& vector) {
	return setValueTemplate<Vector, TYPE_VECTOR>(propertyName, vector);
}

inline bool Plugin::setValueAtTime(const char* propertyName, const Vector& vector, double time) {
	return setValueAtTimeTemplate<Vector, TYPE_VECTOR>(propertyName, vector, time);
}

inline bool Plugin::setValue(const std::string& propertyName, const Vector& vector) {
	return setValue(propertyName.c_str(), vector);
}

inline bool Plugin::setValueAtTime(const std::string& propertyName, const Vector& vector, double time) {
	return setValueAtTime(propertyName.c_str(), vector, time);
}

inline bool Plugin::setValue(const char* propertyName, const Matrix& matrix) {
	return setValueTemplate<Matrix, TYPE_MATRIX>(propertyName, matrix);
}

inline bool Plugin::setValueAtTime(const char* propertyName, const Matrix& matrix, double time) {
	return setValueAtTimeTemplate<Matrix, TYPE_MATRIX>(propertyName, matrix, time);
}

inline bool Plugin::setValue(const std::string& propertyName, const Matrix& matrix) {
	return setValue(propertyName.c_str(), matrix);
}

inline bool Plugin::setValueAtTime(const std::string& propertyName, const Matrix& matrix, double time) {
	return setValueAtTime(propertyName.c_str(), matrix, time);
}

inline bool Plugin::setValue(const char* propertyName, const Transform& transform) {
	return setValueTemplate<Transform, TYPE_TRANSFORM>(propertyName, transform);
}

inline bool Plugin::setValueAtTime(const char* propertyName, const Transform& transform, double time) {
	return setValueAtTimeTemplate<Transform, TYPE_TRANSFORM>(propertyName, transform, time);
}

inline bool Plugin::setValue(const std::string& propertyName, const Transform& transform) {
	return setValue(propertyName.c_str(), transform);
}

inline bool Plugin::setValueAtTime(const std::string& propertyName, const Transform& transform, double time) {
	return setValueAtTime(propertyName.c_str(), transform, time);
}

inline bool Plugin::setValue(const char* propertyName, const Plugin& plugin) {
	return setValueTemplate<int, TYPE_OBJECT>(propertyName, static_cast<int>(plugin.getId()));
}

inline bool Plugin::setValueAtTime(const char* propertyName, const Plugin& plugin, double time) {
	return setValueAtTimeTemplate<int, TYPE_OBJECT>(propertyName, static_cast<int>(plugin.getId()), time);
}

inline bool Plugin::setValue(const std::string& propertyName, const Plugin& plugin) {
	return setValue(propertyName.c_str(), plugin);
}

inline bool Plugin::setValueAtTime(const std::string& propertyName, const Plugin& plugin, double time) {
	return setValueAtTime(propertyName.c_str(), plugin, time);
}

inline bool Plugin::setValue(const char* propertyName, const Plugin& outPlugin, const char* outPropertyName) {
#pragma pack(push, 4)
	struct {
		int type;
		int pluginID;
		const char* propName;
	} buf = { TYPE_PLUGIN_PROP_PTR, static_cast<int>(outPlugin.getId()), outPropertyName };
#pragma pack(pop)
	return pRenderer->setPluginPropertyValue(id, propertyName, &buf);
}

inline bool Plugin::setValue(const std::string& propertyName, const Plugin& outPlugin, const char* outPropertyName) {
	return setValue(propertyName.c_str(), outPlugin, outPropertyName);
}

inline bool Plugin::setValue(const char* propertyName, const Plugin& outPlugin, const std::string& outPropertyName) {
	return setValue(propertyName, outPlugin, outPropertyName.c_str());
}

inline bool Plugin::setValue(const std::string& propertyName, const Plugin& outPlugin, const std::string& outPropertyName) {
	return setValue(propertyName.c_str(), outPlugin, outPropertyName.c_str());
}

inline bool Plugin::setValue(const char* propertyName, const char* str) {
	return setValueTemplate<const char*, TYPE_STRING_PTR>(propertyName, str);
}

inline bool Plugin::setValueAtTime(const char* propertyName, const char* str, double time) {
	return setValueAtTimeTemplate<const char*, TYPE_STRING_PTR>(propertyName, str, time);
}

inline bool Plugin::setValue(const std::string& propertyName, const char* str) {
	return setValue(propertyName.c_str(), str);
}

inline bool Plugin::setValueAtTime(const std::string& propertyName, const char* str, double time) {
	return setValueAtTime(propertyName.c_str(), str, time);
}

inline bool Plugin::setValue(const char* propertyName, const std::string& str) {
	return setValue(propertyName, str.c_str());
}

inline bool Plugin::setValueAtTime(const char* propertyName, const std::string& str, double time) {
	return setValueAtTime(propertyName, str.c_str(), time);
}

inline bool Plugin::setValue(const std::string& propertyName, const std::string& str) {
	return setValue(propertyName.c_str(), str.c_str());
}

inline bool Plugin::setValueAtTime(const std::string& propertyName, const std::string& str, double time) {
	return setValueAtTime(propertyName.c_str(), str.c_str(), time);
}

inline bool Plugin::setValue(const char* propertyName, const Value& value) {
	Internal::BinaryValueBuilder builder;
	if (builder.build(value))
		return pRenderer->setPluginPropertyValue(id, propertyName, builder.getPointer());
	return false;
}

inline bool Plugin::setValueAtTime(const char* propertyName, const Value& value, double time) {
	Internal::BinaryValueBuilder builder;
	if (builder.build(value))
		return pRenderer->setPluginPropertyValueAtTime(id, propertyName, builder.getPointer(), time);
	return false;
}

inline bool Plugin::setValue(const std::string& propertyName, const Value& value) {
	return setValue(propertyName.c_str(), value);
}

inline bool Plugin::setValueAtTime(const std::string& propertyName, const Value& value, double time) {
	return setValueAtTime(propertyName.c_str(), value, time);
}

inline bool Plugin::setValue(const char* propertyName, const ValueList& value) {
	Internal::BinaryValueBuilder builder;
	if (builder.build(value))
		return pRenderer->setPluginPropertyValue(id, propertyName, builder.getPointer());
	return false;
}

inline bool Plugin::setValueAtTime(const char* propertyName, const ValueList& value, double time) {
	Internal::BinaryValueBuilder builder;
	if (builder.build(value))
		return pRenderer->setPluginPropertyValueAtTime(id, propertyName, builder.getPointer(), time);
	return false;
}

inline bool Plugin::setValue(const std::string& propertyName, const ValueList& value) {
	return setValue(propertyName.c_str(), value);
}

inline bool Plugin::setValueAtTime(const std::string& propertyName, const ValueList& value, double time) {
	return setValueAtTime(propertyName.c_str(), value, time);
}

inline bool Plugin::setValue(const char* propertyName, const IntList& value) {
	return setArray(propertyName, &value.front(), value.size());
}

inline bool Plugin::setValueAtTime(const char* propertyName, const IntList& value, double time) {
	return setArrayAtTime(propertyName, &value.front(), value.size(), time);
}

inline bool Plugin::setValue(const std::string& propertyName, const IntList& value) {
	return setArray(propertyName, &value.front(), value.size());
}

inline bool Plugin::setValueAtTime(const std::string& propertyName, const IntList& value, double time) {
	return setArrayAtTime(propertyName, &value.front(), value.size(), time);
}

inline bool Plugin::setValue(const char* propertyName, const FloatList& value) {
	return setArray(propertyName, &value.front(), value.size());
}

inline bool Plugin::setValueAtTime(const char* propertyName, const FloatList& value, double time) {
	return setArrayAtTime(propertyName, &value.front(), value.size(), time);
}

inline bool Plugin::setValue(const std::string& propertyName, const FloatList& value) {
	return setArray(propertyName, &value.front(), value.size());
}

inline bool Plugin::setValueAtTime(const std::string& propertyName, const FloatList& value, double time) {
	return setArrayAtTime(propertyName, &value.front(), value.size(), time);
}

inline bool Plugin::setValue(const char* propertyName, const VectorList& value) {
	return setArray(propertyName, &value.front(), value.size());
}

inline bool Plugin::setValueAtTime(const char* propertyName, const VectorList& value, double time) {
	return setArrayAtTime(propertyName, &value.front(), value.size(), time);
}

inline bool Plugin::setValue(const std::string& propertyName, const VectorList& value) {
	return setArray(propertyName, &value.front(), value.size());
}

inline bool Plugin::setValueAtTime(const std::string& propertyName, const VectorList& value, double time) {
	return setArrayAtTime(propertyName, &value.front(), value.size(), time);
}

inline bool Plugin::setValue(const char* propertyName, const ColorList& value) {
	return setArray(propertyName, &value.front(), value.size());
}

inline bool Plugin::setValueAtTime(const char* propertyName, const ColorList& value, double time) {
	return setArrayAtTime(propertyName, &value.front(), value.size(), time);
}

inline bool Plugin::setValue(const std::string& propertyName, const ColorList& value) {
	return setArray(propertyName, &value.front(), value.size());
}

inline bool Plugin::setValueAtTime(const std::string& propertyName, const ColorList& value, double time) {
	return setArrayAtTime(propertyName, &value.front(), value.size(), time);
}

inline bool Plugin::setArray(const char* propertyName, const int data[], size_t count) {
	return setArrayTemplate<const int*, TYPE_INT_LIST_PTR>(propertyName, count, data);
}

inline bool Plugin::setArrayAtTime(const char* propertyName, const int data[], size_t count, double time) {
	return setArrayAtTimeTemplate<const int*, TYPE_INT_LIST_PTR>(propertyName, count, data, time);
}

inline bool Plugin::setArray(const std::string& propertyName, const int data[], size_t count) {
	return setArray(propertyName.c_str(), data, count);
}

inline bool Plugin::setArrayAtTime(const std::string& propertyName, const int data[], size_t count, double time) {
	return setArrayAtTime(propertyName.c_str(), data, count, time);
}

inline bool Plugin::setArray(const char* propertyName, const float data[], size_t count) {
	return setArrayTemplate<const float*, TYPE_FLOAT_LIST_PTR>(propertyName, count, data);
}

inline bool Plugin::setArrayAtTime(const char* propertyName, const float data[], size_t count, double time) {
	return setArrayAtTimeTemplate<const float*, TYPE_FLOAT_LIST_PTR>(propertyName, count, data, time);
}

inline bool Plugin::setArray(const std::string& propertyName, const float data[], size_t count) {
	return setArray(propertyName.c_str(), data, count);
}

inline bool Plugin::setArrayAtTime(const std::string& propertyName, const float data[], size_t count, double time) {
	return setArrayAtTime(propertyName.c_str(), data, count, time);
}

inline bool Plugin::setArray(const char* propertyName, const double data[], size_t count) {
	return setArrayTemplate<const double*, TYPE_DOUBLE_LIST_PTR>(propertyName, count, data);
}

inline bool Plugin::setArrayAtTime(const char* propertyName, const double data[], size_t count, double time) {
	return setArrayAtTimeTemplate<const double*, TYPE_DOUBLE_LIST_PTR>(propertyName, count, data, time);
}

inline bool Plugin::setArray(const std::string& propertyName, const double data[], size_t count) {
	return setArray(propertyName.c_str(), data, count);
}

inline bool Plugin::setArrayAtTime(const std::string& propertyName, const double data[], size_t count, double time) {
	return setArrayAtTime(propertyName.c_str(), data, count, time);
}

inline bool Plugin::setArray(const char* propertyName, const Vector data[], size_t count) {
	return setArrayTemplate<const Vector*, TYPE_VECTOR_LIST_PTR>(propertyName, count, data);
}

inline bool Plugin::setArrayAtTime(const char* propertyName, const Vector data[], size_t count, double time) {
	return setArrayAtTimeTemplate<const Vector*, TYPE_VECTOR_LIST_PTR>(propertyName, count, data, time);
}

inline bool Plugin::setArray(const std::string& propertyName, const Vector data[], size_t count) {
	return setArray(propertyName.c_str(), data, count);
}

inline bool Plugin::setArrayAtTime(const std::string& propertyName, const Vector data[], size_t count, double time) {
	return setArrayAtTime(propertyName.c_str(), data, count, time);
}

inline bool Plugin::setArray(const char* propertyName, const Color data[], size_t count) {
	return setArrayTemplate<const Color*, TYPE_COLOR_LIST_PTR>(propertyName, count, data);
}

inline bool Plugin::setArrayAtTime(const char* propertyName, const Color data[], size_t count, double time) {
	return setArrayAtTimeTemplate<const Color*, TYPE_COLOR_LIST_PTR>(propertyName, count, data, time);
}

inline bool Plugin::setArray(const std::string& propertyName, const Color data[], size_t count) {
	return setArray(propertyName.c_str(), data, count);
}

inline bool Plugin::setArrayAtTime(const std::string& propertyName, const Color data[], size_t count, double time) {
	return setArrayAtTime(propertyName.c_str(), data, count, time);
}

inline bool Plugin::setValue(const char* propertyName, const void* data, size_t size) {
	return pRenderer->setPluginPropertyValueRaw(id, propertyName, data, size);
}

inline bool Plugin::setValueAtTime(const char* propertyName, const void* data, size_t size, double time) {
	return pRenderer->setPluginPropertyValueRawAtTime(id, propertyName, data, size, time);
}

inline bool Plugin::setValue(const std::string& propertyName, const void* data, size_t size) {
	return setValue(propertyName.c_str(), data, size);
}

inline bool Plugin::setValueAtTime(const std::string& propertyName, const void* data, size_t size, double time) {
	return setValueAtTime(propertyName.c_str(), data, size, time);
}

#ifdef VRAY_SDK_INTEROPERABILITY
inline bool Plugin::setValue(const char* propertyName, const VUtils::IntRefList& intList) {
	return pRenderer->setPluginPropertyValueSDKInterop(id, propertyName, &intList, TYPE_INT_LIST);
}

inline bool Plugin::setValueAtTime(const char* propertyName, const VUtils::IntRefList& intList, double time) {
	return pRenderer->setPluginPropertyValueAtTimeSDKInterop(id, propertyName, &intList, TYPE_INT_LIST, time);
}

inline bool Plugin::setValue(const std::string& propertyName, const VUtils::IntRefList& intList) {
	return setValue(propertyName.c_str(), intList);
}

inline bool Plugin::setValueAtTime(const std::string& propertyName, const VUtils::IntRefList& intList, double time) {
	return setValueAtTime(propertyName.c_str(), intList, time);
}

inline bool Plugin::setValue(const char* propertyName, const VUtils::FloatRefList& floatList) {
	return pRenderer->setPluginPropertyValueSDKInterop(id, propertyName, &floatList, TYPE_FLOAT_LIST);
}

inline bool Plugin::setValueAtTime(const char* propertyName, const VUtils::FloatRefList& floatList, double time) {
	return pRenderer->setPluginPropertyValueAtTimeSDKInterop(id, propertyName, &floatList, TYPE_FLOAT_LIST, time);
}

inline bool Plugin::setValue(const std::string& propertyName, const VUtils::FloatRefList& floatList) {
	return setValue(propertyName.c_str(), floatList);
}

inline bool Plugin::setValueAtTime(const std::string& propertyName, const VUtils::FloatRefList& floatList, double time) {
	return setValueAtTime(propertyName.c_str(), floatList, time);
}

inline bool Plugin::setValue(const char* propertyName, const VUtils::VectorRefList& vectorList) {
	return pRenderer->setPluginPropertyValueSDKInterop(id, propertyName, &vectorList, TYPE_VECTOR_LIST);
}

inline bool Plugin::setValueAtTime(const char* propertyName, const VUtils::VectorRefList& vectorList, double time) {
	return pRenderer->setPluginPropertyValueAtTimeSDKInterop(id, propertyName, &vectorList, TYPE_VECTOR_LIST, time);
}

inline bool Plugin::setValue(const std::string& propertyName, const VUtils::VectorRefList& vectorList) {
	return setValue(propertyName.c_str(), vectorList);
}

inline bool Plugin::setValueAtTime(const std::string& propertyName, const VUtils::VectorRefList& vectorList, double time) {
	return setValueAtTime(propertyName.c_str(), vectorList, time);
}

inline bool Plugin::setValue(const char* propertyName, const VUtils::ColorRefList& colorList) {
	return pRenderer->setPluginPropertyValueSDKInterop(id, propertyName, &colorList, TYPE_COLOR_LIST);
}

inline bool Plugin::setValueAtTime(const char* propertyName, const VUtils::ColorRefList& colorList, double time) {
	return pRenderer->setPluginPropertyValueAtTimeSDKInterop(id, propertyName, &colorList, TYPE_COLOR_LIST, time);
}

inline bool Plugin::setValue(const std::string& propertyName, const VUtils::ColorRefList& colorList) {
	return setValue(propertyName.c_str(), colorList);
}

inline bool Plugin::setValueAtTime(const std::string& propertyName, const VUtils::ColorRefList& colorList, double time) {
	return setValueAtTime(propertyName.c_str(), colorList, time);
}

inline bool Plugin::setValue(const char* propertyName, const VUtils::CharStringRefList& stringList) {
	return pRenderer->setPluginPropertyValueSDKInterop(id, propertyName, &stringList, TYPE_STRING_LIST);
}

inline bool Plugin::setValueAtTime(const char* propertyName, const VUtils::CharStringRefList& stringList, double time) {
	return pRenderer->setPluginPropertyValueAtTimeSDKInterop(id, propertyName, &stringList, TYPE_STRING_LIST, time);
}

inline bool Plugin::setValue(const std::string& propertyName, const VUtils::CharStringRefList& stringList) {
	return setValue(propertyName.c_str(), stringList);
}

inline bool Plugin::setValueAtTime(const std::string& propertyName, const VUtils::CharStringRefList& stringList, double time) {
	return setValueAtTime(propertyName.c_str(), stringList, time);
}

inline bool Plugin::setValue(const char* propertyName, const VUtils::ValueRefList& valueList) {
	return pRenderer->setPluginPropertyValueSDKInterop(id, propertyName, &valueList, TYPE_GENERAL_LIST);
}

inline bool Plugin::setValueAtTime(const char* propertyName, const VUtils::ValueRefList& valueList, double time) {
	return pRenderer->setPluginPropertyValueAtTimeSDKInterop(id, propertyName, &valueList, TYPE_GENERAL_LIST, time);
}

inline bool Plugin::setValue(const std::string& propertyName, const VUtils::ValueRefList& valueList) {
	return setValue(propertyName.c_str(), valueList);
}

inline bool Plugin::setValueAtTime(const std::string& propertyName, const VUtils::ValueRefList& valueList, double time) {
	return setValueAtTime(propertyName.c_str(), valueList, time);
}

inline bool Plugin::setValue(const char* propertyName, const VUtils::CharString& str) {
	return pRenderer->setPluginPropertyValueSDKInterop(id, propertyName, &str, TYPE_STRING);
}

inline bool Plugin::setValueAtTime(const char* propertyName, const VUtils::CharString& str, double time) {
	return pRenderer->setPluginPropertyValueAtTimeSDKInterop(id, propertyName, &str, TYPE_STRING, time);
}

inline bool Plugin::setValue(const std::string& propertyName, const VUtils::CharString& str) {
	return setValue(propertyName.c_str(), str);
}

inline bool Plugin::setValueAtTime(const std::string& propertyName, const VUtils::CharString& str, double time) {
	return setValueAtTime(propertyName.c_str(), str, time);
}
#endif // VRAY_SDK_INTEROPERABILITY

inline bool VRayImage::saveToBmpFile(const char* fileName, bool preserveAlpha, bool invertChannels) const {
	return !!Internal::VRayImage_SaveToBmpFile(this, preserveAlpha, fileName, NULL, invertChannels ? 1 : 0);
}

inline bool VRayImage::saveToBmpFile(const std::string& fileName, bool preserveAlpha, bool invertChannels) const {
	return !!Internal::VRayImage_SaveToBmpFile(this, preserveAlpha, fileName.c_str(), NULL, invertChannels ? 1 : 0);
}

inline bool VRayImage::saveToBmpFile(const char* fileName, const VRayRenderer& renderer, bool preserveAlpha, bool invertChannels) const {
	return !!Internal::VRayImage_SaveToBmpFile(this, preserveAlpha, fileName, renderer.getNativeRenderer(), invertChannels ? 1 : 0);
}

inline bool VRayImage::saveToBmpFile(const std::string& fileName, const VRayRenderer& renderer, bool preserveAlpha, bool invertChannels) const {
	return !!Internal::VRayImage_SaveToBmpFile(this, preserveAlpha, fileName.c_str(), renderer.getNativeRenderer(), invertChannels ? 1 : 0);
}

inline bool VRayImage::saveToPngFile(const char* fileName, bool preserveAlpha) const {
	return !!Internal::VRayImage_compressToPngFile(this, preserveAlpha, fileName, NULL);
}

inline bool VRayImage::saveToPngFile(const std::string& fileName, bool preserveAlpha) const {
	return !!Internal::VRayImage_compressToPngFile(this, preserveAlpha, fileName.c_str(), NULL);
}

inline bool VRayImage::saveToPngFile(const char* fileName, const VRayRenderer& renderer, bool preserveAlpha) const {
	return !!Internal::VRayImage_compressToPngFile(this, preserveAlpha, fileName, renderer.getNativeRenderer());
}

inline bool VRayImage::saveToPngFile(const std::string& fileName, const VRayRenderer& renderer, bool preserveAlpha) const {
	return !!Internal::VRayImage_compressToPngFile(this, preserveAlpha, fileName.c_str(), renderer.getNativeRenderer());
}

inline bool VRayImage::saveToJpegFile(const char* fileName, int quality) const {
	return !!Internal::VRayImage_compressToJpegFile(this, quality, fileName, NULL);
}

inline bool VRayImage::saveToJpegFile(const std::string& fileName, int quality) const {
	return !!Internal::VRayImage_compressToJpegFile(this, quality, fileName.c_str(), NULL);
}

inline bool VRayImage::saveToJpegFile(const char* fileName, const VRayRenderer& renderer, int quality) const {
	return !!Internal::VRayImage_compressToJpegFile(this, quality, fileName, renderer.getNativeRenderer());
}

inline bool VRayImage::saveToJpegFile(const std::string& fileName, const VRayRenderer& renderer, int quality) const {
	return !!Internal::VRayImage_compressToJpegFile(this, quality, fileName.c_str(), renderer.getNativeRenderer());
}

inline Bmp* VRayImage::getBmp(bool preserveAlpha, bool invertChannels) const {
	byte* bufAddr = NULL;
	Internal::VRayImage_convertToBmp(this, preserveAlpha, &bufAddr, NULL, invertChannels ? 1 : 0);
	return reinterpret_cast<Bmp*>(bufAddr);
}

inline Bmp* VRayImage::getBmp(size_t& length, bool preserveAlpha, bool invertChannels) const {
	byte* bufAddr = NULL;
	length = Internal::VRayImage_convertToBmp(this, preserveAlpha, &bufAddr, NULL, invertChannels ? 1 : 0);
	return reinterpret_cast<Bmp*>(bufAddr);
}

inline Bmp* VRayImage::getBmp(const VRayRenderer& renderer, bool preserveAlpha, bool invertChannels) const {
	byte* bufAddr = NULL;
	Internal::VRayImage_convertToBmp(this, preserveAlpha, &bufAddr, renderer.getNativeRenderer(), invertChannels ? 1 : 0);
	return reinterpret_cast<Bmp*>(bufAddr);
}

inline Bmp* VRayImage::getBmp(size_t& length, const VRayRenderer& renderer, bool preserveAlpha, bool invertChannels) const {
	byte* bufAddr = NULL;
	length = Internal::VRayImage_convertToBmp(this, preserveAlpha, &bufAddr, renderer.getNativeRenderer(), invertChannels ? 1 : 0);
	return reinterpret_cast<Bmp*>(bufAddr);
}

inline Png* VRayImage::getPng(bool preserveAlpha) const {
	byte* bufAddr = NULL;
	Internal::VRayImage_compressToPng(this, preserveAlpha, &bufAddr, NULL);
	return reinterpret_cast<Png*>(bufAddr);
}

inline Png* VRayImage::getPng(size_t& length, bool preserveAlpha) const {
	byte* bufAddr = NULL;
	length = Internal::VRayImage_compressToPng(this, preserveAlpha, &bufAddr, NULL);
	return reinterpret_cast<Png*>(bufAddr);
}

inline Png* VRayImage::getPng(const VRayRenderer& renderer, bool preserveAlpha) const {
	byte* bufAddr = NULL;
	Internal::VRayImage_compressToPng(this, preserveAlpha, &bufAddr, renderer.getNativeRenderer());
	return reinterpret_cast<Png*>(bufAddr);
}

inline Png* VRayImage::getPng(size_t& length, const VRayRenderer& renderer, bool preserveAlpha) const {
	byte* bufAddr = NULL;
	length = Internal::VRayImage_compressToPng(this, preserveAlpha, &bufAddr, renderer.getNativeRenderer());
	return reinterpret_cast<Png*>(bufAddr);
}

inline Jpeg* VRayImage::getJpeg(int quality) const {
	byte* bufAddr = NULL;
	Internal::VRayImage_compressToJpeg(this, quality, &bufAddr, NULL);
	return reinterpret_cast<Jpeg*>(bufAddr);
}

inline Jpeg* VRayImage::getJpeg(size_t& length, int quality) const {
	byte* bufAddr = NULL;
	length = Internal::VRayImage_compressToJpeg(this, quality, &bufAddr, NULL);
	return reinterpret_cast<Jpeg*>(bufAddr);
}

inline Jpeg* VRayImage::getJpeg(const VRayRenderer& renderer, int quality) const {
	byte* bufAddr = NULL;
	Internal::VRayImage_compressToJpeg(this, quality, &bufAddr, renderer.getNativeRenderer());
	return reinterpret_cast<Jpeg*>(bufAddr);
}

inline Jpeg* VRayImage::getJpeg(size_t& length, const VRayRenderer& renderer, int quality) const {
	byte* bufAddr = NULL;
	length = Internal::VRayImage_compressToJpeg(this, quality, &bufAddr, renderer.getNativeRenderer());
	return reinterpret_cast<Jpeg*>(bufAddr);
}

inline MemoryBuffer* VRayImage::toBitmapData(size_t& size, bool preserveAlpha, bool invertChannels, int stride) const {
	byte* data = NULL;
	size = Internal::VRayImage_toBitmapData(this, preserveAlpha, &data, invertChannels, stride);
	if(size == 0) {
		return NULL;
	}
	return reinterpret_cast<MemoryBuffer*>(data);
}

inline VRayImage* VRayImage::clone() const {
	return Internal::VRayImage_clone(this);
}

inline VRayImage* VRayImage::create(int width, int height) {
	return Internal::VRayImage_create(width, height);
}

inline int VRayImage::getWidth() const {
	int width;
	Internal::VRayImage_getSize(this, &width, NULL);
	return width;
}

inline int VRayImage::getHeight() const {
	int height;
	Internal::VRayImage_getSize(this, NULL, &height);
	return height;
}

inline bool VRayImage::getSize(int& width, int& height) const {
	return !!Internal::VRayImage_getSize(this, &width, &height);
}

inline AColor* VRayImage::getPixelData() {
	return static_cast<AColor*>(Internal::VRayImage_getPixelDataPointerAndPixelCount(this, NULL));
}

inline const AColor* VRayImage::getPixelData() const {
	return static_cast<const AColor*>(Internal::VRayImage_getPixelDataPointerAndPixelCount(this, NULL));
}

inline AColor* VRayImage::getPixelData(size_t& count) {
	return static_cast<AColor*>(Internal::VRayImage_getPixelDataPointerAndPixelCount(this, &count));
}

inline const AColor* VRayImage::getPixelData(size_t& count) const {
	return static_cast<const AColor*>(Internal::VRayImage_getPixelDataPointerAndPixelCount(this, &count));
}

inline VRayImage* VRayImage::downscale(int width, int height) const {
	return Internal::VRayImage_downscale(this, width, height);
}

inline VRayImage* VRayImage::fitIn(int width, int height) const {
	return Internal::VRayImage_fitIn(this, width, height);
}

inline VRayImage* VRayImage::fitOut(int width, int height) const {
	return Internal::VRayImage_fitOut(this, width, height);
}

inline VRayImage* VRayImage::cutIn(int width, int height) const {
	return Internal::VRayImage_cutIn(this, width, height);
}

inline VRayImage* VRayImage::crop(int x, int y, int width, int height) const {
	return Internal::VRayImage_crop(this, x, y, width, height);
}

inline VRayImage* VRayImage::downscale(int srcX, int srcY, int srcWidth, int srcHeight, int dstWidth, int dstHeight) const {
	return Internal::VRayImage_crop_downscale(this, srcX, srcY, srcWidth, srcHeight, dstWidth, dstHeight);
}

inline bool VRayImage::draw(const VRayImage* image, int x, int y) {
	return !!Internal::VRayImage_drawImage(this, image, x, y);
}

inline bool VRayImage::addColor(const Color& color) {
	return addColor(AColor(color));
}

inline bool VRayImage::addColor(const AColor& color) {
	return !!Internal::VRayImage_addColor(this, &color);
}

inline bool VRayImage::changeExposure(float ev) {
	return !!Internal::VRayImage_changeExposure(this, ev);
}

inline bool VRayImage::changeGamma(float gamma) {
	return !!Internal::VRayImage_changeGamma(this, gamma);
}

inline bool VRayImage::changeContrast(float gamma) {
	return !!Internal::VRayImage_changeContrast(this, gamma);
}

inline bool VRayImage::makeNegative() {
	return !!Internal::VRayImage_makeNegative(this);
}

inline void VRayImage::operator delete (void *img) {
	Internal::C_memory_free(img);
}

inline void MemoryBuffer::operator delete (void *ptr) {
	Internal::C_memory_free(ptr);
}

inline void CompressImage::swap(CompressImage& ci) {
	byte* tmpAddr = ci.bufAddr;
	ci.bufAddr = bufAddr;
	bufAddr = tmpAddr;
	size_t tmpSize = ci.bufSize;
	ci.bufSize = bufSize;
	bufSize = tmpSize;
}

inline bool CompressImage::saveToFile(const char* fileName) const {
	if (bufSize) {
		Internal::FileSaveWriteHelper outfile(fileName);
		if (outfile.ofs.is_open()) {
			outfile.ofs.write(reinterpret_cast<const char*>(bufAddr), bufSize);
			if (outfile.ofs)
				return true;
		}
	}
	return false;
}

inline std::string PropertyMeta::typeToString(const Type type, int count) {
	switch (type) {

	case TYPE_INT:
		return count < 0 ? "int" : "List<Integer>";

	case TYPE_FLOAT:
		return count < 0 ? "float" : "List<Float>";

	case TYPE_DOUBLE:
		return count < 0 ? "double" : "List<Double>";

	case TYPE_BOOL:
		return count < 0 ? "boolean" : "List<Boolean>";

	case TYPE_VECTOR:
		return count < 0 ? "Vector" : "List<Vector>";

	case TYPE_COLOR:
		return count < 0 ? "Color" : "List<Color>";

	case TYPE_ACOLOR:
		return count < 0 ? "AColor" : "List<AColor>";

	case TYPE_MATRIX:
		return count < 0 ? "Matrix" : "List<Matrix>";

	case TYPE_TRANSFORM:
		return count < 0 ? "Transform" : "List<Transform>";

	case TYPE_STRING:
		return count < 0 ? "String" : "List<String>";

	case TYPE_LIST:
		return "List<Object>";

	case TYPE_OBJECT:
		return count < 0 ? "Object" : "List<Object>";

	case TYPE_TEXTURE:
		return count < 0 ? "Texture" : "List<Texture>";

	case TYPE_TEXTUREFLOAT:
		return count < 0 ? "TextureFloat" : "List<TextureFloat>";

	case TYPE_TEXTUREINT:
		return count < 0 ? "TextureInt" : "List<TextureInt>";

	case TYPE_TEXTUREVECTOR:
		return count < 0 ? "TextureVector" : "List<TextureVector>";

	case TYPE_TEXTUREMATRIX:
		return count < 0 ? "TextureMatrix" : "List<TextureMatrix>";

	case TYPE_TEXTURETRANSFORM:
		return count < 0 ? "Object" : "List<Object>";

	case TYPE_OUTPUTTEXTURE:
		return count < 0 ? "OutputTexture" : "List<OutputTexture>";

	case TYPE_OUTPUTTEXTUREFLOAT:
		return count < 0 ? "OutputTextureFloat" : "List<OutputTextureFloat>";

	case TYPE_OUTPUTTEXTUREINT:
		return count < 0 ? "OutputTextureInt" : "List<OutputTextureInt>";

	case TYPE_OUTPUTTEXTUREVECTOR:
		return count < 0 ? "OutputTextureVector" : "List<OutputTextureVector>";

	case TYPE_OUTPUTTEXTUREMATRIX:
		return count < 0 ? "OutputTextureMatrix" : "List<OutputTextureMatrix>";

	case TYPE_OUTPUTTEXTURETRANSFORM:
		return count < 0 ? "OutputTextureTransform" : "List<OutputTextureTransform>";

	case TYPE_UNSPECIFIED:
		return "Unspecified";

	default: // avoid warning
		break;
	}

	return "Object";
}

inline RenderElement::PixelFormat RenderElement::binaryFormatToPixelFormat(BinaryFormat binFormat) {
	switch (binFormat) {

	case BF_FLOAT:
		return PF_BW_FLOAT;

	case BF_3FLOAT:
	case BF_2FLOAT:
	case BF_3FLOAT_SIGNED:
		return PF_RGB_FLOAT;

	case BF_INT:
		return PF_BW_BIT32;

	default: // avoid warning
		break;
	}
	return PF_RGBA_FLOAT;
}

inline size_t RenderElement::getData_internal(InstanceId alphaPluginID, void** data, PixelFormat format, bool rgbOrder, const ImageRegion* rgn) const {
	if (NULL == renderer)
		return 0;
	if (format == PF_DEFAULT)
		format = defaultPixelFormat;
	return renderer->getRenderElementData(pluginID, alphaPluginID, format, rgbOrder, rgn, data);
}

inline std::vector<RenderElement> RenderElements::getAllByType(RenderElement::Type type) const {
	std::vector<RenderElement> result;
	std::vector<Plugin> plugins = renderer.getRenderElementPluginsList(type);
	result.reserve(plugins.size());
	for (size_t i = 0; i < plugins.size(); ++i)
		result.push_back(RenderElement(&renderer, plugins[i]));
	return result;
}

inline RenderElement::RenderElement(const VRayRenderer* renderer, const Plugin &plugin) : renderer(renderer), pluginID(plugin.getId()) {
	Info chInfo;
	if (!renderer->getRenderElementInfo(pluginID, chInfo)) {
#ifndef VRAY_NOTHROW
		throw InvalidRenderElementErr();
#else
		this->pluginID = NO_ID;
		return;
#endif
	}

	name = chInfo.name;
	type = chInfo.type;
	binaryFormat = chInfo.binaryFormat;
	defaultPixelFormat = binaryFormatToPixelFormat(chInfo.binaryFormat);
}

inline Plugin RenderElement::getPlugin() const { return renderer ? renderer->getPlugin_internal(pluginID) : Plugin(); }

inline size_t RenderElement::getData(void** data, PixelFormat format, bool rgbOrder, const ImageRegion* rgn) const {
	return getData_internal(NO_ID, data, format, rgbOrder, rgn);
}

inline size_t RenderElement::getData(const RenderElement& alpha, void** data, PixelFormat format, bool rgbOrder, const ImageRegion* rgn) const {
	return getData_internal(alpha.pluginID, data, format, rgbOrder, rgn);
}

inline size_t RenderElement::getData(const Plugin& alpha, void** data, PixelFormat format, bool rgbOrder, const ImageRegion* rgn) const {
	return getData_internal(alpha.getId(), data, format, rgbOrder, rgn);
}
inline size_t RenderElement::getData(InstanceId alpha, void** data, PixelFormat format, bool rgbOrder, const ImageRegion* rgn) const {
	return getData_internal(alpha, data, format, rgbOrder, rgn);
}

inline void RenderElement::releaseData(void* data) {
	Internal::C_memory_free(data);
}

inline VRayImage* RenderElement::getImage(const ImageRegion* rgn) const {
	return renderer ? renderer->getRenderElementAsImage(pluginID, rgn) : NULL;
}

inline RenderElements::RenderElements(VRayRenderer& renderer) : renderer(renderer) {}

inline RenderElement RenderElements::addNew(RenderElement::Type type, const char* instanceName, const char* displayName) {
	return RenderElement(&renderer, renderer.newRenderElementPlugin(type, instanceName, displayName));
}

inline RenderElement RenderElements::getByType(RenderElement::Type type) const {
	return RenderElement(&renderer, renderer.getRenderElementPlugin(type));
}


} // namespace VRay

namespace std {
	template<> inline void swap(VRay::Value& lhs, VRay::Value& rhs) {
		lhs.swap(rhs);
	}
}
