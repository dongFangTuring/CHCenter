#ifndef ATTITUDEINDICATOR_H
#define ATTITUDEINDICATOR_H

#include <QWidget>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QtCore>
#include <QtGui>
#include <QDebug>


class QADI : public QWidget
{
    Q_OBJECT

public:
    QADI(QWidget *parent = nullptr);
    ~QADI() override;

    void setData(qreal r, qreal p) {
        m_roll = r;
        m_pitch = p;
        if( m_roll < -180 ) m_roll = -180;
        if( m_roll > 180  ) m_roll =  180;
        if( m_pitch < -90 ) m_pitch = -90;
        if( m_pitch > 90  ) m_pitch =  90;
        canvasReplot_slot();
    }

protected slots:
    void canvasReplot_slot(void);

protected:
    void virtual paintEvent(QPaintEvent *event) override;
    void virtual resizeEvent(QResizeEvent *event) override;

protected:
    int     m_sizeMin, m_sizeMax;           ///< widget's min/max size (in pixel)
    int     m_size, m_offset;               ///< current size & offset

    qreal  m_roll;                         ///< roll angle (in degree)
    qreal  m_pitch;                        ///< pitch angle (in degree)

};


/**
 * @brief The QCompass class
 */

class QCompass : public QWidget
{
    Q_OBJECT

public:
    QCompass(QWidget *parent = nullptr);
    ~QCompass() override;
    void setYaw(float val) {
        m_yaw  = val;
        if( m_yaw < 0   ) m_yaw = 360 + m_yaw;
        if( m_yaw > 360 ) m_yaw = m_yaw - 360;
        canvasReplot_slot();
    }

protected slots:
    void canvasReplot_slot(void);

protected:
    void virtual paintEvent(QPaintEvent *event) override;
    void virtual resizeEvent(QResizeEvent *event) override;

protected:
    int m_sizeMin, m_sizeMax;               ///< widget min/max size (in pixel)
    int m_size, m_offset;                   ///< widget size and offset size

    qreal m_yaw;                              ///< yaw angle (in degree)

};
#endif
