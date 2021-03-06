#ifndef GB_UCPROPTABLE_H
#define GB_UCPROPTABLE_H

#include <sys/types.h>
#include <stdlib.h> //NULL

class UCPropTable {
public:
	UCPropTable(unsigned char valueSize = 1, 
		    unsigned char tableBits = 16) ;

	virtual ~UCPropTable() ;
	void reset();

	//void *getValue(uint32_t c);
	const void *getValue(uint32_t c) const {
		uint32_t prefix = c >> m_tableBits;
		uint32_t key = c & m_tableMask;
		if (prefix >= m_numTables) return NULL;
		if (m_data[prefix] == NULL) return NULL;
		return (const void*) (m_data[prefix] + key*m_valueSize);
	}

	bool setValue(uint32_t c, const void *value);
	
	size_t getSize() const { return getStoredSize() + m_numTables*sizeof(char*); }
	size_t getStoredSize() const;
	size_t serialize(char *buf, size_t bufSize) const;
	size_t deserialize(const char *buf, size_t bufSize);

private:
	unsigned char **m_data;

	unsigned char m_valueSize;
	unsigned char m_tableBits;
	uint32_t m_tableSize;
	uint32_t m_tableMask;
	uint32_t m_numTables;
};

#endif // GB_UCPROPTABLE_H
