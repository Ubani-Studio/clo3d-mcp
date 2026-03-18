#include "stdafx.h"

#include "APIUtility.h"

#include <random>
#include "time.h"

#include <QDateTime>
#include <QFileInfo>
#include <QVariant>

using namespace std;

QString GetParentFolderPath(const QString& path)
{
	if (path == "" || path == NULL) return "";
	QString folderPath = path;
	if (folderPath.endsWith('/'))
		folderPath = folderPath.left(folderPath.length() - 1);

	QString filename = folderPath.section('/', -1);
	QString parentPath = folderPath.left(folderPath.size() - (filename.size() + 1));
	return parentPath;
}

QString GetOnlyFileName(const QString& path)
{
	if (path == "" || path == NULL) return "";
	QString filename = path.section('/', -1);
	return filename;
}

QString GetFileExtension(const QString& path)
{
	if (path.size() == 0)
		return QString("");

	QString fileExtension;
	QString fileName = GetOnlyFileName(path);
	if (fileName.contains("."))
	{
		fileExtension = fileName.section('.', -1);
	}

	return fileExtension;
}

void PutMapElement(QString& m_json, QString p, QString v)
{
	QString a = QString("\"%1\": \"%2\"").arg(p).arg(v);
	
	m_json += a;
}

void PutMapIntElement(QString& m_json, QString p, QString v)
{
	QString a = QString("\"%1\": %2").arg(p).arg(v);
	
	m_json += a;
}

void PushBackItem(QString& m_json, QString itemID, QString name, QString itemType, QString path, QString dateTime, long fileSize, QString thumbnailName, QVariantMap metaDataMap)
{
	m_json += "{\n";
	PutMapElement(m_json, "itemID", itemID);

    if(itemType == "FOLDER")
        PutMapElement(m_json, "type", "FOLDER");
    else
    {
        QFileInfo fileInfo(path);
        if (fileInfo.isDir()) {
            m_json += ",\n";
            PutMapElement(m_json, "type", "FOLDER");
        }
        else
        {
            m_json += ",\n";
            PutMapElement(m_json, "type", "FILE");
        }
    }

	m_json += ",\n";
	PutMapElement(m_json, "name", name);

	m_json += ",\n";
	PutMapElement(m_json, "path", path);

	m_json += ",\n";
	PutMapElement(m_json, "dateTime", dateTime);

	QString size;
	size.setNum(fileSize);
	m_json += ",\n";
	PutMapIntElement(m_json, "fileSize", size);

	m_json += ",\n";
	PutMapElement(m_json, "thumbnailName", thumbnailName);

	if (!metaDataMap.empty())
	{
		m_json += ",\n";
		m_json += "\"metaData\" : [\n";

		int curCount = 0;
		int endCount = metaDataMap.size();

		for (QVariantMap::const_iterator iter_Name = metaDataMap.constBegin(); iter_Name != metaDataMap.constEnd(); ++iter_Name) {
			QString metaDataKey = iter_Name.key();
			QString metaDataValue = "\"" + iter_Name.value().toString() + "\"";

			if (curCount == 0)
				m_json += "{\n";
			else if (curCount <= endCount - 1)
				m_json += ",\n";

			PutMapIntElement(m_json, metaDataKey, metaDataValue);

			if (curCount == endCount - 1)
				m_json += "\n}\n]";

			curCount++;
		}
	}

	m_json += "\n}\n";
}

#if defined(_WIN32) || defined(_WIN64)
std::wstring ConvertUTF8CharToWChar(const char* strUTF8)
{
	int nLen = MultiByteToWideChar(CP_UTF8, 0, strUTF8, (int)strlen(strUTF8), NULL, NULL);
	wstring strUnicode;
	strUnicode.resize(nLen);
	MultiByteToWideChar(CP_UTF8, 0, strUTF8, (int)strlen(strUTF8), &strUnicode[0], nLen);
	return strUnicode;
}

std::wstring ConvertANSICharToWChar(const char* strMultibyte)
{
	int nLen = MultiByteToWideChar(CP_ACP, 0, strMultibyte, (int)strlen(strMultibyte), NULL, NULL);
	wstring strUnicode;
	strUnicode.resize(nLen);
	MultiByteToWideChar(CP_ACP, 0, strMultibyte, (int)strlen(strMultibyte), &strUnicode[0], nLen);
	return strUnicode;
}
#endif

FILE* FileOpen(const QString* filename, char const* mode)
{
#if defined(_WIN32) || defined(_WIN64)
	wstring wideFilename = ConvertUTF8CharToWChar(filename->toUtf8().data());
	wstring wideMode = ConvertANSICharToWChar(mode);
	FILE* fp = _wfopen(&wideFilename[0], &wideMode[0]);
	return fp;
#else
	return fopen(filename->toLocal8Bit().data(), mode);
#endif
}

unsigned char* FileLoadViaString(QString path, unsigned int& sizeInByte)
{	
	if (path == NULL || path == "")
		return NULL;

	FILE* pFile = NULL;
	if (!QFileInfo(path).exists())
		return NULL;

#if defined( __APPLE__ ) || defined(__linux__)
	pFile = FileOpen(&path, "r");
#else
	pFile = FileOpen(&path, "rb");
#endif
	if (!pFile)
		return NULL;

	unsigned char* buffer = NULL;

	// obtain file size:
	fseek(pFile, 0, SEEK_END);
	long lSize = ftell(pFile);
	
	// set file pointer to start of file
	fseek(pFile, 0, SEEK_SET);

	// allocate memory to contain the whole file:
	buffer = (unsigned char*)malloc(sizeof(char)*lSize);
	if (buffer == NULL)
	{
		fputs("Memory error", stderr);
		return NULL;
	}

	// copy the file into the buffer:
	size_t size = fread(buffer, 1, lSize, pFile);

	fclose(pFile);

	sizeInByte = (unsigned int)lSize;
	return buffer;
}

bool dateTimeCompare(QString &left, QString &right)
{
	QFileInfo infoLeft = QFileInfo(left);
	QDateTime dateLeft = infoLeft.lastModified();

	QFileInfo infoRight = QFileInfo(right);
	QDateTime dateRight = infoRight.lastModified();
	return dateLeft < dateRight;
}

QString generateJSONFromLibraryAPIItemList(const vector<CLOAPISample::LibraryAPIItem>& list)
{
	QString jsonStr = "[\n"; // start json

	for (size_t i = 0; i < list.size(); ++i)
	{
		const CLOAPISample::LibraryAPIItem& curItem = list[i];
		PushBackItem(jsonStr, curItem.itemID, curItem.itemName, curItem.itemType, curItem.sampleItemData.itemPath, curItem.dateTime, curItem.filesize, curItem.sampleItemData.iconThumbnailPath, curItem.metaData);

		if (i < list.size() - 1)
			jsonStr += ",\n"; // separator 
	}

	jsonStr += "\n]"; // end json

	return jsonStr;
}

QString GetRandomDateTimer()
{
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);

	// randomizing month, hour, minute.
	std::mt19937 rng{ std::random_device{}() };
	
	std::uniform_int_distribution<> month{ 0, 11 };
	std::uniform_int_distribution<> hour{ 0, 23 };
	std::uniform_int_distribution<> minute{ 0, 59 };
	std::uniform_int_distribution<> second{ 0, 59 };

	tstruct.tm_mon = month(rng);
	tstruct.tm_hour = hour(rng);
	tstruct.tm_min = minute(rng);
	tstruct.tm_sec = second(rng);

	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

	return QString::fromLatin1(buf);
}

