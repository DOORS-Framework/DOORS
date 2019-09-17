#include "messagelabel.h"
#include "messagelabel.moc.h"

MessageLabel::MessageLabel(const QString& text, QWidget* parent)
  : QLabel(text, parent, "MessageLabel") 
{ 
}

void
MessageLabel::mouseReleaseEvent(QMouseEvent* e) 
{ 
  emit clicked(); 
}
