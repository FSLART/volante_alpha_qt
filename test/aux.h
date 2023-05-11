#ifndef AUX_H
#define AUX_H
#include <QFile>
#include <QObject>
#include <QTest>
#include <QRandomGenerator>
class AuxSingleton{
	public:
    	static AuxSingleton& getInstance();
		int randomInt(int offset=0, int n=255);
    	AuxSingleton(const AuxSingleton&) = delete;
    	AuxSingleton& operator=(const AuxSingleton&) = delete;

	private:
        AuxSingleton() = default;
};
#endif // AUX_H
