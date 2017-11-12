#include "stdafx.h"
#include "JKStockTradeUtil.h"
#include "JKStockCodeBLL.h"
#include "JKStockCodeTradeBLL.h"
#include "JKProjectBLL.h"

JKStockTradeUtil::JKStockTradeUtil(JKRef_Ptr<JKProjectBLL> _refProject)
	: JKReferenced()
	, refProject(_refProject)
{
}

JKStockTradeUtil::~JKStockTradeUtil()
{
}

double JKStockTradeUtil::getTradeBuyCost(JKRef_Ptr<JKStockCodeTradeBLL> refStockCodeTrade)
{
	double buyPureCost = refStockCodeTrade->getBuyPureCost();
	double buyTax = this->getBuyTax(buyPureCost);
	return  (buyPureCost + buyTax);
}

double JKStockTradeUtil::getTradeBuyCostPrice(JKRef_Ptr<JKStockCodeTradeBLL> refStockCodeTrade)
{
	double buyPureCost = refStockCodeTrade->getBuyPureCost();
	double buyTax = this->getBuyTax(buyPureCost);
	
	return  (buyPureCost + buyTax) / refStockCodeTrade->getCount();
}

//double JKStockTradeUtil::getTradeSellCost(double latestPrice, JKRef_Ptr<JKStockCodeTradeBLL> refStockCodeTrade)
//{
//	double buyCost = refStockCodeTrade->getCount() * latestPrice;
//	double buyTax = this->getSellTax(buyCost);
//
//	return  (buyCost + buyTax)/refStockCodeTrade->getCount();
//}

double JKStockTradeUtil::getRealEarning(double latestPrice, JKRef_Ptr<JKStockCodeTradeBLL> refStockCodeTrade)
{
	double buyCost = getTradeBuyCost(refStockCodeTrade);
	double pureSellCost = latestPrice*refStockCodeTrade->getCount();
	
	double sellTax = this->getSellTax(pureSellCost);

	return  pureSellCost - buyCost - sellTax;
}

double JKStockTradeUtil::getRealEarningPercent(double latestPrice, JKRef_Ptr<JKStockCodeTradeBLL> refStockCodeTrade)
{
	return getRealEarning(latestPrice, refStockCodeTrade) / refStockCodeTrade->getBuyPureCost();
}

double JKStockTradeUtil::getCommission(double buyCost)
{
	double commission = 0;
	if (buyCost <= 10000)
		commission = 10000 * refProject->getCommission();
	else
		commission = buyCost * refProject->getCommission();

	return commission; //Ӷ��
}

double JKStockTradeUtil::getStampTax(double buyCost)
{
	double stampPercent = refProject->getStampTax();
	double stamp = buyCost * stampPercent;

	return stamp;
}

double JKStockTradeUtil::getTransfer(double buyCost)
{
	double transferPercent = refProject->getTransfer();
	double transfer = transferPercent * buyCost;

	return transfer;
}

double JKStockTradeUtil::getBuyTax(double buyCost)
{
	assert(refProject.valid());
	double transfer = this->getTransfer(buyCost);

	double commission = this->getCommission(buyCost);
	
	return transfer + commission; //������ + Ӷ��
}

double JKStockTradeUtil::getSellTax(double sellCost)
{
	assert(refProject.valid());
	double transfer = this->getTransfer(sellCost);

	double commission = this->getCommission(sellCost);

	double stamp = this->getStampTax(sellCost);

	return transfer + commission + stamp; //������ + Ӷ�� + ӡ��˰
}