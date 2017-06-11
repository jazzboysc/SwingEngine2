#ifndef _VRAY_SRV_HPP_
#define _VRAY_SRV_HPP_

#ifndef VRAY_NOTHROW
	#include "vraythrow.hpp"
#endif

#include <cstdint>
#include <cstdlib>
#include <string>

#ifdef VRAY_RUNTIME_LOAD_PRIMARY
	#ifdef _WIN32
		#include <windows.h>
	#else
		#include <dlfcn.h>
	#endif
#endif

namespace VRay {
	class VRayServerNative;
	class VRayServer;
	struct ServerOptions;

	typedef void (*OnServerStart)(void* userObject);
	typedef void (*OnConnect)(const char* host, void* userObject);
	typedef void (*OnDisconnect)(const char* host, void* userObject);
	typedef void (*OnServerClose)(void* userObject);
	typedef void (*OnDumpMessage)(const char* message, int level, void* userObject);
	typedef void (*OnProgress)(const char* message, int progress, int total, void* userObject);
}

#if !defined(VRAY_RUNTIME_LOAD_PRIMARY) && !defined(VRAY_RUNTIME_LOAD_SECONDARY)

extern "C" {
	// VRayServer
	VRay::VRayServerNative* VRay_VRayServer_create(const VRay::ServerOptions* const options, size_t* licErrs);
	int VRay_VRayServer_run(VRay::VRayServerNative* server);
	int VRay_VRayServer_start(VRay::VRayServerNative* server);
	void VRay_VRayServer_stop(VRay::VRayServerNative* server);
	void VRay_VRayServer_release(VRay::VRayServerNative* server);
	void VRay_VRayServer_setServerStartCallback(VRay::VRayServerNative* server, VRay::OnServerStart onServerStart, void* userObject);
	void VRay_VRayServer_setConnectCallback(VRay::VRayServerNative* server, VRay::OnConnect onConnect, void* userObject);
	void VRay_VRayServer_setDisconnectCallback(VRay::VRayServerNative* server, VRay::OnDisconnect onDisconnect, void* userObject);
	void VRay_VRayServer_setDumpMessageCallback(VRay::VRayServerNative* server, VRay::OnDumpMessage onDumpMessage, void* userObject);
	void VRay_VRayServer_setServerCloseCallback(VRay::VRayServerNative* server, VRay::OnServerClose onServerClose, void* userObject);
	void VRay_VRayServer_setProgressCallback(VRay::VRayServerNative* server, VRay::OnProgress onProgress, void* userObject);
	uintptr_t VRay_VRayServer_setUserData(VRay::VRayServerNative* server, int n, uintptr_t data);
	uintptr_t VRay_VRayServer_getUserData(VRay::VRayServerNative* server, int n);
	void VRay_LicenseManager_releaseServerLicense(void);
	char* VRay_getLicenseErrorTextStringFromErrorCode(size_t licErrs);
	void C_memory_free(void* mem);
}

#else

	#undef VRAY_FUNC_POINTER_INIT
	#undef VRAY_FUNC_EXTERN
	#ifdef VRAY_RUNTIME_LOAD_SECONDARY
		#define VRAY_FUNC_POINTER_INIT
		#define VRAY_FUNC_EXTERN extern
	#else
		#define VRAY_FUNC_POINTER_INIT =NULL
		#define VRAY_FUNC_EXTERN
	#endif

	// VRayServer
	VRAY_FUNC_EXTERN VRay::VRayServerNative* (*VRay_VRayServer_create)(const VRay::ServerOptions* const options, size_t* licErrs) VRAY_FUNC_POINTER_INIT;
	VRAY_FUNC_EXTERN int (*VRay_VRayServer_run)(VRay::VRayServerNative* server) VRAY_FUNC_POINTER_INIT;
	VRAY_FUNC_EXTERN int (*VRay_VRayServer_start)(VRay::VRayServerNative* server) VRAY_FUNC_POINTER_INIT;
	VRAY_FUNC_EXTERN void (*VRay_VRayServer_stop)(VRay::VRayServerNative* server) VRAY_FUNC_POINTER_INIT;
	VRAY_FUNC_EXTERN void (*VRay_VRayServer_release)(VRay::VRayServerNative* server) VRAY_FUNC_POINTER_INIT;
	VRAY_FUNC_EXTERN void (*VRay_VRayServer_setServerStartCallback)(VRay::VRayServerNative* server, VRay::OnServerStart onServerStart, void* userObject) VRAY_FUNC_POINTER_INIT;
	VRAY_FUNC_EXTERN void (*VRay_VRayServer_setConnectCallback)(VRay::VRayServerNative* server, VRay::OnConnect onConnect, void* userObject) VRAY_FUNC_POINTER_INIT;
	VRAY_FUNC_EXTERN void (*VRay_VRayServer_setDisconnectCallback)(VRay::VRayServerNative* server, VRay::OnDisconnect onDisconnect, void* userObject) VRAY_FUNC_POINTER_INIT;
	VRAY_FUNC_EXTERN void (*VRay_VRayServer_setDumpMessageCallback)(VRay::VRayServerNative* server, VRay::OnDumpMessage onDumpMessage, void* userObject) VRAY_FUNC_POINTER_INIT;
	VRAY_FUNC_EXTERN void (*VRay_VRayServer_setServerCloseCallback)(VRay::VRayServerNative* server, VRay::OnServerClose onServerClose, void* userObject) VRAY_FUNC_POINTER_INIT;
	VRAY_FUNC_EXTERN void (*VRay_VRayServer_setProgressCallback)(VRay::VRayServerNative* server, VRay::OnProgress onProgress, void* userObject) VRAY_FUNC_POINTER_INIT;
	VRAY_FUNC_EXTERN uintptr_t (*VRay_VRayServer_setUserData)(VRay::VRayServerNative* server, int n, uintptr_t data) VRAY_FUNC_POINTER_INIT;
	VRAY_FUNC_EXTERN uintptr_t (*VRay_VRayServer_getUserData)(VRay::VRayServerNative* server, int n) VRAY_FUNC_POINTER_INIT;
	VRAY_FUNC_EXTERN void (*VRay_LicenseManager_releaseServerLicense)(void) VRAY_FUNC_POINTER_INIT;
	VRAY_FUNC_EXTERN char* (*VRay_getLicenseErrorTextStringFromErrorCode)(size_t licErrs) VRAY_FUNC_POINTER_INIT;
	VRAY_FUNC_EXTERN void (*C_memory_free)(void* mem) VRAY_FUNC_POINTER_INIT;

	#undef VRAY_FUNC_POINTER_INIT
	#undef VRAY_FUNC_EXTERN

#endif

namespace VRay {

	enum MessageLevel {
		MessageError = 9999,	///< 0..9999 - error messages
		MessageWarning = 19999,	///< 10000..19999 - warning messages
		MessageInfo = 29999,	///< 20000..29999 - info messages
		MessageDebug = 30000	///< >30000 - debug messages
	};

	union LicenseError {
		friend class VRayServer;
		friend std::ostream& operator <<(std::ostream& stream, const LicenseError& err);

		enum VRLAuthError {
			vrlauth_connectionError = -200, ///< Some problem with the connection to the license server
			vrlauth_notInitializedError, ///< Auth library not initialized
			vrlauth_allLicensesEngagedError, ///< Server reply was ok, there is a license for the product, but currently all licenses are used up
			vrlauth_noLicenseAtAllError, ///< Server reply was ok, but there isn't a license for this product at all
			vrlauth_unknownProductError, ///< The product ID is unknown to the client lib (cgauth)
			vrlauth_noError = 0 ///< No error, all licenses granted
		};
		typedef int RenderNodeErr;

	protected:
		struct {
			VRLAuthError appSDK;
			RenderNodeErr renderNode;
		};
		size_t errs;

	public:
		LicenseError(VRLAuthError appSDK = vrlauth_noError, int renderNode = 0)
			: appSDK(appSDK), renderNode(renderNode) {}

		bool error() const {
			return !!errs;
		}

		VRLAuthError getAppSDKError() const {
			return appSDK;
		}

		RenderNodeErr renderNodeError() const {
			return renderNode;
		}

		std::string toString() const;
	};

	inline std::string LicenseError::toString() const {
		char* str = ::VRay_getLicenseErrorTextStringFromErrorCode(errs);
		std::string res = str;
		::C_memory_free(str);
		return res;
	}

	inline std::ostream& operator <<(std::ostream& stream, const LicenseError& err) {
		char* str = ::VRay_getLicenseErrorTextStringFromErrorCode(err.errs);
		stream << std::string(str ? str : "");
		::C_memory_free(str);
		return stream;
	}

	struct ServerOptions {
		friend class VRayServer;

		int portNumber;
		int numThreads;

	private:
		mutable const char* internal_remoteScenePath;
		mutable const char* internal_pluginLibraryPath;
		mutable const char* internal_gpuPluginPath;

	public:

		union {
			int flags;
			struct {
				bool noRenderLicensePreCheck : 1; ///< if set appsdk will not check for render license before rendering is started
				bool changeCurrentDirectory : 1;  ///< change the current dir to remoteScenePath
				bool keepSceneInMemory : 1; ///< do not write temporary vrscene-file, keep all the data in memory
				bool overwriteLocalCacheSettings : 1; ///< true to override any local cache settings on the render servers with the value from the client machine.
			};
		};
		std::string remoteScenePath;
		std::string pluginLibraryPath; ///< specifies additional plugin library paths
		std::string gpuPluginPath;     ///< rt_cuda and rt_opencl plugins search path
		enum {
			CACHE_NO_LIMIT = 0,
			CACHE_HOUR_LIMIT = 1,
			CACHE_GB_LIMIT = 2,
		} cachedAssetsLimitType;
		float cachedAssetsLimitValue; ///< Value for the cache limit (hours or GB).

		ServerOptions() : portNumber(), numThreads(), flags(), cachedAssetsLimitType(), cachedAssetsLimitValue() {}

	private:
		const ServerOptions* prepare() const {
			internal_remoteScenePath = remoteScenePath.c_str();
			internal_pluginLibraryPath = pluginLibraryPath.c_str();
			internal_gpuPluginPath = gpuPluginPath.c_str();
			return this;
		}
	};


#if !defined(VRAY_RUNTIME_LOAD_PRIMARY) && !defined(VRAY_RUNTIME_LOAD_SECONDARY)

	class VRayServerInit {
		VRayServerInit(VRayServerInit&);	// copy construction disabled

	public:
		VRayServerInit() {}

		~VRayServerInit() {
			::VRay_LicenseManager_releaseServerLicense();
		}
	};

#else

	class VRayServerInit {
		VRayServerInit(VRayServerInit&);	// copy construction disabled

	public:
		explicit VRayServerInit(const char* const libraryFileName) {
			initialize(libraryFileName);
		}

		~VRayServerInit() {
			if (::VRay_LicenseManager_releaseServerLicense)
				::VRay_LicenseManager_releaseServerLicense();
			release();
		}

		operator bool () const {
			return !!hLib;
		}

	protected:
		VRayServerInit() {}


	#ifdef _WIN32
		HMODULE hLib;
	#else
		void* hLib;
	#endif

	#ifdef VRAY_NOTHROW

		void makeError() {
			hLib = NULL;
		}

	#endif

		void initialize(const char* libraryFileName) {
			if (libraryFileName == NULL) {
	#ifdef __APPLE__
				libraryFileName = "libVRaySDKLibrary.dylib";
	#elif defined(__linux__)
				libraryFileName = "libVRaySDKLibrary.so";
	#else // WIN32
				libraryFileName = "VRaySDKLibrary.dll";
	#endif
			}

	#undef GET_PROC_ADDR
	#undef PROC_TYPE
	#ifdef _WIN32
			hLib = ::LoadLibraryA(libraryFileName);
			#define GET_PROC_ADDR GetProcAddress
			#define PROC_TYPE (FARPROC&)
	#else
			hLib = ::dlopen(libraryFileName, RTLD_LAZY);
			#define GET_PROC_ADDR dlsym
			#define PROC_TYPE (void*&)
	#endif

	#ifndef VRAY_NOTHROW
			if (!hLib)
				throw InitErr("Cannot find V-Ray SDK Library file.");

			// VRayServer
			if (!(PROC_TYPE VRay_VRayServer_create = GET_PROC_ADDR(hLib, "VRay_VRayServer_create")))
				throw InitErr("Cannot find VRay_VRayServer_create in library.");

			if (!(PROC_TYPE VRay_VRayServer_run = GET_PROC_ADDR(hLib, "VRay_VRayServer_run")))
				throw InitErr("Cannot find VRay_VRayServer_run in library.");

			if (!(PROC_TYPE VRay_VRayServer_start = GET_PROC_ADDR(hLib, "VRay_VRayServer_start")))
				throw InitErr("Cannot find VRay_VRayServer_start in library.");

			if (!(PROC_TYPE VRay_VRayServer_stop = GET_PROC_ADDR(hLib, "VRay_VRayServer_stop")))
				throw InitErr("Cannot find VRay_VRayServer_stop in library.");

			if (!(PROC_TYPE VRay_VRayServer_release = GET_PROC_ADDR(hLib, "VRay_VRayServer_release")))
				throw InitErr("Cannot find VRay_VRayServer_release in library.");

			if (!(PROC_TYPE VRay_VRayServer_setServerStartCallback = GET_PROC_ADDR(hLib, "VRay_VRayServer_setServerStartCallback")))
				throw InitErr("Cannot find VRay_VRayServer_setServerStartCallback in library.");

			if (!(PROC_TYPE VRay_VRayServer_setConnectCallback = GET_PROC_ADDR(hLib, "VRay_VRayServer_setConnectCallback")))
				throw InitErr("Cannot find VRay_VRayServer_setConnectCallback in library.");

			if (!(PROC_TYPE VRay_VRayServer_setDisconnectCallback = GET_PROC_ADDR(hLib, "VRay_VRayServer_setDisconnectCallback")))
				throw InitErr("Cannot find VRay_VRayServer_setDisconnectCallback in library.");

			if (!(PROC_TYPE VRay_VRayServer_setDumpMessageCallback = GET_PROC_ADDR(hLib, "VRay_VRayServer_setDumpMessageCallback")))
				throw InitErr("Cannot find VRay_VRayServer_setDumpMessageCallback in library.");

			if (!(PROC_TYPE VRay_VRayServer_setServerCloseCallback = GET_PROC_ADDR(hLib, "VRay_VRayServer_setServerCloseCallback")))
				throw InitErr("Cannot find VRay_VRayServer_setServerCloseCallback in library.");

			if (!(PROC_TYPE VRay_VRayServer_setProgressCallback = GET_PROC_ADDR(hLib, "VRay_VRayServer_setProgressCallback")))
				throw InitErr("Cannot find VRay_VRayServer_setProgressCallback in library.");

			if (!(PROC_TYPE VRay_LicenseManager_releaseServerLicense = GET_PROC_ADDR(hLib, "VRay_LicenseManager_releaseServerLicense")))
				throw InitErr("Cannot find VRay_LicenseManager_releaseServerLicense in library.");

			if (!(PROC_TYPE VRay_getLicenseErrorTextStringFromErrorCode = GET_PROC_ADDR(hLib, "VRay_getLicenseErrorTextStringFromErrorCode")))
				throw InitErr("Cannot find VRay_getLicenseErrorTextStringFromErrorCode in library.");

			if (!(PROC_TYPE C_memory_free = GET_PROC_ADDR(hLib, "C_memory_free")))
				throw InitErr("Cannot find C_memory_free in library.");
	#else
			if (!hLib)
				return;

			// VRayServer
			if (!(PROC_TYPE VRay_VRayServer_create = GET_PROC_ADDR(hLib, "VRay_VRayServer_create")))
				return makeError();

			if (!(PROC_TYPE VRay_VRayServer_run = GET_PROC_ADDR(hLib, "VRay_VRayServer_run")))
				return makeError();

			if (!(PROC_TYPE VRay_VRayServer_start = GET_PROC_ADDR(hLib, "VRay_VRayServer_start")))
				return makeError();

			if (!(PROC_TYPE VRay_VRayServer_stop = GET_PROC_ADDR(hLib, "VRay_VRayServer_stop")))
				return makeError();

			if (!(PROC_TYPE VRay_VRayServer_release = GET_PROC_ADDR(hLib, "VRay_VRayServer_release")))
				return makeError();

			if (!(PROC_TYPE VRay_VRayServer_setServerStartCallback = GET_PROC_ADDR(hLib, "VRay_VRayServer_setServerStartCallback")))
				return makeError();

			if (!(PROC_TYPE VRay_VRayServer_setConnectCallback = GET_PROC_ADDR(hLib, "VRay_VRayServer_setConnectCallback")))
				return makeError();

			if (!(PROC_TYPE VRay_VRayServer_setDisconnectCallback = GET_PROC_ADDR(hLib, "VRay_VRayServer_setDisconnectCallback")))
				return makeError();

			if (!(PROC_TYPE VRay_VRayServer_setDumpMessageCallback = GET_PROC_ADDR(hLib, "VRay_VRayServer_setDumpMessageCallback")))
				return makeError();

			if (!(PROC_TYPE VRay_VRayServer_setServerCloseCallback = GET_PROC_ADDR(hLib, "VRay_VRayServer_setServerCloseCallback")))
				return makeError();

			if (!(PROC_TYPE VRay_VRayServer_setProgressCallback = GET_PROC_ADDR(hLib, "VRay_VRayServer_setProgressCallback")))
				return makeError();

			if (!(PROC_TYPE VRay_LicenseManager_releaseServerLicense = GET_PROC_ADDR(hLib, "VRay_LicenseManager_releaseServerLicense")))
				return makeError();

			if (!(PROC_TYPE VRay_getLicenseErrorTextStringFromErrorCode = GET_PROC_ADDR(hLib, "VRay_getLicenseErrorTextStringFromErrorCode")))
				return makeError();

			if (!(PROC_TYPE C_memory_free = GET_PROC_ADDR(hLib, "C_memory_free")))
				return makeError();
	#endif
		}
		#undef GET_PROC_ADDR
		#undef PROC_TYPE

		void release() {
			if (hLib)
			#ifdef _WIN32
				FreeLibrary(hLib);
			#else
				dlclose(hLib);
			#endif
		}
	};
#endif


	class VRayServer {
		VRayServer(const VRayServer&);
		VRayServer& operator=(const VRayServer&);

		class StartCloseDelegate
		{
			typedef void (*tstub)(void* pobject, VRayServer& server, void *userObj);
			typedef void (*tfunc)(VRayServer& server, void *userObj);

			void *pobject;
			union tmethod {
				tstub pstub;
				tfunc pfunc;
				tmethod() : pstub() {}
				tmethod(tstub pstub) : pstub(pstub) {}
				tmethod(tfunc pfunc) : pfunc(pfunc) {}
			} pmethod;
			void *puserObj;

			template <class T, void (T::*TMethod)(VRayServer&, void*)>
			static void method_stub(void* pobject, VRayServer& server, void *puserObj) {
				T* p = static_cast<T*>(pobject);
				return (p->*TMethod)(server, puserObj);
			}

			StartCloseDelegate(tfunc pfunc, void *puserObj) : pobject(), pmethod(pfunc), puserObj(puserObj) {}
			StartCloseDelegate(void *pobject, tstub pstub, void *puserObj) : pobject(pobject), pmethod(pstub), puserObj(puserObj) {}

		public:
			StartCloseDelegate() {}

			template <class T, void (T::*TMethod)(VRayServer&, void*)>
			static StartCloseDelegate from_method(T* pobject, void *puserObj) {
				return StartCloseDelegate(pobject, &method_stub<T, TMethod>, puserObj);
			}

			static StartCloseDelegate from_function(tfunc pfunc, void *puserObj) {
				return StartCloseDelegate(pfunc, puserObj);
			}

			void operator()(VRayServer& server) const {
				return pobject ? (*pmethod.pstub)(pobject, server, puserObj) : (*pmethod.pfunc)(server, puserObj);
			}
		} serverStartDelegate, serverCloseDelegate;

		class ConnectDisconnectDelegate
		{
			typedef void (*tstub)(void* pobject, VRayServer& server, const char *msg, void *userObj);
			typedef void (*tfunc)(VRayServer& server, const char *msg, void *userObj);

			void *pobject;
			union tmethod {
				tstub pstub;
				tfunc pfunc;
				tmethod() : pstub() {}
				tmethod(tstub pstub) : pstub(pstub) {}
				tmethod(tfunc pfunc) : pfunc(pfunc) {}
			} pmethod;
			void *puserObj;

			template <class T, void (T::*TMethod)(VRayServer&, const char *msg, void*)>
			static void method_stub(void* pobject, VRayServer& server, const char *msg, void *puserObj) {
				T* p = static_cast<T*>(pobject);
				return (p->*TMethod)(server, msg, puserObj);
			}

			ConnectDisconnectDelegate(tfunc pfunc, void *puserObj) : pobject(), pmethod(pfunc), puserObj(puserObj) {}
			ConnectDisconnectDelegate(void *pobject, tstub pstub, void *puserObj) : pobject(pobject), pmethod(pstub), puserObj(puserObj) {}

		public:
			ConnectDisconnectDelegate() {}

			template <class T, void (T::*TMethod)(VRayServer&, const char*, void*)>
			static ConnectDisconnectDelegate from_method(T* pobject, void *puserObj) {
				return ConnectDisconnectDelegate(pobject, &method_stub<T, TMethod>, puserObj);
			}

			static ConnectDisconnectDelegate from_function(tfunc pfunc, void *puserObj) {
				return ConnectDisconnectDelegate(pfunc, puserObj);
			}

			void operator()(VRayServer& server, const char *msg) const {
				return pobject ? (*pmethod.pstub)(pobject, server, msg, puserObj) : (*pmethod.pfunc)(server, msg, puserObj);
			}
		} connectDelegate, disconnectDelegate;

		class DumpMessageDelegate
		{
			typedef void (*tstub)(void* pobject, VRayServer& server, const char *msg, int level, void *userObj);
			typedef void (*tfunc)(VRayServer& server, const char *msg, int level, void *userObj);

			void *pobject;
			union tmethod {
				tstub pstub;
				tfunc pfunc;
				tmethod() : pstub() {}
				tmethod(tstub pstub) : pstub(pstub) {}
				tmethod(tfunc pfunc) : pfunc(pfunc) {}
			} pmethod;
			void *puserObj;

			template <class T, void (T::*TMethod)(VRayServer&, const char *msg, int level, void*)>
			static void method_stub(void* pobject, VRayServer& server, const char *msg, int level, void *puserObj) {
				T* p = static_cast<T*>(pobject);
				return (p->*TMethod)(server, msg, level, puserObj);
			}

			DumpMessageDelegate(tfunc pfunc, void *puserObj) : pobject(), pmethod(pfunc), puserObj(puserObj) {}
			DumpMessageDelegate(void *pobject, tstub pstub, void *puserObj) : pobject(pobject), pmethod(pstub), puserObj(puserObj) {}

		public:
			DumpMessageDelegate() {}

			template <class T, void (T::*TMethod)(VRayServer&, const char*, int, void*)>
			static DumpMessageDelegate from_method(T* pobject, void *puserObj) {
				return DumpMessageDelegate(pobject, &method_stub<T, TMethod>, puserObj);
			}

			static DumpMessageDelegate from_function(tfunc pfunc, void *puserObj) {
				return DumpMessageDelegate(pfunc, puserObj);
			}

			void operator()(VRayServer& server, const char *msg, int level) const {
				return pobject ? (*pmethod.pstub)(pobject, server, msg, level, puserObj) : (*pmethod.pfunc)(server, msg, level, puserObj);
			}
		} dumpMessageDelegate;

		class ProgressDelegate
		{
			typedef void (*tstub)(void* pobject, VRayServer& server, const char *msg, int progress, int total, void *userObj);
			typedef void (*tfunc)(VRayServer& server, const char *msg, int progress, int total, void *userObj);

			void *pobject;
			union tmethod {
				tstub pstub;
				tfunc pfunc;
				tmethod() : pstub() {}
				tmethod(tstub pstub) : pstub(pstub) {}
				tmethod(tfunc pfunc) : pfunc(pfunc) {}
			} pmethod;
			void *puserObj;

			template <class T, void (T::*TMethod)(VRayServer&, const char *msg, int progress, int total, void*)>
			static void method_stub(void* pobject, VRayServer& server, const char *msg, int progress, int total, void *puserObj) {
				T* p = static_cast<T*>(pobject);
				return (p->*TMethod)(server, msg, progress, total, puserObj);
			}

			ProgressDelegate(tfunc pfunc, void *puserObj) : pobject(), pmethod(pfunc), puserObj(puserObj) {}
			ProgressDelegate(void *pobject, tstub pstub, void *puserObj) : pobject(pobject), pmethod(pstub), puserObj(puserObj) {}

		public:
			ProgressDelegate() {}

			template <class T, void (T::*TMethod)(VRayServer&, const char*, int, int, void*)>
			static ProgressDelegate from_method(T* pobject, void *puserObj) {
				return ProgressDelegate(pobject, &method_stub<T, TMethod>, puserObj);
			}

			static ProgressDelegate from_function(tfunc pfunc, void *puserObj) {
				return ProgressDelegate(pfunc, puserObj);
			}

			void operator()(VRayServer& server, const char *msg, int progress, int total) const {
				return pobject ? (*pmethod.pstub)(pobject, server, msg, progress, total, puserObj) : (*pmethod.pfunc)(server, msg, progress, total, puserObj);
			}
		} progressDelegate;

		VRayServerNative* serverNative;

		static void serverStartCallback(void* s) {
			VRayServer* &server = reinterpret_cast<VRayServer*&>(s);
			server->serverStartDelegate(*server);
		}

		static void serverCloseCallback(void* s) {
			VRayServer* &server = reinterpret_cast<VRayServer*&>(s);
			server->serverCloseDelegate(*server);
		}

		static void serverConnectCallback(const char* msg, void* s) {
			VRayServer* &server = reinterpret_cast<VRayServer*&>(s);
			server->connectDelegate(*server, msg);
		}

		static void serverDisconnectCallback(const char* msg, void* s) {
			VRayServer* &server = reinterpret_cast<VRayServer*&>(s);
			server->disconnectDelegate(*server, msg);
		}

		static void dumpMessageCallback(const char* msg, int level, void* s) {
			VRayServer* &server = reinterpret_cast<VRayServer*&>(s);
			server->dumpMessageDelegate(*server, msg, level);
		}

		static void progressCallback(const char* msg, int progress, int total, void* s) {
			VRayServer* &server = reinterpret_cast<VRayServer*&>(s);
			server->progressDelegate(*server, msg, progress, total);
		}

		LicenseError licErr;

		void createInstance(const ServerOptions* options) {
			serverNative = ::VRay_VRayServer_create(options, &licErr.errs);
		#ifndef VRAY_NOTHROW
			if (licErr.error())
				throw InstantiationErr(licErr.toString());
			if (!serverNative)
				throw InstantiationErr();
		#endif
		}

	public:
		VRayServer() {
			createInstance(NULL);
		}

		VRayServer(const ServerOptions &serverOptions) {
			createInstance(serverOptions.prepare());
		}

		~VRayServer() {
			if (serverNative) {
				::VRay_VRayServer_release(serverNative);
			}
		}

		/// If exceptions are not enabled use this function to test whether you have a valid server
		/// @retval true - server instance is valid, false - server instance is invalid
		operator bool () const {
			return !!serverNative;
		}

		LicenseError getLicenseError() const {
			return licErr;
		}

		void setOnServerStart(void (*callback)(VRayServer&, void*), const void* userData = NULL) {
			serverStartDelegate = StartCloseDelegate::from_function(callback, const_cast<void*>(userData));
			::VRay_VRayServer_setServerStartCallback(serverNative, callback ? &serverStartCallback : NULL, this);
		}

		template<class T, void (T::*TMethod)(VRayServer&, void*)>
		void setOnServerStart(T& cls, const void* userData = NULL) {
			serverStartDelegate = StartCloseDelegate::from_method<T, TMethod>(&cls, const_cast<void*>(userData));
			::VRay_VRayServer_setServerStartCallback(serverNative, &serverStartCallback, this);
		}

		void setOnServerClose(void (*callback)(VRayServer&, void*), const void* userData = NULL) {
			serverCloseDelegate = StartCloseDelegate::from_function(callback, const_cast<void*>(userData));
			::VRay_VRayServer_setServerCloseCallback(serverNative, callback ? &serverCloseCallback : NULL, this);
		}

		template<class T, void (T::*TMethod)(VRayServer&, void*)>
		void setOnServerClose(T& cls, const void* userData = NULL) {
			serverCloseDelegate = StartCloseDelegate::from_method<T, TMethod>(&cls, const_cast<void*>(userData));
			::VRay_VRayServer_setServerCloseCallback(serverNative, &serverCloseCallback, this);
		}

		void setOnConnect(void (*callback)(VRayServer&, const char* msg, void*), const void* userData = NULL) {
			connectDelegate = ConnectDisconnectDelegate::from_function(callback, const_cast<void*>(userData));
			::VRay_VRayServer_setConnectCallback(serverNative, callback ? &serverConnectCallback : NULL, this);
		}

		template<class T, void (T::*TMethod)(VRayServer&, const char* msg, void*)>
		void setOnDisconnect(T& cls, const void* userData = NULL) {
			disconnectDelegate = ConnectDisconnectDelegate::from_method<T, TMethod>(&cls, const_cast<void*>(userData));
			::VRay_VRayServer_setDisconnectCallback(serverNative, &serverDisconnectCallback, this);
		}

		void setOnDisconnect(void (*callback)(VRayServer&, const char* msg, void*), const void* userData = NULL) {
			disconnectDelegate = ConnectDisconnectDelegate::from_function(callback, const_cast<void*>(userData));
			::VRay_VRayServer_setDisconnectCallback(serverNative, callback ? &serverDisconnectCallback : NULL, this);
		}

		template<class T, void (T::*TMethod)(VRayServer&, const char* msg, void*)>
		void setOnConnect(T& cls, const void* userData = NULL) {
			connectDelegate = ConnectDisconnectDelegate::from_method<T, TMethod>(&cls, const_cast<void*>(userData));
			::VRay_VRayServer_setConnectCallback(serverNative, &serverConnectCallback, this);
		}

		void setOnDumpMessage(void (*callback)(VRayServer&, const char* msg, int level, void*), const void* userData = NULL) {
			dumpMessageDelegate = DumpMessageDelegate::from_function(callback, const_cast<void*>(userData));
			::VRay_VRayServer_setDumpMessageCallback(serverNative, callback ? &dumpMessageCallback : NULL, this);
		}

		template<class T, void (T::*TMethod)(VRayServer&, const char* msg, int level, void*)>
		void setOnDumpMessage(T& cls, const void* userData = NULL) {
			dumpMessageDelegate = DumpMessageDelegate::from_method<T, TMethod>(&cls, const_cast<void*>(userData));
			::VRay_VRayServer_setDumpMessageCallback(serverNative, &dumpMessageCallback, this);
		}

		void setOnProgress(void (*callback)(VRayServer&, const char* msg, int elementNumber, int elementsCount, void*), const void* userData = NULL) {
			progressDelegate = ProgressDelegate::from_function(callback, const_cast<void*>(userData));
			::VRay_VRayServer_setProgressCallback(serverNative, callback ? &progressCallback : NULL, this);
		}

		template<class T, void (T::*TMethod)(VRayServer&, const char* msg, int elementNumber, int elementsCount, void*)>
		void setOnProgress(T& cls, const void* userData = NULL) {
			progressDelegate = ProgressDelegate::from_method<T, TMethod>(&cls, const_cast<void*>(userData));
			::VRay_VRayServer_setProgressCallback(serverNative, &progressCallback, this);
		}

		/// Starts V-Ray Server. Blocking call.
		int run() const {
			return ::VRay_VRayServer_run(serverNative);
		}

		/// Starts V-Ray Server in a separate thread. Non-blocking call.
		int start() const {
			return ::VRay_VRayServer_start(serverNative);
		}

		/// Terminates the server started by 'start' in a separate thread.
		void stop() const {
			::VRay_VRayServer_stop(serverNative);
		}
	};

}

#endif
