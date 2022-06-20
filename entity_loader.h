#pragma once
#include <iostream>
#include <SDL.h>
#include "sqlite3.h"

namespace Entity_Loader {
	
	sqlite3* db;
	

	struct Data {
		float radius = 0;
		float speed = 0;
		float mass = 0;

	};

	void init_db() {
		sqlite3_open("data.db", &db);
	}

	Data parse_data(const char *name) {// needs to search for  a specific row that I can input in the arguments
		//check if the name exists??
		Data values;
		sqlite3_stmt* stmt;
		char buf[100];
		const char* jj = "SELECT radius, speed, mass FROM unit_data WHERE name = ";
		const char* kk = name;
		strcpy_s(buf, jj);
		strcat_s(buf, kk);

		sqlite3_prepare_v2(db, buf, -1, &stmt, 0);

		while (sqlite3_step(stmt) != SQLITE_DONE) {
			values.radius = sqlite3_column_double(stmt, 0); //0 only increments up when calling more than one column
			values.speed = sqlite3_column_double(stmt, 1); //0 only increments up when calling more than one column
			values.mass= sqlite3_column_double(stmt, 2); //0 only increments up when calling more than one column
		}

		return values;
	}
}	