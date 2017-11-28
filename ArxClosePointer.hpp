#ifndef ARXCLOSEPOINTER_HPP
#define ARXCLOSEPOINTER_HPP

namespace sstd {
	/**  
	class AcDbSymbolTablePointer; 
	class AcDbSymbolTableRecordPointer;
	**/
	template<typename T>
	class ArxClosePointer {
		T * $pointer;
		inline void _p_close() {
			if ($pointer) {
				if ($pointer->objectId().isNull()) {
					delete $pointer;
				}
				else {
					$pointer->close();
				}
			}
		}
	public:
		ArxClosePointer() :$pointer(nullptr) {}
		template<typename U>
		inline ArxClosePointer(U *arg) : $pointer{ arg } {}
		inline ~ArxClosePointer() { _p_close(); }

		inline T * operator->()const { return $pointer; }
		inline T * & operator ->() { return $pointer; }

		inline operator T *&() { return $pointer; }
		inline operator T * ()const { return $pointer; }

		inline T* pointer() const { return $pointer; }
		inline T*&pointer() { return $pointer; }

		inline operator bool() const { return bool($pointer); }

		ArxClosePointer(const ArxClosePointer &) = delete;
		ArxClosePointer&operator=(const ArxClosePointer &) = delete;

		inline ArxClosePointer(ArxClosePointer && arg)
			:$pointer(arg.$pointer) {
			arg.$pointer = nullptr;
		}

		inline ArxClosePointer&operator=(ArxClosePointer && arg) {
			if (this == &arg) { return *this; }
			ArxClosePointer varTmp;
			std::swap(varTmp.$pointer, arg.$pointer);
			std::swap(this->$pointer, varTmp.$pointer);
			return *this;
		}

	};

}/*namespace sstd*/

#endif // ARXCLOSEPOINTER_HPP


