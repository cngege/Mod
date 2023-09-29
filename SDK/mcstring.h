#pragma once
#include <string>

namespace std
{
	class mcstring {
	public:
		union {
			char text[16];  //0x0000
			char* pText = nullptr;          //0x0000
		};

		size_t textLength;         //0x0010
		size_t alignedTextLength;  //0x0018

		mcstring() {
			memset(this, 0, sizeof(mcstring));
		}

		mcstring(mcstring const& copy) {
			memset(this, 0, sizeof(mcstring));
			textLength = copy.textLength;
			alignedTextLength = copy.alignedTextLength;
			if (copy.textLength < 16)
				memcpy(text, copy.text, 16);
			else {
				size_t size = textLength + 1;

				if (size + 1 >= 0x1000)
					size += 8;

				pText = reinterpret_cast<char*>(malloc(size + 1));
				alignedTextLength = size;
				if (pText != 0x0 && size + 1 >= 0x1000) {
					*reinterpret_cast<char**>(pText) = pText;
					pText += 8;
				}

				if (pText != 0x0 && copy.pText != 0x0) {
					memcpy(pText, copy.pText, size);
					pText[size] = 0;
				}
			}
		}

		mcstring& operator=(mcstring const& copy) {
			release();
			memset(this, 0, sizeof(mcstring));
			textLength = copy.textLength;
			alignedTextLength = copy.alignedTextLength;
			if (copy.textLength < 16)
				memcpy(text, copy.text, 16);
			else {
				size_t size = textLength + 1;

				if (size + 1 >= 0x1000)
					size += 8;

				pText = reinterpret_cast<char*>(malloc(size + 1));
				alignedTextLength = size;
				if (pText != 0x0 && size + 1 >= 0x1000) {
					*reinterpret_cast<char**>(pText) = pText;
					pText += 8;
				}

				if (pText != 0x0 && copy.pText != 0x0) {
					memcpy(pText, copy.pText, textLength);
					pText[textLength] = 0;
				}
			}
			return *this;
		}

		bool operator==(mcstring const& copy) {
			if (alignedTextLength != copy.alignedTextLength)
				return false;
			if (alignedTextLength < 16) {
				return std::string(this->data()) == std::string(copy.text);
			}
			else {
				return std::string(this->data()) == std::string(copy.pText);
			}
		}

		mcstring(std::string str) {
			memset(this, 0, sizeof(mcstring));
			textLength = str.size();
			alignedTextLength = textLength | 0xF;
			if (str.size() < 16) {
				memcpy(text, str.c_str(), str.size());
				if (str.size() < 15)
					text[str.size()] = 0;
			}
			else {
				size_t size = str.size();
				if (size + 1 >= 0x1000)
					size += 8;

				pText = reinterpret_cast<char*>(malloc(size + 1));
				alignedTextLength = size;
				if (pText != 0x0 && size + 1 >= 0x1000) {
					*reinterpret_cast<char**>(pText) = pText;
					pText += 8;
				}

				if (pText != 0x0) {
					memcpy(pText, str.c_str(), str.size());
					pText[str.size()] = 0;
				}
			}
		}

		mcstring(void* ptr, size_t sizeOfData) {
			memset(this, 0, sizeof(mcstring));
			textLength = sizeOfData;
			alignedTextLength = sizeOfData;
			if (alignedTextLength < 16)
				memcpy(text, ptr, sizeOfData);
			else
				pText = reinterpret_cast<char*>(ptr);
		}

		void release() {
			if (alignedTextLength >= 16 && pText != nullptr) {
				if (alignedTextLength + 1 >= 0x1000) {
					pText = *reinterpret_cast<char**>(reinterpret_cast<__int64>(pText) - 8);
				}
				free(pText);
			}
		}

		~mcstring() {
			this->release();
		}

		char* data() {
			if (alignedTextLength < 16) {
				return this->text;
			}
			else {
				if (this->pText == nullptr) {
					return {};
				}
				return this->pText;
			}
		}

		const char* c_str() const {
			if (alignedTextLength < 16) {
				return this->text;
			}
			else {
				if (this->pText == nullptr) {
					return {};
				}
				return this->pText;
			}
		}

		size_t size() {
			return textLength;
		}

		void setText(std::string str) {
			this->release();
			memset(this, 0, sizeof(mcstring));
			textLength = str.size();
			alignedTextLength = textLength | 0xF;
			if (str.size() < 16) {
				memcpy(text, str.c_str(), str.size());
				if (str.size() < 15)
					text[str.size()] = 0;
			}
			else {
				size_t size = str.size();
				if (size + 1 >= 0x1000)
					size += 8;

				pText = reinterpret_cast<char*>(malloc(size + 1));
				alignedTextLength = size;
				if (pText != 0x0 && size + 1 >= 0x1000) {
					*reinterpret_cast<char**>(pText) = pText;
					pText += 8;
				}

				if (pText != 0x0) {
					memcpy(pText, str.c_str(), str.size());
					pText[str.size()] = 0;
				}
			}
		}

		/**
		 * @brief 重置但不删除
		*/
		void resetWithoutDelete() {
			*data() = 0;
			textLength = 0;
		}

		string to_string() const{
			return string(c_str());
		}
	};
}