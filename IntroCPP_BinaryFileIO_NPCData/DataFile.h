#pragma once

#include "raylib.h"
#include <string>
#include <vector>

using namespace std;

class DataFile
{
	// struct for each record of a person
public:
	struct Record {
		Image image;
		string name;
		int age;
	};

private:
	int recordCount;
	Record record;

public:
	DataFile();
	~DataFile();

	Record* GetRecord(int index);

	int GetRecordCount() { return recordCount; };

	void Load(string filename, unsigned int index);
};

