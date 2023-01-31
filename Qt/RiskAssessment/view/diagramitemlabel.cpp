#include "diagramitemlabel.hpp"

DiagramItemLabel::DiagramItemLabel(QGraphicsItem* parent): QGraphicsTextItem(parent)
{

}

void DiagramItemLabel::keyReleaseEvent(QKeyEvent* event) {
    centerTextBox();
    QGraphicsTextItem::keyReleaseEvent(event);
}

void DiagramItemLabel::focusInEvent(QFocusEvent* event) {
    auto splits = this->toPlainText().split('_');
    this->setPlainText(splits.back());
    centerTextBox();
    QGraphicsTextItem::focusInEvent(event);
}

void DiagramItemLabel::focusOutEvent(QFocusEvent* event) {
    emit nameChanged(this->toPlainText());
    centerTextBox();
    QGraphicsTextItem::focusOutEvent(event);
}

void DiagramItemLabel::centerTextBox(){
    auto x = parentItem()->boundingRect().center().x() - boundingRect().width()/2.0;
    auto b = parentItem()->boundingRect().bottom() + 1.0f;
    setPos(QPointF(x, b));
}
