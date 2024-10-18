#include <QFile>
#include <QXYSeries>
#include <biquad.h>
#include <include/DspFilters/Butterworth.h>
// #include "butterworth.h"
#include "signalTest.h"


#define PI 3.14159
vector<double> filterr(vector<double>x, vector<double> coeff_b, vector<double> coeff_a)
{
    int len_x = x.size();
    int len_b = coeff_b.size();
    int len_a = coeff_a.size();

    // vector<double> zi(len_b);

    vector<double> filter_x(len_x);
    // filter_x[0] = coeff_b[0] * x[0];
    // filter_x[]
    double zi = 0;
    if (len_a == 1)
    {
        for (int m = len_a; m<len_x; m++)
        {
            filter_x[m] = coeff_b[0] * x[m] + zi;
            for (int i = 1; i<len_b; i++)
            {
                zi = coeff_b[i] * x[m-i] - coeff_a[i] * x[m-1];//-coeff_a[i]*filter_x[m];
            }
        }
    }
    else
    {
        for (int m = 0; m<len_x; m++)
        {
            filter_x[m] = coeff_b[0] * x[m] + zi;
            for (int i = 1; i<len_b; i++)
            {
                zi = coeff_b[i] * x[m-i] - coeff_a[i] * x[m-1];//-coeff_a[i]*filter_x[m];
            }
        }
    }

    return filter_x;
}

vector<double> TrinomialMultiply(int FilterOrder, vector<double> b, vector<double> c)
{
    int i, j;
    vector<double> RetVal(4 * FilterOrder);

    RetVal[2] = c[0];
    RetVal[3] = c[1];
    RetVal[0] = b[0];
    RetVal[1] = b[1];

    for (i = 1; i < FilterOrder; ++i)
    {
        RetVal[2 * (2 * i + 1)] += c[2 * i] * RetVal[2 * (2 * i - 1)] - c[2 * i + 1] * RetVal[2 * (2 * i - 1) + 1];
        RetVal[2 * (2 * i + 1) + 1] += c[2 * i] * RetVal[2 * (2 * i - 1) + 1] + c[2 * i + 1] * RetVal[2 * (2 * i - 1)];

        for (j = 2 * i; j > 1; --j)
        {
            RetVal[2 * j] += b[2 * i] * RetVal[2 * (j - 1)] - b[2 * i + 1] * RetVal[2 * (j - 1) + 1] +
                             c[2 * i] * RetVal[2 * (j - 2)] - c[2 * i + 1] * RetVal[2 * (j - 2) + 1];
            RetVal[2 * j + 1] += b[2 * i] * RetVal[2 * (j - 1) + 1] + b[2 * i + 1] * RetVal[2 * (j - 1)] +
                                 c[2 * i] * RetVal[2 * (j - 2) + 1] + c[2 * i + 1] * RetVal[2 * (j - 2)];
        }

        RetVal[2] += b[2 * i] * RetVal[0] - b[2 * i + 1] * RetVal[1] + c[2 * i];
        RetVal[3] += b[2 * i] * RetVal[1] + b[2 * i + 1] * RetVal[0] + c[2 * i + 1];
        RetVal[0] += b[2 * i];
        RetVal[1] += b[2 * i + 1];
    }

    return RetVal;
}

vector<double> ComputeLP(int FilterOrder)
{
    vector<double> NumCoeffs(FilterOrder + 1);
    int m;
    int i;

    NumCoeffs[0] = 1;
    NumCoeffs[1] = FilterOrder;
    m = FilterOrder / 2;
    for (i = 2; i <= m; ++i)
    {
        NumCoeffs[i] = (double)(FilterOrder - i + 1)*NumCoeffs[i - 1] / i;
        NumCoeffs[FilterOrder - i] = NumCoeffs[i];
    }
    NumCoeffs[FilterOrder - 1] = FilterOrder;
    NumCoeffs[FilterOrder] = 1;

    return NumCoeffs;
}
vector<double> ComputeHP(int FilterOrder)
{
    vector<double> NumCoeffs;
    int i;

    NumCoeffs = ComputeLP(FilterOrder);

    for (i = 0; i <= FilterOrder; ++i)
        if (i % 2) NumCoeffs[i] = -NumCoeffs[i];

    return NumCoeffs;
}


vector<double> ComputeNumCoeffs(int FilterOrder, double Lcutoff, double Ucutoff, vector<double> DenC)
{
    vector<double> TCoeffs;
    vector<double> NumCoeffs(2 * FilterOrder + 1);
    vector<complex<double>> NormalizedKernel(2 * FilterOrder + 1);

    vector<double> Numbers;
    for (double n = 0; n < FilterOrder * 2 + 1; n++)
        Numbers.push_back(n);
    int i;

    TCoeffs = ComputeHP(FilterOrder);

    for (i = 0; i < FilterOrder; ++i)
    {
        NumCoeffs[2 * i] = TCoeffs[i];
        NumCoeffs[2 * i + 1] = 0.0;
    }
    NumCoeffs[2 * FilterOrder] = TCoeffs[FilterOrder];

    double cp[2];
    double Bw, Wn;
    cp[0] = 2 * 2.0*tan(PI * Lcutoff / 2.0);
    cp[1] = 2 * 2.0*tan(PI * Ucutoff / 2.0);

    Bw = cp[1] - cp[0];
    //center frequency
    Wn = sqrt(cp[0] * cp[1]);
    Wn = 2 * atan2(Wn, 4);
    double kern;
    const std::complex<double> result = std::complex<double>(-1, 0);

    for (int k = 0; k< FilterOrder * 2 + 1; k++)
    {
        NormalizedKernel[k] = std::exp(-sqrt(result)*Wn*Numbers[k]);
    }
    double b = 0;
    double den = 0;
    for (int d = 0; d < FilterOrder * 2 + 1; d++)
    {
        b += real(NormalizedKernel[d] * NumCoeffs[d]);
        den += real(NormalizedKernel[d] * DenC[d]);
    }
    for (int c = 0; c < FilterOrder * 2 + 1; c++)
    {
        NumCoeffs[c] = (NumCoeffs[c] * den) / b;
    }

    for (int i = NumCoeffs.size() - 1; i > FilterOrder * 2 + 1; i--)
        NumCoeffs.pop_back();

    return NumCoeffs;
}


vector<double> ComputeDenCoeffs(int FilterOrder, double Lcutoff, double Ucutoff)
{
    int k;            // loop variables
    double theta;     // PI * (Ucutoff - Lcutoff) / 2.0
    double cp;        // cosine of phi
    double st;        // sine of theta
    double ct;        // cosine of theta
    double s2t;       // sine of 2*theta
    double c2t;       // cosine 0f 2*theta
    vector<double> RCoeffs(2 * FilterOrder);     // z^-2 coefficients
    vector<double> TCoeffs(2 * FilterOrder);     // z^-1 coefficients
    vector<double> DenomCoeffs;     // dk coefficients
    double PoleAngle;      // pole angle
    double SinPoleAngle;     // sine of pole angle
    double CosPoleAngle;     // cosine of pole angle
    double a;         // workspace variables

    cp = cos(PI * (Ucutoff + Lcutoff) / 2.0);
    theta = PI * (Ucutoff - Lcutoff) / 2.0;
    st = sin(theta);
    ct = cos(theta);
    s2t = 2.0*st*ct;        // sine of 2*theta
    c2t = 2.0*ct*ct - 1.0;  // cosine of 2*theta

    for (k = 0; k < FilterOrder; ++k)
    {
        PoleAngle = PI * (double)(2 * k + 1) / (double)(2 * FilterOrder);
        SinPoleAngle = sin(PoleAngle);
        CosPoleAngle = cos(PoleAngle);
        a = 1.0 + s2t*SinPoleAngle;
        RCoeffs[2 * k] = c2t / a;
        RCoeffs[2 * k + 1] = s2t*CosPoleAngle / a;
        TCoeffs[2 * k] = -2.0*cp*(ct + st*SinPoleAngle) / a;
        TCoeffs[2 * k + 1] = -2.0*cp*st*CosPoleAngle / a;
    }

    DenomCoeffs = TrinomialMultiply(FilterOrder, TCoeffs, RCoeffs);

    DenomCoeffs[1] = DenomCoeffs[0];
    DenomCoeffs[0] = 1.0;
    for (k = 3; k <= 2 * FilterOrder; ++k)
        DenomCoeffs[k] = DenomCoeffs[2 * k - 2];

    for (int i = DenomCoeffs.size() - 1; i > FilterOrder * 2 + 1; i--)
        DenomCoeffs.pop_back();

    return DenomCoeffs;
}

SignalTest::SignalTest(QObject *parent) : QObject(parent)
{}

void SignalTest::update(QAbstractSeries *axialSeries, QAbstractSeries *filter)
{

    auto axialxySeries = static_cast<QXYSeries *>(axialSeries);

    auto filterSeries = static_cast<QXYSeries *>(filter);

    int f = 5000000;
    int f2 = 50000;
    int fs = 2083000;
    int N = 2048;
    int res = fs/N;
    float freq[N];
    // QFile file("C:/Users/HP/Desktop/HIT_1_1.txt");
    // file.open(QIODevice::ReadOnly);
    // QTextStream in(&file);
    // int j = 0;
    // while (!in.atEnd()) {
    //     float s = in.readLine().toFloat();
    //     firstData[j++] = s;
    // }
    // for (int i = j; i < 40000; ++i) {
    //     firstData[i] = 0;
    // }
    // Append the new data depending on the type
    for (int j(0); j < 40000; j++) {
        qreal x(0);
        qreal y(0);
        // data with sin + random component
        y = qSin(2 * M_PI * f * j/fs)/* + QRandomGenerator::global()->generateDouble()*/;
        y += qSin(2 * M_PI * f2 * j/fs)/* + QRandomGenerator::global()->generateDouble()*/;
        x = (double)j/(double)fs;

        firstData[j] =y;
    }


    // Dsp::Butterworth::LowPassBase lopass;
    // lopass.setup(6, fs, 50000);
    // Dsp::Butterworth::LowPassBase::StateBase<double> t;
    // lopass.process(5000, firstData, t);


    auto start = std::chrono::high_resolution_clock::now();
    ffft::FFTReal <float> fft_object (N);




    //////////////////
    ///
    vector<double> a;
    vector<double> b;
    double FrequencyBands[2] = { 10000.0, 100000.0 };
    a = ComputeDenCoeffs(10,  FrequencyBands[0], FrequencyBands[1]);
    for (int k = 0; k<a.size(); k++)
    {
        qDebug() << "DenC is: %lf\n" << a[k];
    }

    b = ComputeNumCoeffs(10, FrequencyBands[0], FrequencyBands[1], a);
    for (int k = 0; k<b.size(); k++)
    {
        qDebug() << "NumC is: %lf\n" << b[k];
    }
    vector<double> x2(40000);
    for (int i = 0; i < x2.size(); ++i) {
        x2[i] = firstData[i];
    }
    vector<double> y2(40000);

    y2 = filterr(x2, b,a);
    for (int i = 0; i < y2.size(); ++i) {
        firstData[i] = y2[i];
    }
    // std::copy(y2.begin(), y2.end(), firstData);

    fft_object.do_fft (freq, firstData);
    float max = 100000.0;
    int k = 0;
    for (int i = 0; i < N/2; ++i) {
        mSignal.append(QPointF(i*res, std::abs(freq[i])));
    }

    for (int i = 0; i < N/2; ++i) {
        mSignal.append(QPointF((i-N/2)*res, std::abs(freq[N-i])));
    }
    ///
    /// /////////////////////

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    qDebug() << duration.count();
    // for (int i = 0; i < N; ++i) {
    //     mFiltered.append(QPointF(i, freq[i]));
    // }
    axialxySeries->replace(mSignal);
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
