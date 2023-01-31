#ifndef DIAGRAMITEMLABEL_H
#define DIAGRAMITEMLABEL_H

#include <QGraphicsTextItem>

class DiagramItemLabel : public QGraphicsTextItem {
    Q_OBJECT
public:
    explicit DiagramItemLabel(QGraphicsItem *parent = nullptr);

public:
signals:
    void nameChanged(QString name);

    // QGraphicsItem interface
protected:
    void keyReleaseEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
private:
    void centerTextBox();
    friend class DiagramItem;
};

#endif // DIAGRAMITEMLABEL_H
