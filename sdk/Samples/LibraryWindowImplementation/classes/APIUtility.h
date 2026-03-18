#pragma once

#include <QString>
#include <QVariant>
#include <vector>

#include "APIDefine.h"

// natural sort callback structure
// Reference from http://stackoverflow.com/questions/11933883/sort-filenames-naturally-with-qt/11947242
struct naturalSortCompare {

	inline bool isNumber(QChar c)
	{
		return c >= '0' && c <= '9';
	}

	inline bool operator() (const QString& s1, const QString& s2)
	{
		if (s1 == "" || s2 == "")
			return s1 < s2;

		// Move to the first difference between the strings
		int startIndex = -1;
		int length = s1.length() > s2.length() ? s2.length() : s1.length();

		for (int i = 0; i < length; i++) {
			QChar c1 = s1[i];
			QChar c2 = s2[i];
			if (c1 != c2) {
				startIndex = i;
				break;
			}
		}

		// If the strings are the same, exit now.
		if (startIndex < 0)
			return s1 < s2;

		// Now extract the numbers, if any, from the two strings.
		QString sn1;
		QString sn2;
		bool done1 = false;
		bool done2 = false;
		length = s1.length() < s2.length() ? s2.length() : s1.length();

		for (int i = startIndex; i < length; i++) {
			if (!done1 && i < s1.length()) {
				if (isNumber(s1[i])) {
					sn1 += QString(s1[i]);
				}
				else {
					done1 = true;
				}
			}

			if (!done2 && i < s2.length()) {
				if (isNumber(s2[i])) {
					sn2 += QString(s2[i]);
				}
				else {
					done2 = true;
				}
			}

			if (done1 && done2)
				break;
		}

		// If none of the strings contain a number, use a regular comparison.
		if (sn1 == "" && sn2 == "")
			return s1 < s2;

		// If one of the strings doesn't contain a number at that position,
		// we put the string with number first so that, for example,
		// "example1.bin" is before "example.bin"
		if (sn1 == "" && sn2 != "")
			return false;

		if (sn1 != "" && sn2 == "")
			return true;

		return sn1.toInt() < sn2.toInt();
	}
};

QString GetParentFolderPath(const QString& path);
QString GetOnlyFileName(const QString& path);
QString GetFileExtension(const QString& path);

void PutMapElement(QString& m_json, QString p, QString v);
void PutMapIntElement(QString& m_json, QString p, QString v);
void PushBackItem(QString& m_json, QString itemID, QString name, QString itemType, QString path, QString dateTime, long fileSize, QString thumbnailName, QVariantMap metaDataMap);

#if defined(_WIN32) || defined(_WIN64)
std::wstring ConvertUTF8CharToWChar(const char* strUTF8);
std::wstring ConvertANSICharToWChar(const char* strMultibyte);
#endif

FILE* FileOpen(const QString* filename, char const* mode);

unsigned char* FileLoadViaString(QString path, unsigned int& sizeInByte);

bool dateTimeCompare(QString &left, QString &right);

QString generateJSONFromLibraryAPIItemList(const std::vector<CLOAPISample::LibraryAPIItem>& list);

QString GetRandomDateTimer(); // "2019-01-20"