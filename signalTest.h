#ifndef SIGNAL_H
#define SIGNAL_H

#include <QAbstractSeries>
#include <QObject>
#include "fftreal/FFTReal.h"

class SignalTest : public QObject
{
    Q_OBJECT
public:
    SignalTest(QObject *parent = nullptr);


    Q_INVOKABLE void update(QAbstractSeries *axialSeries, QAbstractSeries *filter);
    double minX() const;
    void setMinX(double newMinX);

    double minY() const;
    void setMinY(double newMinY);

    double maxX() const;
    void setMaxX(double newMaxX);

    double maxY() const;
    void setMaxY(double newMaxY);

    double minY2() const;
    void setMinY2(double newMinY2);

    double maxY2() const;
    void setMaxY2(double newMaxY2);

    double maxXHome() const;
    void setMaxXHome(double newMaxXHome);

    double minXHome() const;
    void setMinXHome(double newMinXHome);

    double maxY1Home() const;
    void setMaxY1Home(double newMaxY1Home);

    double minY1Home() const;
    void setMinY1Home(double newMinY1Home);

    double maxY2Home() const;
    void setMaxY2Home(double newMaxY2Home);

    double minY2Home() const;
    void setMinY2Home(double newMinY2Home);

signals:
    void minXChanged();

    void minYChanged();

    void maxXChanged();

    void maxYChanged();

    void minY2Changed();

    void maxY2Changed();

    void maxXHomeChanged();

    void minXHomeChanged();

    void maxY1HomeChanged();

    void minY1HomeChanged();

    void maxY2HomeChanged();

    void minY2HomeChanged();

private:
    double mMinX{0};
    double mMaxX{1024};
    double mMaxY{500};
    double mMinY{-300};


    double mMinY2{0};
    double mMaxY2{1600};
    double mMaxXHome{0.002};
    double mMinXHome{0};
    double mMaxY1Home{2};
    double mMinY1Home{-2};
    double mMaxY2Home{1600};
    double mMinY2Home{0};
    QList<QPointF> m_data;
    QList<QPointF> mFiltered;
    float firstData[2000];

    Q_PROPERTY(double minX READ minX WRITE setMinX NOTIFY minXChanged FINAL)
    Q_PROPERTY(double minY READ minY WRITE setMinY NOTIFY minYChanged FINAL)
    Q_PROPERTY(double maxX READ maxX WRITE setMaxX NOTIFY maxXChanged FINAL)
    Q_PROPERTY(double maxY READ maxY WRITE setMaxY NOTIFY maxYChanged FINAL)
    Q_PROPERTY(double minY2 READ minY2 WRITE setMinY2 NOTIFY minY2Changed FINAL)
    Q_PROPERTY(double maxY2 READ maxY2 WRITE setMaxY2 NOTIFY maxY2Changed FINAL)
    Q_PROPERTY(double maxXHome READ maxXHome WRITE setMaxXHome NOTIFY maxXHomeChanged FINAL)
    Q_PROPERTY(double minXHome READ minXHome WRITE setMinXHome NOTIFY minXHomeChanged FINAL)
    Q_PROPERTY(double maxY1Home READ maxY1Home WRITE setMaxY1Home NOTIFY maxY1HomeChanged FINAL)
    Q_PROPERTY(double minY1Home READ minY1Home WRITE setMinY1Home NOTIFY minY1HomeChanged FINAL)
    Q_PROPERTY(double maxY2Home READ maxY2Home WRITE setMaxY2Home NOTIFY maxY2HomeChanged FINAL)
    Q_PROPERTY(double minY2Home READ minY2Home WRITE setMinY2Home NOTIFY minY2HomeChanged FINAL)
};

#endif // SIGNAL_H
