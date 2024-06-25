#pragma once

#include <QLabel>
#include <QPixmap>
#include <QResizeEvent>


class QImageViewer final : public QLabel
{
    Q_OBJECT
public:
    explicit QImageViewer(QWidget* parent = nullptr);
    int heightForWidth(int width) const override;
    QSize sizeHint() const override;
    QPixmap scaledPixmap() const;

public slots:
    void SetPixmap(const QPixmap&);
    void ResizeEvent(QResizeEvent*);

private:
    QPixmap pix;
};
