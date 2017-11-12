#include "stdafx.h"
#include "JKDatabase.h"
#include "Model/JKProjectModel.h"
#include "Model/JKStockCodeModel.h"
#include "Model/JKStockCodeSettingModel.h"
#include "Model/JKStockCodeTradeModel.h"
#include "Model/JKProjectVersionModel.h"
#include <iostream>
#include <QFile>
#include "QtSql/QSqlDatabase"
#include "QtSql/QSqlError"
#include "QtSql/QSqlQuery"
#include "QVariant"


const JKString JKDatabase::dbName = "JKDatabase.jk";

JKDatabase::JKDatabase()
{
	
}

JKDatabase::~JKDatabase()
{
}

bool JKDatabase::newDatabase(JKString path)
{
	JKString fullFileName = path + "\\" + dbName;
	QFile f(QString::fromStdString(fullFileName));
	if (f.exists())
		return false;
	else
	{
		if (db)
		{
			db->close();
			JK_FreeAndNullptr(db);
		}

		db = new hiberlite::Database();
		db->open(fullFileName);

		db->registerBeanClass<JKProjectVersionModel>();
		db->registerBeanClass<JKProjectModel>();
		db->registerBeanClass<JKStockCodeModel>();
		db->registerBeanClass<JKStockCodeSettingModel>();
		db->registerBeanClass<JKStockCodeTradeModel>();

		db->dropModel();
		db->createModel();

		bean_ptr<JKProjectVersionModel> refProjectVersion = db->createBean<JKProjectVersionModel>();
		refProjectVersion->version = databaseVersion;
	}
	return true;
}

bool JKDatabase::openDatabase(JKString fullName)
{
	JKString fullFileName = fullName;
	QFile f(QString::fromStdString(fullFileName));
	if (f.exists())
	{
		this->upgradeDatabase(fullFileName);

		JK_FreeAndNullptr(db);
		db = new hiberlite::Database();
		db->open(fullFileName);

		db->registerBeanClass<JKProjectVersionModel>();
		db->registerBeanClass<JKProjectModel>();
		db->registerBeanClass<JKStockCodeModel>();
		db->registerBeanClass<JKStockCodeSettingModel>();
		db->registerBeanClass<JKStockCodeTradeModel>();


		return true;
	}
	else
		return false;
}

void JKDatabase::checkModel()
{
	vector<string> msg = db->checkModel();
	for (size_t ci = 0; ci<msg.size(); ci++)
		std::cout << "model check reported: " << msg[ci] << endl;
}

void JKDatabase::upgradeDatabase(JKString fullFileName)
{
	int version = this->getDbVersion(fullFileName);

	QSqlDatabase _db;
	_db = QSqlDatabase::addDatabase("QSQLITE");
	_db.setDatabaseName(QString::fromStdString(fullFileName));
	if (!_db.open())
	{
		QSqlError sqlError = _db.lastError();
		QString szError = sqlError.text();
		throw std::exception(szError.toStdString().c_str());
	}
	try
	{
		if (version < 2)
		{

		}
	}
	catch (const std::exception& e)
	{
		throw e;

		_db.close();
	}
	catch (...)
	{
		_db.close();
	}

	_db.close();

}

int JKDatabase::getDbVersion(JKString _dbFullFileName)
{
	QSqlDatabase _db;
	_db = QSqlDatabase::addDatabase("QSQLITE");
	_db.setDatabaseName(QString::fromStdString(_dbFullFileName));
	if (!_db.open())
	{
		QSqlError sqlError = _db.lastError();
		QString szError = sqlError.text();
		throw std::exception(szError.toStdString().c_str());
	}
	int version = -1;
	try {
		JKString querySqlStr = "select * from JKProjectVersionModel";
		QSqlQuery query(QString::fromStdString(querySqlStr), _db);
		if (query.exec())
		{
			query.next();
			version = query.value(1).toInt();
		}
		else
		{
			JKString error = JKString("exec sql error!") + querySqlStr;
			throw std::exception(error.c_str());
		}
	}
	catch (std::exception &e)
	{

		_db.close();
		throw e;
	}
	
	_db.close();
	return version;
}