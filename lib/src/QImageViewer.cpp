#include <qframgrabber/QImageViewer.hpp>


QImageViewer::QImageViewer(QWidget* parent)
    : QLabel { parent }
{
    setMinimumSize(1, 1);
    setScaledContents(false);
}

int QImageViewer::heightForWidth(int width) const
{
    if (pix.isNull()) [[unlikely]]
        return this->height();

    return static_cast<qreal>(pix.height()) * width / pix.width();
}

QSize QImageViewer::sizeHint() const
{
    const int w = width();
    return { w, heightForWidth(w) };
}

QPixmap QImageViewer::scaledPixmap() const
{
    // return pix.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    auto scaled = pix.scaled(size() * devicePixelRatioF(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    scaled.setDevicePixelRatio(devicePixelRatioF());
    return scaled;
}

void QImageViewer::SetPixmap(const QPixmap& pixmal)
{
    pix = pixmal;
    QLabel::setPixmap(scaledPixmap());
}

void QImageViewer::ResizeEvent(QResizeEvent*)
{
    if(!pix.isNull())
        QLabel::setPixmap(scaledPixmap());
}
