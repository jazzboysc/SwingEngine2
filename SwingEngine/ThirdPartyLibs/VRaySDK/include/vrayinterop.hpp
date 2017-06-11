#ifdef _VRAY_SDK_HPP_
#ifndef _VRAY_INTEROP_HPP_
#define _VRAY_INTEROP_HPP_

#ifndef _MSC_VER
#include <stdint.h>
#endif

namespace VRay {
	namespace VUtils {
		struct CharStringBuf {
			char* buf;

			static CharStringBuf* create(int size) {
				return static_cast<CharStringBuf*>(Internal::CharStringBuf_create(size));
			}

			CharStringBuf* incRefCount() {
				Internal::CharStringBuf_incRefCount(this);
				return this;
			}

			void decRefCount() {
				return Internal::CharStringBuf_decRefCount(this);
			}

			int getRefCount() {
				return Internal::CharStringBuf_getRefCount(this);
			}

			char* resize(int s) {
				return Internal::CharStringBuf_resize(this, s);
			}

		private:
			CharStringBuf();
			virtual ~CharStringBuf();
		};

		/// Safe string copy, with NULL character appending.
		/// @param dest The destination buffer.
		/// @param source The source string.
		/// @param destSize The size of the destination buffer in chars.
		/// @return The number of chars that were copied, including the terminating NUL character.
		inline int vutils_strcpy_n(char *dest, const char *source, int destSize) {
			if (destSize <= 0) return 0;
			int len;
			for (len = 0; (len<destSize - 1) && (source[len] != '\0'); len++) {
				dest[len] = source[len];
			}
			dest[len] = '\0';
			return len + 1;
		}

		/// Safe string length.
		/// @param str The string buffer.
		/// @param maxSize The maximum size of the string buffer in chars.
		/// @return The size of the string in chars in the buffer up to the first NUL character, or
		//          maxSize if the string does not contain a NUL character.
		inline int vutils_strlen_n(const char *str, int maxSize) {
			int len = 0;
			while (len<maxSize && str[len] != '\0') len++;
			return len;
		}

		/// Safe string concatenation, with NULL character appending.
		/// @param dest The destination buffer.
		/// @param source The source string.
		/// @param destSize The size of the destination buffer in chars.
		/// @return The number of chars that were copied, including the terminating NUL character.
		inline int vutils_strcat_n(char *dest, const char *source, int destSize) {
			int len = vutils_strlen_n(dest, destSize);
			return vutils_strcpy_n(dest + len, source, destSize - len);
		}

		/// String compare.
		/// @param strA The first string.
		/// @param strB The second string.
		/// @return 0 if the strings are equal;
		///  less than 0 if strA is less than strB;
		///  greater than 0 if strA is greater than strB.
		inline int vutils_strcmp(const char *strA, const char *strB) {
			const char *a = strA;
			const char *b = strB;
			while ((*a) != '\0' && (*a) == (*b)) { a++; b++; }
			return (*a) - (*b);
		}

		/// String compare.
		/// @param strA The first string.
		/// @param strB The second string.
		/// @param maxSize The maximum number of chars to compare.
		/// @return 0 if the strings are equal;
		///  less than 0 if strA is less than strB;
		///  greater than 0 if strA is greater than strB.
		inline int vutils_strcmp_n(const char *strA, const char *strB, int maxSize) {
			const char *a = strA;
			const char *b = strB;
			int n = 1;
			while ((*a) != '\0' && (*a) == (*b) && (n < maxSize)) { a++; b++; n++; }
			return (*a) - (*b);
		}

		/// A reference-counted character string. The assignment operator and the
		/// copy constructor do not copy the actual string, but try to hold on to
		/// the original buffer for as long as possible.
		class CharString {
			CharStringBuf *buf;
			void freeMem(void) {
				if (buf) buf->decRefCount();
				buf = NULL;
			}

			// Create a new buffer into which we can copy len+1 characters (the last one is the NUL char)
			void splitBuffer(int len, int copyString) {
				if (!buf) buf = CharStringBuf::create(len + 1); // There is no buffer currently, so create one
				else if (buf->getRefCount()>1) { // Our buffer is shared with other objects, we must create a new one
					CharStringBuf *tmpBuf = CharStringBuf::create(len + 1);
					if (copyString)
						vutils_strcpy_n(tmpBuf->buf, buf->buf, len + 1);
					buf->decRefCount();
					buf = tmpBuf;
				}
				else { // Buffer is not shared, we can modify it directly
					buf->resize(len + 1);
				}
			}

			// Set our buffer to the given one
			void setBuffer(CharStringBuf *newBuffer) {
				// Only do stuff if the new buffer is different from the old one.
				if (newBuffer != buf) {
					if (buf) buf->decRefCount(); // If there is a buffer already, release it
					if (!newBuffer) buf = NULL; // The new buffer is empty
					else buf = newBuffer->incRefCount(); // The new buffer is not empty, so just reference it
				}
			}

			// Make our buffer writeable (e.g. if shared, copy it)
			void makeWriteable(void) {
				if (!buf) return;
				if (buf->getRefCount() <= 1) return; // This is the only copy of the buffer, it is safe to modify

													 // There are other references to the buffer, so we must clone it.
				int len = length() + 1;
				CharStringBuf *newBuffer = CharStringBuf::create(len + 1);
				vutils_strcpy_n(newBuffer->buf, buf->buf, len + 1);
				buf->decRefCount();
				buf = newBuffer;
			}

		public:
			/// Constructor.
			CharString(void) :buf(NULL) {}

			/// Constructor from another string. Copies the data from the given string.
			CharString(const char *val) :buf(NULL) {
				if (val) {
					int len = (int)strlen(val);
					buf = CharStringBuf::create(len + 1);
					if (buf->buf) vutils_strcpy_n(buf->buf, val, len + 1);
				}
			}

			/// Contructor from another CharString. Does not copy the data, just increases the reference count.
			CharString(const CharString& r) :buf(NULL) {
				if (r.buf) buf = r.buf->incRefCount();
			}

			/// Constructor from another string with a given length.
			/// @param str The string to copy from. Need not be NUL-terminated if strLen is the exact length.
			/// @param strLen The maximum number of characters to copy.
			CharString(const char *str, int strLen) :buf(NULL) {
				set(str, strLen);
			}

			/// Constructs a string of blank (' ') characters with the given length.
			CharString(unsigned strLen) :buf(NULL) {
				buf = NULL;
				if (strLen) {
					buf = CharStringBuf::create(strLen + 1);
					memset(buf->buf, ' ', sizeof(char)*(strLen + 1));
					buf->buf[strLen] = 0;
				}
			}

			/// Destructor. Frees any memory used by the object.
			~CharString(void) { freeMem(); }

			/// Return a pointer to the string buffer.
			const char* ptr(void) const { return buf ? buf->buf : NULL; }

			/// Return a writeable pointer to the string buffer. Can be used if you need to replace just certain characters etc.
			char* write_ptr(void) {
				if (!buf) return NULL;
				makeWriteable();
				return buf->buf;
			}

			/// Assignment operator from another string.
			CharString& operator=(const char *newBuf) {
				if (!newBuf) { freeMem(); return *this; }

				int len = (int)strlen(newBuf);
				splitBuffer(len, false);
				if (buf->buf) vutils_strcpy_n(buf->buf, newBuf, len + 1);
				return *this;
			}

			/// Assignment operator from another string. Does not copy the data, just increases the reference count.
			CharString& operator=(const CharString& str) {
				setBuffer(str.buf);
				return *this;
			}

			// strLen is without the trailing NUL character
			void set(const char* str, int strLen) {
				if (!str || strLen == 0) { freeMem(); return; }

				splitBuffer(strLen, false);
				if (buf->buf) vutils_strcpy_n(buf->buf, str, strLen + 1);
			}

			/// Set the string from another one.
			void set(const char* str) { *this = str; }

			/// Set the string from the concatenation of the two given strings.
			void set(const char* str1, const char* str2) {
				int sz1 = (int)((str1) ? strlen(str1) : 0);
				int sz2 = (int)((str2) ? strlen(str2) : 0);
				if (!str1 || !str2 || !sz1 || !sz2) { freeMem(); return; }

				splitBuffer(sz1 + sz2, false);
				if (buf->buf) {
					vutils_strcpy_n(buf->buf, str1, sz1 + sz2 + 1);
					vutils_strcpy_n(buf->buf + sz1, str2, sz1 + sz2 + 1);
				}
			}

			/// Return the given character from the string.
			char operator[](int index) const { return buf->buf[index]; }

			/// Return the length of the string, in chars.
			int length(void) const { return buf ? (int)strlen(buf->buf) : 0; }

			/// Set a new length for the string. If it is smaller than the current length,
			/// the string is truncated. If it is larger, the string is padded with the given character.
			/// If the new length is zero, the memory for the string is deallocated.
			void setLength(int newLen, char fillChar = ' ') {
				if (newLen <= 0) { freeMem(); return; }

				int len = length();
				if (newLen == len) return;

				splitBuffer(newLen, true);
				if (newLen>len && fillChar != 0) { for (int i = len; i<newLen; i++) buf->buf[i] = fillChar; }
				buf->buf[newLen] = 0;
			}

			/// Append another string to this one.
			void append(const char *str) {
				if (!str) return;
				if (!buf) *this = str;
				else {
					int strLen = (int)strlen(str);
					int curLen = length();
					splitBuffer(strLen + curLen, true);
					vutils_strcat_n(buf->buf, str, curLen + strLen + 1);
				}
			}

			/// Append another CharString to this one.
			void append(const CharString &str) {
				append(str.ptr());
			}

			/// Append another string to this one, but read only length number of characters.
			void append(const char *str, int length) {
				if (!str) return;
				if (!buf) set(str, length);
				else {
					int curLen = this->length();
					splitBuffer(length + curLen, true);
					vutils_strcat_n(buf->buf, str, curLen + length + 1);
				}
			}

			/// Swap the contents of two strings.
			void swap(CharString &str) {
				CharStringBuf *temp = buf;
				buf = str.buf;
				str.buf = temp;
			}

			/// Free any memory used by this object.
			void clear(void) { freeMem(); }

			/// Return true if this is an empty string.
			bool empty(void) const { return (buf == NULL) || (buf->buf[0] == 0); }
		};

		/// \relates CharString
		/// Concatenate two CharString's.
		inline CharString operator+(const CharString &a, const CharString &b) {
			CharString res(a);
			res.append(b);
			return res;
		}

		/// \relates CharString
		/// Return true if two CharString's are different. The data of the strings is compared, not just the pointers.
		inline bool operator!=(const CharString &a, const char *str) {
			if (NULL == str) {
				if (a.ptr() == NULL) return false; // Both are empty
				return true;
			}
			if (a.ptr() == NULL) return true; // str is empty, a is not
			return (strcmp(a.ptr(), str) != 0);
		}

		/// \relates CharString
		/// Return true if two CharString's are the same. The data of the strings is compared, not just the pointers.
		inline bool operator==(const CharString &a, const char *str) {
			if (NULL == str) {
				if (a.ptr() == NULL) return true; // Both are empty
				return false;
			}
			if (a.ptr() == NULL) return false; // str is empty, a is not
			return (strcmp(a.ptr(), str) == 0);
		}
		/// \relates CharString
		/// Return true if two CharString's are the same. The data of the strings is compared, not just the pointers.
		inline bool operator==(const CharString &a, const CharString &b) {
			if (b.ptr() == NULL) {
				if (a.ptr() == NULL) return true; // Both are empty
				return false;
			}
			if (a.ptr() == NULL) return false; // str is empty, a is not
			return (strcmp(a.ptr(), b.ptr()) == 0);
		}
		/// \relates CharString
		/// Return true if two CharString's are not the same. The data of the strings is compared, not just the pointers.
		inline bool operator!=(const CharString &a, const CharString &b) {
			return !(a == b);
		}

		/// \relates CharString
		/// Return true if a < b.
		inline bool operator<(const CharString &a, const CharString &b) {
			if (b.ptr() == NULL) return false;
			if (a.ptr() == NULL) return true;
			return (vutils_strcmp(a.ptr(), b.ptr())<0);
		}

		inline CharString makeCharString(const char *a, const char *b) {
			CharString res(a);
			res.append(b);
			return res;
		}

		inline CharString makeCharString(const char *a, const char *b, const char *c) {
			CharString res(a);
			res.append(b);
			res.append(c);
			return res;
		}

		inline CharString makeCharString(const char *a, const char *b, const char *c, const char *d) {
			CharString res(a);
			res.append(b);
			res.append(c);
			res.append(d);
			return res;
		}

		template<class T>
		struct RefCounter {
			static RefCounter* create();
			void inc();
			int dec();
			T* allocMem(int n);
			void destroy(T* ptr);
		private:
			RefCounter();
			~RefCounter();
		};

		class Value;
		template<class T> struct PtrArray;

		typedef PtrArray<int> IntRefList;
		typedef PtrArray<float> FloatRefList;
		typedef PtrArray<Vector> VectorRefList;
		typedef PtrArray<Color> ColorRefList;
		typedef PtrArray<CharString> CharStringRefList;
		typedef PtrArray<Value> ValueRefList;

		class Object;

		struct ObjectID {
			InstanceId id;
		};

		struct RuntimeValue {
			Object *obj;
			CharString valueName;
		};

		struct TracePoint {
			double x, y, z;
		};

		struct TraceTransform {
			Matrix m; ///< The matrix portion (rotation+scale).
			int padding; ///< Force an 8 byte alignment for offs (this is for 32-bit Linux/Mac)
			TracePoint ffs; ///< Offset (translation).
		};

		/// Describes the possible types of values stored in a Value class.
		enum VALUE_TYPE {
			UNKNOWN = 0, ///< Undefined value.
			NUMBER, ///< Number value (stored as a double-precision floating-point number).
			LIST, ///< A reference-counted list of other values.
			VECTOR, ///< A Vector value.
			MATRIX, ///< A Matrix value.
			COLOR, ///< A Color value.
			ACOLOR, ///< An AColor value.
			TRANSFORM, ///< A TraceTransform value.
			LISTVECTOR, ///< A VectorRefList value (reference-counted list of Vector's).
			LISTINT, ///< An IntRefList value (reference-counted list of int's).
			LISTCOLOR, ///< A ColorRefList value (reference-counted list of Color's).
			LISTFLOAT, ///< A FloatRefList value (reference-counted list of float's).
			STRING, ///< A string value.
			OBJECT, ///< An Object value.
			OBJECTID, ///< An ObjectID value. This is never created by the parser specifically, but may be used for the RTEngine.
			RUNTIMEVALUE, ///< A RuntimeValue value.
			LISTSTRING ///< A CharStringRefList value (reference-counted list of strings).
		};

		template<>
		struct RefCounter<int> {
			static RefCounter* create() {
				return static_cast<RefCounter*>(Internal::RefCounter_create(TYPE_INT));
			}

			void inc() {
				Internal::RefCounter_increment(this, TYPE_INT);
			}

			int dec() {
				return Internal::RefCounter_decrement(this, TYPE_INT);
			}

			int* allocMem(int n) {
				return static_cast<int*>(Internal::RefCounter_allocMem(this, TYPE_INT, n));
			}

			void destroy(int* ptr) {
				Internal::RefCounter_destroy(this, TYPE_INT, ptr);
			}
		};

		template<>
		struct RefCounter<float> {
			static RefCounter* create() {
				return static_cast<RefCounter*>(Internal::RefCounter_create(TYPE_FLOAT));
			}

			void inc() {
				Internal::RefCounter_increment(this, TYPE_FLOAT);
			}

			int dec() {
				return Internal::RefCounter_decrement(this, TYPE_FLOAT);
			}

			float* allocMem(int n) {
				return static_cast<float*>(Internal::RefCounter_allocMem(this, TYPE_FLOAT, n));
			}

			void destroy(float* ptr) {
				Internal::RefCounter_destroy(this, TYPE_FLOAT, ptr);
			}
		};

		template<>
		struct RefCounter<Vector> {
			static RefCounter* create() {
				return static_cast<RefCounter*>(Internal::RefCounter_create(TYPE_VECTOR));
			}

			void inc() {
				Internal::RefCounter_increment(this, TYPE_VECTOR);
			}

			int dec() {
				return Internal::RefCounter_decrement(this, TYPE_VECTOR);
			}

			Vector* allocMem(int n) {
				return static_cast<Vector*>(Internal::RefCounter_allocMem(this, TYPE_VECTOR, n));
			}

			void destroy(Vector* ptr) {
				Internal::RefCounter_destroy(this, TYPE_VECTOR, ptr);
			}
		};

		template<>
		struct RefCounter<Color> {
			static RefCounter* create() {
				return static_cast<RefCounter*>(Internal::RefCounter_create(TYPE_COLOR));
			}

			void inc() {
				Internal::RefCounter_increment(this, TYPE_COLOR);
			}

			int dec() {
				return Internal::RefCounter_decrement(this, TYPE_COLOR);
			}

			Color* allocMem(int n) {
				return static_cast<Color*>(Internal::RefCounter_allocMem(this, TYPE_COLOR, n));
			}

			void destroy(Color* ptr) {
				Internal::RefCounter_destroy(this, TYPE_COLOR, ptr);
			}
		};

		template<>
		struct RefCounter<CharString> {
			static RefCounter* create() {
				return static_cast<RefCounter*>(Internal::RefCounter_create(TYPE_STRING));
			}

			void inc() {
				Internal::RefCounter_increment(this, TYPE_STRING);
			}

			int dec() {
				return Internal::RefCounter_decrement(this, TYPE_STRING);
			}

			CharString* allocMem(int n) {
				return static_cast<CharString*>(Internal::RefCounter_allocMem(this, TYPE_STRING, n));
			}

			void destroy(CharString* ptr) {
				Internal::RefCounter_destroy(this, TYPE_STRING, ptr);
			}
		};

		template<>
		struct RefCounter<Value> {
			static RefCounter* create() {
				return static_cast<RefCounter*>(Internal::RefCounter_create(TYPE_LIST));
			}

			void inc() {
				Internal::RefCounter_increment(this, TYPE_LIST);
			}

			int dec() {
				return Internal::RefCounter_decrement(this, TYPE_LIST);
			}

			Value* allocMem(int n) {
				return static_cast<Value*>(Internal::RefCounter_allocMem(this, TYPE_LIST, n));
			}

			void destroy(Value* ptr) {
				Internal::RefCounter_destroy(this, TYPE_LIST, ptr);
			}
		};

		/// A wrapper for a pointer to a dynamically allocated array, with reference counting. Deletes the array on destruction.
		template<class T>
		struct PtrArray {
			/// Constructor; the pointer is initialized with NULL.
			PtrArray(void) { ptr = NULL; refCounter = NULL; length = 0; }

			/// Copy constructor.
			PtrArray(const PtrArray &p) { copy(p); }

			/// Assignment operator.
			PtrArray& operator=(const PtrArray &p) {
				freeMem();
				copy(p);
				return (*this);
			}

			/// Constructor; allocates an array for the given number of elements.
			explicit PtrArray(int num) { allocArray(num); }

			/// Constructor; initializes the pointer with the given array. The PtrArray does not assume ownership of the array and will not delete it.
			/// @param ptr The pointer to initialize with;
			/// @param len The length of the array;
			PtrArray(T *ptr, int len) { init(ptr, len); }

			/// Constructor; initializes the pointer with the given array. The PtrArray does not assume ownership of the array and will not delete it.
			/// @param arr The array. The length is automatically extracted from the array declaration;
			template<int len>
			PtrArray(T (&arr)[len]) { init(arr, len); }

			/// Destructor; the array is deleted if the pointer is non-NULL.
			~PtrArray(void) { freeMem(); }

			/// Return the i-th element in the array.
			T& operator[](int i) { return ptr[i]; }

			/// Return the i-th element in the array.
			const T& operator[](int i) const { return ptr[i]; }

			/// Return true if the pointer is NULL.
			bool operator!(void) const { return (ptr == NULL); }

			/// Returns true if the pointer is not NULL.
			operator bool(void) const { return (ptr != NULL); }

			/// Return the pointer itself.
			operator T*(void) { return ptr; }

			/// Return the pointer itself as const.
			operator const T*(void) { return ptr; }

			/// Initializes the array with the given pointer, without assuming ownership.
			/// @param ptr The pointer to initialize with.
			/// @param len The length of the array.
			/// @note The PtrArray does not take ownership of the array and will not delete it when it is destroyed.
			void init(T *ptr, int len) {
				this->ptr = ptr;
				refCounter = NULL;
				length = len;
			}

			/// Delete the array and set the pointer to NULL.
			void freeMem(void) {
				if (!refCounter) { // No counter, so we don't own the pointer; just clear the info.
					ptr = NULL;
					length = 0;
					return;
				}
				if (refCounter->dec()>0) { // There are other references, so just clear the info.
					ptr = NULL;
					refCounter = NULL;
					length = 0;
					return;
				}
				// We own the pointer and this was the last reference to it - delete it.
				refCounter->destroy(ptr);
				ptr = NULL;
				refCounter = NULL;
				length = 0;
			}

			/// Return the pointer itself.
			T* get(void) { return ptr; }
			const T* get(void) const { return ptr; }

			/// Returns the size of the array.
			int size(void) const { return length; }
			int count(void) const { return length; }

		private:
			T *ptr; ///< Pointer to the dynamically allocated array.
			RefCounter<T> *refCounter;
			int length;

			void copy(const PtrArray &p) {
				this->ptr = p.ptr;
				this->refCounter = p.refCounter;
				this->length = p.length;
				if (refCounter) refCounter->inc();
			}

			void allocArray(int num) {
				length = num;
				refCounter = RefCounter<T>::create();
				ptr = refCounter->allocMem(length);
			}
		};

		class Value {
			VALUE_TYPE valueType;
			TraceTransform t;
			CharString str;
			RuntimeValue rt;
			Object *obj;
			ObjectID objectID;

			ValueRefList valueList;
			IntRefList intList;
			FloatRefList floatList;
			VectorRefList vectorList;
			ColorRefList colorList;
			CharStringRefList stringList;

			CharString scenePath;

			void copy(const Value &v) {
				this->valueType = v.valueType;
				this->t = v.t;
				this->str = v.str;
				this->rt = v.rt;
				this->obj = v.obj;
				this->objectID = v.objectID;

				this->valueList = v.valueList;
				this->intList = v.intList;
				this->floatList = v.floatList;
				this->vectorList = v.vectorList;
				this->colorList = v.colorList;
				this->stringList = v.stringList;

				this->scenePath = v.scenePath;
			}

		public:
			/// Default constructors
			Value() : valueType(UNKNOWN) {}

			// Note that the parameters in the constructors are copied using their copy-constructor.
			explicit Value(double val) {
				setDouble(val);
			}
			explicit Value(const ValueRefList &lst) {
				setList(lst);
			}
			explicit Value(const TracePoint &v) {
				setTracePoint(v);
			}
			explicit Value(const Vector &v) {
				setVector(v);
			}
			explicit Value(const Matrix &mat) {
				setMatrix(mat);
			}
			explicit Value(const Color &col) {
				setColor(col);
			}
			explicit Value(const AColor &acol) {
				setAColor(acol);
			}
			explicit Value(const TraceTransform &ttransform) {
				setTraceTransform(ttransform);
			}
			explicit Value(const Transform &transform) {
				setTransform(transform);
			}
			explicit Value(const CharString &str) {
				setString(str);
			}
			explicit Value(const VectorRefList &list) {
				setListVector(list);
			}
			explicit Value(const ColorRefList &list) {
				setListColor(list);
			}
			explicit Value(const FloatRefList &list) {
				setListFloat(list);
			}
			explicit Value(const IntRefList &list) {
				setListInt(list);
			}
			explicit Value(const CharStringRefList &list) {
				setListString(list);
			}
			explicit Value(const RuntimeValue &rv) {
				setRuntimeValue(rv);
			}
			explicit Value(Object *obj) {
				setObject(obj);
			}
			explicit Value(const ObjectID &objectID) {
				setObjectID(objectID);
			}

			explicit Value(const Plugin &plugin) {
				setPlugin(plugin);
			}

			/// Copy constructor
			Value(const Value &v) {
				copy(v);
			}

			/// Assignment operator
			Value &operator=(const Value &v) {
				copy(v);
				return *this;
			}

			/// Returns the type of the value stored in this class. The
			/// value itself can be obtained with one of the get() methods.
			VALUE_TYPE getValueType() {
				return valueType;
			}

			// Double
			void setDouble(double val) {
				double *f = (double*)&t;
				*f = val;
				valueType = NUMBER;
			}
			double getDouble() {
				return *((double*)&t);
			}
			
			// List
			void setList(const ValueRefList &list) {
				valueList = list;
				valueType = LIST;
			}
			ValueRefList getList() {
				return valueList;
			}

			// ListVector
			VectorRefList getListVector() {
				return vectorList;
			}
			void setListVector(const VectorRefList &list) {
				vectorList = list;
				valueType = LISTVECTOR;
			}

			// ListInt
			IntRefList getListInt() {
				return intList;
			}
			void setListInt(const IntRefList &list) {
				intList = list;
				valueType = LISTINT;
			}

			// ListColor
			ColorRefList getListColor() {
				return colorList;
			}
			void setListColor(const ColorRefList &list) {
				colorList = list;
				valueType = LISTCOLOR;
			}

			// ListFloat
			FloatRefList getListFloat() {
				return floatList;
			}
			void setListFloat(const FloatRefList &list) {
				floatList = list;
				valueType = LISTFLOAT;
			}

			// ListString
			CharStringRefList getListString() {
				return stringList;
			}
			void setListString(const CharStringRefList &list) {
				stringList = list;
				valueType = LISTSTRING;
			}

			// Vector
			void setVector(const Vector &v) {
				TracePoint *f = (TracePoint*) &t;
				f->x = v.x, f->y = v.y, f->z = v.z;
				valueType = VECTOR;
			}
			Vector getVector() {
				TracePoint *f = (TracePoint*) &t;
				return Vector(f->x, f->y, f->z);
			}
			
			// TracePoint
			void setTracePoint(const TracePoint &v) {
				TracePoint *f = (TracePoint*) &t;
				*f = v;
				valueType = VECTOR;
			}
			TracePoint getTracePoint() {
				return *((VUtils::TracePoint*) &t);
			}

			// Matrix
			void setMatrix(const Matrix &v) {
				Matrix *f = (Matrix*) &t;
				*f = v;
				valueType = MATRIX;
			}
			Matrix getMatrix() {
				return *((Matrix*) &t);
			}

			// Color
			void setColor(const Color &v) {
				Color *f = (Color*) &t;
				*f = v;
				valueType = COLOR;
			}
			Color getColor() {
				return *((Color*) &t);
			}
			
			// AColor
			void setAColor(const AColor &v) {
				AColor *f = (AColor*) &t;
				*f = v;
				valueType = ACOLOR;
			}
			AColor getAColor() {
				return *((AColor*) &t);
			}
			
			// String
			CharString getString() {
				return str;
			}
			void setString(const CharString &str) {
				this->str = str;
				valueType = STRING;
			}

			// Transform
			TraceTransform getTraceTransform() {
				return t;
			}
			void setTraceTransform(const TraceTransform &tr) {
				this->t = tr;
				valueType = TRANSFORM;
			}
			Transform getTransform() {
				return Transform(t.m, Vector(t.ffs.x, t.ffs.y, t.ffs.z));
			}
			void setTransform(const Transform &tr) {
				t.m = tr.matrix;
				t.ffs.x = tr.offset.x;
				t.ffs.y = tr.offset.y;
				t.ffs.z = tr.offset.z;
				valueType = TRANSFORM;
			}

			// Object 
			Object* getObject() {
				return obj;
			}
			void setObject(Object *obj) {
				this->obj = obj;
				valueType = OBJECT;
			}

			// ObjectID
			ObjectID getObjectID() {
				return objectID;
			}
			void setObjectID(const ObjectID &id) {
				this->objectID = id;
				valueType = OBJECTID;
			}


			// Plugin
			const Plugin getPlugin(VRayRenderer &renderer) {
				struct PluginWrap : public Plugin {
					PluginWrap(VRayRenderer& renderer, InstanceId id)
						: Plugin(renderer, id) { }
				};
				return PluginWrap(renderer, this->objectID.id);
			}
			void setPlugin(const Plugin &plugin) {
				this->objectID.id = plugin.getId();
				valueType = OBJECTID;
			}

			// RuntimeValue
			void setRuntimeValue(const RuntimeValue& rv) {
				rt = rv;
				valueType = RUNTIMEVALUE;
			}
			RuntimeValue getRuntimeValue() {
				return rt;
			}
		};
	}
}

#endif // _VRAY_INTEROP_HPP_

#else // _VRAY_SDK_HPP_

# ifndef VRAY_SDK_INTEROPERABILITY
#  define VRAY_SDK_INTEROPERABILITY
# endif
# include "vraysdk.hpp"

#endif // _VRAY_SDK_HPP_
