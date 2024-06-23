#include <QXYSeries>
#include "signalTest.h"

SignalTest::SignalTest(QObject *parent) : QObject(parent)
{}

void SignalTest::update(QAbstractSeries *axialSeries)
{

    auto axialxySeries = static_cast<QXYSeries *>(axialSeries);
    m_data.clear();

    int f = 100000;
    int f2 = 1000;
    int fs = 1000000;
    // Append the new data depending on the type
        for (int j(0); j < mMaxX; j++) {
            qreal x(0);
            qreal y(0);
                // data with sin + random component
                y = qSin(2 * M_PI * f * j/fs)/* + QRandomGenerator::global()->generateDouble()*/;
            y += qSin(2 * M_PI * f2 * j/fs)/* + QRandomGenerator::global()->generateDouble()*/;
                x = j;

            m_data.append(QPointF(x, y));
        }
        // setMaxX(1024/fs);
    axialxySeries->replace(m_data);
}


double SignalTest::minX() const
{
    return mMinX;
}

void SignalTest::setMinX(double newMinX)
{
    if (qFuzzyCompare(mMinX, newMinX))
        return;
    mMinX = newMinX;
    emit minXChanged();
}

double SignalTest::minY() const
{
    return mMinY;
}

void SignalTest::setMinY(double newMinY)
{
    if (qFuzzyCompare(mMinY, newMinY))
        return;
    mMinY = newMinY;
    emit minYChanged();
}

double SignalTest::maxX() const
{
    return mMaxX;
}

void SignalTest::setMaxX(double newMaxX)
{
    if (qFuzzyCompare(mMaxX, newMaxX))
        return;
    mMaxX = newMaxX;
    emit maxXChanged();
}

double SignalTest::maxY() const
{
    return mMaxY;
}

void SignalTest::setMaxY(double newMaxY)
{
    if (qFuzzyCompare(mMaxY, newMaxY))
        return;
    mMaxY = newMaxY;
    emit maxYChanged();
}

double SignalTest::minY2() const
{
    return mMinY2;
}

void SignalTest::setMinY2(double newMinY2)
{
    if (qFuzzyCompare(mMinY2, newMinY2))
        return;
    mMinY2 = newMinY2;
    emit minY2Changed();
}

double SignalTest::maxY2() const
{
    return mMaxY2;
}

void SignalTest::setMaxY2(double newMaxY2)
{
    if (qFuzzyCompare(mMaxY2, newMaxY2))
        return;
    mMaxY2 = newMaxY2;
    emit maxY2Changed();
}

double SignalTest::maxXHome() const
{
    return mMaxXHome;
}

void SignalTest::setMaxXHome(double newMaxXHome)
{
    if (qFuzzyCompare(mMaxXHome, newMaxXHome))
        return;
    mMaxXHome = newMaxXHome;
    emit maxXHomeChanged();
}

double SignalTest::minXHome() const
{
    return mMinXHome;
}

void SignalTest::setMinXHome(double newMinXHome)
{
    if (qFuzzyCompare(mMinXHome, newMinXHome))
        return;
    mMinXHome = newMinXHome;
    emit minXHomeChanged();
}

double SignalTest::maxY1Home() const
{
    return mMaxY1Home;
}

void SignalTest::setMaxY1Home(double newMaxY1Home)
{
    if (qFuzzyCompare(mMaxY1Home, newMaxY1Home))
        return;
    mMaxY1Home = newMaxY1Home;
    emit maxY1HomeChanged();
}

double SignalTest::minY1Home() const
{
    return mMinY1Home;
}

void SignalTest::setMinY1Home(double newMinY1Home)
{
    if (qFuzzyCompare(mMinY1Home, newMinY1Home))
        return;
    mMinY1Home = newMinY1Home;
    emit minY1HomeChanged();
}

double SignalTest::maxY2Home() const
{
    return mMaxY2Home;
}

void SignalTest::setMaxY2Home(double newMaxY2Home)
{
    if (qFuzzyCompare(mMaxY2Home, newMaxY2Home))
        return;
    mMaxY2Home = newMaxY2Home;
    emit maxY2HomeChanged();
}

double SignalTest::minY2Home() const
{
    return mMinY2Home;
}

void SignalTest::setMinY2Home(double newMinY2Home)
{
    if (qFuzzyCompare(mMinY2Home, newMinY2Home))
        return;
    mMinY2Home = newMinY2Home;
    emit minY2HomeChanged();
}
