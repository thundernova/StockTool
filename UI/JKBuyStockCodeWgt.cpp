#include "stdafx.h"
#include "JKBuyStockCodeWgt.h"
#include "BLL\JKProjectBLL.h"
#include "BLL\JKStockCodeBLL.h"
#include "BLL\JKStockCodeTradeBLL.h"
#include <qdatetime.h>

JKBuyStockCodeWgt::JKBuyStockCodeWgt(JKRef_Ptr<JKStockCodeBLL> _refStockCode, QWidget* parent)
	: QDialog(parent)
	, refStockCode(_refStockCode)
{
	ui.setupUi(this);

	this->initUI();
}

JKBuyStockCodeWgt::~JKBuyStockCodeWgt()
{

}

void JKBuyStockCodeWgt::okClicked()
{
	QString buyDate = ui.lEditDate->date().toString(Qt::ISODate);
	QString buyCount = ui.lEdtCount->text();
	QString buyPrice = ui.lEditPrice->text();

	double price = buyPrice.toDouble();
	JKUInt count = buyCount.toUInt();

	JKRef_Ptr<JKStockCodeTradeBLL> _refStockCodeTrade = refStockCode->newStockCodeTrade();
	_refStockCodeTrade->setParams(TradeType::BUY, buyDate.toStdString(), count, price);

	this->accept();

}
void JKBuyStockCodeWgt::cancleClicked()
{
	this->reject();
}

void JKBuyStockCodeWgt::initUI()
{
	ui.lEditDate->setDate(QDate::currentDate());


	connect(ui.pBtnOK, SIGNAL(clicked()), this, SLOT(okClicked()));
	connect(ui.pBtnCancel, SIGNAL(clicked()), this, SLOT(cancleClicked()));

}