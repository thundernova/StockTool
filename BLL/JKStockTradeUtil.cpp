#include "stdafx.h"
#include "JKStockTradeUtil.h"
#include "JKStockCodeBLL.h"
#include "JKStockCodeTradeBLL.h"
#include "JKProjectBLL.h"

JKStockTradeUtil::JKStockTradeUtil(JKRef_Ptr<JKProjectBLL> _refProject)
	: JKReferenced()
{
	if (_refProject.valid())
	{
		stampTax = _refProject->getStampTax();
		transfer = _refProject->getTransfer();
		commission = _refProject->getCommission();
	}
}

JKStockTradeUtil::JKStockTradeUtil(float _stampTax, float _transfer, float _commission)
{
	stampTax = _stampTax;
	transfer = _transfer;
	commission = _commission;
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

double JKStockTradeUtil::getExpactEarning(double latestPrice, JKRef_Ptr<JKStockCodeTradeBLL> refStockCodeTrade)
{
	size_t couldSellCount = refStockCodeTrade->getCouldSellCount();
	size_t sumCount = refStockCodeTrade->getCount();

	double buyCost = getTradeBuyCost(refStockCodeTrade)*couldSellCount / sumCount;

	double pureSellCost = latestPrice*couldSellCount;

	double sellTax = this->getSellTax(pureSellCost);

	return  pureSellCost - buyCost - sellTax;
}

double JKStockTradeUtil::getExpactEarningPercent(double latestPrice, JKRef_Ptr<JKStockCodeTradeBLL> refStockCodeTrade)
{
	return getExpactEarning(latestPrice, refStockCodeTrade) / refStockCodeTrade->getBuyPureCost();
}

double JKStockTradeUtil::getRealEarning(JKRef_Ptr<JKStockCodeTradeBLL> refStockCodeTrade)
{
	return refStockCodeTrade->getRealEarning();
}

double JKStockTradeUtil::getRealEarningPercent(JKRef_Ptr<JKStockCodeTradeBLL> refStockCodeTrade)
{
	size_t soldCount = refStockCodeTrade->getSoldCount();
	size_t sumCount = refStockCodeTrade->getCount();

	return getRealEarning(refStockCodeTrade) / (refStockCodeTrade->getBuyPureCost()*soldCount/sumCount);
}

double JKStockTradeUtil::getCommission(double buyCost)
{
	double _commission = 0;
	if (buyCost <= 10000)
		_commission = 10000 * commission;
	else
		_commission = buyCost * commission;

	return _commission; //佣金
}

double JKStockTradeUtil::getStampTax(double buyCost)
{
	double stampPercent = stampTax;
	double stamp = buyCost * stampPercent;

	return stamp;
}

double JKStockTradeUtil::getTransfer(double buyCost)
{
	double transferPercent = transfer;
	double transfer = transferPercent * buyCost;

	return transfer;
}

double JKStockTradeUtil::getBuyTax(double buyCost)
{
	double transfer = this->getTransfer(buyCost);

	double commission = this->getCommission(buyCost);
	
	return transfer + commission; //过户费 + 佣金
}

double JKStockTradeUtil::getSellTax(double sellCost)
{
	double transfer = this->getTransfer(sellCost);

	double commission = this->getCommission(sellCost);

	double stamp = this->getStampTax(sellCost);

	return transfer + commission + stamp; //过户费 + 佣金 + 印花税
}