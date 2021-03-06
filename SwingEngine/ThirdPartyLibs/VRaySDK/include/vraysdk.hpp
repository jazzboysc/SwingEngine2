#ifndef _VRAY_SDK_HPP_
#define _VRAY_SDK_HPP_

#ifndef VRAY_NOTHROW
	#include "vraythrow.hpp"
#endif

#include "errcodes.h"

#include <new>
#include <cmath>
#include <utility>
#ifndef _MSC_VER
#include <stdint.h>
#endif
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>

#ifdef VRAY_RUNTIME_LOAD_PRIMARY
	#ifdef _WIN32
		#include <windows.h>
	#else
		#include <dlfcn.h>
	#endif
#endif

#define VRAY_MAXIMUM2(a, b) ((a) > (b) ? (a) : (b))
#define VRAY_MAXIMUM3(a, b, c) VRAY_MAXIMUM2(VRAY_MAXIMUM2(a, b), c)
#define VRAY_MAXIMUM4(a, b, c, d) VRAY_MAXIMUM2(VRAY_MAXIMUM3(a, b, c), d)

namespace VRay {

#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(disable: 4201) // nonstandard extension used : nameless struct/union
#endif // _MSC_VER

#ifdef _MSC_VER
#	define VRAY_DEPRECATED(msg) __declspec(deprecated(msg))
#else // unix
// GCC <= 4.4 doesn't support the optional message parameter
#	if defined(__clang__) || COMPILER_VERSION >= 450
#		define VRAY_DEPRECATED(msg) __attribute__((deprecated(msg)))
#	else
#		define VRAY_DEPRECATED(msg) __attribute__((deprecated))
#	endif
#endif // _MSC_VER

#ifdef VRAY_SDK_INTEROPERABILITY
	struct Vector;
	struct Color;
	namespace VUtils {
		class Value;
		class CharString;
		template<class T> struct PtrArray;
		typedef PtrArray<int> IntRefList;
		typedef PtrArray<float> FloatRefList;
		typedef PtrArray<Vector> VectorRefList;
		typedef PtrArray<Color> ColorRefList;
		typedef PtrArray<CharString> CharStringRefList;
		typedef PtrArray<Value> ValueRefList;
	}
#endif // VRAY_SDK_INTEROPERABILITY

	enum RendererState {
		UNKNOWN = -1,
		IDLE_INITIALIZED, ///< Initial state only after construction.
		IDLE_STOPPED,     ///< Rendering was interrupted by stop() or from the UI.
		IDLE_ERROR,       ///< Rendering was aborted due to an error.
		IDLE_FRAME_DONE,  ///< Waiting for continueSequence between the frames of an animation sequence.
		IDLE_DONE,        ///< Last rendering was completed successfully.
		PREPARING,        ///< Preparing to start rendering the scene.
		RENDERING,        ///< Rendering is ongoing. This may be any mode of rendering and any stage such as a prepass.
		RENDERING_PAUSED, ///< Interactive rendering has been paused.
	};

	enum Type {

		// Plugin Property Types
		TYPE_INT = 0,   ///< Integer number.
		TYPE_FLOAT = 1,   ///< Single-precision floating-point number.
		TYPE_DOUBLE = 2,   ///< Double-precision floating-point number.
		TYPE_BOOL = 3,   ///< Boolean parameter.
		TYPE_VECTOR = 4,   ///< A Vector parameter.
		TYPE_COLOR = 5,   ///< A Color parameter.
		TYPE_ACOLOR = 6,   ///< An AColor parameter.
		TYPE_MATRIX = 7,   ///< A Matrix parameter.
		TYPE_TRANSFORM = 8,   ///< A TraceTransform parameter.

		TYPE_STRING = 9,   ///< A string parameter.
		TYPE_OBJECT = 10,   ///< Parameter that points to another plugin.
		TYPE_TEXTURE = 11,   ///< Input AColor texture paramete.
		TYPE_LIST = 12,   ///< A list parameter, consisting of other components.
		TYPE_TEXTUREFLOAT = 13,   ///< Input floating-point texture parameter.
		TYPE_TEXTUREINT = 14,   ///< Input integer texture parameter.
		TYPE_TEXTUREVECTOR = 15,   ///< Input Vector texture parameter.
		TYPE_TEXTUREMATRIX = 16,   ///< Input Matrix texture parameter.
		TYPE_TEXTURETRANSFORM = 17,   ///< Input Transform texture parameter.

		// List types
		TYPE_GENERAL_LIST = TYPE_LIST,

		TYPE_INT_LIST = 100,
		TYPE_FLOAT_LIST = 101,
		TYPE_BOOL_LIST = 103,
		TYPE_VECTOR_LIST = 104,
		TYPE_COLOR_LIST = 105,
		TYPE_ACOLOR_LIST = 106,
		TYPE_MATRIX_LIST = 107,
		TYPE_TRANSFORM_LIST = 108,
		TYPE_STRING_LIST = 109,

		// Special types
		TYPE_STRING_PTR = 200,
		TYPE_PLUGIN_PROP_PTR = 201,

		TYPE_INT_LIST_PTR = 2000,
		TYPE_FLOAT_LIST_PTR = 2001,
		TYPE_DOUBLE_LIST_PTR = 2002,
		TYPE_VECTOR_LIST_PTR = 2004,
		TYPE_COLOR_LIST_PTR = 2005,

		// Output (multi-texture) Property Types
		TYPE_OUTPUTTEXTURE = 1000, ///< Output AColor texture parameter.
		TYPE_OUTPUTTEXTUREFLOAT = 1001, ///< Output floating-point texture parameter.
		TYPE_OUTPUTTEXTUREINT = 1002, ///< Output integer texture parameter.
		TYPE_OUTPUTTEXTUREVECTOR = 1003, ///< Output Vector texture parameter.
		TYPE_OUTPUTTEXTUREMATRIX = 1004, ///< Output Matrix texture parameter.
		TYPE_OUTPUTTEXTURETRANSFORM = 1005, ///< Output Transform texture parameter.

		TYPE_UNSPECIFIED = 10000,

		TYPE_ERROR = -1
	};

	/// Identifies the type of message from the DumpMessage callback. *NOTICE*: level may not be equal, these are range constraints
	enum MessageLevel {
		MessageError = 9999,	///< 0..9999 - error messages
		MessageWarning = 19999,	///< 10000..19999 - warning messages
		MessageInfo = 29999,	///< 20000..29999 - info messages
		MessageDebug = 30000	///< >30000 - debug messages
	};

	enum BucketRegionGenerationMode {
		TOP_TO_BOTTOM = 0,
		LEFT_TO_RIGHT = 1,
		CHECKER = 2,
		SPIRAL = 3,
		TRIANGLE = 4,
		HILBERT = 5
	};

	/// A rectangle defined by top-left and bottom-right frame coordinates
	struct ImageRegion
	{
	private:
		int x0, y0, x1, y1;

	public:
		ImageRegion(int x = 0, int y = 0, int w = 0, int h = 0) {
			set(x, y, w, h);
		}

		void set(int x, int y, int w, int h) {
			x0 = x;
			y0 = y;
			x1 = x + w;
			y1 = y + h;
		}

		void setX(int x) {
			x0 = x;
		}

		void setY(int y) {
			y0 = y;
		}

		void setWidth(int width) {
			x1 = x0 + width;
		}

		void setHeight(int height) {
			y1 = y0 + height;
		}

		int getX() const {
			return x0;
		}

		int getY() const {
			return y0;
		}

		int getWidth() const {
			return x1 - x0;
		}

		int getHeight() const {
			return y1 - y0;
		}
	};

	struct VRLSettings;

	typedef unsigned long long InstanceId; // FOR INTERNAL USE ONLY
	const InstanceId NO_ID = InstanceId(-1);

	typedef unsigned char byte;
	typedef int RGB32;
	typedef int Bool;

	class VRayRenderer;
	class Value;
	struct Color;
	struct AColor;

	/// Wrapper around a piece of heap memory. Takes care of calling the proper deallocating function.
	class MemoryBuffer {

	protected:
		MemoryBuffer() {}
		MemoryBuffer(const MemoryBuffer&);

	public:
		void* getData() {
			return static_cast<void*>(this);
		}
		void operator delete(void* ptr);
	};

	/// The raw memory contents of a PNG image.
	class Png : public MemoryBuffer {};
	/// The raw memory contents of a JPEG image.
	class Jpeg : public MemoryBuffer {};
	/// The raw memory contents of a serialized VFB state.
	class VFBState : public MemoryBuffer {};

	#pragma pack(push, 1)

	/// Equivalent to the Windows API BITMAPFILEHEADER struct
	struct BitmapFileHeader {
		unsigned short bfType;
		unsigned bfSize;
		unsigned bfReserved;
		unsigned bfOffBits; ///< Offset of raw pixel data in *bytes* relative to the start of this header
	};

	/// Equivalent to the Windows API BITMAPINFOHEADER struct
	struct BitmapInfoHeader {
		unsigned biSize;
		int      biWidth;
		int      biHeight;
		unsigned short biPlanes;
		unsigned short biBitCount;
		unsigned biCompression;
		unsigned biSizeImage;
		int      biXPelsPerMeter;
		int      biYPelsPerMeter;
		unsigned biClrUsed;
		unsigned biClrImportant;
	};

	/// Wrapper around BMP metainfo with pixel accessor method
	struct BmpHeader: BitmapFileHeader, BitmapInfoHeader {
		void* getPixels() const {
			return (byte*)this + bfOffBits;
		}

		unsigned getRowStride() const {
			return (getWidth()*(biBitCount/8) + 3) & ~3;
		}

		int getWidth() const {
			return abs(biWidth);
		}

		int getHeight() const {
			return abs(biHeight);
		}

		int getBitsPerPixel() const {
			return biBitCount;
		}
	};

	/// The raw memory contents of a BMP image.
	class Bmp : public BmpHeader, public MemoryBuffer {};

	/// Image helper class containing raw full float32 data that can export to 8-bit formats - BMP, JPEG, PNG.
	/// It also supplies some basic image manipulation methods.
	struct VRayImage {
		static VRayImage* create(int width, int height);
		void operator delete (void *bmp);
		VRayImage* clone() const;

		int getWidth() const;
		int getHeight() const;
		bool getSize(int& width, int& height) const;

		VRayImage* crop(int x, int y, int width, int height) const;
		VRayImage* downscale(int width, int height) const;
		VRayImage* downscale(int srcX, int srcY, int srcWidth, int srcHeight, int dstWidth, int dstHeight) const;
		VRayImage* fitIn(int width, int height) const;
		VRayImage* fitOut(int width, int height) const;
		VRayImage* cutIn(int width, int height) const;

		static VRayImage* createFromBmp(const void* buffer, size_t size = 0);
		static VRayImage* createFromBmp(const VRayRenderer& renderer, const void* buffer, size_t size = 0);

		static VRayImage* createFromPng(const void* buffer, size_t size);
		static VRayImage* createFromPng(const VRayRenderer& renderer, const void* buffer, size_t size);

		static VRayImage* createFromJpeg(const void* buffer, size_t size);
		static VRayImage* createFromJpeg(const VRayRenderer& renderer, const void* buffer, size_t size);

		bool draw(const VRayImage* image, int x, int y);
		bool addColor(const Color& color);
		bool addColor(const AColor& color);
		bool changeExposure(float ev);
		bool changeGamma(float gamma);
		bool changeContrast(float contrast);
		bool makeNegative();

		bool saveToBmpFile(const char* fileName, bool preserveAlpha = false, bool swapChannels = false) const;
		bool saveToBmpFile(const char* fileName, const VRayRenderer& renderer, bool preserveAlpha = false, bool swapChannels = false) const;
		bool saveToBmpFile(const std::string& fileName, bool preserveAlpha = false, bool swapChannels = false) const;
		bool saveToBmpFile(const std::string& fileName, const VRayRenderer& renderer, bool preserveAlpha = false, bool swapChannels = false) const;

		bool saveToPngFile(const char* fileName, bool preserveAlpha = false) const;
		bool saveToPngFile(const char* fileName, const VRayRenderer& renderer, bool preserveAlpha = false) const;
		bool saveToPngFile(const std::string& fileName, bool preserveAlpha = false) const;
		bool saveToPngFile(const std::string& fileName, const VRayRenderer& renderer, bool preserveAlpha = false) const;

		bool saveToJpegFile(const char* fileName, int quality = 0) const;
		bool saveToJpegFile(const char* fileName, const VRayRenderer& renderer, int quality = 0) const;
		bool saveToJpegFile(const std::string& fileName, int quality = 0) const;
		bool saveToJpegFile(const std::string& fileName, const VRayRenderer& renderer, int quality = 0) const;

		Bmp* getBmp(bool preserveAlpha = false, bool swapChannels = false) const;
		Bmp* getBmp(size_t& size, bool preserveAlpha = false, bool swapChannels = false) const;
		Bmp* getBmp(const VRayRenderer& renderer, bool preserveAlpha = false, bool swapChannels = false) const;
		Bmp* getBmp(size_t& size, const VRayRenderer& renderer, bool preserveAlpha = false, bool swapChannels = false) const;

		Png* getPng(bool preserveAlpha = false) const;
		Png* getPng(size_t& size, bool preserveAlpha = false) const;
		Png* getPng(const VRayRenderer& renderer, bool preserveAlpha = false) const;
		Png* getPng(size_t& size, const VRayRenderer& renderer, bool preserveAlpha = false) const;

		Jpeg* getJpeg(int quality = 0) const;
		Jpeg* getJpeg(size_t& size, int quality = 0) const;
		Jpeg* getJpeg(const VRayRenderer& renderer, int quality = 0) const;
		Jpeg* getJpeg(size_t& size, const VRayRenderer& renderer, int quality = 0) const;

		/// Extracts 8-bit bitmap data from this image (without BMP headers). Default channel order is BGR, no alpha, top-bottom.
		/// @param size [out] The size of the returned buffer in bytes.
		/// @param preserveAlpha [in] false - BGR/RGB (bytes_per_pixel = 3), true - BGRA/RGBA (bytes_per_pixel = 4).
		/// @param swapChannels [in] false - BGR(A) order, true - BRGB(A) order.
		/// @param reverseY [in] false - fill top-bottom bitmap, true - fill bottom-top bitmap.
		/// @param stride [in] the byte offset between the beginning of two consecutive rows of the output bitmap.
		///  if (stride > 0) use max(stride, width*bytes_per_pixel).
		///  if (stride== 0) use width*bytes_per_pixel rounded up to the smallest factor of 4 (this the default for BMP).
		///  if (stride < 0) use width*bytes_per_pixel rounded up to the smallest factor of -stride; -stride must be a power of 2.
		/// @return A pointer to the BMP pixel data. On error, NULL pointer is returned and size == 0.
		MemoryBuffer* toBitmapData(size_t& size, bool preserveAlpha = false, bool swapChannels = false, bool reverseY = false, int stride = 0) const;

		AColor* getPixelData();
		const AColor* getPixelData() const;
		AColor* getPixelData(size_t& count);
		const AColor* getPixelData(size_t& count) const;

	private:
		VRayImage();
		VRayImage(const VRayImage&);
		VRayImage& operator=(const VRayImage &image);
	};

	#pragma pack(pop)

	/// A variation of VRayImage to be allocated on the stack.
	class LocalVRayImage {
		VRayImage* img;
		LocalVRayImage(const LocalVRayImage& img);
		LocalVRayImage& operator=(const LocalVRayImage& img);

	public:
		LocalVRayImage(VRayImage* img) : img(img) {}

		operator VRayImage* () {
			return img;
		}

		VRayImage* operator->() {
			return img;
		}

		VRayImage& operator*() {
			return *img;
		}

		LocalVRayImage& operator=(VRayImage* img) {
			delete this->img;
			this->img = img;
			return *this;
		}

		~LocalVRayImage() {
			delete img;
		}
	};

	/// Options passed to VRayRenderer::saveImage
	struct ImageWriterOptions {
		union ImageWriterFlags {
			int	asInt;

			struct {
				bool multipleFiles : 1; ///< unused
				bool skipAlpha : 1;  ///< do not write the alpha channel into a separate file
				bool frameNumber : 1;  ///< the current frame number will be appended to the file name(s)
				bool noAlpha : 1; ///<  do not write the alpha channel together with the color data
				bool singleChannel : 1;  ///< only a single channel is written to the file (the current one in vfb)
				bool skipRGB : 1; ///< do not write the RGB channel into a separate file
				bool writeIntegerIDs : 1; ///< write to EXR as integer data
				bool velocityZeroBased : 1; ///< unused
				bool multiChannel : 1;  ///< create a single multi-channel file if the file format supports it
				bool applyColorCorrections : 1; ///< bake the VFB corrections to the output file
			};

			ImageWriterFlags() : asInt() {}
		} flags;

		enum CompressionType {
			DEFAULT,
			NO_COMPRESSION,
			RUN_LENGTH,
			ZIP_SCANLINE,
			ZIP_BLOCK,
			PIZ,
			PXR24
		} compressionType; ///< for EXR format

		int bitsPerChannel; ///< either 16 or 32

		ImageWriterOptions() : flags(), compressionType(), bitsPerChannel(32) {}
	};

	/// General purpose vector in 3D space using float32
	struct Vector {
		union {
			struct { float x, y, z; };
			float f[3];
		};

		Vector() : x(), y(), z() {}

		template <typename T1, typename T2, typename T3>
		Vector(T1 x_, T2 y_, T3 z_) :  x(float(x_)), y(float(y_)), z(float(z_)) {}

		template <typename T>
		Vector(T value)  : x(float(value)), y(float(value)), z(float(value)) {}

		std::string toString() const;

		/// Sets the components of the vector
		Vector& set(float x_, float y_, float z_) {
			x = x_, y = y_, z = z_;
			return *this;
		}

		/// Sets the components of the vector
		Vector& set(double x_, double y_, double z_) {
			x = float(x_), y = float(y_), z = float(z_);
			return *this;
		}

		/// Sets the components of the vector
		Vector& makeZero(void) {
			z = y = x = 0;
			return *this;
		}

		/// Adds the components of the given vector
		Vector& operator +=(const Vector &other) {
			x += other.x, y += other.y, z += other.z;
			return *this;
		}

		/// Subtracts the components of the given vector
		Vector& operator -=(const Vector &other) {
			x -= other.x, y -= other.y, z -= other.z;
			return *this;
		}

		/// Multiplies all components by the given number
		Vector& operator *=(int factor) {
			x *= float(factor), y *= float(factor), z *= float(factor);
			return *this;
		}

		/// Multiplies all components by the given number
		Vector& operator *=(float factor) {
			x *= factor, y *= factor, z *= factor;
			return *this;
		}
		
		/// Multiplies all components by the given number
		Vector& operator *=(double factor) {
			x *= float(factor), y *= float(factor), z*=float(factor);
			return *this;
		}

		/// Divides all components by the given number
		Vector& operator /=(int divisor) {
			x/=float(divisor); y/=float(divisor); z/=float(divisor);
			return *this;
		}

		/// Divides all components by the given number
		Vector& operator /=(float divisor) {
			x/=divisor; y/=divisor; z/=divisor;
			return *this;
		}

		/// Divides all components by the given number
		Vector& operator /=(double divisor) {
			x/=float(divisor); y/=float(divisor); z/=float(divisor);
			return *this;
		}

		/// Reverses the sign of all components
		Vector operator -(void) const {
			return Vector(-x, -y, -z);
		} 

		/// Returns the i-th component (0 for x, 1 for y, 2 for z)
		float& operator [](const int index) {
			return f[index];
		}

		/// Returns the i-th component (0 for x, 1 for y, 2 for z) as a const
		const float& operator [](const int index) const {
			return f[index];
		}

		/// Returns the length of the vector
		float length(void) const {
			return float(sqrt(x*x + y*y + z*z));
		}

		/// Returns the squared length of the vector
		float lengthSqr(void) const {
			return x*x + y*y + z*z;
		}

		/// Rotates the vector along the given axis. The length of the axis is the tangent of the angle of rotation.
		void rotate(const Vector &axis);
	};

	/// \relates Vector
	/// Multiplies the given vector by the given number.
	inline Vector operator *(const Vector &a, int factor) {
		Vector res(a);
		res *= factor;
		return res;
	}

	/// \relates Vector
	/// Multiplies the given vector by the given number.
	inline Vector operator *(const Vector &a, float factor) {
		Vector res(a);
		res *= factor;
		return res;
	}

	/// \relates Vector
	/// Multiplies the given vector by the given number.
	inline Vector operator *(const Vector &a, double factor) {
		Vector res(a);
		res *= factor;
		return res;
	}

	/// \relates Vector
	/// Multiplies all components of the given vector by the given number.
	inline Vector operator *(int factor, const Vector &a) {
		Vector res(a);
		res *= factor;
		return res;
	}

	/// \relates Vector
	/// Multiplies all components of the given vector by the given number.
	inline Vector operator *(float factor, const Vector &a) {
		Vector res(a);
		res *= factor;
		return res;
	}

	/// \relates Vector
	/// Multiplies all components of the given vector by the given number.
	inline Vector operator *(double factor, const Vector &a) {
		Vector res(a);
		res *= factor;
		return res;
	}

	/// \relates Vector
	/// Divides all components of the given vector by the given number.
	inline Vector operator /(const Vector &a, int factor) {
		Vector res(a);
		res /= factor;
		return res;
	}

	/// \relates Vector
	/// Divides all components of the given vector by the given number.
	inline Vector operator /(const Vector &a, float factor) {
		Vector res(a);
		res /= factor;
		return res;
	}

	/// \relates Vector
	/// Divides all components of the given vector by the given number.
	inline Vector operator /(const Vector &a, double factor) {
		Vector res(a);
		res /= factor;
		return res;
	}

	/// \relates Vector
	/// Dot product of two vectors
	inline double operator *(const Vector &a, const Vector &b) {
		return double(a.x)*double(b.x) + double(a.y)*double(b.y) + double(a.z)*double(b.z);
	}

	/// \relates Vector
	/// Adds the components of the given vectors
	inline Vector operator +(const Vector &a, const Vector &b) {
		Vector res(a);
		res += b;
		return res;
	}

	/// \relates Vector
	/// Subtracts the components of the given vectors
	inline Vector operator -(const Vector &a, const Vector &b) {
		Vector res(a);
		res -= b;
		return res;
	}

	/// \relates Vector
	/// Divides the components of the given vectors
	inline Vector operator /(const Vector &a, const Vector &b) {
		return Vector(a.x/b.x, a.y/b.y, a.z/b.z);
	}

	/// \relates Vector
	/// Mutiplies the components of the given vectors
	inline Vector mul(const Vector &a, const Vector &b) {
		return Vector(a.x*b.x, a.y*b.y, a.z*b.z);
	}

	/// \relates Vector
	/// Checks two vectors for inequality
	inline bool operator !=(const Vector &a, const Vector &b) {
		return a.x != b.x || a.y != b.y || a.z != b.z;
	}

	/// \relates Vector
	/// Checks two vectors for *exact* equality
	/// NOTE: You may want to perform your own approximate comparison, element by element
	inline bool operator ==(const Vector &a, const Vector &b) {
		return a.x == b.x && a.y == b.y && a.z == b.z;
	}

	/// \relates Vector
	/// Returns the cross product of two vectors
	inline Vector operator ^(const Vector &a, const Vector &b) {
		return Vector(
			double(a.y)*double(b.z) - double(b.y)*double(a.z),
			double(a.z)*double(b.x) - double(b.z)*double(a.x),
			double(a.x)*double(b.y) - double(b.x)*double(a.y)
		);
	}

	/// \relates Vector
	/// Returns the mixed product of the three vectors ((a^b)*c)
	inline double mixed(const Vector &a, const Vector &b, const Vector &c) {
		return 
			(double(a.y)*double(b.z) - double(b.y)*double(a.z))*double(c.x)+
			(double(a.z)*double(b.x) - double(b.z)*double(a.x))*double(c.y)+
			(double(a.x)*double(b.y) - double(b.x)*double(a.y))*double(c.z);
	}

	/// \relates Vector
	/// Returns a unit vector along the direction of the given one
	inline Vector normalize(const Vector &a) {
		return a/a.length();
	}

	/// \relates Vector
	/// Returns the length of the given vector
	inline float length(const Vector &a) {
		return a.length();
	}

	/// \relates Vector
	/// Returns the squared length of the given vector
	inline float lengthSqr(const Vector &a) {
		return a.lengthSqr();
	}

	/// \relates Vector
	/// Rotates the vector along the given axis. The length of the axis is the tangent of the angle of rotation.
	inline void Vector::rotate(const Vector &axis) {
		float oldLen = length();

		Vector d = axis ^ (*this);
		d += (*this);

		float newLen = d.length();
		if (newLen > 1e-6f)
			(*this) = d*oldLen/newLen;
	}

	/// \relates Vector
	/// Rotates the vector along the given axis. The length of the axis is the tangent of the angle of rotation.
	inline Vector rotate(const Vector &v, const Vector &axis) {
		Vector r(v);
		r.rotate(axis);
		return r;
	}

	inline std::ostream& operator <<(std::ostream& stream, const Vector& vector) {
		stream << "Vector(" << vector.x << ", " <<  vector.y << ", " <<  vector.z << ")";
		return stream;
	}

	inline std::string Vector::toString() const {
		std::ostringstream stream;
		stream << *this;
		return stream.str();
	}

	/// RGB color, float32
	struct Color {
		union {
			struct { float r, g, b; };
			float rgb[3];
		};

		Color() : r(), g(), b() {}

		template <typename T1, typename T2, typename T3>
		Color(T1 r_, T2 g_, T3 b_) :  r(float(r_)), g(float(g_)), b(float(b_)) {}

		template <typename T>
		Color(T value) : r(float(value)), g(float(value)), b(float(value)) {}

		enum RGBColorSpace {
			CIE, sRGB
		};

		/// Return the normalized RGB color of a black body with the given temperature in Kelvin.
		static Color fromTemperature(float temperature, RGBColorSpace colorSpace = sRGB);

		std::string toString() const;
	};

	/// RGBA color, float32
	struct AColor {
		Color color;
		float  alpha;

		AColor() : color(), alpha() {}

		template <typename T>
		AColor(Color c, T a = T(1)) : color(c), alpha(float(a)) {}

		template <typename T1, typename T2, typename T3, typename T4>
		AColor(T1 r, T2 g, T3 b, T4 a = T4(1)) :  color(r, g, b), alpha(float(a)) {}

		template <typename T>
		AColor(T value) : color(value), alpha() {}

		std::string toString() const ;
	};

	inline std::ostream& operator <<(std::ostream& stream, const AColor& acolor) {
		stream << "AColor(" << acolor.color.r << ", " <<  acolor.color.g << ", " <<  acolor.color.b << ", " <<  acolor.alpha << ")";
		return stream;
	}

	inline std::string AColor::toString() const {
		std::ostringstream stream;
		stream << *this;
		return stream.str();
	}

	/// 3x3 column-major matrix, float32
	struct Matrix {
		Vector v0, v1, v2;

		std::string toString() const;

		/// Return the i-th column of the matrix (i=0,1,2).
		Vector& operator [](const int index) {
			return (&v0)[index];
		}

		/// Return the i-th column of the matrix as a const object (i=0,1,2).
		const Vector& operator [](const int index) const {
			return (&v0)[index];
		}

		/// Constructor - does not perform any initialization.
		Matrix(void) {}

		/// Constructor to diagonal matrix.
		Matrix(float value) : v0(value, 0, 0), v1(0 ,value, 0), v2(0, 0, value) {}

		/// Constructor from the three columns of the matrix.
		/// @param a The first column.
		/// @param b The second column.
		/// @param c The third column.
		Matrix(const Vector &a, const Vector &b, const Vector &c) : v0(a), v1(b), v2(c) {}

		/// Copy-constructor from another matrix.
		/// @param other The other matrix.
		Matrix(const Matrix& other) : v0(other.v0), v1(other.v1), v2(other.v2) {}

		/// Set the columns of the matrix.
		/// @param a The first column.
		/// @param b The second column.
		/// @param c The third column.
		void set(const Vector &a, const Vector &b, const Vector &c) {
			v0 = a, v1 = b, v2 = c;
		}

		/// Make diagonal matrix.
		void set(float value) {
			v0.set(value, 0, 0);
			v1.set(0, value, 0);
			v2.set(0, 0, value);
		}

		/// Set the matrix from another matrix.
		/// @param other The other matrix.
		void set(const Matrix& other) {
			v0 = other.v0;
			v1 = other.v1;
			v2 = other.v2;
		}

		/// Set the i-th column of the matrix.
		/// @param i The index of the column to set (i=0,1,2).
		/// @param a The value for the column.
		void setCol(const int i, const Vector &a) {
			(*this)[i] = a;
		}

		/// Set the i-th row of the matrix.
		/// @param i The index of the row to set (i=0,1,2).
		/// @param a The value of the row.
		void setRow(const int i, const Vector &a) {
			(*this)[0][i] = a.x, (*this)[1][i] = a.y, (*this)[2][i]=a.z;
		}

		/// Make the matrix the zero matrix (all elements are zeroes).
		void makeZero(void) {
			v0.makeZero();
			v1.makeZero();
			v2.makeZero();
		}

		/// Make the matrix the identity matrix.
		void makeIdentity(void) {
			v0.set(1.0f, 0.0f, 0.0f);
			v1.set(0.0f, 1.0f, 0.0f);
			v2.set(0.0f, 0.0f, 1.0f);
		}

		/// Make the matrix a diagonal matrix with the given elements along the diagonal.
		/// @param a A vector holding the diagonal values. All other elements are initialized to zeroes.
		void makeDiagonal(const Vector &a) {
			v0.set(a.x, 0.0f, 0.0f);
			v1.set(0.0f, a.y, 0.0f);
			v2.set(0.0f, 0.0f, a.z);
		}

		void makeOuterProduct(const Vector &a, const Vector &b) {
			v0 = a.x*b;
			v1 = a.y*b;
			v2 = a.z*b;
		}

		/// Make the matrix to be the transpose of its currrent value.
		void makeTranspose(void) {
			float t;
			t = v0[1], v0[1] = v1[0], v1[0] = t;
			t = v0[2], v0[2] = v2[0], v2[0] = t;
			t = v1[2], v1[2] = v2[1], v2[1] = t;
		}

		/// Make the matrix to be the inverse of its current value. Checks if the matrix
		/// determinant is zero and in that case, zeroes out the matrix.
		/// @retval zero if the determinant is zero, and non-zero otherwise.
		bool makeInverse(void) {
			Matrix r;
			double d = mixed(v0, v1, v2);
			bool ret = true;
			if (d) {
				r.setRow(0, (v1^v2)/d);
				r.setRow(1, (v2^v0)/d);
				r.setRow(2, (v0^v1)/d);
			}
			else {
				r.makeZero();
				ret = false;
			}
			*this = r;
			return ret;
		}

		void addDiagonal(const Vector &a) {
			v0[0] += a[0];
			v1[1] += a[1];
			v2[2] += a[2];
		}

		void addTranspose(const Matrix &b) {
			v0[0] += b.v0[0];
			v0[1] += b.v1[0];
			v0[2] += b.v2[0];
		
			v1[0] += b.v0[1];
			v1[1] += b.v1[1];
			v1[2] += b.v2[1];

			v2[0] += b.v0[2];
			v2[1] += b.v1[2];
			v2[2] += b.v2[2];
		}

		/// Multiply all elements in the matrix by the given number.
		void operator *=(float x) {
			v0 *= x, v1 *= x, v2 *= x;
		}

		/// Divide all elements in the matrix by the given number. Does not perform a check for divide by zero.
		void operator /=(float x) {
			v0 /= x, v1 /= x, v2 /= x;
		}

		/// Add another matrix to this one.
		/// @param m The matrix to add.
		void operator +=(const Matrix &m) {
			v0 += m.v0;
			v1 += m.v1;
			v2 += m.v2;
		}

		/// Subtract another matrix from this one.
		/// @param m The matrix to subtract.
		void operator -=(const Matrix &m) {
			v0-=m.v0;
			v1-=m.v1;
			v2-=m.v2;
		}

		/// Applies a rotation along the given axis. The length of the axis is the tangent of the angle of rotation.
		void rotate(const Vector &axis) {
			v0.rotate(axis);
			v1.rotate(axis);
			v2.rotate(axis);
		}

		void makeOrthogonal(void) {
			Matrix t;
			t.v0 = normalize(v0);
			t.v1 = normalize(v2 ^ v0);
			t.v2 = t.v0 ^ t.v1;
			v0 = length(v0) * t.v0;
			v1 = length(v1) * t.v1;
			v2 = length(v2) * t.v2;
		}

		void makeOrthonormal(void) {
			v0 = normalize(v0);
			v1 = normalize(v2 ^ v0);
			v2 = normalize(v0 ^ v1);
		}
	};

	/// \relates Matrix
	/// Compute the inverse of a matrix.
	inline Matrix inverse(const Matrix &m) {
		Matrix r;
		double d = m.v0*(m.v1^m.v2);
		r.setRow(0, (m.v1^m.v2)/d);
		r.setRow(1, (m.v2^m.v0)/d);
		r.setRow(2, (m.v0^m.v1)/d);
		return r;
	}

	/// \relates Matrix
	/// Multiply a vector by a matrix. Note that this is different from multiplying the matrix by the same vector.
	inline Vector operator *(const Vector &a, const Matrix &m) {
		return Vector(a*m.v0, a*m.v1, a*m.v2);
	}

	/// \relates Matrix
	/// Multiply a matrix by a vector. Note that this is different from multiplying the same vector by the same matrix.
	inline Vector operator *(const Matrix &m, const Vector &a) {
		return a.x*m.v0 + a.y*m.v1 + a.z*m.v2;
	}

	/// \relates Matrix
	/// Cross product of a matrix and a vector.
	inline Matrix operator ^(const Matrix &m, const Vector &a) {
		return Matrix(m.v0^a, m.v1^a, m.v2^a);
	}

	/// \relates Matrix
	/// Cross product of a vector and a matrix.
	inline Matrix operator ^(const Vector &a, const Matrix &m) {
		return Matrix(a^m.v0, a^m.v1, a^m.v2);
	}

	/// \relates Matrix
	/// Multiply a matrix by a number.
	inline Matrix operator *(const Matrix &m, float x) {
		return Matrix(m.v0*x, m.v1*x, m.v2*x);
	}

	/// \relates Matrix
	/// Multiply a number by a matrix. This is the same as multiplying the matrix by the number.
	inline Matrix operator *(float x, const Matrix &m) {
		return Matrix(x*m.v0, x*m.v1, x*m.v2);
	}

	/// \relates Matrix
	/// Divide all elements of the matrix by a given number. Does not check if the number is zero.
	inline Matrix operator /(const Matrix &m, float x) {
		return Matrix(m.v0/x, m.v1/x, m.v2/x);
	}

	/// \relates Matrix
	/// Divide a number by the determinant of a matrix. Does not check if the determinant is zero.
	inline float operator /(float x, const Matrix &m) {
		return x / float(m.v0*(m.v1^m.v2));
	}

	/// \relates Matrix
	/// Add two matrices, so that if p is a Vector, (a+b)*p == (a*p)+(b*p).
	inline Matrix operator +(const Matrix &a, const Matrix &b) {
		return Matrix(a.v0 + b.v0, a.v1 + b.v1, a.v2 + b.v2);
	}

	/// \relates Matrix
	/// Subtract two matrices, so that if p is a Vector, (a-b)*p == (a*p)-(b*p).
	inline Matrix operator -(const Matrix &a, const Matrix &b) {
		return Matrix(a.v0 - b.v0, a.v1 - b.v1, a.v2 - b.v2);
	}

	/// \relates Matrix
	/// Multiply two matrices, so that if p is a Vector, (a*b)*p == a*(b*p).
	inline Matrix operator *(const Matrix &a, const Matrix &b) {
		Matrix r(
			Vector(
				a.v0[0]*b.v0[0]+a.v1[0]*b.v0[1]+a.v2[0]*b.v0[2],
				a.v0[1]*b.v0[0]+a.v1[1]*b.v0[1]+a.v2[1]*b.v0[2],
				a.v0[2]*b.v0[0]+a.v1[2]*b.v0[1]+a.v2[2]*b.v0[2]),
			Vector(
				a.v0[0]*b.v1[0]+a.v1[0]*b.v1[1]+a.v2[0]*b.v1[2],
				a.v0[1]*b.v1[0]+a.v1[1]*b.v1[1]+a.v2[1]*b.v1[2],
				a.v0[2]*b.v1[0]+a.v1[2]*b.v1[1]+a.v2[2]*b.v1[2]),
			Vector(
				a.v0[0]*b.v2[0]+a.v1[0]*b.v2[1]+a.v2[0]*b.v2[2],
				a.v0[1]*b.v2[0]+a.v1[1]*b.v2[1]+a.v2[1]*b.v2[2],
				a.v0[2]*b.v2[0]+a.v1[2]*b.v2[1]+a.v2[2]*b.v2[2])
		);
		return r;
	}

	/// \relates Matrix
	/// Checks two matrices for inequality.
	inline bool operator !=(const Matrix &a, const Matrix &b) {
		return a.v0 != b.v0 || a.v1 != b.v1 || a.v2 != b.v2;
	}

	/// \relates Matrix
	/// Checks two matrices for *exact* equality.
	/// NOTE: You may want to perform your own approximate comparison, element by element
	inline bool operator ==(const Matrix &a, const Matrix &b) {
		return a.v0 == b.v0 && a.v1 == b.v1 && a.v2 == b.v2;
	}

	/// \relates Matrix
	/// Invert the signs of all elements in a matrix.
	inline Matrix operator -(const Matrix &a) {
		return Matrix(-a.v0, -a.v1, -a.v2);
	}

	inline Matrix outerProductMatrix(const Vector &a, const Vector &b) {
		return Matrix(a.x*b, a.y*b, a.z*b);
	}

	/// \relates Matrix
	inline Matrix transpose(const Matrix &m) {
		return Matrix(
			Vector(m.v0[0], m.v1[0], m.v2[0]),
			Vector(m.v0[1], m.v1[1], m.v2[1]),
			Vector(m.v0[2], m.v1[2], m.v2[2]));
	}

	/// \relates Matrix
	/// Applies a rotation along the given axis. The length of the axis is the tangent of the angle of rotation.
	inline Matrix rotate(const Matrix &m, const Vector &axis) {
		return Matrix(rotate(m.v0, axis), rotate(m.v1, axis), rotate(m.v2, axis));
	}

	/// \relates Matrix
	/// Make a matrix orthonormal
	inline Matrix normalize(const Matrix &m) {
		Vector v0 = normalize(m.v0);
		Vector v1 = normalize(m.v2^m.v0);
		return Matrix(v0, v1, v0^v1);
	}

	/// \relates Matrix
	/// Remove scaling from a matrix (make all column unit vectors)
	inline Matrix noScale(const Matrix &m) {
		return Matrix(normalize(m.v0), normalize(m.v1), normalize(m.v2));
	}

	/// \relates Matrix
	/// Creates a new matrix representing a linear transformation - *clockwise* rotation around the X axis
	/// @param xrot Angle in radians
	VRAY_DEPRECATED("This method is deprecated in favor of makeRotationMatrixX which is properly counter-clockwise.")
	inline Matrix makeRotMatrixX(float xrot) {
		const float s = sin(xrot);
		const float c = cos(xrot);
		return Matrix(Vector(1.0f, 0.0f, 0.0f), Vector(0, c, -s), Vector(0, s, c));
	}

	/// \relates Matrix
	/// Creates a new matrix representing a linear transformation - *clockwise* rotation around the Y axis
	/// @param yrot Angle in radians
	VRAY_DEPRECATED("This method is deprecated in favor of makeRotationMatrixY which is properly counter-clockwise.")
	inline Matrix makeRotMatrixY(float yrot){
		const float s = sin(yrot);
		const float c = cos(yrot);
		return Matrix(Vector(c, 0, s), Vector(0.0f, 1.0f, 0.0f), Vector(-s, 0, c));
	}

	/// \relates Matrix
	/// Creates a new matrix representing a linear transformation - *clockwise* rotation around the Z axis
	/// @param zrot Angle in radians
	VRAY_DEPRECATED("This method is deprecated in favor of makeRotationMatrixZ which is properly counter-clockwise.")
	inline Matrix makeRotMatrixZ(float zrot) {
		const float s = sin(zrot);
		const float c = cos(zrot);
		return Matrix(Vector(c, -s, 0), Vector(s, c, 0), Vector(0.0f, 0.0f, 1.0f));
	}

	/// \relates Matrix
	/// Creates a new matrix representing a linear transformation - rotation around the X axis
	/// @param xrot Angle in radians
	inline Matrix makeRotationMatrixX(float xrot) {
		const float s = sin(xrot);
		const float c = cos(xrot);
		return Matrix(Vector(1.0f, 0.0f, 0.0f), Vector(0, c, s), Vector(0, -s, c));
	}

	/// \relates Matrix
	/// Creates a new matrix representing a linear transformation - rotation around the Y axis
	/// @param yrot Angle in radians
	inline Matrix makeRotationMatrixY(float yrot) {
		const float s = sin(yrot);
		const float c = cos(yrot);
		return Matrix(Vector(c, 0, -s), Vector(0.0f, 1.0f, 0.0f), Vector(s, 0, c));
	}

	/// \relates Matrix
	/// Creates a new matrix representing a linear transformation - rotation around the Z axis
	/// @param zrot Angle in radians
	inline Matrix makeRotationMatrixZ(float zrot) {
		const float s = sin(zrot);
		const float c = cos(zrot);
		return Matrix(Vector(c, s, 0), Vector(-s, c, 0), Vector(0.0f, 0.0f, 1.0f));
	}

	/// This returns a matrix for transforming normals for objects transformed with the given matrix
	inline Matrix normalTransformMatrix(const Matrix &m) {
		return Matrix(m[1]^m[2], m[2]^m[0], m[0]^m[1]);
	}

	/// This returns a matrix for transforming normals for objects transformed with the given matrix
	inline Matrix normalTransformMatrixX(const Matrix &m, bool &DlessThanZero ) {
		if((m[0]^m[1])*m[2] < 0){
			DlessThanZero = true;
			return Matrix(m[2]^m[1], m[0]^m[2], m[1]^m[0]);
		}
		DlessThanZero = false;
		return Matrix(m[1]^m[2], m[2]^m[0], m[0]^m[1]);
	}

	inline std::ostream& operator <<(std::ostream& stream, const Matrix& matrix) {
		stream << "Matrix(" << matrix.v0 << ", " << matrix.v1 << ", " << matrix.v2 << ")";
		return stream;
	}

	inline std::string Matrix::toString() const {
		std::ostringstream stream;
		stream << *this;
		return stream.str();
	}

	/// A math object used to define object position, rotation and scale in 3D space
	struct Transform {
		Matrix matrix;
		Vector offset;

		/// Constructor - does not initialize anything.
		Transform() {}

		/// Constructor from a matrix and a vector.
		/// @param matrix The matrix (rotation+scale) portion.
		/// @param offset The offset (translation) portion.
		Transform(const Matrix& matrix, const Vector& offset) : matrix(matrix), offset(offset) {}

		/// Constructor to either the zero or the identity transformation.
		/// @param i If this is 0, the transformation is initialized to the zero transformation;
		/// if this is 1, the transformation is initialized to the identity transformation.
		Transform(int i) {
			if (i == 1) makeIdentity();
			if (i == 0) makeZero();
		}

		/// Make the transformation the zero transformation.
		void makeZero(void) {
			matrix.makeZero();
			offset.makeZero();
		}

		/// Make the transformation the identity transformation.
		void makeIdentity(void) {
			matrix.makeIdentity();
			offset.makeZero();
		}

		/// Multiply the transformation by a number.
		void operator *=(float x) {
			matrix *= x;
			offset *= x;
		}

		/// Add another transformation to this one.
		void operator +=(const Transform &tm) {
			matrix += tm.matrix;
			offset += tm.offset;
		}

		/// Subtract another tranformation from this one.
		void operator -=(const Transform &tm) {
			matrix -= tm.matrix;
			offset -= tm.offset;
		}

		/// Transform a vector by the transformation (ignores the translation part). This is
		/// different from the operator * with a vector, which adds in the translation part as well.
		Vector transformVec(const Vector &a) const {
			return matrix * a;
		}

		/// Make the transformation the inverse of its current value.
		void makeInverse(void) {
			matrix.makeInverse();
			offset = -matrix * offset;
		}

		std::string toString() const;
	};
	
	/// \relates Transform
	/// Checks two transforms for inequality.
	inline bool operator !=(const Transform &a, const Transform &b) {
		return a.matrix != b.matrix || a.offset != b.offset;
	}

	/// \relates Transform
	/// Checks two transforms for *exact* equality.
	/// NOTE: You may want to perform your own approximate comparison, element by element
	inline bool operator ==(const Transform &a, const Transform &b) {
		return a.matrix == b.matrix && a.offset == b.offset;
	}

	/// \relates Transform
	/// Reverses the sign of the transformation.
	inline Transform operator -(const Transform &tm) {
		return Transform(-tm.matrix, -tm.offset);
	}

	/// \relates Transform
	/// Mutiply all components of a transformation by a number.
	inline Transform operator *(const Transform &tm, float x) {
		return Transform(tm.matrix * x, tm.offset * x);
	}

	/// \relates Transform
	/// Mutiply all components of a transformation by a number.
	inline Transform operator *(float x, const Transform &tm) {
		return Transform(x * tm.matrix, x * tm.offset);
	}

	/// \relates Transform
	/// Divide all components of a transformation by a number. Does not check if the number is zero.
	inline Transform operator /(const Transform &tm, float x) {
		return Transform(tm.matrix / x, tm.offset / x);
	}

	/// \relates Transform
	/// Add two transformations, so that if p is a Vector, (a+b)*p == (a*p)+(b*p).
	inline Transform operator +(const Transform &a, const Transform &b) {
		return Transform(a.matrix + b.matrix, a.offset + b.offset);
	}

	/// \relates Transform
	/// Subtract two transformations, so that if p is a Vector, (a-b)*p == (a*p)-(b*p).
	inline Transform operator -(const Transform &a, const Transform &b) {
		return Transform(a.matrix - b.matrix, a.offset - b.offset);
	}

	/// \relates Transform
	/// Mutiply two transformations, so that if p is a Vector, (a*b)*p == a*(b*p).
	inline Transform operator *(const Transform &a, const Transform &b) {
		return Transform(a.matrix * b.matrix, a.matrix * b.offset + a.offset);
	}

	/// \relates Transform
	/// Mutiply a transformation by a point. Note that this is different from multiplying a point by a transformation.
	inline Vector operator *(const Transform &tm, const Vector &a) {
		return tm.matrix * a + tm.offset;
	}

	/// \relates Transform
	/// Mutiply a point by a transformation. Note that this is different from multiplying a transformation by a point.
	inline Vector operator *(const Vector &a, const Transform &tm) {
		return (a + tm.offset) * tm.matrix;
	}

	inline std::ostream& operator <<(std::ostream& stream, const Transform& transform) {
		stream << "Transform(" << transform.matrix << ", " << transform.offset << ")";
		return stream;
	}

	inline std::string Transform::toString() const {
		std::ostringstream stream;
		stream << *this;
		return stream.str();
	}


	typedef int RGB32;

	class Plugin;
	struct RendererOptions;
	class PluginMeta;
	class PropertyMeta;
	class PropertyRuntimeMeta;

	class VRayRendererNative;

	/// Helper class that wraps a plugin instance enabling a certain render element
	class RenderElement {
		friend class RenderElements;

	public:
		/// The render channal types/aliases are used to identify some common channels.
		enum Type {
			NONE = -1, ///< Unspecified channel

			ATMOSPHERE = 100, ///< Atmospheric effects channel.
			DIFFUSE, ///< Diffuse filter VFB channel. This channel is generated by V-Ray.
			REFLECT, ///< Reflection VFB channel. This channel must be generated by shaders.
			REFRACT, ///< Refraction VFB channel. This channel must be generated by shaders.
			SELFILLUM, ///< Self-illumination VFB channel. This channel must be generated by shaders.
			SHADOW, ///< Shadow VFB channel. This channel is generated by V-Ray.
			SPECULAR, ///< Specular VFB channel. This channel is generated by V-Ray.
			LIGHTING, ///< Lighting VFB channel. This channel is generated by V-Ray.
			GI, ///< GI VFB channel. This channel is generated by V-Ray.
			CAUSTICS, ///< Caustics VFB channel. This channel is generated by V-Ray.
			RAWGI, ///< Raw GI VFB channel. This channel is generated by V-Ray.
			RAWLIGHT, ///< Raw light VFB channel. This channel is generated by V-Ray.
			RAWSHADOW, ///< Raw shadow VFB channel. This channel is generated by V-Ray.

			VELOCITY, ///< Velocity VFB channel. This channel is generated by V-Ray.
			RENDERID, ///< Render ID VFB channel. This channel is generated by V-Ray.
			MTLID, ///< Mtl ID VFB channel. This channel is generated by V-Ray.
			NODEID, ///< Node ID VFB channel. This channel is generated by V-Ray.
			ZDEPTH, ///< Z-Depth VFB channel. This channel is generated by V-Ray.

			REFLECTION_FILTER, ///< Reflection filter VFB channel. This channel must be generated by shaders.
			RAW_REFLECTION, ///< Raw reflection VFB channel. This channel must be generated by shaders.

			REFRACTION_FILTER, ///< Refraction filter VFB channel. This channel must be generated by shaders.
			RAW_REFRACTION, ///< Raw refraction VFB channel. This channel must be generated by shaders.

			REALCOLOR, ///< Real color VFB channel.
			NORMALS, ///< Normals VFB channel. This channel is generated by V-Ray.
			BACKGROUND, ///< Background VFB channel. This channel is generated by V-Ray.
			
			/// ! These are created by default by V-Ray and can't have more instances.
			/// ! To get the color and alpha images use the getImage API.
			//ALPHA, ///< The alpha VFB channel. This channel is generated by V-Ray.
			//COLOR, ///< The RGB color VFB channel. This channel is generated by V-Ray.

			// +2 here for Alpha and Color to stay in sync with V-Ray's definition
			WIRECOLOR = BACKGROUND + 3, ///< Wire color channel where each object appears with a solid color. This channel is generated by V-Ray.
			MATTESHADOW, ///< Matte shadow channel where full shadows are white and areas not in shadow are black; essentially this is (rawShadow)/(rawLight+rawShadow). This channel is generated by V-Ray.

			TOTALLIGHT, ///< The total diffuse lighting (direct+indirect) falling on a surface. This channel is generated by V-Ray.
			RAWTOTALLIGHT, ///< The raw total diffuse lighting (direct+indirect) falling on a surface. This channel is generated by V-Ray.

			BUMPNORMALS, ///< The normals modified with bump map. This channel must be generated by shaders.

			SAMPLERATE, ///< The sample rate for the image samplers.
	
			SSS, ///< A channel used for VRayFastSSS2 material sub-surface portion.
	
			DRBUCKET, ///< A channel that keeps track of which DR server rendered a particular bucket (it is a Color buffer that stores server name strings prerendered, as text, white on black)

			VRMTL_REFLECTGLOSS, ///< A VRayMtl reflection glossiness  VFB channel. This channel is generated by V-Ray.
			VRMTL_REFLECTHIGLOSS, ///< A VRayMtl reflection hilight glossiness  VFB channel. This channel is generated by V-Ray.
			VRMTL_REFRACTGLOSS, ///< A VRayMtl reflection glossiness  VFB channel. This channel is generated by V-Ray.

			SHADEMAP_EXPORT, ///< A channel that keeps the fragment coordinates in camera space
			REFLECT_ALPHA, ///< Used by matte materials to store the alpha of the reflections

			VRMTL_REFLECTIOR, ///< A VRayMtl reflection IOR VFB channel. This channel is generated by V-Ray.
			MTLRENDERID, ///< Mtl render ID VFB channel. This channel is generated by V-Ray.

			NOISE_LEVEL, ///< The noise level as estimated by the Adaptive and Progressive image samplers. Used for denoising purposes.
			WORLD_POSITION, ///< The position in world space. Used for denoising purposes.
			DENOISED, ///< A denoised version of the image. Adding this render element enables denoising.
			WORLD_BUMPNORMAL, ///< Normal with bump mapping in world space.
			DEFOCUS_AMOUNT, ///< Pixel blur, combination of DOF and moblur. Used for denoising purposes.

			LAST, ///< not a type, just a delimiter

			USER=1000, ///< User defined channel indices start from here
		};

		/// Render channel binary format / storage data type.
		enum BinaryFormat {
			BF_FLOAT  = 1, ///< A single float number (e.g. z-depth).
			BF_3FLOAT, ///< 3 float numbers (e.g. RGB color).
			BF_2FLOAT, ///< 2 signed float numbers (e.g. uv coordinates or pixel screen velocity).
			BF_INT, ///< A single integer number (e.g. render ID, material ID etc).
			BF_3FLOAT_SIGNED, ///< 3 signed float numbers (e.g. surface normals).
			BF_4FLOAT, ///< 4 float numbers (e.g. RGBA color).
		};

		/// This describes the desired data format for getData().
		enum PixelFormat {
			PF_DEFAULT = -1,
			PF_BW_BIT8 = 100, ///< 1-byte greyscale/integer.
			PF_BW_BIT16, ///< 2-byte greyscale/integer.
			PF_BW_BIT32, ///< 4-byte greyscale/integer.
			PF_BW_HALF, ///< 2-byte half-float greyscale (currently not supported).
			PF_BW_FLOAT, ///< 4-byte float greyscale.
	
			PF_RGB_BIT8 = 200, ///< 3x1 bytes RGB.
			PF_RGB_BIT16, ///< 3x2 bytes RGB.
			PF_RGB_HALF, ///< 3x2 bytes half-float RGB (currently not supported).
			PF_RGB_FLOAT, ///< 3x4 bytes float RGB.

			PF_RGBA_BIT8 = 300, ///< 4x1 bytes RGBA.
			PF_RGBA_BIT16, ///< 4x2 bytes RGBA.
			PF_RGBA_HALF, ///< 4x2 bytes half-float RGBA (currently not supported).
			PF_RGBA_FLOAT, ///< 4x4 bytes float RGBA.
		};

		/// This structure is used to return information about a particular channel in the VFB.
		struct Info {
			BinaryFormat binaryFormat; ///< The channel binary format.
			const char *name; ///< The name of the channel as it appears in the VFB.
			Type type; ///< The channel type/alias.
		};

	private:
		const VRayRenderer* renderer;
		InstanceId pluginID;
		std::string name;
		Type type;
		BinaryFormat binaryFormat;
		PixelFormat defaultPixelFormat;

		RenderElement(const VRayRenderer* renderer, const Plugin &plugin);
		static PixelFormat binaryFormatToPixelFormat(BinaryFormat binFormat);

		size_t getData_internal(InstanceId alphaPluginID, void** data, PixelFormat format = PF_DEFAULT, bool rgbOrder = false, const ImageRegion* rgn = NULL) const;

	public:
		/// Default construct an invalid object
		RenderElement() : renderer(NULL), pluginID(NO_ID), type(NONE), binaryFormat(BF_FLOAT), defaultPixelFormat(PF_DEFAULT) {}

		/// Get the plugin instance in the scene that enables this render element
		Plugin getPlugin() const;

		/// Get the name of the plugin instance for this render element
		std::string getName() const {
			return name;
		}

		/// Get the type of the render element
		Type getType() const {
			return type;
		}

		/// Get the binary format of pixels in this render element
		BinaryFormat getBinaryFormat() const {
			return binaryFormat;
		}

		/// Get the default format used by the getData methods for this render element
		PixelFormat getDefaultPixelFormat() const {
			return defaultPixelFormat;
		}

		/// Retrieves the render element raw data.
		/// @param [out] data a pointer to a void pointer which will receive the address of the raw data buffer.
		/// @param format the desired pixel format.
		/// @param rgbOrder true - RGB order, false - BGR order
		/// @param rgn an optional image region rectange; NULL means the whole image.
		/// @retval the length in bytes of the returned memory buffer.
		/// @remark the returned memory buffer must be freed with releaseData(data).
		size_t getData(void** data, PixelFormat format = PF_DEFAULT, bool rgbOrder = false, const ImageRegion* rgn = NULL) const;

		/// Retrieves the render element raw data.
		/// @param alphaPlugin the plugin describing the alpha channel render element
		/// @param [out] data a pointer to a void pointer which will receive the address of the raw data buffer.
		/// @param format the desired pixel format.
		/// @param rgbOrder true - RGB order, false - BGR order
		/// @param rgn an optional image region rectange; NULL means the whole image.
		/// @retval the length in bytes of the returned memory buffer.
		/// @remark the returned memory buffer must be freed with releaseData(data).
		size_t getData(const Plugin& alphaPlugin, void** data, PixelFormat format = PF_DEFAULT, bool rgbOrder = false, const ImageRegion* rgn = NULL) const;

		/// Retrieves the render element raw data.
		/// @param alpha a render element to be used as alpha channel
		/// @param [out] data a pointer to a void pointer which will receive the address of the raw data buffer.
		/// @param format the desired pixel format.
		/// @param rgbOrder true - RGB order, false - BGR order
		/// @param rgn an optional image region rectange; NULL means the whole image.
		/// @retval the length in bytes of the returned memory buffer.
		/// @remark the returned memory buffer must be freed with releaseData(data).
		size_t getData(const RenderElement& alpha, void** data, PixelFormat format = PF_DEFAULT, bool rgbOrder = false, const ImageRegion* rgn = NULL) const;

		/// Call this to release data allocated by the getData methods
		static void releaseData(void* data);

		/// Allocate a new copy of the channel data as a VRayImage instance
		VRayImage* getImage(const ImageRegion* rgn = NULL) const;

		/// Check if there is a valid plugin representing this render element in the scene. If false, creation failed.
		operator bool () const {
			return pluginID != NO_ID;
		}
	};

	/// This class groups methods related to management of render elements in the scene
	class RenderElements {
		friend class VRayRenderer;

		VRayRenderer& renderer;

		RenderElements(VRayRenderer& renderer);
		RenderElements& operator=(const RenderElements&);

	public:
		/// Attempts to create a new render element. Check the result against false for failure.
		/// @param type The type of render element. Some types allow more than one instance.
		/// @param displayName An optional name to show in the VFB. If NULL, a default name will be used.
		/// @param instanceName An optional name for the plugin instance that will be created.
		RenderElement addNew(RenderElement::Type type, const char* displayName, const char* instanceName);

		/// Returns the first render element of the specified type.
		/// Often there is only one element of a given type
		RenderElement getByType(RenderElement::Type type) const;

		/// Returns all render elements of the specified type.
		/// If type is NONE, all render elements of all types will be returned.
		std::vector<RenderElement> getAllByType(RenderElement::Type type) const;
	};

	/// A helper struct with info regarding file exports for plugins of a specified type.
	struct SubFileInfo {
		/// Allowed types: "view", "lights", "geometry", "nodes", "materials", "textures", "bitmaps", "render_elements"
		std::string pluginType;
		/// Substring to append at the end of the common part of the filename passed to the export function
		std::string fileNameSuffix;
	};

	/// A struct that encapsulates the export scene options.
	struct VRayExportSettings {
		bool compressed; ///< enables zlib compression of large data arrays; requires useHexFormat==true
		bool framesInSeparateFiles; ///< Animation data for each frame will be saved to a separate file. Non-animated data will go to the main file.
		bool useHexFormat; ///< data arrays will be encoded in hex if compressed==false
		bool renderElementsSeparateFolders; ///< controls the default value of SettingsOutput.relements_separateFolders

		/// A list of files to split the scene into, based on plugin type. See SubFileInfo struct comments.
		std::vector<SubFileInfo> subFileInfos;

		VRayExportSettings(): compressed(true), framesInSeparateFiles(false), useHexFormat(false),
			renderElementsSeparateFolders(false) {}
	};

	/// Describes a sub-sequence of an animation range at regular steps (usually step=1)
	struct SubSequenceDesc {
		int start, end, step;
	};

	/// Used by VRayRenderer::getImage
	struct GetImageOptions {
		ImageRegion region; ///< Optional crop rectangle
		bool doColorCorrect; ///< Whether to apply VFB color corrections

		GetImageOptions() : doColorCorrect(false) {}
	};

	/// Used for GPU device selection by VRayRenderer::getComputeDevicesCurrentEngine() & co
	struct ComputeDeviceInfo {
		std::string name; ///< Non-unique displayable name. Device index is its unique identifier.
		int enabled; ///< True if device is set to be used at the time of the query.
		enum ComputeDeviceType {
			devtype_unknown = 1,
			devtype_CPU = 2,
			devtype_GPU = 4,
			devtype_Accelerator = 8,
		} type;
		unsigned long long totalMemoryMB; ///< MegaBytes
		int numConnectedDisplays; ///< Number of displays (monitors) attached to this device. -1 if this information is unavailable
	};

	/// Returned by VRayRenderer::getRTStatistics when rendering in RT mode
	struct RTStatistics {
		double pathsPerSecond; ///< Number of camera rays traced per second
		unsigned elapsedTicks; ///< Time since start
		int sampleLevel; ///< Number of samples per pixel accumulated
		int numPasses; ///< Number of sampling passes done
	};

	/// This is used by VRayRenderer::readProxyFullData().
	/// Use the constructor parameter or setFlags() to define which parts of the proxy data you need.
	/// If readProxyFullData succeeds, the various get* methods give you access to the data.
	class ProxyReadData {
	public:
		enum ReadFlags {
			MESH_VERTICES = (1 << 4),
			MESH_NORMALS = (1 << 5),
			MESH_MTLIDS = (1 << 6),
			MESH_VELOCITIES = (1 << 7),
			MESH_UVS = (1 << 8),
			MESH_ALL = MESH_VERTICES | MESH_NORMALS | MESH_VELOCITIES | MESH_MTLIDS | MESH_UVS,
		};

		/// Remember to call setFlags() before calling the read function.
		ProxyReadData(int flags=0);

		/// Pass a combination of ReadFlags for the parts of the proxy data you need to read.
		void setFlags(int flags) { d.flags = flags; }

		/// Get full mesh vertex positions array.
		const std::vector<Vector> &getVertices() const { return vertices; }

		/// Get indices in the vertex array for each triangle (every 3 consecutive ints are one triangle).
		const std::vector<int>    &getVertexIndices() const { return indices; }

		/// Get full mesh normal vectors array.
		const std::vector<Vector> &getNormals() const { return normals; }

		/// Get indices in the normal for each triangle (every 3 consecutive ints are one triangle).
		const std::vector<int>    &getNormalIndices() const { return normalIndices; }

		/// Get material IDs for each consecutive triangle in the full mesh.
		const std::vector<unsigned> &getMaterialIDs() const { return materialIDs; }

		/// Get velocity vectors for each vertex in the vertex array.
		const std::vector<Vector> &getVelocities() const { return velocities; }

		/// Get the number of available UV data sets (mapping channels).
		int                        getUVSetsCount() const;

		/// Get an array with the original proxy file indices of the UV sets. They may not equal their index in the data arrays here.
		std::vector<int>           getUVOriginalIndices() const;

		/// Get the name of the UV set with the given index.
		std::string                getUVSetName(int setIndex) const;

		/// Get the length of the array returned by getUVValues().
		size_t                     getUVValuesCount(int setIndex) const;

		/// Get the length of the array returned by getUVValueIndices().
		size_t                     getUVValueIndicesCount(int setIndex) const;

		/// Get an array of UVW mapping coordinates for the given mapping channel index.
		const Vector *             getUVValues(int setIndex) const;

		/// Get an index array for the coordinates returned by getUVValues().
		const int *                getUVValueIndices(int setIndex) const;

	private:
		friend class VRayRenderer;
		void allocateArrays();

		struct Internal {
			int version;
			union {
				long long int flags;
				struct {
					bool maxToMayaTM : 1;
					bool mayaToMaxTM : 1;
					bool reserved_bit2 : 1;
					bool reserved_bit3 : 1;
					bool use_vertices : 1;
					bool use_normals : 1;
					bool use_mtlIDs : 1;
					bool use_velocities : 1;
					bool use_UVs : 1;
				};
			};

			float* vertices;
			size_t vertices_count; ///< number of floats, 3 per vertex
			int*   indices;
			size_t indices_count; ///< number of ints, 3 per triangle
			float* normals;
			size_t normals_count; ///< number of floats, 3 per normal
			int*   normalIndices;
			size_t normalIndices_count; ///< number of ints, 3 per triangle
			unsigned* materialIDs;
			size_t materialIDs_count; ///< number of ints, one per triangle
			float* velocities;
			size_t velocities_count; ///< number of floats, 3 per vertex
			
			char*  uvSetNames; ///< NUL separated list of uvSets_count channel names, e.g. "first\0second\0\0fourth\0", notice third is empty
			int*   uvSetIndices; ///< uvSets_count original channel indices, which may not equal the consecutive 0-based indices in the arrays
			float* uvValues; ///< multiple concatenated arrays of UVW 3-tuples, each being uvValues_counts[i] floats long
			int*   uvValueIndices; ///< multiple concatenated arrays of per-vertex indices, each being uvValueIndices_counts[i] ints long
			size_t uvSets_count; ///< number of UV mapping channels
			size_t uvSetNames_length; ///< number of bytes to allocate for uvSetNames, including terminating zeroes
			size_t uvValues_counts[100]; ///< uvSets_count array lengths for uvValues; the rest of the array is filled with zeroes
			size_t uvValueIndices_counts[100]; ///< uvSets_count array lengths for uvValueIndices; the rest of the array is filled with zeroes
		} d;

		std::vector<Vector> vertices;
		std::vector<int> indices;
		std::vector<Vector> normals;
		std::vector<int> normalIndices;
		std::vector<unsigned> materialIDs;
		std::vector<Vector> velocities;
		std::vector<char> uvSetNames;
		std::vector<int> uvSetIndices;
		std::vector<Vector> uvValues;
		std::vector<int> uvValueIndices;
	};

	/// Used by VRayRenderer::createProxyFileMesh()
	struct ProxyCreateParams {
		ProxyCreateParams() {
			memset(this, 0, sizeof(*this));
			previewType = SIMPLIFY_COMBINED;
		}

	private:

#	if (__GNUC__*100 + __GNUC_MINOR__*10) >= 500
#		pragma GCC diagnostic push
#		pragma GCC diagnostic ignored "-Wunused-private-field"
#   elif defined(__clang__)
#		pragma clang diagnostic push
#		pragma clang diagnostic ignored "-Wunused-private-field"
#	endif
		int version;
#	if (__GNUC__*100 + __GNUC_MINOR__*10) >= 500
#		pragma GCC diagnostic pop
#   elif defined(__clang__)
#		pragma clang diagnostic pop
#	endif

	public:
		char *destination; ///< destination file name
		int elementsPerVoxel; ///< number of triangles in each voxel
		int previewFaces; ///< approximate number of preview triangles
		int previewType; ///< @see enum PreviewTypes
		int animOn; ///< enables saving multiple animated frames in the proxy
		int startFrame; ///< used if animOn is true
		int endFrame; ///< used if animOn is true

		enum PreviewTypes {
			/// Copy faces from the original model, leaving them unchanged.
			/// This method is the fastest, but the resulting triangles are not connected,
			/// and for a detailed model it will produce a semi-transparent cloud of triangles.
			SIMPLIFY_FACE_SAMPLING = 0,
			/// Split the model with a grid and keep one vertex from each cell
			/// Runs in O(n) time, deals well with disconnected triangles
			/// and produces relatively good results. It will not produce exactly the desired number of faces.
			SIMPLIFY_CLUSTERING,
			/// Choose an edge based on a heuristic and collapse it to a single
			/// vertex, repeat until the desired number of faces is reached.
			/// Produces the best results, except on disconnected triangles, but is slower - O(nlogn).
			SIMPLIFY_EDGE_COLLAPSE,
			/// Combines clustering and edge collapse. First, vertex clustering is applied,
			/// targeting 4 times the final number of faces. Then edge collapse is applied.
			/// This is almost as fast as face sampling, deals well with disconnected triangles,
			/// and the results are almost as good as edge collapse.
			/// This is the recommended type if you don't need the fastest possible
			/// preview generation, which is face sampling.
			SIMPLIFY_COMBINED
		};
	};

	struct ResumableRenderingOptions {
		/// Optional output path. This is needed when SettingsOutput.img_file is not set.
		/// (Currently only for progressive rendering. Extension is replaced with .vrprog)
		const char *outputFileName;

		/// Set to false to disable compression. (These files can get quite large)
		int compressProgressiveFiles;

		/// Interval for saving intermediate (.vrprog) files. If set to 0 a file will only
		/// be saved at the end of the rendering. It can still be resumed at a lower noise
		/// threshold or higher time/sample limit.
		int progressiveAutoSaveSeconds;

		ResumableRenderingOptions()
			: outputFileName(NULL), compressProgressiveFiles(1), progressiveAutoSaveSeconds(0)
		{}
	};

	/// Axis-aligned bounding box
	struct Box {
		Vector pmin; ///< The lower bounds for the box along the three axes
		Vector pmax; ///< The upper bounds for the box along the three axes
		std::string toString() const;
	};

	inline std::ostream& operator <<(std::ostream& stream, const Box& box) {
		stream << "Box(min: " << box.pmin << ", max: " << box.pmax << ")";
		return stream;
	}

	inline std::string Box::toString() const {
		std::ostringstream stream;
		stream << *this;
		return stream.str();
	}

	#pragma pack(push, 1)
	union VersionUnion {
		struct {
			unsigned char  revision;
			unsigned char  minor;
			unsigned short major;
		};
		unsigned allFields;


		#ifdef _MSC_VER
		#pragma warning(disable : 4996)
		#endif
		std::string toString() const {
			char buf[64];
			std::sprintf(buf, "%u.%02u.%02u", major, minor, revision);
			return buf;
		}
		#ifdef _MSC_VER
		#pragma warning(default : 4996)
		#endif
	};
	typedef VersionUnion VRayVersion;
	typedef VersionUnion SDKVersion;
	#pragma pack(pop)

	inline std::ostream& operator <<(std::ostream& stream, const VRayVersion& version) {
		stream << version.toString();
		return stream;
	}

	struct ScannedMaterialParams;

// internal function declarations
#include "_vraysdk_import.hpp"

	/// Unlimited sampling passes by default
	const int DEFAULT_SAMPLE_LEVEL = -1;
	/// No noise threshold by default
	const float DEFAULT_NOISE_THRESHOLD = -1.0f;
	/// No time limit by default
	const long DEFAULT_TIMEOUT = -1;

	/// Combines various options required for the rendering process
	struct RendererOptions {
		friend class VRayRenderer;

		enum RenderMode {
			RENDER_MODE_PRODUCTION = -1,
			RENDER_MODE_RT_CPU = 0,
			RENDER_MODE_RT_GPU_OPENCL = 1,
			RENDER_MODE_RT_GPU_CUDA = 4,
			RENDER_MODE_RT_GPU = RENDER_MODE_RT_GPU_CUDA,
			RENDER_MODE_PRODUCTION_OPENCL = 101,
			RENDER_MODE_PRODUCTION_CUDA = 104,
		};

		enum DefaultSizes {
			DEFAULT_WIDTH = 0,	///< default means that the renderer will read the width from the scene file
			DEFAULT_HEIGHT = 0	///< default means that the renderer will read the height from the scene file
		};
		
		/// Specifies the drawing style of the VFB widgets
		enum ThemeStyle {
			ThemeStyleAuto = 0,
			ThemeStyleStandalone,
			ThemeStyle3dsMax,
			ThemeStyle3dsMaxQt,
			ThemeStyleMaya,
		};

	public:
		int imageWidth;  ///< rendered image width
		int imageHeight; ///< rendered image height

	private:
		mutable const char* internal_pluginLibraryPath;
		mutable const char* internal_gpuPluginPath;

	public:
		RenderMode renderMode;      ///< type of the V-Ray engine to be used - production, RT CPU, RT GPU
		int rtSampleLevel;          ///< maximum camera samples per pixel when using the RT engine
		float rtNoiseThreshold;     ///< noise threshold for a frame when using the RT engine
		long rtTimeout;             ///< a timeout value (in milliseconds) for a frame when using the RT engine
		int numThreads;             ///< the number of rendering threads (default is 0 - all available)
		ThemeStyle vfbDrawStyle;    ///< specifies VFB drawing style

		union {
			int flags;
			struct {
				bool enableFrameBuffer : 1; ///< true to enable the frame buffer
				bool showFrameBuffer : 1;   ///< true to initially show the frame buffer
				bool inProcess : 1;         ///< specifies whether the executing process participates in the rendering 
				                            ///< or whether the rendering is executed entirely by V-Ray DR render slaves
				bool keepRTRunning : 1;     ///< don't quit RT rendering even if timeout or noise threshold are reached
				bool transferAssets : 1;    ///< transfer missing assets
				bool useCachedAssets : 1;   ///< check for assets in the asset cache folder before transferring them
				bool bakeGamma : 1;         ///< true to bake the gamma in VRayImages, false to keep them in linear space
				bool noRenderLicensePreCheck : 1; ///< if set appsdk will not check for render node license before rendering is started
				bool noDR : 1;              ///< if set distributed rendering will be disabled
				bool useDefaultVfbTheme : 1; ///< enabled by default. If set to false the parent application theme is inherited.
			};
		};
		std::string pluginLibraryPath; ///< specifies additional plugin library paths
		std::string gpuPluginPath;     ///< rt_cuda and rt_opencl plugins search path

		RendererOptions() :
			imageWidth(DEFAULT_WIDTH),
			imageHeight(DEFAULT_HEIGHT),
			renderMode(RENDER_MODE_RT_CPU),
			rtSampleLevel(DEFAULT_SAMPLE_LEVEL),
			rtNoiseThreshold(DEFAULT_NOISE_THRESHOLD),
			rtTimeout(DEFAULT_TIMEOUT),
			numThreads(0),
			vfbDrawStyle(ThemeStyleAuto)
		{
			flags = 0;
			enableFrameBuffer = true;
			showFrameBuffer = true;
			inProcess = true;
			useDefaultVfbTheme = true;
			transferAssets = true;
			useCachedAssets = true;
		}

		RendererOptions(const RendererOptions& options);

		RendererOptions& operator=(const RendererOptions& options);

		void swap(RendererOptions& options);

	private:
		const RendererOptions* prepare() const;

		explicit RendererOptions(const RendererOptions* options);
	};


#if !defined(VRAY_RUNTIME_LOAD_PRIMARY) && !defined(VRAY_RUNTIME_LOAD_SECONDARY)
	/// This class should be used when linking VRaySDKLibrary at compile time (with VRaySDKLibrary.lib on Windows or -lVRaySDKLibrary on Linux/Mac)
	/// An instance of it should exist for the whole lifetime of the application.
	class VRayInit {
		VRayInit(VRayInit&);	// copy construction disabled

	public:
		VRayInit() {}

		/// @param enableFrameBuffer Enable or disable frame buffer globally
		explicit VRayInit(bool enableFrameBuffer) {
			Internal::VRay_enableFrameBuffer(enableFrameBuffer);
		}

#ifdef _WIN32
		/// Call this before creating a renderer for the first time if you are going to parent it to your Win32 window.
		/// WARNING: You will get a deadlock if you call setUseParentEventLoop and you don't execute vfb.setParentWindow before showing the VFB
		void setUseParentEventLoop() {
			Internal::VRay_blendFrameBuffer();
		}
#endif // _WIN32

		~VRayInit() {
			Internal::VRay_LicenseManager_releaseLicense();
		}
	};

#elif defined(VRAY_RUNTIME_LOAD_PRIMARY)
	/// This class should be used when linking VRaySDKLibrary at runtime.
	/// An instance of it should exist for the whole lifetime of the application.
	class VRayInit {
		VRayInit(VRayInit&);	// copy construction disabled

	public:
		/// @param VRaySDKLibrary DLL/SO/DYLIB name (full path optional). If NULL, default name is used
		/// @note Remember to set up PATH/LD_LIBRARY_PATH/DYLD_LIBRARY_PATH respectively for Windows/Linux/Mac
		///   to point to the location of this file
		explicit VRayInit(const char* const libraryFileName) {
			initialize(libraryFileName);
		}

		/// @param VRaySDKLibrary DLL/SO/DYLIB name (full path optional). If NULL, default name is used
		/// @note Remember to set up PATH/LD_LIBRARY_PATH/DYLD_LIBRARY_PATH respectively for Windows/Linux/Mac
		///   to point to the location of this file
		/// @param  enable or disable frame buffer
		VRayInit(const char* const libraryFileName, bool enableFrameBuffer) {
			initialize(libraryFileName);
			if (Internal::VRay_enableFrameBuffer) Internal::VRay_enableFrameBuffer(enableFrameBuffer);
		}

#ifdef _WIN32
		/// Call this before creating a renderer for the first time if you are going to parent it to your Win32 window.
		/// WARNING: You will get a deadlock if you call setUseParentEventLoop and you don't execute vfb.setParentWindow before showing the VFB
		void setUseParentEventLoop() {
			Internal::VRay_blendFrameBuffer();
		}
#endif

		~VRayInit() {
			if (Internal::VRay_LicenseManager_releaseLicense)
				Internal::VRay_LicenseManager_releaseLicense();
			release();
		}

		operator bool () const {
			return !!hLib;
		}

		/// Return string explaining why loading the library failed
		std::string getLibraryError() const {
			return errString;
		}

	protected:
		VRayInit() {}

	#ifdef _WIN32
		HMODULE hLib;
	#else
		void* hLib;
	#endif

		std::string errString;

	#ifdef VRAY_NOTHROW

		void makeError(const std::string &errMsg) {
			errString = errMsg;
			hLib = NULL;
		}

	#endif

		void initialize(const char* libraryFileName);

		void release() {
			if (hLib)
			#ifdef _WIN32
				FreeLibrary(hLib);
			#else
				dlclose(hLib);
			#endif
		}
	};
#endif // #if !defined(VRAY_RUNTIME_LOAD_PRIMARY) && !defined(VRAY_RUNTIME_LOAD_SECONDARY)

	/// Returns the SDK version as a union. See type SDKVersion.
	inline SDKVersion getSDKVersion() {
		SDKVersion ver;
		ver.allFields = Internal::VRay_getSDKVersion();
		return ver;
	}
	
	/// Returns the V-Ray core version as a union. See type VRayVersion.
	inline VRayVersion getVRayVersion() {
		VRayVersion ver;
		ver.allFields = Internal::VRay_getVRayVersion();
		return ver;
	}
	
	/// Return a string containing the SDK version (and version hash), build tag and build date.
	inline const char* getSDKVersionDetails() {
		return Internal::VRay_getSDKVersionDetails();
	}

	/// Return a string containing the SDK version (and version hash) and build date.
	inline const char* getVRayVersionDetails() {
		return Internal::VRay_getVRayVersionDetails();
	}

	/// Parameters for setLicenseServer(). Only serverName is obligatory.
	struct LicenseServerSettings {
		std::string serverName;	///< These strings can hold a hostname or IP address
		int serverPort;
		std::string proxyName;
		int proxyPort;
		std::string username;
		std::string password;

		std::string serverName1;
		int serverPort1;

		std::string serverName2;
		int serverPort2;

		LicenseServerSettings(void) {
			serverPort = 0;
			proxyPort = 0;
			serverPort1 = 0;
			serverPort2 = 0;
		}
	};

	/// Call this before creating a Renderer to point to your license server
	/// Returns 0 on success or error code otherwise
	/// Error return values: -1: You can't change server settings while license is engaged
	/// -2: Cannot initialize authentication server. This is an internal error
	/// -3: Cannot find the appropriate method in vray.dll. This is most probably a version mismatch
	inline int setLicenseServers(const LicenseServerSettings& settings);

	class Error {

	protected:
		ErrorCode errorCode;

	public:
		Error(int errorCode = SUCCESS) : errorCode(static_cast<ErrorCode>(errorCode)) {
		}

		Error(ErrorCode errorCode) : errorCode(errorCode) {
		}

		bool error() const {
			return errorCode != SUCCESS;
		}

		ErrorCode getCode() const {
			return errorCode;
		}

		const char* toString() const {
			return Internal::VRay_getErrorTextStringFromErrorCode(errorCode);
		}

		friend inline bool operator==(const Error& err0, const Error& err1) {
			return err0.errorCode == err1.errorCode;
		}

		friend inline bool operator!=(const Error& err0, const Error& err1) {
			return err0.errorCode != err1.errorCode;
		}

		friend inline bool operator==(const Error& err, ErrorCode errCode) {
			return err.errorCode == errCode;
		}

		friend inline bool operator==(ErrorCode errCode, const Error& err) {
			return errCode == err.errorCode;
		}

		friend inline bool operator!=(const Error& err, ErrorCode errCode) {
			return err.errorCode != errCode;
		}

		friend inline bool operator!=(ErrorCode errCode, const Error& err) {
			return errCode != err.errorCode;
		}

	};

	inline std::ostream& operator <<(std::ostream& stream, const Error& err) {
		stream << err.toString();
		return stream;
	}

	/// Returned by VRayRenderer::getLastParserError. Use to diagnose problems when loading vrscene files.
	struct ParserError {
		std::string fileName;
		std::string errorText;
		int fileLine;
	};

	/// Returned by VRayRenderer::getLicensError. Use to diagnose licensing problems when renderer creation fails or rendering fails.
	union LicenseError {
		friend class VRayRenderer;
		friend std::ostream& operator <<(std::ostream& stream, const LicenseError& err);

		enum VRLAuthError {
			cgauth_dynamicLibNotFoundError = -6666, ///< cgauth.dll and/or vray.dll is missing
			cgauth_functionsNotFoundError, ///< Functions are missing from cgauth.dll and/or vray.dll
			cgauth_unfunctionalError, ///< cgauth.dll does not work as expected
			cgauth_unsupportedWithThisLicenseError, ///< Selected license type does not allow this program to run

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

		/// Returns true if there was any license error
		bool error() const {
			return !!errs;
		}

		/// Check license error code for creating VRayRenderer
		VRLAuthError getAppSDKError() const {
			return appSDK;
		}

		/// Check license error code for rendering
		RenderNodeErr renderNodeError() const {
			return renderNode;
		}

		std::string toString() const;
	};

	/// See encodeScannedMaterialParams()
	struct ScannedMaterialLicenseError {
		LicenseError::VRLAuthError errorCode;

		ScannedMaterialLicenseError(LicenseError::VRLAuthError errorCode = LicenseError::vrlauth_noError)
			: errorCode(errorCode) {}

		bool error() const {
			return errorCode != LicenseError::vrlauth_noError;
		}

		const char* toString() const;
	};


	/// This structure contains the parameters  used in the rendering of the scanned materials.
	struct ScannedMaterialParams {
		enum Plain {
			PL_NONE     = 0, // no plain strategy 
			PL_HMG      = 1, // the material is homogenous, an averaged brdf is used instead texture (LighShot::vb), uv mapping must be provided
			PL_HMGSYM   = 2, // the material is homogenous, symmetrized brdf is used instead texture (LightShot::vbs), no uv mapping is needed
			PL_SCRAMBLE = 3  // the material has symmetric average brdf but small details are existing (sand,carpaint, ). the texture is used after scrambling , no uv mapping is needed 
		};

		enum MapChannels {
			VRSM_PAINT  = 0, // the color of the material
			VRSM_FILTER = 1, // general color multiplier
			VRSM_CCMULT = 2  // clear coat multiplier
		};

		/// These values can be combined. Each bit enables the corresponding feature.
		enum MapChannelsMask {
			CH_PAINT_MASK  = 1 << VRSM_PAINT,
			CH_FILTER_MASK = 1 << VRSM_FILTER,
			CH_CCMULT_MASK = 1 << VRSM_CCMULT
		};

		enum ColorSpace {
			CS_SRGB     = 0, // sRGB - linear 
			CS_ADOBE    = 1, // adobe rgb
			CS_PP       = 2  // pro photo rgb
		};

		Plain           plain; // this option modifies the rendering in order to improve the result for materials without large details
		float           invgamma; // inverse gamma
		float           saturation;
		float           depthMul; // displacement/paralax  multiplier
		int             disablewmap; // disable use of the enlargement, directly the original small sample is used for rendering
		Color           filter; // general result multiplier (tint)
		Transform       uvtrans; // uv mapping transformation matrix
		int             usemap; // not used
		float           bumpmul; // bump multiplier for the inclined views
		float           bumpstart; // the z value of the view vector below which bump is gradualy applied
		int             nsamples; // the reflection rays count,  the UI contains subdivision parameter, this must  be seto to the square value of the subdivisions
		float           cutoff; // stop of the indirect ray if the total weight is below this value
		int             mapChannel; // the mapping channel
		int             dome; // prevent double lighting by direct and reflection rays
		float           multdirect, multrefl, multgi; // multipliers for different parts of the light
		int             traceDepth; //limit for the tracing recursion
		float           scrambleSize; // used when the plain option is set to PL_SCRAMBLE, determines the size of the pieces that are continuous in the uv space , measured in cm
		float           sceneScale; // the size of one scene unit in cm
		float           ccior; // the clear coat ior, used for carpaints and other materials with thin refractive layer
		int             cchlight; // clear coat highlights
		float	        ccbump; // used to make the clear coat not perfect
		int	            unfRefl; // uniform reflections distribution, when nonzero the importance sampling is not used
		int             twoside;// same as two side in the standard material
		int             displace;// edge displacement, used to prevent the flat appearance of the edges
		int             noPrimGI; // the primary rays are rendered without using the GI engine (better acuracity, lower speed)
		float           retrace; // the minimal path length of a reflected ray that allows to use GI, if the path is shorter "fear" ray tracing is used
		int             noTransp; // force opaque rendering even if backside lighting is present.
		float           transpMul; // multiplies the transparency (only the transparency, the translucency remains!)
		MapChannelsMask mapChBitmask; // bitmask for the maps used in the rendering
		float           paintStrength; // strength of the replace paint effect
		float           fltStrength; // strength of the filter
		ColorSpace      clrSpace; //the output color space

		void setDefaults() // these values will be used if the scene contains no info about certain parameters, these values are NOT the default UI values
		{
			memset(this, 0, sizeof(*this));
			depthMul = 1;
			invgamma = 1;
			saturation = 1;
			multrefl = 1;
			multdirect = 1;
			multgi = 1;
			mapChannel = 1;
			sceneScale = 2.54f; // by default max starts in inches 
			scrambleSize = 0.0066f * 10; // ten pixels 
			ccior = 1.0f;
			cchlight = 1;
			ccbump = 1;
			noPrimGI = 1;
			transpMul = 1;
			paintStrength = 1;
			fltStrength = 1;

			filter = Color(1.0f, 1.0f, 1.0f);
			uvtrans.makeIdentity();
		}

		/// Calls setDefaults() internally.
		ScannedMaterialParams() { setDefaults(); }
	};


	typedef std::vector<int>    IntList;
	typedef std::vector<float>  FloatList;
	typedef std::vector<Color>  ColorList;
	typedef std::vector<Vector> VectorList;
	typedef std::vector<Value>  ValueList;


	/// Encodes the parameters for the BRDFScanned::param_block plugin property
	/// @param mtlParams [in] the parameters to be encoded
	/// @param paramBlock [out] the encoded parameters
	/// @retval true on success; false on error (most probably a license error)
	bool encodeScannedMaterialParams(const ScannedMaterialParams& mtlParams, IntList& paramBlock);

	/// Encodes the parameters for the BRDFScanned::param_block plugin property
	/// @param mtlParams [in] the parameters to be encoded
	/// @param paramBlock [out] the encoded parameters
	/// @param licError [out] the license error
	/// @retval true on success; false on error. If it's a license error, licError contains the error.
	///  If it's not a license error (licError.error()==false) there is some internal error (the SDK is broken)
	bool encodeScannedMaterialParams(const ScannedMaterialParams& mtlParams, IntList& paramBlock, ScannedMaterialLicenseError& licError);

	namespace VUtils {
		class Value;
	}

	/// A thin *wrapper* (or a handle) around actual V-Ray scene objects. It holds a reference to the real object,
	/// so constructing and destructing a Plugin does no effect the scene.
	/// Only VRayRenderer methods can create and remove the underlying V-Ray objects.
	class Plugin {
		friend class Value;
		friend class VRayRenderer;
		friend class PropertyRuntimeMeta;
		friend struct PluginDistancePair;
		friend class RenderElement;
		friend class VUtils::Value;

	protected:
		const VRayRenderer *pRenderer;
		InstanceId id;

		Plugin(VRayRenderer& renderer, const char* name);
		Plugin(VRayRenderer& renderer, const std::string& name);
		Plugin(VRayRenderer& renderer, InstanceId id);

		InstanceId getId() const;

		template<typename T, int TYPE>
		bool setValueTemplate(const char* propertyName, const T& value);

		template<typename T, int TYPE>
		bool setValueAtTimeTemplate(const char* propertyName, const T& value, double time);

		template<typename T, int TYPE>
		bool setArrayTemplate(const char* propertyName, size_t count, const T& value);

		template<typename T, int TYPE>
		bool setArrayAtTimeTemplate(const char* propertyName, size_t count, const T& value, double time);

	public:
		/// Default construct an invalid Plugin - not referencing a V-Ray scene object.
		Plugin() : pRenderer(), id(NO_ID) {}

		/// Copy construct from another Plugin. They both point to the same underlying object. This is like having T& a=x; T& b=x;
		Plugin(const Plugin &plugin);

		/// Assign from another Plugin. They both point to the same underlying object. This is like having T& a=x; T& b=x;
		Plugin& operator=(const Plugin &plugin);

		/// Returns true if both Plugins belong to the same renderer and point to the same scene object instance.
		bool operator==(const Plugin &plugin) const;

		/// Swaps the underlying scene object instance reference and associated renderer reference.
		void swap(Plugin &plugin);

		/// Returns false if the Plugin does not hold a valid reference (e.g. default constructed Plugin())
		/// This should be checked after calling methods that return a Plugin, but success is not guaranteed.
		operator bool() const;

		/// Returns false if the Plugin does not reference a valid V-Ray scene object at this time.
		/// This is a superset of `operator bool()` because it also checks if the referenced V-Ray object still exists.
		/// This may not be true if the object was deleted through another Plugin reference.
		/// This check may have greater performance impact if called very often. The operator bool() check is very light.
		bool isValid() const;

		/// Return the renderer this Plugin is associated with
		VRayRenderer* getRenderer() const;

		/// Returns the name of the plugin instance.
		/// @retval The pointer is valid while the plugin instance inside VRayRenderer is alive
		/// or until a new name is set with setName().
		const char* getName() const;

		/// Sets a new name for this plugin instance.
		/// @param newName If another plugin with the same name already exists this call fails.
		/// @retval true on success, false on failure.
		bool setName(const char* newName);

		/// @overload
		bool setName(const std::string& newName);

		/// Returns the type of this plugin as string.
		const char* getType() const;

		/// Returns an object used to obtain meta information about this plugin's type.
		/// @retval PluginMeta
		PluginMeta getMeta() const;

		/// Returns an object used to obtain meta information about the property runtime instance.
		/// @retval PropertyRuntimeMeta
		PropertyRuntimeMeta getPropertyRuntimeMeta(const std::string &propertyName) const;

		/// Returns the value of a property as string.
		/// @param propertyName
		std::string getValueAsString(const char* propertyName) const;

		/// @overload
		std::string getValueAsString(const std::string& propertyName) const;

		/// Returns the value of a property as string.
		/// @param propertyName
		/// @param ok - set to true if the property has been successfully read, set to false otherwise
		std::string getValueAsString(const char* propertyName, bool& ok) const;

		/// @overload
		std::string getValueAsString(const std::string& propertyName, bool& ok) const;

		/// Sets the value of a property as string. The property may be of any type.
		/// @param propertyName
		/// @param value For example "3.71" or "Color(1.0, 0.0, 0.0)" or "myTexturePlugin::out_alpha"
		/// @retval true - success, false - failure
		bool setValueAsString(const char* propertyName, const char* value);

		/// @overload
		bool setValueAsString(const char* propertyName, const std::string& value);

		/// @overload
		bool setValueAsString(const std::string& propertyName, const char* value);

		/// @overload
		bool setValueAsString(const std::string& propertyName, const std::string& value);

		/// Sets the value of an animated property as string. The property may be of any type.
		/// @param propertyName
		/// @param value For example "3.71" or "Color(1.0, 0.0, 0.0)" or "myTexturePlugin::out_alpha"
		/// @param time Keyframe time
		/// @retval true - success, false - failure
		bool setValueAsStringAtTime(const char* propertyName, const char* value, double time);

		/// @overload
		bool setValueAsStringAtTime(const char* propertyName, const std::string& value, double time);

		/// @overload
		bool setValueAsStringAtTime(const std::string& propertyName, const char* value, double time);

		/// @overload
		bool setValueAsStringAtTime(const std::string& propertyName, const std::string& value, double time);

		/// Returns the value of a property as bool
		bool getBool(const char* propertyName) const;

		/// @overload
		bool getBool(const std::string& propertyName) const;

		/// Returns the value of a property as bool
		/// @param propertyName
		/// @param ok - set to true if the property has been successfully read, set to false otherwise
		bool getBool(const char* propertyName, bool& ok) const;

		/// @overload
		bool getBool(const std::string& propertyName, bool& ok) const;

		/// Returns the value of a property as int
		/// @param propertyName
		int getInt(const char* propertyName) const;

		/// @overload
		int getInt(const std::string& propertyName) const;

		/// Returns the value of a property as int
		/// @param propertyName
		/// @param ok - set to true if the property has been successfully read, set to false otherwise
		int getInt(const char* propertyName, bool& ok) const;

		/// @overload
		int getInt(const std::string& propertyName, bool& ok) const;

		/// Returns the value of a property as float
		float getFloat(const char* propertyName) const;

		/// @overload
		float getFloat(const std::string& propertyName) const;

		/// Returns the value of a property as float
		/// @param propertyName
		/// @param ok - set to true if the property has been successfully read, set to false otherwise
		float getFloat(const char* propertyName, bool& ok) const;

		/// @overload
		float getFloat(const std::string& propertyName, bool& ok) const;

		/// Returns the value of a property as Color
		Color getColor(const char* propertyName) const;

		/// @overload
		Color getColor(const std::string& propertyName) const;

		/// Returns the value of a property as Color
		/// @param propertyName
		/// @param ok - set to true if the property has been successfully read, set to false otherwise
		Color getColor(const char* propertyName, bool& ok) const;

		/// @overload
		Color getColor(const std::string& propertyName, bool& ok) const;

		/// Returns the value of a property as AColor
		AColor getAColor(const char* propertyName) const;

		/// @overload
		AColor getAColor(const std::string& propertyName) const;

		/// Returns the value of a property as AColor
		/// @param propertyName
		/// @param ok - set to true if the property has been successfully read, set to false otherwise
		AColor getAColor(const char* propertyName, bool& ok) const;

		/// @overload
		AColor getAColor(const std::string& propertyName, bool& ok) const;

		/// Returns the value of a property as Vector
		Vector getVector(const char* propertyName) const;

		/// @overload
		Vector getVector(const std::string& propertyName) const;

		/// Returns the value of a property as Vector
		/// @param propertyName
		/// @param ok - set to true if the property has been successfully read, set to false otherwise
		Vector getVector(const char* propertyName, bool& ok) const;

		/// @overload
		Vector getVector(const std::string& propertyName, bool& ok) const;

		/// Returns the value of a property as Matrix
		Matrix getMatrix(const char* propertyName) const;

		/// @overload
		Matrix getMatrix(const std::string& propertyName) const;

		/// Returns the value of a property as Matrix
		/// @param propertyName
		/// @param ok - set to true if the property has been successfully read, set to false otherwise
		Matrix getMatrix(const char* propertyName, bool& ok) const;

		/// @overload
		Matrix getMatrix(const std::string& propertyName, bool& ok) const;

		/// Returns the value of a property as Transform
		Transform getTransform(const char* propertyName) const;

		/// @overload
		Transform getTransform(const std::string& propertyName) const;

		/// Returns the value of a property as Transform
		/// @param propertyName
		/// @param ok - set to true if the property has been successfully read, set to false otherwise
		Transform getTransform(const char* propertyName, bool& ok) const;

		/// @overload
		Transform getTransform(const std::string& propertyName, bool& ok) const;

		/// Returns the value of a property as Plugin
		Plugin getPlugin(const char* propertyName) const;

		/// @overload
		Plugin getPlugin(const std::string& propertyName) const;

		/// Returns the value of a property as Plugin
		/// @param propertyName
		/// @param ok - set to true if the property has been successfully read, set to false otherwise
		Plugin getPlugin(const char* propertyName, bool& ok) const;

		/// @overload
		Plugin getPlugin(const std::string& propertyName, bool& ok) const;

		/// Returns the value of a property as Plugin with specific output (plugin::property)
		/// @param propertyName
		/// @param [out] outPropertyName
		Plugin getPluginProperty(const char* propertyName, std::string& outPropertyName) const;

		/// @overload
		Plugin getPluginProperty(const std::string& propertyName, std::string& outPropertyName) const;

		/// Returns the value of a property as Plugin with specific output (plugin::property)
		/// @param propertyName
		/// @param [out] outPropertyName
		/// @param [out] ok - set to true if the property has been successfully read, set to false otherwise
		Plugin getPluginProperty(const char* propertyName, std::string& outPropertyName, bool& ok) const;

		/// @overload
		Plugin getPluginProperty(const std::string& propertyName, std::string& outPropertyName, bool& ok) const;

		/// Returns the value of a property as string
		std::string getString(const char* propertyName) const;

		/// @overload
		std::string getString(const std::string& propertyName) const;

		/// Returns the value of a property as string
		/// @param propertyName
		/// @param ok - set to true if the property has been successfully read, set to false otherwise
		std::string getString(const char* propertyName, bool& ok) const;

		/// @overload
		std::string getString(const std::string& propertyName, bool& ok) const;

		/// Returns the value of a property as IntList
		IntList getIntList(const char* propertyName) const;

		/// @overload
		IntList getIntList(const std::string& propertyName) const;

		/// Returns the value of a property as IntList
		/// @param propertyName
		/// @param ok - set to true if the property has been successfully read, set to false otherwise
		IntList getIntList(const char* propertyName, bool& ok) const;

		/// @overload
		IntList getIntList(const std::string& propertyName, bool& ok) const;

		/// Returns the value of a property as FloatList
		FloatList getFloatList(const char* propertyName) const;

		/// @overload
		FloatList getFloatList(const std::string& propertyName) const;

		/// Returns the value of a property as FloatList
		/// @param propertyName
		/// @param ok - set to true if the property has been successfully read, set to false otherwise
		FloatList getFloatList(const char* propertyName, bool& ok) const;

		/// @overload
		FloatList getFloatList(const std::string& propertyName, bool& ok) const;

		/// Returns the value of a property as ColorList
		ColorList getColorList(const char* propertyName) const;

		/// @overload
		ColorList getColorList(const std::string& propertyName) const;

		/// Returns the value of a property as ColorList
		/// @param propertyName
		/// @param ok - set to true if the property has been successfully read, set to false otherwise
		ColorList getColorList(const char* propertyName, bool& ok) const;

		/// @overload
		ColorList getColorList(const std::string& propertyName, bool& ok) const;

		/// Returns the value of a property as VectorList
		VectorList getVectorList(const char* propertyName) const;

		/// @overload
		VectorList getVectorList(const std::string& propertyName) const;

		/// Returns the value of a property as VectorList
		/// @param propertyName
		/// @param ok - set to true if the property has been successfully read, set to false otherwise
		VectorList getVectorList(const char* propertyName, bool& ok) const;

		/// @overload
		VectorList getVectorList(const std::string& propertyName, bool& ok) const;

		/// Returns the value of a property as ValueList
		ValueList getValueList(const char* propertyName) const;

		/// @overload
		ValueList getValueList(const std::string& propertyName) const;

		/// Returns the value of a property as ValueList
		/// @param propertyName
		/// @param ok - set to true if the property has been successfully read, set to false otherwise
		ValueList getValueList(const char* propertyName, bool& ok) const;

		/// @overload
		ValueList getValueList(const std::string& propertyName, bool& ok) const;

		/// Returns the value of a property encapsulated in Value class
		/// Can return simple as well as complex properties such as lists
		Value getValue(const char* propertyName) const;

		/// @overload
		Value getValue(const std::string& propertyName) const;

		/// Returns the value of a property encapsulated in Value class
		/// Can return simple as well as complex properties such as lists
		/// @param propertyName
		/// @param ok - set to true if the property has been successfully read, set to false otherwise
		Value getValue(const char* propertyName, bool& ok) const;

		/// @overload
		Value getValue(const std::string& propertyName, bool& ok) const;

		/// Returns whether the current property contains animated values or not
		/// @retval true - animated, false - not animated
		bool isAnimated(const char* propertyName) const;

		/// @overload
		bool isAnimated(const std::string& propertyName) const;

		/// Sets the value of a property as bool
		/// @retval true - success, false - failure
		bool setValue(const char* propertyName, const bool value);

		/// @overload
		bool setValue(const std::string& propertyName, const bool value);

		/// Sets the value of a property as bool
		/// @retval true - success, false - failure
		bool setValueAtTime(const char* propertyName, const bool value, double time);

		/// @overload
		bool setValueAtTime(const std::string& propertyName, const bool value, double time);

		/// Sets the value of a property as int
		/// @retval true - success, false - failure
		bool setValue(const char* propertyName, const int value);

		/// @overload
		bool setValue(const std::string& propertyName, const int value);

		/// Sets the value of a property as int
		/// @retval true - success, false - failure
		bool setValueAtTime(const char* propertyName, const int value, double time);

		/// @overload
		bool setValueAtTime(const std::string& propertyName, const int value, double time);

		/// Sets the value of a property as float
		/// @retval true - success, false - failure
		bool setValue(const char* propertyName, const float value);

		/// @overload
		bool setValue(const std::string& propertyName, const float value);

		/// Sets the value of a property as float
		/// @retval true - success, false - failure
		bool setValueAtTime(const char* propertyName, const float value, double time);

		/// @overload
		bool setValueAtTime(const std::string& propertyName, const float value, double time);

		/// Sets the value of a property as Color
		/// @retval true - success, false - failure
		bool setValue(const char* propertyName, const Color& value);

		/// @overload
		bool setValue(const std::string& propertyName, const Color& value);

		/// Sets the value of a property as Color
		/// @retval true - success, false - failure
		bool setValueAtTime(const char* propertyName, const Color& value, double time);

		/// @overload
		bool setValueAtTime(const std::string& propertyName, const Color& value, double time);

		/// Sets the value of a property as AColor
		/// @retval true - success, false - failure
		bool setValue(const char* propertyName, const AColor& value);

		/// @overload
		bool setValue(const std::string& propertyName, const AColor& value);

		/// Sets the value of a property as AColor
		/// @retval true - success, false - failure
		bool setValueAtTime(const char* propertyName, const AColor& value, double time);

		/// @overload
		bool setValueAtTime(const std::string& propertyName, const AColor& value, double time);

		/// Sets the value of a property as Vector
		/// @retval true - success, false - failure
		bool setValue(const char* propertyName, const Vector& value);

		/// @overload
		bool setValue(const std::string& propertyName, const Vector& value);

		/// Sets the value of a property as Vector
		/// @retval true - success, false - failure
		bool setValueAtTime(const char* propertyName, const Vector& value, double time);

		/// @overload
		bool setValueAtTime(const std::string& propertyName, const Vector& value, double time);

		/// Sets the value of a property as Matrix
		/// @retval true - success, false - failure
		bool setValue(const char* propertyName, const Matrix& value);

		/// @overload
		bool setValue(const std::string& propertyName, const Matrix& value);

		/// Sets the value of a property as Matrix
		/// @retval true - success, false - failure
		bool setValueAtTime(const char* propertyName, const Matrix& value, double time);

		/// @overload
		bool setValueAtTime(const std::string& propertyName, const Matrix& value, double time);

		/// Sets the value of a property as Transform
		/// @retval true - success, false - failure
		bool setValue(const char* propertyName, const Transform& transform);

		/// @overload
		bool setValue(const std::string& propertyName, const Transform& transform);

		/// Sets the value of a property as Transform
		/// @retval true - success, false - failure
		bool setValueAtTime(const char* propertyName, const Transform& transform, double time);

		/// @overload
		bool setValueAtTime(const std::string& propertyName, const Transform& transform, double time);

		/// Sets the value of a property as Plugin
		/// @retval true - success, false - failure
		bool setValue(const char* propertyName, const Plugin& plugin);

		/// @overload
		bool setValue(const std::string& propertyName, const Plugin& plugin);

		/// Sets the value of a property as Plugin
		/// @retval true - success, false - failure
		bool setValueAtTime(const char* propertyName, const Plugin& plugin, double time);

		/// @overload
		bool setValueAtTime(const std::string& propertyName, const Plugin& plugin, double time);

		/// Sets the value of a property to string
		/// @retval true - success, false - failure
		bool setValue(const char* propertyName, const char* str);

		/// @overload
		bool setValue(const std::string& propertyName, const char* str);

		/// Sets the value of a property to string
		/// @retval true - success, false - failure
		bool setValueAtTime(const char* propertyName, const char* str, double time);

		/// @overload
		bool setValueAtTime(const std::string& propertyName, const char* str, double time);

		/// Sets the value of a property to string
		/// @retval true - success, false - failure
		bool setValue(const char* propertyName, const std::string& str);

		/// @overload
		bool setValue(const std::string& propertyName, const std::string& str);

		/// Sets the value of a property to string
		/// @retval true - success, false - failure
		bool setValueAtTime(const char* propertyName, const std::string& str, double time);

		/// @overload
		bool setValueAtTime(const std::string& propertyName, const std::string& str, double time);

		/// Sets the value of a property as Plugin with specific output (plugin::property)
		/// @retval true - success, false - failure
		bool setValue(const char* propertyName, const Plugin& outPlugin, const char* outPropertyName);

		/// @overload
		bool setValue(const std::string& propertyName, const Plugin& outPlugin, const char* outPropertyName);

		/// @overload
		bool setValue(const char* propertyName, const Plugin& outPlugin, const std::string& outPropertyName);

		/// @overload
		bool setValue(const std::string& propertyName, const Plugin& outPlugin, const std::string& outPropertyName);

		/// Sets the value of a property as Value (can set complex structures like lists and lists of lists)
		/// @retval true - success, false - failure
		bool setValue(const char* propertyName, const Value& value);

		/// @overload
		bool setValue(const std::string& propertyName, const Value& value);

		/// Sets the value of a property as Value (can set complex structures like lists and lists of lists)
		/// @retval true - success, false - failure
		bool setValueAtTime(const char* propertyName, const Value& value, double time);

		/// @overload
		bool setValueAtTime(const std::string& propertyName, const Value& value, double time);

		/// Sets the value of a property as a list of Values
		/// @retval true - success, false - failure
		bool setValue(const char* propertyName, const ValueList& value);

		/// @overload
		bool setValue(const std::string& propertyName, const ValueList& value);

		/// Sets the value of a property as a list of Values
		/// @retval true - success, false - failure
		bool setValueAtTime(const char* propertyName, const ValueList& value, double time);

		/// @overload
		bool setValueAtTime(const std::string& propertyName, const ValueList& value, double time);

		/// Sets the value of a property to a list of ints
		/// @retval true - success, false - failure
		bool setValue(const char* propertyName, const IntList& value);

		/// @overload
		bool setValue(const std::string& propertyName, const IntList& value);

		/// Sets the value of a property to a list of ints
		/// @retval true - success, false - failure
		bool setValueAtTime(const char* propertyName, const IntList& value, double time);

		/// @overload
		bool setValueAtTime(const std::string& propertyName, const IntList& value, double time);

		/// Sets the value of a property to a list of floats
		/// @retval true - success, false - failure
		bool setValue(const char* propertyName, const FloatList& value);

		/// @overload
		bool setValue(const std::string& propertyName, const FloatList& value);

		/// Sets the value of a property to a list of floats
		/// @retval true - success, false - failure
		bool setValueAtTime(const char* propertyName, const FloatList& value, double time);

		/// @overload
		bool setValueAtTime(const std::string& propertyName, const FloatList& value, double time);

		/// Sets the value of a property to a list of Vectors
		/// @retval true - success, false - failure
		bool setValue(const char* propertyName, const VectorList& value);

		/// @overload
		bool setValue(const std::string& propertyName, const VectorList& value);

		/// Sets the value of a property to a list of Vectors
		/// @retval true - success, false - failure
		bool setValueAtTime(const char* propertyName, const VectorList& value, double time);

		/// @overload
		bool setValueAtTime(const std::string& propertyName, const VectorList& value, double time);

		/// Sets the value of a property to a list of Colors
		/// @retval true - success, false - failure
		bool setValue(const char* propertyName, const ColorList& value);

		/// @overload
		bool setValue(const std::string& propertyName, const ColorList& value);

		/// Sets the value of a property to a list of Colors
		/// @retval true - success, false - failure
		bool setValueAtTime(const char* propertyName, const ColorList& value, double time);

		/// @overload
		bool setValueAtTime(const std::string& propertyName, const ColorList& value, double time);

		/// Sets the value of a property as a list of ints and count elements
		/// @retval true - success, false - failure
		bool setArray(const char* propertyName, const int data[], size_t count);

		/// @overload
		bool setArray(const std::string& propertyName, const int data[], size_t count);

		/// Sets the value of a property as a list of ints and count elements
		/// @retval true - success, false - failure
		bool setArrayAtTime(const char* propertyName, const int data[], size_t count, double time);

		/// @overload
		bool setArrayAtTime(const std::string& propertyName, const int data[], size_t count, double time);

		/// Sets the value of a property as a list of floats and count elements
		/// Can also be used to set Vector, Color, AColor, Matrix amd Transform and lists of them
		/// @retval true - success, false - failure
		bool setArray(const char* propertyName, const float data[], size_t count);

		/// @overload
		bool setArray(const std::string& propertyName, const float data[], size_t count);

		/// Sets the value of a property as a list of floats and count elements
		/// Can also be used to set Vector, Color, AColor, Matrix amd Transform and lists of them
		/// @retval true - success, false - failure
		bool setArrayAtTime(const char* propertyName, const float data[], size_t count, double time);

		/// @overload
		bool setArrayAtTime(const std::string& propertyName, const float data[], size_t count, double time);

		/// Sets the value of a property as a list of floats and count elements
		/// The doubles are fast converted to floats during the operation
		/// Can also be used to set Vector, Color, AColor, Matrix amd Transform and lists of them
		/// @retval true - success, false - failure
		bool setArray(const char* propertyName, const double data[], size_t count);

		/// @overload
		bool setArray(const std::string& propertyName, const double data[], size_t count);

		/// Sets the value of a property as a list of floats and count elements
		/// The doubles are fast converted to floats during the operation
		/// Can also be used to set Vector, Color, AColor, Matrix amd Transform and lists of them
		/// @retval true - success, false - failure
		bool setArrayAtTime(const char* propertyName, const double data[], size_t count, double time);

		/// @overload
		bool setArrayAtTime(const std::string& propertyName, const double data[], size_t count, double time);

		/// Sets the value of a property as a list of Vectors and count elements
		/// @retval true - success, false - failure
		bool setArray(const char* propertyName, const Vector data[], size_t count);

		/// @overload
		bool setArray(const std::string& propertyName, const Vector data[], size_t count);

		/// Sets the value of a property as a list of Vectors and count elements
		/// @retval true - success, false - failure
		bool setArrayAtTime(const char* propertyName, const Vector data[], size_t count, double time);

		/// @overload
		bool setArrayAtTime(const std::string& propertyName, const Vector data[], size_t count, double time);

		/// Sets the value of a property as a list of Colors and count elements
		/// @retval true - success, false - failure
		bool setArray(const char* propertyName, const Color data[], size_t count);

		/// @overload
		bool setArray(const std::string& propertyName, const Color data[], size_t count);

		/// Sets the value of a property as a list of Colors and count elements
		/// @retval true - success, false - failure
		bool setArrayAtTime(const char* propertyName, const Color data[], size_t count, double time);

		/// @overload
		bool setArrayAtTime(const std::string& propertyName, const Color data[], size_t count, double time);

		/// Sets the value of a property as a list of the underlying type and size in bytes, not elements!
		/// @retval true - success, false - failure
		/// @remark This is a fast but potentially dangerous method
		bool setValue(const char* propertyName, const void* data, size_t size);

		/// @overload
		bool setValue(const std::string& propertyName, const void* data, size_t size);

		/// Sets the value of a property as a list of the underlying type and size in bytes, not elements!
		/// @retval true - success, false - failure
		/// @remark This is a fast but potentially dangerous method
		bool setValueAtTime(const char* propertyName, const void* data, size_t size, double time);

		/// @overload
		bool setValueAtTime(const std::string& propertyName, const void* data, size_t size, double time);

#ifdef VRAY_SDK_INTEROPERABILITY
		bool setValue(const char* propertyName, const VUtils::IntRefList& intList);
		bool setValueAtTime(const char* propertyName, const VUtils::IntRefList& intList, double time);
		bool setValue(const std::string& propertyName, const VUtils::IntRefList& intList);
		bool setValueAtTime(const std::string& propertyName, const VUtils::IntRefList& intList, double time);

		bool setValue(const char* propertyName, const VUtils::FloatRefList& intList);
		bool setValueAtTime(const char* propertyName, const VUtils::FloatRefList& intList, double time);
		bool setValue(const std::string& propertyName, const VUtils::FloatRefList& intList);
		bool setValueAtTime(const std::string& propertyName, const VUtils::FloatRefList& intList, double time);

		bool setValue(const char* propertyName, const VUtils::VectorRefList& intList);
		bool setValueAtTime(const char* propertyName, const VUtils::VectorRefList& intList, double time);
		bool setValue(const std::string& propertyName, const VUtils::VectorRefList& intList);
		bool setValueAtTime(const std::string& propertyName, const VUtils::VectorRefList& intList, double time);

		bool setValue(const char* propertyName, const VUtils::ColorRefList& intList);
		bool setValueAtTime(const char* propertyName, const VUtils::ColorRefList& intList, double time);
		bool setValue(const std::string& propertyName, const VUtils::ColorRefList& intList);
		bool setValueAtTime(const std::string& propertyName, const VUtils::ColorRefList& intList, double time);

		bool setValue(const char* propertyName, const VUtils::CharStringRefList& intList);
		bool setValueAtTime(const char* propertyName, const VUtils::CharStringRefList& intList, double time);
		bool setValue(const std::string& propertyName, const VUtils::CharStringRefList& intList);
		bool setValueAtTime(const std::string& propertyName, const VUtils::CharStringRefList& intList, double time);

		bool setValue(const char* propertyName, const VUtils::ValueRefList& valueList);
		bool setValueAtTime(const char* propertyName, const VUtils::ValueRefList& valueList, double time);
		bool setValue(const std::string& propertyName, const VUtils::ValueRefList& valueList);
		bool setValueAtTime(const std::string& propertyName, const VUtils::ValueRefList& valueList, double time);

		bool setValue(const char* propertyName, const VUtils::CharString& str);
		bool setValueAtTime(const char* propertyName, const VUtils::CharString& str, double time);
		bool setValue(const std::string& propertyName, const VUtils::CharString& str);
		bool setValueAtTime(const std::string& propertyName, const VUtils::CharString& str, double time);
#endif // VRAY_SDK_INTEROPERABILITY
	};

	namespace Internal {
		class BinaryValueParser;
		class BinaryValueBuilder;
	}

	/// A generic value holder used by Plugin parameters
	class Value {
		friend class Internal::BinaryValueParser;
		friend class Internal::BinaryValueBuilder;

	protected:
		struct Plugin_t {
			InstanceId id;
			VRayRenderer *renderer;
		};
		union {
			char val[VRAY_MAXIMUM3(sizeof(Transform), sizeof(std::string), sizeof(ValueList))];
			Plugin_t plugin;
		};
		Type type;

		void copyValue(const Value& value);
		void destroyValue();
		template<typename T, Type TYPE> void constructOnly_();
		template<typename T, Type TYPE> void constructListWithIterators_(const T* first, const T* last);
		template<typename T, Type TYPE> void constructWithValue_(const T& value);
		template<typename T, Type TYPE> void setToValue_(const T& value);
		template<typename T> T& interpretAs_();
		template<typename T> const T& interpretAs_() const;
		template<typename T, Type TYPE> T& viewAs_();
		template<typename T, Type TYPE> T getValue_() const;

	public:
		Value();
		Value(const Value& value);

		explicit Value(int value);
		explicit Value(float value);
		explicit Value(bool value);
		explicit Value(const Color& value);
		explicit Value(const AColor& value);
		explicit Value(const Vector& value);
		explicit Value(const Matrix& value);
		explicit Value(const Transform& value);
		explicit Value(const IntList& value);
		explicit Value(const FloatList& value);
		explicit Value(const VectorList& value);
		explicit Value(const ColorList& value);
		explicit Value(const ValueList& value);
		explicit Value(const char* value);
		explicit Value(const std::string& value);
		explicit Value(const Plugin& value);
		Value(VRayRenderer *renderer, InstanceId id);
		Value(const int* values, size_t count);
		Value(const float* values, size_t count);
		Value(const Vector* values, size_t count);
		Value(const Color* values, size_t count);

		~Value();

		void swap(Value& val);

		Value& operator=(const Value& value);

		/// Get the actual type of the stored value
		Type getType() const;

		/// Returns number of list elements if the value is a list type
		size_t getCount() const;

		/// False if this holds a single value
		bool isList() const;

		/// Stores the passed value and sets the appropriate type
		template<typename T> void set(const T& value);

		/// Stores the passed value array and sets the appropriate type
		template<typename T> void set(const T* values, size_t count);

		/// Returns a mutable reference to the value interpreted as T type. Use only when you're sure of the type.
		/// If VRAY_NOTHROW is defined this is done *without checking*. Otherwise an exception is thrown.
		template<typename T> T& as();

		/// Returns a copy of the value if it is indeed of type T. Otherwise an exception is thrown.
		/// If VRAY_NOTHROW is defined a default constructed T is returned.
		template<typename T> T get() const;

		/// Works if the value is int, float or boolean. Otherwise returns 0.
		int getInt() const;

		/// Works if the value is int, float or boolean. Otherwise returns 0.0f.
		float getFloat() const;

		/// Works if the value is int, float or boolean. Otherwise returns false.
		bool getBool() const;

		/// Works if the value is Color or AColor. Otherwise returns zero color.
		Color getColor() const;

		/// Returns zero AColor if the value is not AColor. Keep in mind the value can be an AColor and still be zero.
		AColor getAColor() const;

		/// Returns zero Vector if the value is not Vector. Keep in mind the value can be a Vector and still be zero.
		Vector getVector() const;

		/// Returns default constructed (uninitialized) Matrix if the value is not Matrix.
		Matrix getMatrix() const;

		/// Returns default constructed (uninitialized) Transform if the value is not Transform.
		Transform getTransform() const;

		/// Returns empty list if the value is not IntList. Keep in mind the value can be an IntList and still be empty.
		/// To avoid copy constucting a large list, use the as() method.
		IntList getIntList() const;

		/// Returns empty list if the value is not FloatList. Keep in mind the value can be a FloatList and still be empty.
		/// To avoid copy constucting a large list, use the as() method.
		FloatList getFloatList() const;

		/// Returns empty list if the value is not ColorList. Keep in mind the value can be a ColorList and still be empty.
		/// To avoid copy constucting a large list, use the as() method.
		ColorList getColorList() const;

		/// Returns empty list if the value is not VectorList. Keep in mind the value can be a VectorList and still be empty.
		/// To avoid copy constucting a large list, use the as() method.
		VectorList getVectorList() const;

		/// Returns empty list if the value is not ValueList. Keep in mind the value can be a ValueList and still be empty.
		/// To avoid copy constucting a large list, use the as() method.
		ValueList getValueList() const;

		/// Returns empty string if the value is not string. Keep in mind the value can be a string and still be empty.
		std::string getString() const;

		/// Returns invalid Plugin if the value is not Plugin. Keep in mind the value can be a Plugin and still be invalid.
		Plugin getPlugin() const;

		/// If the value is a list, returns the i-th element.
		/// Otherwise throws an exception (if they are enabled) or the 0-th and only value if exceptions are disabled.
		Value& operator[](int i);

		/// True if the value can be used
		bool isOK() const;
		/// True if there was an error and the value can't be used
		bool isBad() const;

		/// Returns a string representation of the type
		const char* getStringType() const;

		/// Writes the value as string to the stream
		std::ostream& toString(std::ostream& stream) const;
		/// Returns the value as string
		std::string toString() const;
	};

	std::ostream& operator <<(std::ostream& stream, const IntList& list);
	std::ostream& operator <<(std::ostream& stream, const FloatList& list);
	std::ostream& operator <<(std::ostream& stream, const VectorList& list);
	std::ostream& operator <<(std::ostream& stream, const ColorList& list);
	std::ostream& operator <<(std::ostream& stream, const ValueList& list);
	std::ostream& operator <<(std::ostream& stream, const Value& value);

	/// See VRayRenderer::pickPlugins
	struct PluginDistancePair {
		Plugin plugin;
		double distance;
	};


	/// This class wraps the underlying V-Ray render engine and its scene contents
	class VRayRenderer {
		friend class Plugin;
		friend class PluginMeta;
		friend class PropertyMeta;
		friend class PropertyRuntimeMeta;
		friend class VRayServer;
		friend class RenderElement;
		friend class RenderElements;
		friend struct VRayImage;
		friend class LocalPng;
		friend class LocalJpeg;
		friend class LocalBmp;

		/// Renderer object isn't something you normally need to copy
		/// (use pointers if you really need to)
		/// so copy contruction and copy assigment are not allowed.
		VRayRenderer(const VRayRenderer&);
		VRayRenderer& operator=(const VRayRenderer&);

		class Delegate1 {
			typedef void (*tstub)(void* pobject, VRayRenderer& renderer, void *userObj);
			typedef void (*tfunc)(VRayRenderer& renderer, void *userObj);

			void *pobject;
			union tmethod {
				tstub pstub;
				tfunc pfunc;
				tmethod() : pstub() {}
				tmethod(tstub pstub) : pstub(pstub) {}
				tmethod(tfunc pfunc) : pfunc(pfunc) {}
			} pmethod;
			void *puserObj;

			template <class T, void (T::*TMethod)(VRayRenderer&, void*)>
			static void method_stub(void* pobject, VRayRenderer& renderer, void *puserObj) {
				T* p = static_cast<T*>(pobject);
				return (p->*TMethod)(renderer, puserObj);
			}

			Delegate1(tfunc pfunc, void *puserObj) : pobject(), pmethod(pfunc), puserObj(puserObj) {}
			Delegate1(void *pobject, tstub pstub, void *puserObj) : pobject(pobject), pmethod(pstub), puserObj(puserObj) {}

		public:
			Delegate1() {}

			template <class T, void (T::*TMethod)(VRayRenderer&, void*)>
			static Delegate1 from_method(T* pobject, void *puserObj) {
				return Delegate1(pobject, &method_stub<T, TMethod>, puserObj);
			}

			static Delegate1 from_function(tfunc pfunc, void *puserObj) {
				return Delegate1(pfunc, puserObj);
			}

			void operator()(VRayRenderer& renderer) const {
				return pobject ? (*pmethod.pstub)(pobject, renderer, puserObj) : (*pmethod.pfunc)(renderer, puserObj);
			}
		} renderStartDelegate, imageReadyDelegate, rendererCloseDelegate, sequenceStartDelegate, sequenceDoneDelegate, vfbClosedDelegate, postEffectsUpdatedDelegate;

		class Delegate2 {
			typedef void (*tstub)(void* pobject, VRayRenderer& renderer, VRayImage *img, void *userObj);
			typedef void (*tfunc)(VRayRenderer& renderer, VRayImage *img, void *userObj);

			void *pobject;
			union tmethod {
				tstub pstub;
				tfunc pfunc;
				tmethod() : pstub() {}
				tmethod(tstub pstub) : pstub(pstub) {}
				tmethod(tfunc pfunc) : pfunc(pfunc) {}
			} pmethod;
			void *puserObj;

			template <class T, void (T::*TMethod)(VRayRenderer&, VRayImage* img, void*)>
			static void method_stub(void* pobject, VRayRenderer& renderer, VRayImage* img, void *puserObj) {
				T* p = static_cast<T*>(pobject);
				return (p->*TMethod)(renderer, img, puserObj);
			}

			Delegate2(tfunc pfunc, void *puserObj) : pobject(), pmethod(pfunc), puserObj(puserObj) {}
			Delegate2(void *pobject, tstub pstub, void *puserObj) : pobject(pobject), pmethod(pstub), puserObj(puserObj) {}

		public:
			Delegate2() {}

			template <class T, void (T::*TMethod)(VRayRenderer&, VRayImage*, void*)>
			static Delegate2 from_method(T* pobject, void *puserObj) {
				return Delegate2(pobject, &method_stub<T, TMethod>, puserObj);
			}

			static Delegate2 from_function(tfunc pfunc, void *puserObj) {
				return Delegate2(pfunc, puserObj);
			}

			void operator()(VRayRenderer& renderer, VRayImage* img) const {
				return pobject ? (*pmethod.pstub)(pobject, renderer, img, puserObj) : (*pmethod.pfunc)(renderer, img, puserObj);
			}
		} rtImageUpdatedDelegate;

		class Delegate3 {
			typedef void (*tstub)(void* pobject, VRayRenderer& renderer, const char *msg, int level, void *userObj);
			typedef void (*tfunc)(VRayRenderer& renderer, const char *msg, int level, void *userObj);

			void *pobject;
			union tmethod {
				tstub pstub;
				tfunc pfunc;
				tmethod() : pstub() {}
				tmethod(tstub pstub) : pstub(pstub) {}
				tmethod(tfunc pfunc) : pfunc(pfunc) {}
			} pmethod;
			void *puserObj;

			template <class T, void (T::*TMethod)(VRayRenderer&, const char *msg, int level, void*)>
			static void method_stub(void* pobject, VRayRenderer& renderer, const char *msg, int level, void *puserObj) {
				T* p = static_cast<T*>(pobject);
				return (p->*TMethod)(renderer, msg, level, puserObj);
			}

			Delegate3(tfunc pfunc, void *puserObj) : pobject(), pmethod(pfunc), puserObj(puserObj) {}
			Delegate3(void *pobject, tstub pstub, void *puserObj) : pobject(pobject), pmethod(pstub), puserObj(puserObj) {}

		public:
			Delegate3() {}

			template <class T, void (T::*TMethod)(VRayRenderer&, const char*, int, void*)>
			static Delegate3 from_method(T* pobject, void *puserObj) {
				return Delegate3(pobject, &method_stub<T, TMethod>, puserObj);
			}

			static Delegate3 from_function(tfunc pfunc, void *puserObj) {
				return Delegate3(pfunc, puserObj);
			}

			void operator()(VRayRenderer& renderer, const char *msg, int level) const {
				return pobject ? (*pmethod.pstub)(pobject, renderer, msg, level, puserObj) : (*pmethod.pfunc)(renderer, msg, level, puserObj);
			}
		} dumpMessageDelegate;

		class Delegate4 {
			typedef void (*tstub)(void* pobject, VRayRenderer& renderer, int x, int y, int width, int height, const char *host, void *userObj);
			typedef void (*tfunc)(VRayRenderer& renderer, int x, int y, int width, int height, const char *host, void *userObj);

			void *pobject;
			union tmethod {
				tstub pstub;
				tfunc pfunc;
				tmethod() : pstub() {}
				tmethod(tstub pstub) : pstub(pstub) {}
				tmethod(tfunc pfunc) : pfunc(pfunc) {}
			} pmethod;
			void *puserObj;

			template <class T, void (T::*TMethod)(VRayRenderer&, int x, int y, int width, int height, const char* host, void*)>
			static void method_stub(void* pobject, VRayRenderer& renderer, int x, int y, int width, int height, const char* host, void *puserObj) {
				T* p = static_cast<T*>(pobject);
				return (p->*TMethod)(renderer, x, y, width, height, host, puserObj);
			}

			Delegate4(tfunc pfunc, void *puserObj) : pobject(), pmethod(pfunc), puserObj(puserObj) {}
			Delegate4(void *pobject, tstub pstub, void *puserObj) : pobject(pobject), pmethod(pstub), puserObj(puserObj) {}

		public:
			Delegate4() {}

			template <class T, void (T::*TMethod)(VRayRenderer&, int, int, int, int, const char*, void*)>
			static Delegate4 from_method(T* pobject, void *puserObj) {
				return Delegate4(pobject, &method_stub<T, TMethod>, puserObj);
			}

			static Delegate4 from_function(tfunc pfunc, void *puserObj) {
				return Delegate4(pfunc, puserObj);
			}

			void operator()(VRayRenderer& renderer, int x, int y, int width, int height, const char* host) const {
				return pobject ? (*pmethod.pstub)(pobject, renderer, x, y, width, height, host, puserObj) : (*pmethod.pfunc)(renderer, x, y, width, height, host, puserObj);
			}
		} bucketInitDelegate, bucketFailedDelegate;

		class Delegate5 {
			typedef void (*tstub)(void* pobject, VRayRenderer& renderer, int x, int y, const char* host, VRayImage* img, void *userObj);
			typedef void (*tfunc)(VRayRenderer& renderer, int x, int y, const char* host, VRayImage* img, void *userObj);

			void *pobject;
			union tmethod {
				tstub pstub;
				tfunc pfunc;
				tmethod() : pstub() {}
				tmethod(tstub pstub) : pstub(pstub) {}
				tmethod(tfunc pfunc) : pfunc(pfunc) {}
			} pmethod;
			void *puserObj;

			template <class T, void (T::*TMethod)(VRayRenderer&, int x, int y, const char* host, VRayImage* img, void*)>
			static void method_stub(void* pobject, VRayRenderer& renderer, int x, int y, const char* host, VRayImage* img, void *puserObj) {
				T* p = static_cast<T*>(pobject);
				return (p->*TMethod)(renderer, x, y, host, img, puserObj);
			}

			Delegate5(tfunc pfunc, void *puserObj) : pobject(), pmethod(pfunc), puserObj(puserObj) {}
			Delegate5(void *pobject, tstub pstub, void *puserObj) : pobject(pobject), pmethod(pstub), puserObj(puserObj) {}

		public:
			Delegate5() {}

			template <class T, void (T::*TMethod)(VRayRenderer&, int x, int y, const char*, VRayImage*, void*)>
			static Delegate5 from_method(T* pobject, void *puserObj) {
				return Delegate5(pobject, &method_stub<T, TMethod>, puserObj);
			}

			static Delegate5 from_function(tfunc pfunc, void *puserObj) {
				return Delegate5(pfunc, puserObj);
			}

			void operator()(VRayRenderer& renderer, int x, int y, const char *host, VRayImage* img) const {
				return pobject ? (*pmethod.pstub)(pobject, renderer, x, y, host, img, puserObj) : (*pmethod.pfunc)(renderer, x, y, host, img, puserObj);
			}
		} bucketReadyDelegate;

		class Delegate6 {
			typedef void (*tstub)(void* pobject, VRayRenderer& renderer, const char *msg, int progress, int total, void *userObj);
			typedef void (*tfunc)(VRayRenderer& renderer, const char *msg, int progress, int total, void *userObj);

			void *pobject;
			union tmethod {
				tstub pstub;
				tfunc pfunc;
				tmethod() : pstub() {}
				tmethod(tstub pstub) : pstub(pstub) {}
				tmethod(tfunc pfunc) : pfunc(pfunc) {}
			} pmethod;
			void *puserObj;

			template <class T, void (T::*TMethod)(VRayRenderer&, const char *msg, int progress, int total, void*)>
			static void method_stub(void* pobject, VRayRenderer& renderer, const char *msg, int progress, int total, void *puserObj) {
				T* p = static_cast<T*>(pobject);
				return (p->*TMethod)(renderer, msg, progress, total, puserObj);
			}

			Delegate6(tfunc pfunc, void *puserObj) : pobject(), pmethod(pfunc), puserObj(puserObj) {}
			Delegate6(void *pobject, tstub pstub, void *puserObj) : pobject(pobject), pmethod(pstub), puserObj(puserObj) {}

		public:
			Delegate6() {}

			template <class T, void (T::*TMethod)(VRayRenderer&, const char*, int, int, void*)>
			static Delegate6 from_method(T* pobject, void *puserObj) {
				return Delegate6(pobject, &method_stub<T, TMethod>, puserObj);
			}

			static Delegate6 from_function(tfunc pfunc, void *puserObj) {
				return Delegate6(pfunc, puserObj);
			}

			void operator()(VRayRenderer& renderer, const char *msg, int progress, int total) const {
				return pobject ? (*pmethod.pstub)(pobject, renderer, msg, progress, total, puserObj) : (*pmethod.pfunc)(renderer, msg, progress, total, puserObj);
			}
		} progressDelegate;

		class Delegate7 {
			typedef void (*tstub)(void* pobject, VRayRenderer& renderer, const char *propName, void *userObj);
			typedef void (*tfunc)(VRayRenderer& renderer, const char *propName, void *userObj);

			void *pobject;
			union tmethod {
				tstub pstub;
				tfunc pfunc;
				tmethod() : pstub() {}
				tmethod(tstub pstub) : pstub(pstub) {}
				tmethod(tfunc pfunc) : pfunc(pfunc) {}
			} pmethod;
			void *puserObj;

			template <class T, void (T::*TMethod)(VRayRenderer&, const char *str, void*)>
			static void method_stub(void* pobject, VRayRenderer& renderer, const char *str, void *puserObj) {
				T* p = static_cast<T*>(pobject);
				return (p->*TMethod)(renderer, str, puserObj);
			}

			Delegate7(tfunc pfunc, void *puserObj) : pobject(), pmethod(pfunc), puserObj(puserObj) {}
			Delegate7(void *pobject, tstub pstub, void *puserObj) : pobject(pobject), pmethod(pstub), puserObj(puserObj) {}

		public:
			Delegate7() {}

			template <class T, void (T::*TMethod)(VRayRenderer&, const char*, void*)>
			static Delegate7 from_method(T* pobject, void *puserObj) {
				return Delegate7(pobject, &method_stub<T, TMethod>, puserObj);
			}

			static Delegate7 from_function(tfunc pfunc, void *puserObj) {
				return Delegate7(pfunc, puserObj);
			}

			void operator()(VRayRenderer& renderer, const char *str) const {
				return pobject ? (*pmethod.pstub)(pobject, renderer, str, puserObj) : (*pmethod.pfunc)(renderer, str, puserObj);
			}
		} renderViewChangedDelegate;

		class Delegate8 {
			typedef void(*tstub)(void* pobject, VRayRenderer& renderer, bool isRendering, void *userObj);
			typedef void(*tfunc)(VRayRenderer& renderer, bool isRendering, void *userObj);

			void *pobject;
			union tmethod {
				tstub pstub;
				tfunc pfunc;
				tmethod() : pstub() {}
				tmethod(tstub pstub) : pstub(pstub) {}
				tmethod(tfunc pfunc) : pfunc(pfunc) {}
			} pmethod;
			void *puserObj;

			template <class T, void (T::*TMethod)(VRayRenderer&, bool isRendering, void*)>
			static void method_stub(void* pobject, VRayRenderer& renderer, bool isRendering, void *puserObj) {
				T* p = static_cast<T*>(pobject);
				return (p->*TMethod)(renderer, isRendering, puserObj);
			}

			Delegate8(tfunc pfunc, void *puserObj) : pobject(), pmethod(pfunc), puserObj(puserObj) {}
			Delegate8(void *pobject, tstub pstub, void *puserObj) : pobject(pobject), pmethod(pstub), puserObj(puserObj) {}

		public:
			Delegate8() {}

			template <class T, void (T::*TMethod)(VRayRenderer&, bool isRendering, void*)>
			static Delegate8 from_method(T* pobject, void *puserObj) {
				return Delegate8(pobject, &method_stub<T, TMethod>, puserObj);
			}

			static Delegate8 from_function(tfunc pfunc, void *puserObj) {
				return Delegate8(pfunc, puserObj);
			}

			void operator()(VRayRenderer& renderer, int flags) const {
				return pobject ? (*pmethod.pstub)(pobject, renderer, flags & 1, puserObj) : (*pmethod.pfunc)(renderer, flags & 1, puserObj);
			}
		} renderLastDelegate;

		static void renderStartCallback(void* r);
		static void imageReadyCallback(void* r);
		static void rendererCloseCallback(void* r);
		static void rtImageUpdatedCallback(int, int, VRayImage* img, void* r);
		static void dumpMessageCallback(const char* msg, int level, void* r);
		static void bucketInitCallback(int x, int y, int width, int height, const char* host, void* r);
		static void bucketReadyCallback(int x, int y, int, int, const char* host, VRayImage* img, void* r);
		static void bucketFailedCallback(int x, int y, int width, int height, const char* host, void* r);
		static void sequenceStartCallback(void* r);
		static void sequenceDoneCallback(void* r);
		static void progressCallback(const char* msg, int progress, int total, void* r);
		static void renderViewChangedCallback(const char* propName, void* r);
		static void renderLastCallback(int flags, void* r);
		static void vfbClosedCallback(void* r);
		static void postEffectsUpdatedCallback(void* r);

	public:
		/// This class groups all VFB related methods
		class VFB {
			VRayRendererNative *rendererNative;
			friend class VRayRenderer;
			VFB() {}
			VFB(const VFB&);
			VFB& operator=(const VFB&);

		public:
			/// Controls whether the VFB window is shown or hidden
			/// @param show If this is false the VFB window will be hidden
			/// @param setFocus If this is false the VFB may remain under other windows and without input focus when shown
			void show(bool show, bool setFocus = true);

			/// Check if the VFB window is currently shown
			bool isShown() const;

			/// Toggles the always-on-top behavior of the VFB window (disabled by default)
			void setAlwaysOnTop(bool set);

			/// Enables camera control using mouse and keyboard from the VFB image area
			void enableInteractivity(bool enable = true);

			/// Check if camera control from the VFB is enabled
			bool isInteractivityEnabled() const;

			/// Set the top-left coordinate of the VFB window in screen coordinates
			void setPosition(int x, int y);

			/// Set the top-left coordinate and size of the VFB window in screen coordinates
			void setPositionAndSize(int x, int y, int w, int h);

			/// Get the top-left coordinate of the VFB window in screen coordinates
			bool getPositionAndSize(int& x, int& y, int& w, int& h) const;
			
			/// This can be used to save the current VFB window state so that it can be restored later with setState()
			/// @param dataSize [output] The byte size of the returned state.
			/// @param includeMain Whether to read the "main set" of state data - window position and size, button state, etc.
			/// @param includeHistory Whether to read the history state data.
			/// @return A buffer with opaque serialized state data.
			/// @note When finished release the buffer by calling delete on the returned pointer
			VFBState* getState(size_t& dataSize, bool includeMain = true, bool includeHistory = true) const;

			/// Applies the settings contained in the vfbStateData binary buffer.
			/// The data should be obtained by a previous call to getState().
			/// @param vfbStateData An opaque serialized array that originally came from vfb.getState.
			/// @param dataSize The byte size of vfbStateData.
			/// @param setMain Whether to use the "main set" of state data - window position and size, button state, etc.
			/// @param setHistory Whether to use the history state data.
			/// @note Setting a missing part of the data will be silently skipped (e.g. vfbStateData has only history state, but setMain=true)
			void setState(const void* vfbStateData, size_t dataSize, bool setMain = true, bool setHistory = true);

			/// Returns the window handle associated with the current V-Ray VFB
			/// It's actually HWND on Windows or QWidget* for Qt builds.
			/// WARNING: Use this method with extreme caution or better do not use
			/// it at all! Always use higher level methods when available.
			/// @retval Current V-Ray VFB window handle.
			void* getWindowHandle() const;

			/// Sets the parent of the current V-Ray VFB window.
			/// WARNING: Use this method with extreme caution or better do not use
			/// it at all! Always use higher level methods when available.
			/// @param hWnd The parent window handle. It's actually HWND on Windows or QWidget* for Qt builds.
			void setParentWindow(void* hWnd);

			/// Loads the global state of the color correction controls
			/// @param filename Path to the preset to be loaded including the ".vccglb" extension
			/// @retval Success or failure
			bool loadColorCorrectionsPreset(const char* filename);
			
			/// Saves the global state of the color correction controls
			/// @param filename Path to the preset to be saved. Automatically appends ".vccglb" extension
			/// @retval Success or failure
			bool saveColorCorrectionsPreset(const char* filename) const;

			/// Sets a prefix for the VFB window title
			/// @param prefix Prefix
			/// @retval true on success, false on failure
			bool setTitlePrefix(const char* prefix);
		} vfb; ///< "property" or "namespace" vfb within VRayRenderer

	private:
		LicenseError licErr;
		bool autoCommit : 1;

		void createInstance(const RendererOptions* options);

	public:
		/// Default constructor which uses default rendering options
		VRayRenderer();

		/// Constructor which uses supplied rendering options
		VRayRenderer(const RendererOptions &rendererOptions);

		/// Destructor
		~VRayRenderer();

		/// If exceptions are not enabled use this function to test whether you have a valid renderer
		/// @retval true - renderer instance is valid, false - renderer instance is invalid
		operator bool() const;

		/// Returns the last error that occurred when a method was called. Use this to understand why a method returned false.
		Error getLastError() const;

		/// Returns the license error (if any) that occurred when the renderer was constructed or when rendering was started.
		LicenseError getLicenseError() const;

		/// Returns the parser error that occurred when a scene was last loaded or appended.
		ParserError getLastParserError() const;

		/// Change the render mode to be used for the *next* rendering. If a rendering is in process it will not be affected.
		/// You can switch between Production and RT mode with this without resetting the scene.
		bool setRenderMode(RendererOptions::RenderMode mode);

		/// Get the current render mode.
		RendererOptions::RenderMode getRenderMode();

		/// Pass a new set of options. The renderer must be idle for this to work.
		bool setOptions(const RendererOptions &options);

		/// Return the currently used options
		RendererOptions getOptions();

		/// Stops any ongoing rendering and clears the scene contents. This is similar to creating a new renderer with the same options.
		bool reset();

		/// Stops any ongoing rendering and clears the scene contents. This is similar to creating a new renderer with the newly passed options.
		bool reset(const RendererOptions &options);

		/// Serializes the whole scene (plugins with their property values) contained in the renderer into a single memory stream no matter
		/// if they originate from a loaded or appended vrscene file or are created by code. The whole stream will be always transferred when
		/// DR hosts are connected. No DR hosts should have been connected before serialization.
		/// (Currently only one type of serialization is supported but other types may be added in the future.)
		/// This method is here mainly for debug purposes but it can be helpful in some other cases as well.
		/// Do not use it unless you know exactly what you're doing.
		bool serializeScene();

		/// Execute this on an empty scene to create some settings plugins and initialize some of their
		/// properties to values better than the defaults. The defaults remain as they are for compatibility reasons.
		/// This should be done before applying your own settings and before starting a render.
		/// To see what got changed, export the scene after that to a file and inspect it.
		bool setImprovedDefaultSettings();

		/// Get the currently set animation frame number. This is related to the current scene time through the SettingsOutput.frames_per_second parameter.
		int getCurrentFrame() const;

		/// Change the current animation frame number. Setting properties after this
		/// will add keyframe values for this frame if useAnimatedValues(true) was called.
		/// This is related to the current scene time through the SettingsOutput.frames_per_second parameter.
		void setCurrentFrame(int frame);

		/// Gets the current scene time. This is related to the current frame number through the SettingsOutput.frames_per_second parameter.
		double getCurrentTime() const;

		/// Sets the current scene time. This is related to the current frame number through the SettingsOutput.frames_per_second parameter.
		void setCurrentTime(double time);

		/// Removes all keyframe values at times less than 'time'
		bool clearAllPropertyValuesUpToTime(double time);

		/// When enabled, setting parameter values will add keyframe values for animation.
		/// Otherwise the old value is just overwritten. The initial state is off.
		void useAnimatedValues(bool on);

		/// Returns true if setting parameter values adds keyframes and false if it overwrites.
		bool getUseAnimatedValuesState() const;

		/// Get the current frame number while rendering a sequence.
		int getSequenceFrame() const;

		/// Returns the current bounding box of the scene
		Box getBoundingBox() const;

		/// Returns the current bounding box of the scene
		Box getBoundingBox(bool& ok) const;

		/// This method is only used when doing out-of-process rendering (RendererOptions::inProcess=false).
		/// It updates the camera plugin used on DR machines when adding/removing CameraPhysical or another specific camera.
		bool setCamera(const Plugin& plugin);

		/// Gets the current camera plugin. Don't use this unless you need to use setCamera() - see the comments on setCamera.
		Plugin getCamera() const;

		/// Returns some performance statistics from the RT engine
		RTStatistics getRTStatistics() const;

		/// Pauses RT rendering. Only sampling is stopped, so that the CPU/GPU will be idle, but everything else is the same.
		bool pause() const;

		/// Resumes RT rendering.
		bool resume() const;

		/// Returns the frame buffer width.
		int getWidth() const;

		/// Returns the frame buffer height.
		int getHeight() const;

		/// Returns the frame buffer width and height.
		bool getImageSize(int& width, int& height) const;

		/// Sets the frame buffer width.
		bool setWidth(int width, bool resetCropRegion = true, bool resetRenderRegion = true);

		/// Sets the frame buffer height.
		bool setHeight(int height, bool resetCropRegion = true, bool resetRenderRegion = true);

		/// Sets the frame buffer width and height.
		bool setImageSize(int width, int height, bool resetCropRegion = true, bool resetRenderRegion = true);

		/// Gets the rendering region within the image width and height.
		/// (width and height are the rendering sizes set by other methods or constructors, etc.)
		bool getRenderRegion(int& rgnLeft, int& rgnTop, int& rgnWidth, int& rgnHeight);

		/// Sets the rendering region within the image width and height.
		/// (width and height are the rendering sizes set by other methods or constructors, etc.)
		/// Set to an invalid size, like (0, 0, 0, 0), to reset the region and render the whole image.
		bool setRenderRegion(int rgnLeft, int rgnTop, int rgnWidth, int rgnHeight);

		/// Gets the virtual crop region (RgnLeft, RgnTop, RgnWidth, RgnHeigt) within a virtual source image space with sizes SrcWidth, SrcHeight.
		bool getCropRegion(int& srcWidth, int& srcHeight, float& rgnLeft, float& rgnTop, float& rgnWidth, float& rgnHeight);

		/// Sets a virtual crop region (rgnLeft, rgnTop, width, height) within a virtual source image space with size srcWidth, srcHeight.
		/// width and height are the rendering sizes set by other methods or constructors, etc.
		/// Set to an invalid size, like (0, 0, 0, 0), to reset the region and render the whole image.
		bool setCropRegion(int srcWidth, int srcHeight, float rgnLeft, float rgnTop);

		/// Sets a virtual crop region (rgnLeft, rgnTop, rgnWidth, rgnHeigt) within a virtual source image space with sizes srcWidth, srcHeight.
		/// The output image pixel width and height do not change.
		/// Set to an invalid size, like (0, 0, 0, 0, 0, 0), to reset the region and render the whole image.
		bool setCropRegion(int srcWidth, int srcHeight, float rgnLeft, float rgnTop, float rgnWidth, float rgnHeight);

		/// Loads the specified V-Ray scene file (.vrscene) into memory. Previous scene contents are wiped.
		/// The file can be exported from a previous session with this SDK or from V-Ray for 3dsMax, Maya, etc.
		/// @param fileName The file path to the .vrscene file which will be loaded.
		/// @retval 0 - no error; check getLastParserError() if != 0
		int load(const char* fileName) const;

		/// @overload
		int load(const std::string& fileName) const;

		/// Similar to load() but this takes as input directly the text contents of a scene file
		/// @retval 0 - no error; check getLastParserError() if != 0
		int loadAsText(const char* text) const;

		/// @overload
		int loadAsText(const std::string& text) const;

		/// Appends the specified scene file data to the currently loaded scene without wiping it.
		/// @param fileName The file path to the .vrscene file which will be appended.
		/// @retval 0 - no error; check getLastParserError() if != 0
		int append(const char* fileName) const;

		/// @overload
		int append(const std::string& fileName) const;

		/// Similar to append() but this takes as input directly the text contents of a scene file
		/// @retval 0 - no error; check getLastParserError() if != 0
		int appendAsText(const char* text) const;

		/// @overload
		int appendAsText(const std::string& text) const;

		/// Begins the rendering process in a separate thread. A non-blocking call.
		/// Scene changes and DR host changes are forbidden until the RenderStarted event.
		void start() const;

		/// Begins the rendering process in a separate thread. Blocks until the preparation stage is complete and rendering has actually started.
		void startSync() const;

		/// Begins rendering an animation sequence in a separate thread. A non-blocking call.
		void renderSequence() const;

		/// Begins rendering an animation sequence in a separate thread. A non-blocking call.
		/// @param descriptions Frame ranges to render
		/// @param count The number of elements in descriptions
		void renderSequence(SubSequenceDesc descriptions[], size_t count) const;

		/// When rendering a sequence this starts the next frame after the previous one has completed. It does not continue automatically.
		void continueSequence() const;

		/// Flags the image rendering thread to stop and waits for it to join.
		void stop() const;

		/// Exports the current scene contents to the specified file. The exporter
		/// serializes all plugins in text format.
		/// @param filePath The path to the file where the scene will be exported. The
		/// file path must contain the name and extension of the destination file.
		/// @retval 0 - no error
		int exportScene(const char* filePath) const;

		/// @overload
		int exportScene(const std::string& filePath) const;

		/// Exports the current scene contents to the specified file. The exporter
		/// serializes all plugins in text format.
		/// @param filePath The path to the file where the scene will be exported. The
		/// file path must contain the name and extension of the destination file.
		/// @param settings Additional options such as compression and file splitting
		/// @retval 0 - no error
		int exportScene(const char* filePath, const VRayExportSettings& settings) const;

		/// @overload
		int exportScene(const std::string& filePath, const VRayExportSettings& settings) const;

		/// When using the bucket sampler in production mode this sets a "focus point" for
		/// the follow mouse cursor feature of the VFB. This API allows you to do this
		/// without the VFB. Buckets are generated in a circle outward from the point.
		/// @param x The x-coordinate of the focus point. If set to negative this will reset the "follow cursor" behavior and continue with the normal bucket generator.
		/// @param y The y-coordinate of the focus point. If set to negative this will reset the "follow cursor" behavior and continue with the normal bucket generator.
		void bucketRenderNearest(int x, int y) const;

		/// Adds hosts to the list for distributed rendering. If rendering is already in process, they are connected directly.
		/// @param hosts IP:port (or hostname:port) pairs as string delimited by ',' or ';'
		/// @retval 0 - no error
		int addHosts(const char* hosts) const;

		/// @overload
		int addHosts(const std::string& hosts) const;

		/// Disconnects and removes hosts from the distributed rendering host lists.
		/// @param hosts IP:port (or hostname:port) pairs as string delimited by ',' or ';'
		/// @retval 0 - no error
		int removeHosts(const char* hosts) const;

		/// @overload
		int removeHosts(const std::string& hosts) const;

		/// Resets the hosts list for distributed rendering. If rendering is in process, hosts that are not on the new list are disconnected.
		/// @param hosts IP:port (or hostname:port) pairs as string delimited by ',' or ';'
		/// @retval 0 - no error
		int resetHosts(const char* hosts = NULL) const;

		/// @overload
		int resetHosts(const std::string& hosts) const;

		/// Returns all hosts set for distributed rendering.
		/// @retval semicolon separated string of IP:port (or hostname:port) pairs
		std::string getAllHosts() const;

		/// Returns all active hosts during distributed rendering.
		/// @retval semicolon separated string of IP:port (or hostname:port) pairs
		std::string getActiveHosts() const;

		/// Returns all inactive hosts during distributed rendering.
		/// @retval semicolon separated string of IP:port (or hostname:port) pairs
		std::string getInactiveHosts() const;

		/// Commits any accumulated scene changes. This is necessary if the autoCommit flag is set to false.
		/// @param force If true, immediate flush commit is forced. Generally this should not be set to true.
		/// @retval true if the changes were sucessfully committed, false otherwise.
		bool commit(bool force = false);

		/// Returns a pointer to a copy of the image contained in the renderer image buffer at this moment.
		/// The user is responsible for deleting it.
		VRayImage* getImage() const;

		/// Returns a pointer to a copy of the image contained in the renderer image buffer at this moment.
		/// The user is responsible for deleting it.
		/// @param options Use to modify behavior compared to getImage(void) - crop, color correct, etc.
		VRayImage* getImage(const GetImageOptions &options) const;

		/// Save the current image buffer as if you used the save button on the VFB. Format is deduced from the filename extension.
		/// @retval True on success
		bool saveImage(const char* fileName) const;

		/// @overload
		bool saveImage(const std::string& fileName) const;

		/// Save the current image buffer as if you used the save button on the VFB. Format is deduced from the filename extension.
		/// @param fileName Output file name
		/// @param options Additional options for writing
		bool saveImage(const char* fileName, const ImageWriterOptions& options) const;

		/// @overload
		bool saveImage(const std::string& fileName, const ImageWriterOptions& options) const;

		/// Returns the current state of the renderer.
		/// @note The state could change asynchronously as rendering occurs in another thread.
		/// @see RendererState
		RendererState getState() const;

		/// Checks if rendering is complete. If it ended in failure or got cancelled, this will still return true.
		/// Notice that an intermediate image may be available from getImage even if this returns false (because rendering is ongoing).
		/// @remarks It is recommended to use waitForImageReady instead of polling this in a loop.
		bool isImageReady() const;

		/// Returns if the rendering has been aborted.
		/// @retval true if the user has clicked on the Stop button in VFB, false otherwise.
		bool isAborted() const;

		/// Gets the status of the sequence rendering process. This is not meaningful if you're not rendering a sequence.
		/// @retval true if the whole sequence has been rendered, false otherwise.
		bool isSequenceDone() const;

		/// Waits for rendering to finish, regardless of the outcome (success, cancel, error).
		/// If the keepRTRunning flag is set, this event may occur without ending the render session.
		/// @retval true if image has been rendered, false if an error occurred.
		bool waitForImageReady() const;

		/// Waits for rendering to finish but no more than the specified timeout.
		/// If the keepRTRunning flag is set, this event may occur without ending the render session.
		/// @param timeout in milliseconds
		/// @retval true if image has been rendered, false if timeout or error occurred.
		bool waitForImageReady(const int timeout) const;

		/// Waits for rendering of the sequence to finish, regardless of the outcome (success, cancel, error).
		/// @retval true if rendering of the sequence has finished successfully, false if an error occurred.
		bool waitForSequenceDone() const;

		/// Waits for rendering of the sequence to finish but no more than the specified timeout.
		/// @param timeout in milliseconds
		/// @retval true if rendering of the sequence has finished, false if timeout or error occurred.
		bool waitForSequenceDone(const int timeout) const;

		/// Returns the names of all V-Ray plugin instances existing in the current scene.
		std::vector<std::string> getPluginNames() const;

		/// Returns all plugin instances in the current scene.
		std::vector<Plugin> getPlugins() const;

		/// Returns all plugin instances of a given type T in the current scene.
		template <class T> std::vector<T> getPlugins() const;

		/// @overload
		std::vector<Plugin> getPlugins(const char* pluginClassName) const;

		/// @overload
		std::vector<Plugin> getPlugins(const std::string& pluginClassName) const;

		/// Returns the class names of all available V-Ray plugin classes loaded from dynamic libraries.
		std::vector<std::string> getAvailablePlugins() const;

		/// Returns the (last) instance of a given plugin type if available.
		/// Should be used for singleton plugins. Settings plugins are singletons.
		/// @param pluginType The plugin class name.
		/// @retval The plugin instance which may be invalid if one does not exist.
		Plugin getInstanceOf(const char* pluginType) const;

		/// @overload
		Plugin getInstanceOf(const std::string& pluginType) const;

		/// @overload
		template<class T> T getInstanceOf() const;

		/// Returns the (last) instance of a given plugin type or creates
		/// a new instance if not available.
		/// Should be used for singleton plugins. Settings plugins are singletons.
		/// Use getOrCreatePlugin() for other plugin types.
		/// @param pluginType The plugin class name.
		/// @retval The plugin instance which may be invalid if one does not exist.
		Plugin getInstanceOrCreate(const char* pluginType);

		/// @overload
		Plugin getInstanceOrCreate(const std::string& pluginType);

		/// @overload
		template<class T> T getInstanceOrCreate();

		/// Returns a plugin instance of the given type if available or creates
		/// a new one with the given name and of the given type if not.
		/// Should be used for singleton plugins. Settings plugins are singletons.
		/// Use getOrCreatePlugin() for other plugin types.
		/// @param pluginName The plugin instance name if it has to be created.
		/// @param pluginType The plugin class name.
		/// @retval The plugin instance which may be invalid if one does not exist.
		Plugin getInstanceOrCreate(const char* pluginName, const char* pluginType);

		/// @overload
		Plugin getInstanceOrCreate(const std::string& pluginName, const char* pluginType);

		/// @overload
		Plugin getInstanceOrCreate(const char* pluginName, const std::string& pluginType);

		/// @overload
		Plugin getInstanceOrCreate(const std::string& pluginName, const std::string& pluginType);

		/// @overload
		template<class T> T getInstanceOrCreate(const char* pluginName);

		/// @overload
		template<class T> T getInstanceOrCreate(const std::string& pluginName);

		/// Set callback function for the event when rendering has started after the preparation stage.
		void setOnRenderStart(void(*callback)(VRayRenderer&, void*), const void* userData = NULL);

		/// Set callback method for the event when rendering has started after the preparation stage.
		template<class T, void (T::*TMethod)(VRayRenderer&, void*)>
		void setOnRenderStart(T& cls, const void* userData = NULL);

		/// Set callback function for the event when rendering has finished, successfully or not.
		void setOnImageReady(void(*callback)(VRayRenderer&, void*), const void* userData = NULL);

		/// Set callback method for the event when rendering has finished, successfully or not.
		template<class T, void (T::*TMethod)(VRayRenderer&, void*)>
		void setOnImageReady(T& cls, const void* userData = NULL);

		/// Set callback function for the event when the VRayRenderer object is destroyed.
		void setOnRendererClose(void(*callback)(VRayRenderer&, void*), const void* userData = NULL);

		/// Set callback method for the event when the VRayRenderer object is destroyed.
		template<class T, void (T::*TMethod)(VRayRenderer&, void*)>
		void setOnRendererClose(T& cls, const void* userData = NULL);

		/// Set callback function for the event when an updated image is available in progressive rendering mode.
		void setOnRTImageUpdated(void(*callback)(VRayRenderer&, VRayImage* img, void*), const void* userData = NULL);

		/// Set callback method for the event when an updated image is available in progressive rendering mode.
		template<class T, void (T::*TMethod)(VRayRenderer&, VRayImage* img, void*)>
		void setOnRTImageUpdated(T& cls, const void* userData = NULL);

		/// Set callback function for the event when V-Ray logs a text message. See enum MessageLevel.
		void setOnDumpMessage(void(*callback)(VRayRenderer&, const char* msg, int level, void*), const void* userData = NULL);

		/// Set callback method for the event when V-Ray logs a text message. See enum MessageLevel.
		template<class T, void (T::*TMethod)(VRayRenderer&, const char* msg, int level, void*)>
		void setOnDumpMessage(T& cls, const void* userData = NULL);

		/// Set callback function for the event when an image bucket has been assigned and started rendering.
		void setOnBucketInit(void(*callback)(VRayRenderer&, int x, int y, int width, int height, const char* host, void*), const void* userData = NULL);

		/// Set callback method for the event when an image bucket has been assigned and started rendering.
		template<class T, void (T::*TMethod)(VRayRenderer&, int x, int y, int width, int height, const char* host, void*)>
		void setOnBucketInit(T& cls, const void* userData = NULL);

		/// Set callback function for the event when an image bucket is completed.
		void setOnBucketReady(void(*callback)(VRayRenderer&, int x, int y, const char* host, VRayImage* img, void*), const void* userData = NULL);

		/// Set callback method for the event when an image bucket is completed.
		template<class T, void (T::*TMethod)(VRayRenderer&, int x, int y, const char* host, VRayImage* img, void*)>
		void setOnBucketReady(T& cls, const void* userData = NULL);

		/// Set callback function for the event when an image bucket fails to render. This usually happens when a DR host drops.
		void setOnBucketFailed(void(*callback)(VRayRenderer&, int x, int y, int width, int height, const char* host, void*), const void* userData = NULL);

		/// Set callback method for the event when an image bucket fails to render. This usually happens when a DR host drops.
		template<class T, void (T::*TMethod)(VRayRenderer&, int x, int y, int width, int height, const char* host, void*)>
		void setOnBucketFailed(T& cls, const void* userData = NULL);

		/// Set callback function for the event when an animation sequence has started rendering after the initial preparation stage.
		void setOnSequenceStart(void(*callback)(VRayRenderer&, void*), const void* userData = NULL);

		/// Set callback method for the event when an animation sequence has started rendering after the initial preparation stage.
		template<class T, void (T::*TMethod)(VRayRenderer&, void*)>
		void setOnSequenceStart(T& cls, const void* userData = NULL);

		/// Set callback function for the event when an animation sequence has finished rendering.
		void setOnSequenceDone(void(*callback)(VRayRenderer&, void*), const void* userData = NULL);

		/// Set callback method for the event when an animation sequence has finished rendering.
		template<class T, void (T::*TMethod)(VRayRenderer&, void*)>
		void setOnSequenceDone(T& cls, const void* userData = NULL);

		/// Set callback function for the event when V-Ray updates its current computation task and the number of workunits done.
		/// This is usually used to implement progress bars.
		void setOnProgress(void(*callback)(VRayRenderer&, const char* msg, int elementNumber, int elementsCount, void*), const void* userData = NULL);

		/// Set callback method for the event when V-Ray updates its current computation task and the number of workunits done.
		/// This is usually used to implement progress bars.
		template<class T, void (T::*TMethod)(VRayRenderer&, const char* msg, int elementNumber, int elementsCount, void*)>
		void setOnProgress(T& cls, const void* userData = NULL);

		/// Set callback function for the event when the RenderView (camera) plugin has changed in RT mode.
		void setOnRenderViewChanged(void(*callback)(VRayRenderer&, const char* propName, void*), const void* userData = NULL);

		/// Set callback method for the event when the RenderView (camera) plugin has changed in RT mode.
		template<class T, void (T::*TMethod)(VRayRenderer&, const char* propName, void*)>
		void setOnRenderViewChanged(T& cls, const void* userData = NULL);

		/// Set callback function for the event when the "render last" VFB button is pressed.
		void setOnRenderLast(void(*callback)(VRayRenderer&, bool isRendering, void*), const void* userData = NULL);

		/// Set callback method for the event when the "render last" VFB button is pressed.
		template<class T, void (T::*TMethod)(VRayRenderer&, bool isRendering, void*)>
		void setOnRenderLast(T& cls, const void* userData = NULL);

		/// Set callback function for the event when the VFB window is closed.
		void setOnVFBClosed(void(*callback)(VRayRenderer&, void*), const void* userData = NULL);
		
		/// Set callback method for the event when the VFB window is closed.
		template<class T, void (T::*TMethod)(VRayRenderer&, void*)>
		void setOnVFBClosed(T& cls, const void* userData = NULL);

		/// Set callback function for the event when a new post-processed (denoised, lens effects) image is available.
		void setOnPostEffectsUpdated(void(*callback)(VRayRenderer&, void*), const void* userData = NULL);

		/// Set callback function for the event when a new post-processed (denoised, lens effects) image is available.
		template<class T, void (T::*TMethod)(VRayRenderer&, void*)>
		void setOnPostEffectsUpdated(T& cls, const void* userData = NULL);

		/// Sets the difference threshold for emitting an RTImageUpdated event.
		/// If V-Ray produces an image below the threshold, the event will not be emitted.
		/// The initial threshold value is 2.0.
		float setRTImageUpdateDifference(float difference);

		/// Set the maximum time before emitting an RTImageUpdated event.
		/// If the timeout is reached, an image will be emitted even if it's the same as the previous update.
		/// @retval The previous timeout value
		unsigned long setRTImageUpdateTimeout(unsigned long timeout);

		/// This method allows you to change the default behavior of keeping an image copy in a RTImageUpdated callback parameter
		void setKeepRTframesInCallback(bool keep);

		/// This method allows you to change the default behavior of keeping an image copy in a BucketReady callback parameter
		void setKeepBucketsInCallback(bool keep);

		/// Search for a plugin instance with the given name.
		/// @retval Invalid Plugin object if not found.
		Plugin getPlugin(const char *pluginName) const;

		/// @overload
		Plugin getPlugin(const std::string &pluginName) const;

		/// @overload
		template<class T> T getPlugin(const char *pluginName) const;

		/// @overload
		template<class T> T getPlugin(const std::string &pluginName) const;

		/// Return static meta information about a given plugin type
		PluginMeta getPluginMeta(const std::string &pluginClassName) const;

		/// Creates an instance of a specific Plugin type with the given instance name.
		/// @param pluginName The plugin name.
		/// @param pluginType The plugin class name.
		/// @retval The new plugin instance (an invalid Plugin on error).
		Plugin newPlugin(const char* pluginName, const char* pluginType);

		/// @overload
		Plugin newPlugin(const std::string& pluginName, const std::string& pluginType);

		/// @overload
		Plugin newPlugin(const char* pluginName, const std::string& pluginType);

		/// @overload
		Plugin newPlugin(const std::string& pluginName, const char* pluginType);

		/// @overload
		template<class T> T newPlugin(const char* pluginName);

		/// @overload
		template<class T> T newPlugin(const std::string& pluginName);

		/// Creates an instance of a specific Plugin type with an autogenerated instance name.
		/// @param pluginType The plugin class name.
		/// @retval The new plugin instance (an invalid Plugin on error).
		Plugin newPlugin(const char* pluginType);

		/// @overload
		Plugin newPlugin(const std::string &pluginType);

		/// @overload
		template<class T> T newPlugin();

		/// Returns a plugin instance with the given instance name and of the given type.
		/// A new instance is created if one does not exist with the given name.
		/// For singleton plugins like the settings plugins use getInstanceOrCreate().
		/// @param pluginName The plugin instance name.
		/// @param pluginType The plugin class name.
		/// @retval The plugin instance which should not be invalid unless the plugin type is not available.
		Plugin getOrCreatePlugin(const char* pluginName, const char* pluginType);

		/// @overload
		Plugin getOrCreatePlugin(const std::string& pluginName, const char* pluginType);

		/// @overload
		Plugin getOrCreatePlugin(const char* pluginName, const std::string& pluginType);

		/// @overload
		Plugin getOrCreatePlugin(const std::string& pluginName, const std::string& pluginType);

		/// @overload
		template<class T> T getOrCreatePlugin(const char* pluginName);

		/// Returns the first scene Node intersected by tracing a camera ray from image space coordinates (x;y), if any
		/// @param x Image space X
		/// @param y Image space Y
		Plugin pickPlugin(int x, int y) const;

		/// Returns the first scene Node intersected by tracing a camera ray from image space coordinates (x;y), if any
		/// @param x Image space X
		/// @param y Image space Y
		/// @param timeout Maximum milliseconds to spend before returning a result
		Plugin pickPlugin(int x, int y, int timeout) const;

		/// Returns a list of scene Node intersected by tracing a camera ray from image space coordinates (x;y) ordered by depth
		/// @param x Image space X
		/// @param y Image space Y
		/// @param maxcount Maximum number of nodes to include in the list
		/// @param timeout Maximum milliseconds to spend before returning a result
		std::vector<PluginDistancePair> pickPlugins(double x, double y, int maxcount = 0, int timeout = -1) const;

		/// Remove the plugin from the scene. The Plugin object becomes invalid if the call is successful.
		/// @retval true if removal was successful
		bool removePlugin(const Plugin& plugin);

		/// Remove the plugin from the scene. Plugin object handles becomes invalid if the call is successful.
		/// @retval true if removal was successful
		bool removePlugin(const std::string& pluginName);

		/// Remove the plugin from the scene. Plugin object handles becomes invalid if the call is successful.
		/// @retval true if removal was successful
		bool removePlugin(const char* pluginName);

		/// Replaces all references (in the scene) of oldPlugin with newPlugin.
		/// @param oldPlugin
		///                  The plugin whose references are to be replaced.
		/// @param newPlugin
		///                  The plugin which will replace the old one.
		///                  This can be an empty Plugin() in which case all references to oldPlugin will be removed.
		/// @retval true on success, flase on failure
		/// @remark The oldPlugin does not get removed from the scene by this call
		bool replacePlugin(const Plugin& oldPlugin, const Plugin& newPlugin);

		/// Check if a plugin with the given instance name exists in the current scene
		/// @retval bool
		bool pluginExists(const char* pluginName) const;

		/// @overload
		bool pluginExists(const std::string& pluginName) const;

		/// Get the current state of the autoCommit flag
		bool getAutoCommit() const;

		/// Change the autoCommit state of the renderer. When true, every parameter change is applied immediately.
		/// Otherwise you have to call commit() yourself to apply changes. This allows you to batch changes together for efficiency.
		void setAutoCommit(bool autoCommit);

		/// Returns an object that groups render element management methods
		RenderElements getRenderElements();

		/// @param proxyPlugin A GeomMeshFile plugin instance to read the file path and parameters from.
		/// @param vertices Output vertex preview data. The array can be passed on directly to DirectX/OpenGL etc. (and the indices too)
		/// @param indices Output face index data. Each three consecutive elements in make up one triangle.
		/// @param countHint An optional number of triangles to return. The exact number of elements in 'vertices' may differ slightly.
		/// If countHint is not specified, proxyPlugin::num_preview_faces is used.
		bool readProxyPreviewGeometry(const Plugin &proxyPlugin, VectorList &vertices, IntList &indices, int countHint = 0) const;
		
		/// @param proxyPlugin A GeomMeshFile plugin instance to read the file path and parameters from.
		/// @param vertices Output vertex preview data. Consecutive vertices form hair strands. The i-th strand has lengths[i] consecutive vertices.
		/// @param lengths Output hair strand length data. Each strand can have a different number of vertices.
		/// @param countHint The number of hair strands to return. The exact number of elements in 'lengths' may differ slightly.
		/// If countHint is not specified, proxyPlugin::num_preview_faces is used.
		bool readProxyPreviewHair(const Plugin &proxyPlugin, VectorList &vertices, IntList &lengths, int countHint=0) const;
		
		/// @param proxyPlugin A GeomMeshFile plugin instance to read the file path and parameters from.
		/// @param positions Output particle preview data. The positions of the preview subset of all particles.
		/// @param countHint An optional number of particles to return. The exact number of elements in 'positions' may differ slightly.
		/// If countHint is not specified, proxyPlugin::num_preview_faces is used.
		bool readProxyPreviewParticles(const Plugin &proxyPlugin, VectorList &positions, int countHint=0) const;

		/// @param proxyPlugin A GeomMeshFile plugin instance to read the file path and parameters from.
		/// @param readData A structure defining which parts of the data to read (with .setFlags()) and holding the resulting arrays.
		/// @return false if an error ocurred. Check getLastError() for additional info.
		bool readProxyFullData(const Plugin &proxyPlugin, ProxyReadData &readData) const;

		/// @param geomStaticMeshPlugin A GeomStaticMesh instance supplying the data for the proxy file
		/// @param transform A transformation to bake into the geometry data. Pass NULL to skip.
		/// @param params A ProxyCreateParams container for the various parameters, including file name
		/// @return false if an error ocurred. Check getLastError() for additional info.
		bool createProxyFileMesh(const Plugin &geomStaticMeshPlugin, const Transform *transform, const ProxyCreateParams &params);
		
		/// @param geomStaticMeshPlugins An array of GeomStaticMesh instances to combine in the proxy file as one mesh
		/// @param transforms Transformations for the separate input meshes to bake into the final mesh. Size must equal the mesh array size. Pass NULL to skip.
		/// @param params A ProxyCreateParams container for the various parameters, including file name
		/// @return false if an error ocurred. Check getLastError() for additional info. Will return false without "last error" set if a transform array of mismatching size is passed.
		bool createCombinedProxyFileMesh(const std::vector<Plugin> &geomStaticMeshPlugins, const std::vector<Transform> *transforms, const ProxyCreateParams &params);

		/// @return The size in bytes of the irradiance map in memory. This is 0 if no IM is available.
		size_t getIrradianceMapSize();
		/// @param fileName Destination irradiance map file
		/// @return false if an error ocurred or there is no data to save. Check getLastError() for additional info.
		bool saveIrradianceMapFile(const std::string &fileName);
		/// @overload
		bool saveIrradianceMapFile(const char* fileName);

		/// @return The size in bytes of the light cache in memory. This is 0 if no LC is available.
		size_t getLightCacheSize();
		/// @param fileName Destination light cache file
		/// @return false if an error ocurred or there is no data to save. Check getLastError() for additional info.
		bool saveLightCacheFile(const std::string &fileName);
		/// @overload
		bool saveLightCacheFile(const char* fileName);

		/// @return The size in bytes of the global photon map in memory. This is 0 if no PM is available.
		size_t getPhotonMapSize();
		/// @param fileName Destination global photon map file
		/// @return false if an error ocurred or there is no data to save. Check getLastError() for additional info.
		bool savePhotonMapFile(const std::string &fileName);
		/// @overload
		bool savePhotonMapFile(const char* fileName);

		/// @return The size in bytes of the caustics photon map in memory. This is 0 if no data is available.
		size_t getCausticsSize();
		/// @param fileName Destination caustics photon map file
		/// @return false if an error ocurred or there is no data to save. Check getLastError() for additional info.
		bool saveCausticsFile(const std::string &fileName);
		/// @overload
		bool saveCausticsFile(const char* fileName);

		/// Returns a list of all devices available for use by the CUDA engine.
		/// @return If the list is empty, check getLastError() for failure.
		/// @see struct ComputeDeviceInfo
		std::vector<ComputeDeviceInfo> getComputeDevicesCUDA();

		/// Returns a list of all devices available for use by the OpenCL engine.
		/// @return If the list is empty, check getLastError() for failure.
		/// @see struct ComputeDeviceInfo
		std::vector<ComputeDeviceInfo> getComputeDevicesOpenCL();

		/// Returns a list of all devices available for use by the current engine (CUDA or OpenCL).
		/// @return If the list is empty, check getLastError() for failure. The list will be empty without error if the current render mode is CPU.
		/// @see struct ComputeDeviceInfo
		std::vector<ComputeDeviceInfo> getComputeDevicesCurrentEngine();

		/// Set which devices to use for rendering with the CUDA engine.
		/// @param indices The indices of the devices to use as returned by getComputeDevicesCUDA
		/// @return false if an error occured. Check getLastError() for additional info.
		bool setComputeDevicesCUDA(const std::vector<int> &indices);

		/// Set which devices to use for rendering with the OpenCL engine.
		/// @param indices The indices of the devices to use as returned by getComputeDevicesOpenCL
		/// @return false if an error occured. Check getLastError() for additional info.
		bool setComputeDevicesOpenCL(const std::vector<int> &indices);

		/// Set which devices to use for rendering with the current engine (CUDA or OpenCL).
		/// @param indices The indices of the devices to use as returned by getComputeDevicesCurrentEngine
		/// @return false if an error occured. Check getLastError() for additional info. This will fail without error if the current render mode is CPU.
		bool setComputeDevicesCurrentEngine(const std::vector<int> &indices);

		/// Use this to enable resumable rendering before starting a render.
		/// @param enable Whether to enable or disable for this VRayRenderer
		/// @param options optional additional settings. Passing NULL is valid.
		/// @return false if an error occured. Check getLastError() for additional info.
		bool setResumableRendering(bool enable, const ResumableRenderingOptions *options);
		
		/// Explicitly starts the denoiser. Requires a RenderChannelDenoiser instance in the scene.
		/// This can be run while rendering is ongoing or when it's paused or after it is stopped.
		/// Denoising parameters are taken from the RenderChannelDenoiser plugin.
		/// The denoising is run asynchronously, so this function will return as soon as the thread is started.
		/// When denoising is done the postEffectsUpdated callback will be called.
		/// @return true if the denoising thread is started successfully and false if there is a problem with the parameters or renderer state
		bool denoiseNow();

	protected:
		VRayRendererNative* getNativeRenderer() const;

		InstanceId getPluginId_internal(const char* pluginName) const;
		Plugin getPlugin_internal(InstanceId pluginID) const;
		template<class T> T getPlugin_internal(InstanceId pluginID) const;
		const char* getPluginName_internal(InstanceId pluginID) const;
		bool pluginExists_internal(InstanceId pluginID) const;

		static std::vector<InstanceId> getPluginIdsOfType(VRayRendererNative *rendererNative, const char* pluginType = NULL);
		template<class T> static std::vector<T> getPluginsOfType(VRayRenderer &renderer, const char* pluginType);
		static std::vector<std::string> getStringVector(void *pvector);
		std::vector<std::string> getPluginPropertyNames(const char* pluginClassName) const; // Returns the names of all properties of a plugin.
		const char* getPluginType(const char* name) const;
		const char* getPluginType(InstanceId pluginID) const;
		bool setPluginName(InstanceId pluginID, const char* name) const;
		Type getPluginPropertyRuntimeType(const char* pluginName, const char* propertyName) const;
		Type getPluginPropertyRuntimeType(InstanceId pluginID, const char* propertyName) const;
		int getPluginPropertyElementsCount(InstanceId pluginID, const char* propertyName) const;
		const void* getPluginPropertyDefinition(const char* pluginName, const char* propertyName) const;
		const void* getPluginPropertyDefinition(InstanceId pluginID, const char* propertyName) const;
		const void* getPluginPropertyDefinitionFromClass(const char* pluginClassName, const char* propertyName) const;
		std::string getPluginPropertyValueAsString(const char* pluginName, const char* propertyName) const;
		std::string getPluginPropertyValueAsString(InstanceId pluginID, const char* propertyName, bool& ok) const;
		bool setPluginPropertyValueAsString(InstanceId pluginID, const char* propertyName, const char* value) const;
		bool setPluginPropertyValueAsStringAtTime(InstanceId pluginID, const char* propertyName, const char* value, double time) const;
		bool getPluginPropertyBool(const char* pluginName, const char* propertyName, bool& ok) const;
		bool getPluginPropertyBool(InstanceId pluginID, const char* propertyName, bool& ok) const;
		int getPluginPropertyInt(const char* pluginName, const char* propertyName, bool& ok) const;
		int getPluginPropertyInt(InstanceId pluginID, const char* propertyName, bool& ok) const;
		float getPluginPropertyFloat(const char* pluginName, const char* propertyName, bool& ok) const;
		float getPluginPropertyFloat(InstanceId pluginID, const char* propertyName, bool& ok) const;
		Color getPluginPropertyColor(const char* pluginName, const char* propertyName, bool& ok) const;
		Color getPluginPropertyColor(InstanceId pluginID, const char* propertyName, bool& ok) const;
		AColor getPluginPropertyAColor(const char* pluginName, const char* propertyName, bool& ok) const;
		AColor getPluginPropertyAColor(InstanceId pluginID, const char* propertyName, bool& ok) const;
		Vector getPluginPropertyVector(const char* pluginName, const char* propertyName, bool& ok) const;
		Vector getPluginPropertyVector(InstanceId pluginID, const char* propertyName, bool& ok) const;
		Matrix getPluginPropertyMatrix(const char* pluginName, const char* propertyName, bool& ok) const;
		Matrix getPluginPropertyMatrix(InstanceId pluginID, const char* propertyName, bool& ok) const;
		Transform getPluginPropertyTransform(const char* pluginName, const char* propertyName, bool& ok) const;
		Transform getPluginPropertyTransform(InstanceId pluginID, const char* propertyName, bool& ok) const;
		Plugin getPluginPropertyPluginProperty(InstanceId pluginID, const char* propertyName, const char** outPropertyName, bool& ok) const;
		Value getPluginPropertyValue(InstanceId pluginID, const char* propertyName, bool& ok) const;
		template<typename T> T getPluginPropertyTyped(InstanceId pluginID, const char* propertyName, bool& ok) const;
		bool isPropertyAnimated(InstanceId pluginID, const char* propertyName) const;
		bool setPluginPropertyValue(InstanceId pluginID, const char* propertyName, const void *pval) const;
		bool setPluginPropertyValueAtTime(InstanceId pluginID, const char* propertyName, const void *pval, double time) const;
		bool setPluginPropertyValueRaw(InstanceId pluginID, const char* propertyName, const void *pval, size_t count) const;
		bool setPluginPropertyValueRawAtTime(InstanceId pluginID, const char* propertyName, const void *pval, size_t count, double time) const;

#ifdef VRAY_SDK_INTEROPERABILITY
		bool setPluginPropertyValueSDKInterop(InstanceId pluginID, const char* propertyName, const void *pval, int type) const;
		bool setPluginPropertyValueAtTimeSDKInterop(InstanceId pluginID, const char* propertyName, const void *pval, int type, double time) const;
#endif // VRAY_SDK_INTEROPERABILITY

		Plugin newRenderElementPlugin(int type, const char *instanceName, const char *displayName);
		Plugin getRenderElementPlugin(int type) const;
		std::vector<Plugin> getRenderElementPluginsList(int type) const;
		bool getRenderElementInfo(InstanceId pluginID, RenderElement::Info& chInfo) const;
		size_t getRenderElementData(InstanceId pluginID, InstanceId alphaPluginID, RenderElement::PixelFormat format, bool rgbOrder, const ImageRegion* rgn, void** buf) const;
		VRayImage* getRenderElementAsImage(InstanceId pluginID, const ImageRegion* rgn) const;
	};

	/// Static meta information about a plugin property
	class PropertyMeta {
		friend class PluginMeta;

	protected:
		std::string name;
		const void *pParamDef;

		PropertyMeta(const VRayRenderer *pRenderer, const char* pluginClassName, const std::string &propertyName) : name(propertyName) {
			pParamDef = pRenderer->getPluginPropertyDefinitionFromClass(pluginClassName, propertyName.c_str());
		}

	public:
		PropertyMeta(const PropertyMeta &propertyMeta) : name(propertyMeta.name), pParamDef(propertyMeta.pParamDef) {
		}

		PropertyMeta& operator=(const PropertyMeta &propertyMeta) {
			name = propertyMeta.name;
			pParamDef = propertyMeta.pParamDef;
			return *this;
		}

		void swap(PropertyMeta &propertyMeta) {
			name.swap(propertyMeta.name);
			std::swap(pParamDef, propertyMeta.pParamDef);
		}

		operator bool () const {
			return !!pParamDef;
		}

		/// Returns the number of elements for the property if it is a list and -1 otherwise.
		int getElementsCount() const {
			return Internal::VRay_PluginUtils_getPropertyElementsCount(pParamDef);
		}

		/// Returns the name of this property.
		std::string getName() const {
			return name;
		}

		/// Returns the type of the property value by definition.
		/// The runtime type may be slightly different for texture objects. They can be Plugin references or plain Color, Vector, etc. values.
		Type getType() const {
			return Internal::VRay_PluginUtils_getPropertyType(pParamDef);
		}

		/// Returns the default value of this property.
		std::string getDefaultValue() const {
			char* const value = Internal::VRay_PluginUtils_getPropertyDefaultValue(pParamDef);
			std::string str(value ? value : "");
			Internal::C_memory_free(value);
			return str;
		}

		/// Returns a short description of this property. May be empty.
		std::string getDescription() const {
			const char* const description = Internal::VRay_PluginUtils_getPropertyDescription(pParamDef);
			return description ? description : "";
		}

		/// Returns a short description of what the name of this property in UI should be, min and max values, etc.
		/// This information is not available for many properties.
		std::string getUIGuides() const {
			const char* const uiGuides = Internal::VRay_PluginUtils_getPropertyUIGuides(pParamDef);
			return uiGuides ? uiGuides : "";
		}

		static std::string typeToString(const Type type, int count = -1);

		std::string getTypeAsString() const {
			return typeToString(getType(), getElementsCount());
		}
	};

	/// Extends PropertyMeta with type information only available at runtime for a specific property instance
	class PropertyRuntimeMeta : public PropertyMeta {
		friend class Plugin;

		const VRayRenderer* renderer;
		InstanceId id; 

	protected:
		PropertyRuntimeMeta(const Plugin& plugin, const std::string& propertyName)
			: PropertyMeta(plugin.pRenderer, plugin.getType(), propertyName), renderer(plugin.pRenderer), id(plugin.id) {
		}

	public:
		PropertyRuntimeMeta(const PropertyRuntimeMeta& propertyRuntimeMeta)
			: PropertyMeta(propertyRuntimeMeta), renderer(propertyRuntimeMeta.renderer), id(propertyRuntimeMeta.id) {
		}

		PropertyRuntimeMeta& operator=(const PropertyRuntimeMeta &propertyRuntimeMeta) {
			PropertyMeta::operator=(propertyRuntimeMeta);
			renderer = propertyRuntimeMeta.renderer;
			id = propertyRuntimeMeta.id;
			return *this;
		}

		/// Return the current number of elements in a list property.
		int getRuntimeElementsCount() const {
			return renderer->getPluginPropertyElementsCount(id, name.c_str());
		}

		/// Get the current type of the property value. This can differ from the result of getType().
		Type getRuntimeType() const {
			return renderer->getPluginPropertyRuntimeType(id, name.c_str());
		}

		std::string getRuntimeTypeAsString() const {
			return typeToString(getRuntimeType(), getRuntimeElementsCount());
		}

		void swap(PropertyRuntimeMeta &propertyRuntimeMeta) {
			PropertyMeta::swap(propertyRuntimeMeta);
			std::swap(renderer, propertyRuntimeMeta.renderer);
			std::swap(id, id);
		}
	};

	/// Provides static meta information about a specific Plugin type
	class PluginMeta {
		friend class VRayRenderer;
		friend class Plugin;

		std::string type;
		const VRayRenderer *pRenderer;

	protected:
		PluginMeta(const Plugin& plugin) : type(plugin.getType()), pRenderer(plugin.getRenderer()) { }

		PluginMeta(const VRayRenderer *pRenderer, const std::string &type) : type(type), pRenderer(pRenderer) { }

	public:
		PluginMeta(const PluginMeta &pluginMeta) : type(pluginMeta.type), pRenderer(pluginMeta.pRenderer) { }

		PluginMeta& operator=(const PluginMeta &pluginMeta) {
			type = pluginMeta.type;
			pRenderer = pluginMeta.pRenderer;
			return *this;
		}

		void swap(PluginMeta &pluginMeta) {
			type.swap(pluginMeta.type);
			std::swap(pRenderer, pluginMeta.pRenderer);
		}

		/// Returns the type (name of the class) of the plugin.
		std::string getType() const {
			return type;
		}

		/// Returns the names of all properties.
		std::vector<std::string> getPropertyNames() const {
			return pRenderer->getPluginPropertyNames(type.c_str());
		}

		/// Returns an object used to obtain meta information about a plugin property.
		PropertyMeta getProperty(const std::string &propertyName) const {
			return PropertyMeta(pRenderer, type.c_str(), propertyName);
		}
	};

	class CompressImage;

	namespace Internal {
		class FileSaveWriteHelper {
			friend class VRay::CompressImage;

			std::ofstream ofs;

			FileSaveWriteHelper(const char* fileName) : ofs(fileName, std::ofstream::binary) {}

			~FileSaveWriteHelper() {
				ofs.close();
			}
		};
	}

	/// Base class for LocalJpeg, LocalPng, LocalBmp. Can't be instantiated.
	class CompressImage {
		CompressImage(const CompressImage& ci);
		CompressImage& operator=(const CompressImage& ci);

	protected:
		byte* bufAddr;
		size_t bufSize;

		CompressImage() : bufAddr(), bufSize() {}

	public:
		void swap(CompressImage& ci);

		operator size_t () const {
			return bufSize;
		}

		/// Returns a pointer to the image data
		void* getBuf() const {
			return bufAddr;
		}

		/// Returns size of image data in bytes
		size_t getLen() const {
			return bufSize;
		}

		/// Write the data to disk
		bool saveToFile(const char* fileName) const;

		/// @overload
		bool saveToFile(const std::string &fileName) const {
			return saveToFile(fileName.c_str());
		}

		~CompressImage() {
			if (bufSize)
				Internal::C_memory_free(bufAddr);
		}
	};

	/// A wrapper around JPEG data in memory. It's meant to be stack allocated to free the data when done.
	class LocalJpeg : public CompressImage {

	public:
		/// Compression is performed in this constructor
		/// @param img The source image to compress. The source is not modified.
		/// @param quality If negative, abs(quality) is used and progressive encoding is performed
		LocalJpeg(const VRayImage* img, int quality = 0) {
			bufSize = Internal::VRayImage_compressToJpeg(img, quality, &bufAddr, NULL);
		}

		/// Same as the other constructor. The renderer is used to log error messages.
		LocalJpeg(const VRayImage* img, const VRayRenderer& renderer, int quality = 0) {
			bufSize = Internal::VRayImage_compressToJpeg(img, quality, &bufAddr, renderer.getNativeRenderer());
		}
	};

	class LocalPng : public CompressImage {

	public:
		/// Compression is performed in this constructor
		/// @param img The source image to compress. The source is not modified.
		/// @param preserveAlpha You need to set this explicitly to true to keep the alpha channel in the PNG
		LocalPng(const VRayImage* img, bool preserveAlpha = false) {
			bufSize = Internal::VRayImage_compressToPng(img, preserveAlpha, &bufAddr, NULL);
		}

		/// Same as the other constructor. The renderer is used to log error messages.
		LocalPng(const VRayImage* img, const VRayRenderer& renderer, bool preserveAlpha = false) {
			bufSize = Internal::VRayImage_compressToPng(img, preserveAlpha, &bufAddr, renderer.getNativeRenderer());
		}
	};

	class LocalBmp : public CompressImage {

	public:
		/// Constructs a BMP format image in memory. This format does not compress image data.
		/// @param img The source image to compress. The source is not modified.
		/// @param preserveAlpha You need to set this explicitly to true to keep the alpha channel in the BMP
		/// @param swapChannels If set to true, channel order will be BGR instead of RGB
		LocalBmp(const VRayImage* img, bool preserveAlpha = false, bool swapChannels = false) {
			bufSize = Internal::VRayImage_convertToBmp(img, preserveAlpha, &bufAddr, NULL, swapChannels ? 1 : 0);
		}

		/// Same as the other constructor. The renderer is used to log error messages.
		LocalBmp(const VRayImage* img, const VRayRenderer& renderer, bool preserveAlpha = false, bool swapChannels = false) {
			bufSize = Internal::VRayImage_convertToBmp(img, preserveAlpha, &bufAddr, renderer.getNativeRenderer(), swapChannels ? 1 : 0);
		}
	};

	/// Use this to cast a generic plugin to a specific type that you are certain is correct
	template<class T>
	T plugin_cast(Plugin plugin) {
		return *static_cast<T*>(&plugin);
	}

	/// Use this to cast a generic plugin to a specific type but you are not certain it matches
	/// It will return an invalid T object if the type doesn't match and can be checked with operator bool()
	template<class T>
	T plugin_checked_cast(Plugin plugin) {
		return plugin_cast<T>(strcmp(plugin.getType(), T::getType()) ? Plugin() : plugin);
	}
} // namespace VRay

#ifdef VRAY_SDK_INTEROPERABILITY
#include "vrayinterop.hpp"
#endif

#include "_vraysdk_implementation.hpp"

#ifdef _MSC_VER
#	pragma warning(pop) // 4201
#endif // _MSC_VER

#endif // _VRAY_SDK_HPP_
