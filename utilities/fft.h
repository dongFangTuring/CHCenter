#ifndef FFT_H
#define FFT_H

#include <QObject>
#include <QVector>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <QDebug>
#include <QThread>

#define MAX_N 8192    //FFT點數


class fft_trans:public QObject
{
    Q_OBJECT
public:
    explicit fft_trans(QObject *parent = nullptr);
    ~fft_trans();
    /*
     * 初始化FFT訊號序列大小（參與計算的數據長度，真實輸入的訊號序列會進行多刪少補）
    */
    int32_t init_size(int32_t new_size);
    QVector<double> fft_data;

private:
    struct complex{         //定義一個結構體表示覆數的型別
        double real;
        double imag;
    };

    void change();
    void transform();
    void fft();
    double cal_norm_and_find_max(complex* x, int32_t size_of_x, double *norm, int32_t size_of_norm);
    void add(complex a, complex b, complex *c);
    void sub(complex a, complex b, complex *c);
    void mul(complex a, complex b, complex *c);
    complex x[MAX_N], *W;   //定義輸入序列和旋轉因子
    double l[MAX_N] = {0};
    int size = MAX_N;   //參與計算的訊號序列長度，必須是2的冪次方
    const double PI = atan(1)*4; //定義π 因為tan(π/4)=1 所以arctan（1）* 4=π，增加π的精度
public slots:
    /*
     * index: 序號，用於區分不同訊號序列的計算結果
     * sample_frq: 訊號序列的採樣頻率，錯誤的採樣頻率會導致頻譜圖的對映關係出錯
     * data: 參與計算的訊號序列
    */
    void fft_calculate(qint8 index, qint32 sample_frq, QVector<double> data);//index只是區分是哪個曲線的FFT結果
signals:
    /*
     * 訊號需要連線到槽才可使用
     * index: 序號，用於區分不同訊號序列的計算結果
     * x_ticks: 頻譜圖的X軸
     * result : 頻譜圖的Y軸
    */
    void fft_result(qint8 index, QVector<double> x_ticks, QVector<double> result);
};

#endif // FFT_H
