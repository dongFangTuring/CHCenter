#include "attitudeindicator.h"

QADI::QADI(QWidget *parent)
    : QWidget(parent)
{
    m_sizeMin = 150;
    m_sizeMax = 150;
    m_offset = 2;
    m_size = m_sizeMin - 2*m_offset;

    setMinimumSize(m_sizeMin, m_sizeMin);
    setMaximumSize(m_sizeMax, m_sizeMax);
    resize(m_sizeMin, m_sizeMin);

    setFocusPolicy(Qt::NoFocus);

    m_roll  = 0.0;
    m_pitch = 0.0;
}

QADI::~QADI()
{
}


void QADI::canvasReplot_slot(void)
{
    update();
}


void QADI::resizeEvent(QResizeEvent *event)
{
    m_size = qMin(width(),height()) - 2*m_offset;
}

void QADI::paintEvent(QPaintEvent *event)
{
//    QTime t;
//    t.start();

    QPainter painter(this);

    QBrush bgSky(QColor(142,198,221));
    QBrush bgGround(QColor(140,99,55));

    QPen   whitePen(Qt::white);
    QPen   blackPen(Qt::black);
    QPen   pitchPen(Qt::white);
    QPen   pitchZero(Qt::white);

    whitePen.setWidth(2);
    blackPen.setWidth(2);
    pitchZero.setWidth(3);

    //painter.setRenderHints(QPainter::Antialiasing);

    painter.translate(width() / 2, height() / 2);



    // FIXME: AHRS output left-hand values
    qreal pitch_tem = -m_pitch;

    // draw background
    {

        painter.rotate(-m_roll);

        int y_min, y_max;

        y_min = int(m_size/2*-40.0/45.0);
        y_max = int(m_size/2* 40.0/45.0);

        int y = int(-m_size/2*pitch_tem/45.);
        if( y < y_min ) y = y_min;
        if( y > y_max ) y = y_max;

        int x = int(sqrt(m_size*m_size/4 - y*y));
        qreal gr = atan((qreal)(y)/x);
        gr = gr * 180./3.1415926;

        painter.setPen(blackPen);
        painter.setBrush(bgSky);
        painter.drawChord(-m_size/2, -m_size/2, m_size, m_size,
                          int(gr*16), int((180-2*gr)*16));

        painter.setBrush(bgGround);
        painter.drawChord(-m_size/2, -m_size/2, m_size, m_size,
                          int(gr*16), int(-(180+2*gr)*16));

    }


    // set mask
    QRegion maskRegion(-m_size/2, -m_size/2, m_size, m_size, QRegion::Ellipse);
    painter.setClipRegion(maskRegion);


    // draw pitch lines & marker
    {
        int x, y, x1, y1;
        int textWidth;
        qreal p, r;
        int ll = m_size/8, l;

        int fontSize = 10;
        QString s;

        pitchPen.setWidth(2);

        QFont font_pitch=QFont("", fontSize);
        //font_pitch.setStyleStrategy(QFont::NoAntialias);
        font_pitch.setWeight(QFont::Bold);
        painter.setFont(font_pitch);


        // draw lines
        for(short i=-9; i<=9; i=i+3) {
            p = i*10;

            s = QString("%1").arg(p);

            if( i % 3 == 0 )
                l = ll;
            else
                l = ll/2;

            if( i == 0 ) {
                painter.setPen(pitchZero);
                l = l * 1.8;
            } else {
                painter.setPen(pitchPen);
            }

            y = int(m_size/2*p/45.0 + m_size/2*pitch_tem/45.);
            x = l;

            r = sqrt(x*x + y*y);
            if( r > m_size/2 ) continue;

            painter.drawLine(QPointF(-l, 1.0*y), QPointF(l, 1.0*y));

            textWidth = 100;

            if( i % 3 == 0 && i != 0 ) {
                painter.setPen(QPen(Qt::white));

                x1 = -x-2-textWidth;
                y1 = y - fontSize/2 - 1;
                painter.drawText(QRectF(x1, y1, textWidth, fontSize+2),
                                 Qt::AlignRight|Qt::AlignVCenter, s);
            }
        }

        painter.rotate(m_roll);

        // draw marker
        int     markerSize = m_size/10;
        qreal   fx1, fy1, fx2, fy2, fx3, fy3;

        painter.setBrush(QBrush(QColor(235,149,58)));
        painter.setPen(Qt::NoPen);

        fx1 = markerSize;
        fy1 = 0;
        fx2 = fx1 + markerSize;
        fy2 = -markerSize/2;
        fx3 = fx1 + markerSize;
        fy3 = markerSize/2;

        QPointF points[3] = {
            QPointF(fx1, fy1),
            QPointF(fx2, fy2),
            QPointF(fx3, fy3)
        };
        painter.drawPolygon(points, 3);

        QPointF points2[3] = {
            QPointF(-fx1, fy1),
            QPointF(-fx2, fy2),
            QPointF(-fx3, fy3)
        };
        painter.drawPolygon(points2, 3);
    }


    // draw roll degree lines
    {
        int     nRollLines = 12;
        qreal   rotAng = 360.0 / nRollLines;
        int     rollLineLeng = m_size/25;
        qreal   fx1, fy1, fx2, fy2;
        int     fontSize = 10;
        QString s;

        QFont font_roll=QFont("", fontSize);
        font_roll.setStyleStrategy(QFont::NoAntialias);
        font_roll.setWeight(QFont::Bold);
        blackPen.setWidth(3);
        painter.setPen(blackPen);
        painter.setFont(font_roll);

        for(short i=0; i<nRollLines; i++) {
            if( i < nRollLines/2 )
                s = QString("%1").arg(-i*rotAng);
            else
                s = QString("%1").arg(360-i*rotAng);

            fx1 = 0;
            fy1 = -m_size/2 + m_offset;
            fx2 = 0;

            if( i % 3 == 0 ) {
                fy2 = fy1 + rollLineLeng;
                painter.drawLine(QPointF(fx1, fy1), QPointF(fx2, fy2));

                fy2 = fy1 + rollLineLeng+2;
                painter.drawText(QRectF(-50, fy2, 100, fontSize+2),
                                 Qt::AlignCenter, s);
            } else {
                fy2 = fy1 + rollLineLeng/2;
                painter.drawLine(QPointF(fx1, fy1), QPointF(fx2, fy2));
            }

            painter.rotate(rotAng);
        }
    }

    // draw roll marker
    {
        int     rollMarkerSize = m_size/25;
        qreal  fx1, fy1, fx2, fy2, fx3, fy3;

        painter.rotate(-m_roll);
        painter.setBrush(QBrush(Qt::black));

        fx1 = 0;
        fy1 = -m_size/2 + m_offset;
        fx2 = fx1 - rollMarkerSize/2;
        fy2 = fy1 + rollMarkerSize;
        fx3 = fx1 + rollMarkerSize/2;
        fy3 = fy1 + rollMarkerSize;

        QPointF points[3] = {
            QPointF(fx1, fy1),
            QPointF(fx2, fy2),
            QPointF(fx3, fy3)
        };
        painter.drawPolygon(points, 3);
    }


//    qDebug()<<t.elapsed();
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


QCompass::QCompass(QWidget *parent)
    : QWidget(parent)
{

    m_sizeMin = 150;
    m_sizeMax = 150;
    m_offset = 2;
    m_size = m_sizeMin - 2*m_offset;

    setMinimumSize(m_sizeMin, m_sizeMin);
    setMaximumSize(m_sizeMax, m_sizeMax);
    resize(m_sizeMin, m_sizeMin);

    setFocusPolicy(Qt::NoFocus);

    m_yaw  = 0.0;
}

QCompass::~QCompass()
{
}


void QCompass::canvasReplot_slot(void)
{
    update();
}

void QCompass::resizeEvent(QResizeEvent *event)
{
    m_size = qMin(width(),height()) - 2*m_offset;
}

void QCompass::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QBrush bgGround(QColor(41,45,46));

    QColor materialred=QColor(226,81,65);
    QPen   whitePen(Qt::white);
    QPen   blackPen(Qt::black);
    QPen   redPen(materialred);

    whitePen.setWidth(1);
    blackPen.setWidth(2);
    redPen.setWidth(2);

    painter.setRenderHint(QPainter::Antialiasing);

    painter.translate(width() / 2, height() / 2);


    // draw background
    {
        painter.setPen(blackPen);
        painter.setBrush(bgGround);

        painter.drawEllipse(-m_size/2, -m_size/2, m_size, m_size);
    }


    // draw yaw lines
    {
        int     nyawLines = 12;
        qreal   rotAng = 360.0 / nyawLines;
        int     yawLineLeng = m_size/25;
        qreal  fx1, fy1, fx2, fy2;
        int     fontSize = 10;
        QString s;

        QFont font_yaw=QFont("", fontSize);
        font_yaw.setWeight(QFont::Bold);
        painter.setPen(whitePen);

        for(short i=0; i<nyawLines; i++) {

            if( i == 0 ) {
                s = "N";
                painter.setPen(whitePen);

                painter.setFont(font_yaw);
            } else if ( i == 9 ) {
                s = "W";
                painter.setPen(whitePen);

                painter.setFont(font_yaw);
            } else if ( i == 18 ) {
                s = "S";
                painter.setPen(whitePen);

                painter.setFont(font_yaw);
            } else if ( i == 27 ) {
                s = "E";
                painter.setPen(whitePen);

                painter.setFont(font_yaw);
            } else {
                s = QString("%1").arg(i*rotAng);
                painter.setPen(whitePen);

                painter.setFont(font_yaw);
            }

            fx1 = 0;
            fy1 = -m_size/2 + m_offset;
            fx2 = 0;

            if( i % 3 == 0 ) {
                fy2 = fy1 + yawLineLeng;
                painter.drawLine(QPointF(fx1, fy1), QPointF(fx2, fy2));

                fy2 = fy1 + yawLineLeng+4;
                painter.drawText(QRectF(-50, fy2, 100, fontSize+2),
                                 Qt::AlignCenter, s);
            } else {
                fy2 = fy1 + yawLineLeng/2;
                painter.drawLine(QPointF(fx1, fy1), QPointF(fx2, fy2));
            }

            painter.rotate(-rotAng);
        }
    }

    // draw S/N arrow
    {
        int     arrowWidth = m_size/6;
        qreal  fx1, fy1, fx2, fy2, fx3, fy3;

        painter.rotate(-m_yaw);
        fx1 = 0;
        fy1 = -m_size/2 + m_offset + m_size/25 + 15;
        fx2 = -arrowWidth/2;
        fy2 = 0;
        fx3 = arrowWidth/2;
        fy3 = 0;

        painter.setPen(Qt::NoPen);

        painter.setBrush(QBrush(materialred));
        QPointF pointsN[3] = {
            QPointF(fx1, fy1),
            QPointF(fx2, fy2),
            QPointF(fx3, fy3)
        };
        painter.drawPolygon(pointsN, 3);


        fx1 = 0;
        fy1 = m_size/2 - m_offset - m_size/25 - 15;
        fx2 = -arrowWidth/2;
        fy2 = 0;
        fx3 = arrowWidth/2;
        fy3 = 0;

        painter.setBrush(QBrush(Qt::white));
        QPointF pointsS[3] = {
            QPointF(fx1, fy1),
            QPointF(fx2, fy2),
            QPointF(fx3, fy3)
        };
        painter.drawPolygon(pointsS, 3);

        painter.rotate(m_yaw);
    }

}
