#ifndef GANE_ANALYTICS_WRAPPER_H
#define GANE_ANALYTICS_WRAPPER_H

class GameAnalyticsWrapper
{
MAKE_INSTANCE_H(GameAnalyticsWrapper);

public:
    void startSession();
    void stopSession();

    void setAccount(string acctid);
    void setAccountType(int t);
    void setLevel(int t);
    void setGameServer(string gameServer);

    void fireEvent(string evtName, string data);

    void chargeRequest(string orderId, string product_id, double currencyAmount, string currencyType, double diamondAmount, string paymentType);
    void chargeSuccess(string orderId);
    void reward(double diamondAmount, string reason);

    void purchase(string id, int cnt, double price);

    void stageBegin(string id);
    void stageCompleted(string id);
    void stageFailed(string id, string reason);
};

#endif//GANE_ANALYTICS_WRAPPER_H
