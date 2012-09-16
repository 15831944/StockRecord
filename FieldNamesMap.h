/**
 *	Used for map (convert) English field names (column names) into Chinese.
 *  The converted Chinese names will be displayed on dlg screen.
 *  Date: 2012-09-16 19:03:27
 */

#ifndef FILED_NAMES_MAP_H
#define FILED_NAMES_MAP_H

#include <map>
#include <string>
using namespace std;

class FieldNamesMap
{
private:
	static std::map<std::string, std::string> m_FieldNamesMap;
	static bool m_isMapInited;

public:
	FieldNamesMap(void);
	~FieldNamesMap();
	
	static int InitFieldNamesMap(void);
	static string GetChNameByEnName(string enName);
};

#endif