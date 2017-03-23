#ifndef _VRAY_THROW_HPP_
#define _VRAY_THROW_HPP_

#include <string>

namespace VRay {
	class VRayException {
		std::string msg;

	public:
		explicit VRayException(const std::string& msg) : msg(msg) { }
		explicit VRayException() : msg("Unknown V-Ray exception.") { }

		const char* what() const {
			return msg.c_str();
		}

		std::string toString() const {
			return msg;
		}
	};

	/// This is thrown when loading the VRaySDKLibrary dynamic library fails.
	struct InitErr : public VRayException {
		explicit InitErr(const std::string& msg) : VRayException(msg) { }
		explicit InitErr() : VRayException("Initialization error.") { }
	};

	/// This is thrown when a renderer instance can't be created. This is usually due to a license issue.
	struct InstantiationErr : public VRayException {
		explicit InstantiationErr(const std::string& msg) : VRayException(msg) { }
		explicit InstantiationErr() : VRayException("Instantiation error.") { }
	};

	struct FileReadErr : public VRayException {
		explicit FileReadErr(const std::string& msg) : VRayException(msg) { }
		explicit FileReadErr() : VRayException("File reading error or file not found.") { }
	};

	/// This is thrown when a plugin class is not available. The DLL may be missing or not on the path.
	struct PluginUnavailableErr : public VRayException {
		explicit PluginUnavailableErr(const std::string& msg) : VRayException(msg) { }
		explicit PluginUnavailableErr() : VRayException("No such plugin error.") { }
	};

	/// This is thrown when a plugin passed to the render element API is not really a render element plugin.
	struct InvalidRenderElementErr : public VRayException {
		explicit InvalidRenderElementErr(const std::string& msg) : VRayException(msg) { }
		explicit InvalidRenderElementErr() : VRayException("Invalid Render element.") { }
	};

	/// This is thrown when an invalid type conversion/interpretation is attempted.
	struct InvalidTypeErr : public VRayException {
		explicit InvalidTypeErr(const std::string& msg) : VRayException(msg) { }
		explicit InvalidTypeErr() : VRayException("Invalid type.") { }
	};

	struct InvalidListTypeErr : public InvalidTypeErr {
		explicit InvalidListTypeErr(const std::string& msg) : InvalidTypeErr(msg) { }
		explicit InvalidListTypeErr() : InvalidTypeErr("Invalid list type.") { }
	};
}

#endif
